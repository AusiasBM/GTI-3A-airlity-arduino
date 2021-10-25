#ifndef _MEDICION_h
#define _MEDICION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#elif defined(SPARK)
#include "application.h"
#else
//#include "WProgram.h"
#endif

class Medicion{

   private:
    Stream *_mySerial;
    long dataArray[11]; //Multipurpose array
      
    unsigned long sleepTime;
    bool isSleeping;

    public:
    //-------------------------------------------
    //Constructores
    //-------------------------------------------
    Medicion(Stream *mySerial) : _mySerial(mySerial) {
      isSleeping = false;
    }
    
    Medicion(Stream &mySerial) : _mySerial(&mySerial) {
      isSleeping = false;
    }


    //-------------------------------------------
    //iniciarMedicion()
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
    //getConcentracionGas()
    //-------------------------------------------
    long getConcentracionGas()
    {
      return dataArray[1];
    }

    
    //-------------------------------------------
    //getTemperatura()
    //-------------------------------------------
    float getTemperatura()
    {  
      return dataArray[2]; //return integer in degC
    }

    //-------------------------------------------
    //getRH()
    //-------------------------------------------
    float getRh()
    {
      return dataArray[3];
    }



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
