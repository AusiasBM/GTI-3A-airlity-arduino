// -*- mode: c++ -*-

// ----------------------------------------------------------
// EmisoraBLE.h
// @autor: Aitor Benítez Estruch
// @fecha: 2021/10/05
// 
// @Descripcion: 
// Clase EmisoraBLE permite inicializar y parar la emisora bluetooth, así como 
// emitir beacons.
// 
// ----------------------------------------------------------

#ifndef EMISORA_H_INCLUIDO
#define EMISORA_H_INCLUIDO

// Buena introducción: https://learn.adafruit.com/introduction-to-bluetooth-low-energy/gap
// https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/

// fuente: https://www.instructables.com/id/Beaconeddystone-and-Adafruit-NRF52-Advertise-Your-/
// https://github.com/nkolban/ESP32_BLE_Arduino/blob/master/src/BLEBeacon.h

// https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/
// https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/bleadvertising

// ----------------------------------------------------------
// ----------------------------------------------------------
#include "ServicioEnEmisora.h"

// ----------------------------------------------------------
// ----------------------------------------------------------
class EmisoraBLE {
private:

  const char * nombreEmisora;
  const uint16_t fabricanteID;
  const int8_t txPower;

public:

  // .........................................................
  // connHandle:N -> CallbackConexionEstablecida()
  // .........................................................
  using CallbackConexionEstablecida = void ( uint16_t connHandle );

  // .........................................................
  // connHandle:N,
  //reason:N -> CallbackConexionTerminada()
  // .........................................................
  using CallbackConexionTerminada = void ( uint16_t connHandle, uint8_t reason);

  // .........................................................
  // nombreEmisora:Text,
  // fabricanteID:N,
  // txPower : Z-> Constructor() ->
  // .........................................................
  EmisoraBLE( const char * nombreEmisora_, const uint16_t fabricanteID_,
			  const int8_t txPower_ ) 
	:
	nombreEmisora( nombreEmisora_ ) ,
	fabricanteID( fabricanteID_ ) ,
	txPower( txPower_ )
  {

  } // ()

	
  // .........................................................
  // encenderEmisora() ->
  //
  // Este método inicia la emisora bluetooth de la clase Bluefuit
  // .........................................................
  void encenderEmisora() {
	// Serial.println ( "Bluefruit.begin() " );
	 Bluefruit.begin(); 

	 // por si acaso:
	 (*this).detenerAnuncio();
  } // ()

  // .........................................................
  // CallbackConexionEstablecida,
  // CallbackConexionTerminada->encenderEmisora() ->
  // .........................................................
  void encenderEmisora( CallbackConexionEstablecida cbce,
						CallbackConexionTerminada cbct ) {

	encenderEmisora();

	instalarCallbackConexionEstablecida( cbce );
	instalarCallbackConexionTerminada( cbct );

  } // ()

  // .........................................................
  // detenerEmisora() <-
  //
  // detenerAnuncio() comprueba si la emisora de Bluefruit esta emitiendo en ese momento,
  // y en caso afirmativo la avisa que pare.
  // .........................................................
  void detenerAnuncio() {

	if ( (*this).estaAnunciando() ) {
	  // Serial.println ( "Bluefruit.Advertising.stop() " );
	  Bluefruit.Advertising.stop(); 
	}

  }  // ()
  
  // .........................................................
  // V/F <- estaAnunciando() 
  // 
  // Método que comprueba si la emisora está emitiendo beacons
  //
  // @return V/F Valor booleano sobre si está emitiendo o no
  // .........................................................
  bool estaAnunciando() {
	return Bluefruit.Advertising.isRunning();
  } // ()

  // .........................................................
  // beaconUUID:Text,
  // major:Z,
  // minor : Z,
  // rssi : Z->emitirAnuncioIBeacon()
  //
  // emitirAnuncioIBeacon() es el método que permite enviar beacons
  //
  // @params beaconUUID Texto con un UUID definido 
  // @params major Entero de 16 bits
  // @params minor Entero de 16 bits
  // @params rssi  Entero de 8 bits
  //
  // .........................................................
  void emitirAnuncioIBeacon( uint8_t * beaconUUID, int16_t major, int16_t minor, uint8_t rssi ) {

	//
	(*this).detenerAnuncio();
	
	//
	// creo el beacon 
	//
	BLEBeacon elBeacon( beaconUUID, major, minor, rssi );
	elBeacon.setManufacturer( (*this).fabricanteID );

	//
	// parece que esto debe ponerse todo aquí
	//

	Bluefruit.setTxPower( (*this).txPower );
	Bluefruit.setName( (*this).nombreEmisora );
	Bluefruit.ScanResponse.addName(); // para que envíe el nombre de emisora (?!)

	//
	// pongo el beacon
	//
	Bluefruit.Advertising.setBeacon( elBeacon );

	//
	// ? qué valorers poner aquí
	//
	Bluefruit.Advertising.restartOnDisconnect(true); // no hace falta, pero lo pongo

  //Información de la clase BLEAdvertising de Bluefruit:
  //#define BLE_ADV_INTERVAL_FAST_DFLT       32  // 20    ms (in 0.625 ms unit)
  //#define BLE_ADV_INTERVAL_SLOW_DFLT       244 // 152.5 ms (in 0.625 ms unit)
  //#define BLE_ADV_FAST_TIMEOUT_DFLT        30  // in seconds
  //Probemos con los valores anteriores (según recomiendan)
	Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);

	//
	// empieza el anuncio, 0 = tiempo indefinido (ya lo pararán)
	//
	Bluefruit.Advertising.start(); 
	
  } // ()

  // .........................................................
  //
  // Ejemplo de Beacon (31 bytes)
  //
  // https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/
  //
  // The iBeacon Prefix contains the hex data : 0x0201061AFF004C0215. This breaks down as follows:
  //
  // 0x020106 defines the advertising packet as BLE General Discoverable and BR/EDR high-speed incompatible.
  // Effectively it says this is only broadcasting, not connecting.
  //
  // 0x1AFF says the following data is 26 bytes long and is Manufacturer Specific Data.
  //
  // 0x004C is Apple’s Bluetooth Sig ID and is the part of this spec that makes it Apple-dependent.
  //
  // 0x02 is a secondary ID that denotes a proximity beacon, which is used by all iBeacons.
  //
  // 0x15 defines the remaining length to be 21 bytes (16+2+2+1).
  //
  // Por ejemmplo:
  //
  // 1. prefijo: 9bytes
  //       0x02, 0x01, 0x06,       // advFlags 3bytes
  //       0x1a, 0xff,             // advHeader 2 (0x1a = 26 = 25(lenght de 0x4c a 0xca)+1)   0xFF -> BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA
  //       0x4c, 0x00,             // companyID 2bytes
  //       0x02,                   // ibeacon type 1 byte
  //       0x15,                   // ibeacon length 1 byte (dec=21 lo que va a continuación: desde  la 'f' hasta 0x01)
  // 
  // 2. uuid: 16bytes
  // 'f', 'i', 's', 't', 'r', 'o', 'f', 'i', 's', 't', 'r', 'o', 0xa7, 0x10, 0x96, 0xe0
  // 
  // 2 major: 2bytes
  // 0x04, 0xd2,
  // 
  // minor: 2bytes
  // 0x10, 0xe1,
  // 
  // 0xca, // tx power : 1bytes
  //
  // 0x01, // este es el byte 31 = BLE_GAP_ADV_SET_DATA_SIZE_MAX, parece que sobra
  //
  // .........................................................
  // Para enviar como carga libre los últimos 21 bytes de un iBeacon (lo que normalmente sería uuid-16 major-2 minor-2 txPower-1)
  // .........................................................


  // .........................................................
  // servicio: ServicioEnEmisora -> anyadirServicio()
  // V/F <-
  // .........................................................
  bool anyadirServicio( ServicioEnEmisora & servicio ) {

	Globales::elPuerto.escribir( " Bluefruit.Advertising.addService( servicio ); \n");

	bool r = Bluefruit.Advertising.addService( servicio );

	if ( ! r ) {
	  Serial.println( " SERVICION NO AÑADIDO \n");
	}
	

	return r;
	 // nota: uso conversión de tipo de servicio (ServicioEnEmisora) a BLEService
	 // para addService()
  } // ()

  
  // .........................................................
  // servicio: ServicioEnEmisora -> anyadirServicioConSusCaracteristicas()
  // V/F <-
  // .........................................................
  bool anyadirServicioConSusCaracteristicas( ServicioEnEmisora & servicio ) { 
	return (*this).anyadirServicio( servicio );
  } // 

   // .........................................................
  // servicio: ServicioEnEmisora,
  // caracteristica: caracteristica
  // <T> -> anyadirServicioConSusCaracteristicas()
  // V/F <-
  // .........................................................
  template <typename ... T>
  bool anyadirServicioConSusCaracteristicas( ServicioEnEmisora & servicio,
											 Caracteristica & caracteristica,
											 T& ... restoCaracteristicas) {

	servicio.anyadirCaracteristica( caracteristica );

	return anyadirServicioConSusCaracteristicas( servicio, restoCaracteristicas... );
	
  } // ()

  // .........................................................
  // servicio: ServicioEnEmisora,
  // <T> -> anyadirServicioConSusCaracteristicasYActivar()
  // V/F <-
  // .........................................................
  template <typename ... T>
  bool anyadirServicioConSusCaracteristicasYActivar( ServicioEnEmisora & servicio,
													 // ServicioEnEmisora::Caracteristica & caracteristica,
													 T& ... restoCaracteristicas) {

	bool r = anyadirServicioConSusCaracteristicas( servicio, restoCaracteristicas... );

	servicio.activarServicio();

	return r;
	
  } // ()

  // .........................................................
  // CallbackConexionEstablecida:Cb -> instalarCallbackConexionEstablecida()
  // .........................................................
  void instalarCallbackConexionEstablecida( CallbackConexionEstablecida cb ) {
	Bluefruit.Periph.setConnectCallback( cb );
  } // ()

  // .........................................................
  // CallbackConexionTerminada:Cb -> instalarCallbackConexionTerminada() 
  // .........................................................
  void instalarCallbackConexionTerminada( CallbackConexionTerminada cb ) {
	Bluefruit.Periph.setDisconnectCallback( cb );
  } // ()

  // .........................................................
  // connHandle:N -> getConexion()
  // BLEConnection <-
  // .........................................................
  BLEConnection * getConexion( uint16_t connHandle ) {
	return Bluefruit.Connection( connHandle );
  } // ()

}; // class

#endif

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
