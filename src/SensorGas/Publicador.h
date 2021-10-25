// -*- mode: c++ -*-
// ----------------------------------------------------------
// Publicador.h
// @autor: Aitor Benítez Estruch
// @fecha: 2021/10/05
// 
// @Descripcion:
// La clase Publicador inicializa un objeto de la clase EmisoraBLE 
// que nos permitirá manejarlo para poder publicar los resultados de
// las mediciones de gas, temperatura y humedad relativa.
// ----------------------------------------------------------

#ifndef PUBLICADOR_H_INCLUIDO
#define PUBLICADOR_H_INCLUIDO

// --------------------------------------------------------------
// --------------------------------------------------------------
class Publicador {

  // ............................................................
  // ............................................................
private:

  uint8_t beaconUUID[16] = { 
	'E', 'P', 'S', 'G', '-', 'G', 'T', 'I', 
	'-', 'P', 'R', 'O', 'Y', '-', '3', 'A'
	};

 
public:
  // ............................................................
  // Declaramos un objeto de la clase EmisoraBLE 
  // ............................................................
  EmisoraBLE laEmisora {
	  "GTI-3A-ABENEST", //  nombre emisora
	  0x004c, // fabricanteID (Apple)
	  4 // txPower
  };
  
  const int RSSI = -53; // por poner algo, de momento no lo uso

  // ............................................................
  // ............................................................
public:

  // ............................................................
  // Enum para enviarlo como flag del tipo de medida que estamos enviando
  // ............................................................
  enum MedicionesID  {
	CONCENTRACION = 11,
	TEMPERATURA = 12,
	HUMEDAD = 13
  };

  // ............................................................
  // Constructor()
  // ............................................................
  Publicador( ) {
	// ATENCION: no hacerlo aquí. (*this).laEmisora.encenderEmisora();
	// Pondremos un método para llamarlo desde el setup() más tarde
  } // ()

  // ............................................................
  //  encenderEmisora() -> 
  // 
  // encenderEmisora() hace referencia al método encenderEmisora() del objeto EmisoraBLE laEmisora
  // ............................................................
  void encenderEmisora() {
	(*this).laEmisora.encenderEmisora();
  } // ()

  // ............................................................
  // valorConcentracion: N,
  // tiempoEspera : N -> publicarConcentracion() <-
  //
  // publicarConcentracion() hace referencia al método emitirAnuncioIBeacon() del objeto laEmisora
  // para publicar el valor de concentración de gas con un flag de que la medida enviada es gas (enviado en Major),
  // y el valor de la medición en Minor.
  //
  // @params valorConcentracion Valor del gas medido
  // @params tiempoEspera Tiempo de espera hasta que se llame al método detenerAnuncio() de laEmisora
  // ............................................................
  void publicarConcentracion( int16_t valorConcentracion, long tiempoEspera ) {

    	//
    	// 1. empezamos anuncio
      // operador << desplaza los bits de la variable Concentracion 8 posiciones a la izquierda
      // para indicar el tipo de medida que se envía (11 = Concentracion)
    	//
    	uint16_t major = (MedicionesID::CONCENTRACION << 8);
    	(*this).laEmisora.emitirAnuncioIBeacon( (*this).beaconUUID, 
    											major,
    											valorConcentracion, // minor
    											(*this).RSSI // rssi
    									);
    
    	//
    	// 2. esperamos el tiempo que nos digan
    	//
    	delay( tiempoEspera );
    
    	//
    	// 3. paramos anuncio
    	//
    	(*this).laEmisora.detenerAnuncio();
  } // ()

  // ............................................................
  // valorTemperatura: N,
  // tiempoEspera : N -> publicarTemperatura() <-
  //
  // publicarTemperatura() hace referencia al método emitirAnuncioIBeacon() del objeto laEmisora
  // para publicar el valor de concentración de gas con un flag de que la medida enviada es gas (enviado en Major),
  // y el valor de la medición en Minor.
  //
  // @params valorTemperatura Valor del gas medido
  // @params tiempoEspera Tiempo de espera hasta que se llame al método detenerAnuncio() de laEmisora
  // ............................................................
  void publicarTemperatura( int16_t valorTemperatura, long tiempoEspera ) {

      //
      // 1. empezamos anuncio 
      // operador << desplaza los bits de la variable TEMPERATURA 8 posiciones a la izquierda para indicar el tipo de medida que se envía (12 = TEMPERATURA)
    	uint16_t major = (MedicionesID::TEMPERATURA << 8);
      
    	(*this).laEmisora.emitirAnuncioIBeacon( (*this).beaconUUID, 
    											major,
    											valorTemperatura, // minor
    											(*this).RSSI // rssi
    									);

                      
      //
      // 2. esperamos el tiempo que nos digan
      //           
    	delay( tiempoEspera );

      //
      // 3. paramos anuncio
      //
    	(*this).laEmisora.detenerAnuncio();
  } // ()

   // ............................................................
  // valorRH: N,
  // tiempoEspera : N -> publicarRH() <-
  //
  // publicarRH() hace referencia al método emitirAnuncioIBeacon() del objeto laEmisora
  // para publicar el valor de concentración de gas con un flag de que la medida enviada es gas (enviado en Major),
  // y el valor de la medición en Minor.
  //
  // @params valorRH Valor del gas medido
  // @params tiempoEspera Tiempo de espera hasta que se llame al método detenerAnuncio() de laEmisora
  // ............................................................
  void publicarRH( int16_t valorRH,long tiempoEspera ) {

      //
      // 1. empezamos anuncio
      // operador << desplaza los bits de la variable CO2 8 posiciones a la izquierda para indicar el tipo de medida que se envía (13 = RH)
      //
      uint16_t major = (MedicionesID::HUMEDAD << 8);
      (*this).laEmisora.emitirAnuncioIBeacon( (*this).beaconUUID, 
                          major,
                          valorRH, // minor
                          (*this).RSSI // rssi
                      );
    
      //
      // 2. esperamos el tiempo que nos digan
      //
      delay( tiempoEspera );
    
      //
      // 3. paramos anuncio
      //
      (*this).laEmisora.detenerAnuncio();
  } // ()
	
}; // class

// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
#endif
