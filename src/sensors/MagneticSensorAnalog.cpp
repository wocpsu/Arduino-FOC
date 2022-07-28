#include "MagneticSensorAnalog.h"
/** MagneticSensorAnalog(uint8_t _pinSin,uint8_t _pinCos, uint8_t _zero )
 * @param _pinAnalog  the pin that is reading the pwm from magnetic sensor
 * @param _min_raw_count  the smallest expected reading.  Whilst you might expect it to be 0 it is often ~15.  Getting this wrong results in a small click once per revolution
 * @param _max_raw_count  the largest value read.  whilst you might expect it to be 2^10 = 1023 it is often ~ 1020. Note: For ESP32 (with 12bit ADC the value will be nearer 4096)
 * @param _zero zero voltage for sine cosine in 8 bit width
 */


MagneticSensorAnalog::MagneticSensorAnalog(uint8_t _pinSin,uint8_t _pinCos, int _zeroSineCos ){

  pinSin = _pinSin;
  pinCos = _pinCos;
  zeroSineCos = _zeroSineCos;
  //cpr = _max_raw_count - _min_raw_count;
  //min_raw_count = _min_raw_count;
  //max_raw_count = _max_raw_count;

 // if(pullup == Pullup::USE_INTERN){
 //   pinMode(pinAnalog, INPUT_PULLUP);
 // }else{
    pinMode(pinSin, INPUT);
    pinMode(pinCos, INPUT);
	#ifndef __AVR__
	analogReadResolution(10);//Set Read Resolution to 10 bits//curry
	StartTasking();///Start ADC Recording Task

	#endif
  //}

}


void MagneticSensorAnalog::init(){
	//raw_count = getRawCount();
  this->Sensor::init(); // call base class init
}

//  Shaft angle calculation
//  angle is in radians [rad]
float MagneticSensorAnalog::getSensorAngle(){
  // raw data from the sensor
  #ifdef __AVR__
  sinInt = getRawInt(pinSin);   
  cosInt = getRawInt(pinCos); 
  #else
  sinInt = taskADCReadSin;
  cosInt = taskADCReadCos;
  #endif
  //uint8_t A = arctanSimple((uint8_t)cosInt/4 - zeroSineCos, (uint8_t)sinInt/4 - zeroSineCos);   //devide by 4 is to go from 10 bit to 8 bit
  //float rad = atan2((uint8_t)cosInt/4 - zeroSineCos, (uint8_t)sinInt/4 - zeroSineCos);
  //float rad = 0.0061419257*(float)A; //1024 to rad
  
#ifndef __AVR__
  //cordic.atan2sqrt(sinInt - zeroSineCos*4, cosInt - zeroSineCos*4); ///using cordic 10 us faster than atan
  //float rad = (float)cordic.angle*_2PI/65536; ///using cordic 10 us faster than atan
  //float rad = _normalizeAngle(atan2(sinInt - zeroSineCos*4, cosInt - zeroSineCos*4));///using atan kinda slow
  float rad = radTask;
  return rad;
#else
	
  cordic.atan2sqrt(sinInt - zeroSineCos*4, cosInt - zeroSineCos*4);
  //signed int A = arctanSimple((byte)(cosInt/4) - (byte)zeroSineCos, (byte)(sinInt/4) -(byte)zeroSineCos);
  Ang1 = (byte)(cosInt/4) - (byte)zeroSineCos;
  Ang2 = (byte)(sinInt/4) - (byte)zeroSineCos;
  //float rad = (float)A*0.3519;//*_2PI/1024;
  float rad = (float)cordic.angle*_2PI/65536;
  return rad;
#endif
}

// function reading the raw counter of the magnetic sensor
int MagneticSensorAnalog::getRawInt(uint8_t _pinRead){
#ifndef __AVR__
	return adcRead(_pinRead);////Much much faster than analogRead 10X faster!!!! 10us/sample - 100khz
	
	//return analogRead(_pinRead);
#else
	return analogRead(_pinRead);
#endif
}
