#ifndef MAGNETICSENSORANALOG_LIB_H
#define MAGNETICSENSORANALOG_LIB_H

#include "Arduino.h"
#include "../common/base_classes/Sensor.h"
#include "../common/foc_utils.h"
#include "../common/time_utils.h"

#ifdef __AVR__
	#include <cordicuno.h>
#else
	#include <cordic.h>
    #include "current_sense/hardware_specific/esp32/esp32_adc_driver.h"
	#include <soc/sens_reg.h>
    #include <soc/sens_struct.h>
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
/* 	portMUX_TYPE DRAM_ATTR timerMux = portMUX_INITIALIZER_UNLOCKED; 
    TaskHandle_t complexHandlerTask;
    hw_timer_t * adcTimer = NULL; // our timer
	 void IRAM_ATTR onTimer() {
	  portENTER_CRITICAL_ISR(&timerMux);
	  taskADCReadSin = getRawInt(pinSin);
	  taskADCReadCos = getRawInt(pinCos);

	  portEXIT_CRITICAL_ISR(&timerMux);
	} */
    //const int taskCore = 0;
// void coreTask( void * pvParameters ){
 
 
    // while(true){
	  // taskADCReadSin = getRawInt(pinSin);
	  // taskADCReadCos = getRawInt(pinCos);
        // delay(1);
    // }
	
 
// }
	// void taskOne( void * parameter )
	// {
	 
    // while(true){
	  // taskADCReadSin = getRawInt(pinSin);
	  // taskADCReadCos = getRawInt(pinCos);
        // delay(1);
    // }
	 
	// }
};


#endif
