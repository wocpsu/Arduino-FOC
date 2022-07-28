#ifndef MAGNETICSENSORANALOG_LIB_H
#define MAGNETICSENSORANALOG_LIB_H

#include "Arduino.h"
#include "../common/base_classes/Sensor.h"
#include "../common/foc_utils.h"
#include "../common/time_utils.h"

#ifdef __AVR__
	#include <cordicuno.h>
#else
	//#include <cordic.h>
    #include "current_sense/hardware_specific/esp32/esp32_adc_driver.h"
	#include <soc/sens_reg.h>
    #include <soc/sens_struct.h>
	#include "soc/timer_group_struct.h"
    #include "soc/timer_group_reg.h"
#endif

/**
 * This sensor has been tested with AS5600 running in 'analog mode'.  This is where output pin of AS6000 is connected to an analog pin on your microcontroller.
 * This approach is very simple but you may more accurate results with MagneticSensorI2C if that is also supported as its skips the DAC -> ADC conversion (ADC supports MagneticSensorI2C)
 */
 

 
class MagneticSensorAnalog: public Sensor{
	
//int MagneticSensorAnalog::taskCore = 0;
 public:
    /**
     * MagneticSensorAnalog class constructor
     * @param _pinAnalog  the pin to read the PWM signal
     */
    MagneticSensorAnalog(uint8_t _pinSin, uint8_t _pinCos, int _zeroSineCos);
	  

    /** sensor initialise pins */
    void init();
    cordic10 cordic;
    int pinSin; //!< encoder hardware pin Sine
    int pinCos; //!< encoder hardware pin Cosine
	int zeroSineCos;
    int Ang1;
	int Ang2;
    // Encoder configuration
    Pullup pullup;
    // implementation of abstract functions of the Sensor class
    /** get current angle (rad) */
    float getSensorAngle() override;
    /** raw count (typically in range of 0-1023), useful for debugging resolution issues */
    int sinInt;
	int cosInt;
	volatile int taskADCReadSin = 0;
	volatile int taskADCReadCos = 0;
	volatile float radTask;

  private:
    //int min_raw_count;
    //int max_raw_count;
    //int cpr;
    int read();

    /**
     * Function getting current angle register value
     * it uses angle_register variable
     */
    int getRawInt(uint8_t _pinRead);
	int arctanSimple(int V1, int V2);


  public: 
    int testVar = 0;
    char request[100] = "";
    volatile bool taskFinished = true;
	volatile unsigned int taskTime;
  private: 
    const int taskCore = 0;
    const int taskPriority = 1;
    TaskHandle_t Task1;
  
  // public:  AsyncHTTPSRequest(void){
    
  // }
  
  public: void StartTasking(void)
  {      
    taskFinished = false;
    //Start Task with input parameter set to "this" class
    xTaskCreatePinnedToCore(
      this->coreTask,        //Function to implement the task 
      "coreTask",            //Name of the task
      5000,                   //Stack size in words 
      this,                   //Task input parameter 
      taskPriority,           //Priority of the task 
      &Task1,                 //Task handle.
      taskCore);              //Core where the task should run 
  }
  
  
  private: static void coreTask(void *pvParameters)
  {   
    MagneticSensorAnalog *l_pThis = (MagneticSensorAnalog *) pvParameters;   
    // Handle the request here
    //int pin1* MagneticSensorAnalog::pinCos;
    //int pin2* MagneticSensorAnalog::pinSin;
    //delay(1000); //Pretend work
    
    l_pThis->testVar = 1;

    l_pThis->taskFinished = true;
    while(true){
	  unsigned int microsStart = _micros();
      unsigned int TempSin, TempCos;
      TempCos = adcRead(l_pThis->pinCos);
      TempSin = adcRead(l_pThis->pinSin);
      for(int i=1;i<5;i++)
      {
      TempCos = TempCos + adcRead(l_pThis->pinCos);
      TempSin = TempSin + adcRead(l_pThis->pinSin);
      }
      l_pThis->taskADCReadSin = TempSin/5;
      l_pThis->taskADCReadCos = TempCos/5;
      l_pThis->radTask = _normalizeAngle(atan2(l_pThis->taskADCReadSin - l_pThis->zeroSineCos*4, l_pThis->taskADCReadCos - l_pThis->zeroSineCos*4));
	  //Other Cordic Option...
	  //cordic.atan2sqrt(sinInt - zeroSineCos*4, cosInt - zeroSineCos*4); ///using cordic 10 us faster than atan
      //float rad = (float)cordic.angle*_2PI/65536; ///using cordic 10 us faster than atan
	  //
	  l_pThis->taskTime = _micros() - microsStart;
      delayMicroseconds(10);
      TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
      TIMERG0.wdt_feed=1;
      TIMERG0.wdt_wprotect=0;
      //esp_task_wdt_reset();
//      sensor.update();
//      Serial.print("Angle= ");
//      Serial.println(sensor.getAngle());
//      Serial.println("Cosine= ");
//      Serial.println(sensor.taskADCReadCos);
//      vTaskDelayUntil(&xLastWakeTime, xTaskFrequency + 1);
    }
  }
     
};

#endif
