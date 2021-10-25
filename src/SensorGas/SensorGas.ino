// -*-c++-*-

// --------------------------------------------------------------
// SensorGas.ino
// Aitor Benítez Estruch
// 2021/10/05
//
// Descripción:
// Ejecución del programa para medir y obtener las magnitudes del sensor,
// y enviar vía bluetooth las mediciones obtenidas.
//
// --------------------------------------------------------------

// https://learn.sparkfun.com/tutorials/nrf52840-development-with-arduino-and-circuitpython
// https://stackoverflow.com/questions/29246805/can-an-ibeacon-have-a-data-payload

// --------------------------------------------------------------
// --------------------------------------------------------------

#include <bluefruit.h>
#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial


#undef min // vaya tela, están definidos en bluefruit.h y  !
#undef max // colisionan con los de la biblioteca estándar

// --------------------------------------------------------------
// --------------------------------------------------------------
#include "PuertoSerie.h"

// --------------------------------------------------------------
// --------------------------------------------------------------
namespace Globales {

  //Inicializamos el puerto serie 0 (UART0)
  PuertoSerie elPuerto ( 9600 );

};

// --------------------------------------------------------------
// --------------------------------------------------------------
#include "EmisoraBLE.h"
#include "Publicador.h"
#include "Medidor.h"


// --------------------------------------------------------------
// --------------------------------------------------------------
namespace Globales {

  Publicador elPublicador;

  Medidor elMedidor(&Serial1);

}; // namespace



// --------------------------------------------------------------
// La función inicializarComunicacionSensor() se utiliza para iniciar el puerto Serie 1 (UART entre la placa y sensor)
//
// inicializarComunicacionSensor () ->
// --------------------------------------------------------------
void inicializarComunicacionSensor () {

  Serial1.begin(9600);

} // ()

// --------------------------------------------------------------
// setup()
// --------------------------------------------------------------
void setup() {

  // 
  // Iniciamos el Serial1 para establecer la comunicación UART con el sensor
  // 
  inicializarComunicacionSensor();


  // 
  // Inicializamos la emisora BLE de Bluefruit...
  // 
  Globales::elPublicador.encenderEmisora();

  delay( 1000 );

  Globales::elPuerto.escribir( "---- setup(): fin ---- \n " );

} // setup ()



// --------------------------------------------------------------
// loop ()
// --------------------------------------------------------------
namespace Loop {
  uint8_t cont = 0;
};

// ..............................................................
// ..............................................................
void loop () {

  using namespace Loop;
  using namespace Globales;

  cont++;

  // 
  // Mido del sensor 
  // 
  elMedidor.iniciarMedicion('\r');

  elPuerto.escribir( "\n---- loop(): empieza " );
  elPuerto.escribir( cont );
  elPuerto.escribir( "\n" );

  // 
  // Obtengo la concentración de gas y publico
  // 
  uint16_t valorConcentracion = elMedidor.getConcentracionGas();
  int intervaloEmision = 1000;

  elPuerto.escribir( "\n---- Envio C02: empieza \n" );
  elPuerto.escribir( valorConcentracion );
  elPublicador.publicarConcentracion( valorConcentracion,
              intervaloEmision // intervalo de emisión
              );

  elPuerto.escribir( "\n---- Envio C02: TERMINA \n" );

  // 
  // Obtengo la temperatura y publico
  // 
  int valorTemperatura = elMedidor.getTemperatura();

  elPuerto.escribir( "\n---- Envio Temperatura: empieza \n" );
  elPuerto.escribir( valorTemperatura );
  elPublicador.publicarTemperatura( valorTemperatura, 
                  intervaloEmision // intervalo de emisión
                  );
  elPuerto.escribir( "\n---- Envio Temperatura: TERMINA \n" );


  // 
  // Obtengo la humedad y publico
  // 
  int valorRH = elMedidor.getRH();

  elPuerto.escribir( "\n---- Envio Humedad Relativa: empieza \n" );
  elPuerto.escribir( valorRH );
  elPublicador.publicarRH( valorRH, 
                  intervaloEmision // intervalo de emisión
                  );
  elPuerto.escribir( "\n---- Envio Humedad Relativa: TERMINA \n" );

  elPuerto.escribir( "---- loop(): acaba **** " );
  elPuerto.escribir( cont );
  elPuerto.escribir( "\n" );
  elPublicador.laEmisora.detenerAnuncio();
  delay( 10000 );
  // 
  // 
  // 
  
} // loop ()
// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
