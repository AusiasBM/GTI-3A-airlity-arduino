// -*-c++-*-

// --------------------------------------------------------------
// testMedidor.ino
// Aitor Benítez Estruch
// 2021/10/13
//
// Descripción:
// Test para provar la lectura del sensor. Para ello, es necesario
// conectar el sensor de gas a la placa a través de una conexión
// UART entre los pines 17 (TX) y 15(RX) de la placa a los pines
// 2(RX) y 3(TX).
//
// --------------------------------------------------------------

#include "Medicion.h"
#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial

//Creamos el objeto Medicion y le asignamos el Serial1
Medicion mySensor1(&Serial1);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial1.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

Serial.println("...Iniciando nueva medida...");
  //Le pasamos el caracter '\r' para que haga la lectura del sensor
  mySensor1.iniciarMedicion('\r');

  Serial.print("Concentration = ");
  Serial.println(mySensor1.getConcentracionGas());
  
  Serial.print("Average Temperature = ");
  Serial.println(mySensor1.getTemperatura());

  Serial.print("Average Humidity = ");
  Serial.println(mySensor1.getRh());
  Serial.println();
  Serial.println();

  delay(5000);
}
