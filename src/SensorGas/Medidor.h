// ----------------------------------------------------------
// Medidor.h
// @autor: Aitor Ben�tez Estruch
// @fecha: 2021/10/13
// 
// @Descripcion:
// La clase medidor sirve para obtener las mediciones del sensor de gas.
// A través de esta clase se puede obtener la concentración de gas (en ppb),
// la temperatura (en grados Celsius) y la humedad relativa (en %).
// 
// ----------------------------------------------------------

#ifndef _MEDIDOR_h
#define _MEDIDOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#elif defined(SPARK)
#include "application.h"
#else
//#include "WProgram.h"
#endif

class Medidor{

   private:
    Stream *_mySerial;
    long dataArray[11]; //Multipurpose array
    String informacionSensor[5]; //Stores the character based EEPROM data
      
    //unsigned long sleepTime;
    //bool isSleeping;

    public:
    //-------------------------------------------
    // mySerial: Stream -> Constructor() ->
    // 
    // Constructor de la clase Medidor.
    //
    //@params mySerial Puerto serie para la comunicación entre sensor y placa
    //
    //-------------------------------------------
    Medidor(Stream *mySerial) : _mySerial(mySerial) {
      //isSleeping = false;
    }
    
    Medidor(Stream &mySerial) : _mySerial(&mySerial) {
      //isSleeping = false;
    }


    //-------------------------------------------
    // iniciarMedicion() <-
    // 
    // getInformacionSensor() es el método mediante el cual se recibe la parte de
    // de la información del sensor. Nos servirá para saber el tipo de gas que mide
    // de manera que si cambiamos de tipo de sensor sabremos que estamos midiendo. 
    //
    // @return [Texto] Información sobre el sensor [Barcode, Serial_Number, Part_Number, Gas, Date_Code, Sensitivity_Code]
    //-------------------------------------------
    void getInformacionSensor(void)
    {
     
      while (_mySerial->available()) _mySerial->read();
      _mySerial->flush();
      _mySerial->write('e');
    
      while (!_mySerial->available()) {}
    
      String data = _mySerial->readStringUntil('\n');
      while (!_mySerial->available()) {}
      for (int i = 14; i < 19; i++) {
        data = _mySerial->readStringUntil('\n');
        String subS1 = data.substring(0, data.indexOf('='));
        String subS2 = data.substring(data.indexOf('=') + 2);
        informacionSensor[i - 14] = subS2;
       
      }
      data = _mySerial->readStringUntil('\n');
      String subS1 = data.substring(0, data.indexOf('='));
      String subS2 = data.substring(data.indexOf('=') + 2);
     
      while (_mySerial->available()) _mySerial->read();
    
    }

    //-------------------------------------------
    //c: caracter -> iniciarMedicion() 
    // 
    // iniciarMedicion() es el método mediante el cual se envia un caracter
    // por el puerto serie establecido en el constructor al sensor con la instrucción
    // de medir los parámetros de concentración de gas, temperatura y humedad relativa.
    //
    // @params c Caracter que indica la instrucción de medir (descrito en el Datasheet del sensor)
    //
    // @return V/F Valor booleano de si la operación se a ejecutado bién
    // @return [N] Valores obtenidos por el sensor
    //-------------------------------------------
    int iniciarMedicion(char c)
    {
      delay(500);
      String dataString;
      _mySerial->write(c);
      while (!_mySerial->available()) {
      }
      dataString = _mySerial->readStringUntil('\n');
      for (int i = 0; i < 11; i++) {
        String subS = dataString.substring(0, dataString.indexOf(','));
        if (subS.length() == 0) return 0;
        dataArray[i] = subS.toInt();
        dataString = dataString.substring(dataString.indexOf(',') + 2);
      }
      return 1;
    }


    //-------------------------------------------
    // N <- getConcentracionGas() <-
    // 
    // getConcentraciomGas() retorna el valor en la posición 1 del array devuelto
    // por el método iniciarMedicion() que indica el valor de concentración de gas
    // en partes por billon (ppb)
    //
    // @return N Retorna el valor de la medición
    //-------------------------------------------
    int getConcentracionGas()
    {
      return dataArray[1];
    }

    
    //-------------------------------------------
    // Z <- getTemperatura() <-
    // 
    // getTemperatura() retorna el valor en la posición 2 del array devuelto
    // por el método iniciarMedicion() que indica el valor de temperatura
    // en grados Celsius
    //
    // @return N Retorna el valor de la medición
    //-------------------------------------------
    int getTemperatura()
    {  
      return dataArray[2]; //return integer in degC
    }

    //-------------------------------------------
    //N <- getRH() <-
    // 
    // getRH() retorna el valor en la posición 3 del array devuelto
    // por el método iniciarMedicion() que indica el valor de humedad relativa
    // en %
    //
    // @return N Retorna el valor de la medición
    //-------------------------------------------
    int getRH()
    {
      return dataArray[3];
    }

    //-------------------------------------------
    //Texto <- getTipoMedicion() <-
    // 
    // getTipoMedicion() retorna el tipo de gas que mide el sensor almacenado en la posición 3 del array devuelto
    // por el método getInformacionSensor().
    //
    // @return Texto Retorna el tipo de gas
    //-------------------------------------------
    String getTipoMedicion(){
      return informacionSensor[3];
    }


    //De momento esto no se usa pero puede ser interesante en el futuro
    /*void sleepSensor(unsigned long sleepy)
    {
      isSleeping = true;
      sleepTime = (1000 * sleepy) + millis();
      while (_mySerial->available()) _mySerial->read();
      _mySerial->write('s');  
    }
    
    bool wakeSensor(void)
    {
      if (isSleeping && (millis() > sleepTime)){
        isSleeping = false;
        while (_mySerial->available()) _mySerial->read();
        _mySerial->write('r');
        delay(2000);
      } 
      return !isSleeping;
    }*/

};

#endif
