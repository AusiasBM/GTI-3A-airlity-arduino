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

  uint8_t beaconUUID_IAQ[16] = { 
  'A', 'I', 'R', 'L', 'I', 'T', 'Y', '-', 
  'G', 'A', 'S', '-', '-', 'I', 'A', 'Q'
  };

  uint8_t beaconUUID_SO2[16] = { 
  'A', 'I', 'R', 'L', 'I', 'T', 'Y', '-', 
  'G', 'A', 'S', '-', '-', 'S', 'O', '2'
  };

  uint8_t beaconUUID_NO2[16] = { 
  'A', 'I', 'R', 'L', 'I', 'T', 'Y', '-', 
  'G', 'A', 'S', '-', '-', 'N', 'O', '2'
  };

  uint8_t beaconUUID_O3[16] = { 
  'A', 'I', 'R', 'L', 'I', 'T', 'Y', '-', 
  'G', 'A', 'S', '-', '-', '_', 'O', '3'
  };

  uint8_t beaconUUID[16] = { 
  'T', 'R', 'I', 'C', 'O', 'E', 'N', 'V', 
  'I', 'R', 'O', 'N', 'M', 'E', 'N', 'T'
  };
 
public:
  // ............................................................
  // Declaramos un objeto de la clase EmisoraBLE 
  // ............................................................
  EmisoraBLE laEmisora {
	  "GTI-AIRLITY", //  nombre emisora
	  0x004c, // fabricanteID (Apple)
	  4 // txPower
  };
  
  const int RSSI = -53; // por poner algo, de momento no lo uso

  // ............................................................
  // ............................................................
public:

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
  void publicarMedicion( int16_t valorConcentracion, uint8_t valorTemperatura,uint8_t valorHumedad, String tipoMedicion, long tiempoEspera ) {

      //
      // 1. empezamos anuncio 
      // operador << desplaza los bits de la variable TEMPERATURA 8 posiciones a la izquierda para indicar el tipo de medida que se envía (12 = TEMPERATURA)

      uint8_t uuid[16] = {};

      //No puedo usar un switch con un String!! Alternativa facil If-Else...
      if(tipoMedicion == "IAQ"){
        memcpy( &uuid[0], &beaconUUID_IAQ[0], 16 );
      }else if(tipoMedicion == "NO2"){
        memcpy( &uuid[0], &beaconUUID_NO2[0], 16 ); 
      }else if(tipoMedicion == "SO2"){
        memcpy( &uuid[0], &beaconUUID_SO2[0], 16 );    
      }else if(tipoMedicion == "O3"){
        memcpy( &uuid[0], &beaconUUID_O3[0], 16 );     
      }else{
        memcpy( &uuid[0], &beaconUUID_IAQ[0], 16 );
      }
        
      uint16_t minor = (valorTemperatura << 8) + (valorHumedad << 0);
      (*this).laEmisora.emitirAnuncioIBeacon( uuid, 
          12,
          minor, // minor
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
