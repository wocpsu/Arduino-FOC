/**
 * Comprehensive BLDC motor control example using magnetic sensor
 *
 * Using serial terminal user can send motor commands and configure the motor and FOC in real-time:
 * - configure PID controller constants
 * - change motion control loops
 * - monitor motor variabels
 * - set target values
 * - check all the configuration values
 *
 * See more info in docs.simplefoc.com/commander_interface
 */
#include <SimpleFOC.h>
static int taskCore = 0;
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
// magnetic sensor instance - Sine/Cos
// MagneticSensorAnalog(uint8_t _sinPin,uint8_t _cosPin, uint8_t _sineCosZero)
MagneticSensorAnalog sensor = MagneticSensorAnalog(32, 33,612);//Curry
//MagneticSensorAnalog sensor = MagneticSensorAnalog(32, 33,612);//Curry
#include "esp_task_wdt.h"

// BLDC motor & driver instance
// BLDCMotor(pole pair number, phase resistance [Ohms], KV rating [rpm/V])
BLDCMotor motor = BLDCMotor( 5, 0.0027, 38.462 );
//  BLDCDriver6PWM( int phA_h, int phA_l, int phB_h, int phB_l, int phC_h, int phC_l, int en)
//  - phA_h, phA_l - A phase pwm pin high/low pair 
//  - phB_h, phB_l - B phase pwm pin high/low pair
//  - phB_h, phC_l - C phase pwm pin high/low pair
//  - enable pin    - (optional output)
BLDCDriver6PWM driver = BLDCDriver6PWM(21, 22, 23, 25 , 26, 27);
//BLDCDriver6PWM driver = BLDCDriver6PWM(21, 22, 23, 25 , 28, 29, 18);
//driver.dead_zone = 0.02; //Amount of dead-time for each pwm cycle, as a proportion of 100% duty cycle. A float in the range [0,1]. Values under 10% make sense.
// Hall sensor instance
// HallSensor(int hallA, int hallB , int hallC , int pp)
//  - hallA, hallB, hallC    - HallSensor A, B and C pins
//  - pp                     - pole pairs
//HallSensor sensor = HallSensor(2, 3, 4, 5);
// velocity set point variable
float target_velocity = 0;
// commander interface
Commander command = Commander(Serial);
void onMotor(char* cmd){ command.motor(&motor, cmd); }
//void doTarget(char* cmd){ command.scalar(&target_velocity, cmd); }
void coreTask( void * pvParameters ){
 //    TickType_t xLastWakeTime;
//    const TickType_t xTaskFrequency = 1; // (ms)
//    xLastWakeTime = xTaskGetTickCount(); // Initialise the xLastVoltageWakeTime variable with the current time.
    //String taskMessage = "Task running on core ";
    //taskMessage = taskMessage + xPortGetCoreID();
 
    while(true){
      unsigned int TempSin, TempCos;
      TempCos = adcRead(sensor.pinCos);
      TempSin = adcRead(sensor.pinSin);
      for(int i=1;i<100;i++)
      {
      TempCos = TempCos + adcRead(sensor.pinCos);
      TempSin = TempSin + adcRead(sensor.pinSin);
      }
      
      sensor.taskADCReadSin = TempSin/100;
      sensor.taskADCReadCos = TempCos/100;
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
void setup() {
  
  Serial.begin(115200);
    xTaskCreatePinnedToCore(
                    coreTask,   /* Function to implement the task */
                    "coreTask", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    5,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    taskCore);  /* Core where the task should run */
 
  Serial.println("Task created...");
  delay(500);
  for(int a =1; a>1000;a++)
  {
  sensor.update(); 
  }
      Serial.print("Angle= ");
      Serial.println(sensor.getSensorAngle());
      Serial.println("Cosine= ");
      Serial.println(sensor.taskADCReadCos);
  
  //float minDeltaT = 0.000100; // default is 100 microseconds, or 10kHz
  //sensor.minDeltaT = 0.01;////attempting to do this for noise on velocity//Curry
  // initialise magnetic sensor hardware
   sensor.init();//Curry
   motor.LPF_angle.Tf = 0.001;
  // link the motor to the sensor
  motor.linkSensor(&sensor);//Curry

//motor.sensor_offset = -1.9; // default 0 rad
/// motor.sensor_direction = Direction::CW; // CW or CCW
// motor.sensor_offset = 5.424; // default 0 rad
  // driver config
  // power supply voltage [V]
  driver.voltage_power_supply = 24;
  ///10kHz currently with simpleFocStudio this is the limit
  driver.pwm_frequency = 10000;
  driver.dead_zone = 0.005;
  driver.init();
  // link driver
  motor.linkDriver(&driver);

  // choose FOC modulation
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;//Curry

  // set control loop type to be used
  //motor.controller = MotionControlType::velocity_openloop;//Curry
  motor.controller = MotionControlType::velocity;
  //motor.controller = MotionControlType::torque; ///consider trying this
  //motor.torque_controller = TorqueControlType::voltage;
  // contoller configuration based on the control type
 // motor.PID_velocity.P = 0.2f;//Curry
 // motor.PID_velocity.I = 20;//Curry
 // motor.PID_velocity.D = 0;//Curry
  // default voltage_power_supply
  motor.voltage_limit = 12;
  motor.current_limit = 10;///May not be used closed loop
  motor.voltage_sensor_align = 12; // Volts

  // velocity low pass filtering time constant
  motor.LPF_velocity.Tf = 0.9f;

  sensor.minDeltaT = 0.0001;////attempting to do this for noise on velocity//Curry
  // angle loop controller
  //motor.P_angle.P = 20;//Curry
  // angle loop velocity limit
  motor.velocity_limit = 50;

  // use monitoring with serial for motor init
  // monitoring port
  // comment out if not needed
  motor.useMonitoring(Serial);//Curry
 
  // initialise motor
  sensor.update();
  motor.init();
  // align encoder and start FOC
  motor.initFOC(); //Curry

  // set the inital target value
  motor.target = 0;

  // define the motor id
  command.add('A', onMotor, "motor");

 // command.add('T', doTarget, "target velocity");
  // Run user commands to configure and the motor (find the full command list in docs.simplefoc.com)
  //Serial.println(F("Motor commands sketch | Initial motion control > torque/voltage : target 2V."));

  _delay(1000);
}


void loop() {
  // iterative setting FOC phase voltage
  //sensor.update();
  motor.loopFOC();//Curry
  //sensor.update();
  // iterative function setting the outter loop target
  // velocity, position or voltage
  // if tatget not set in parameter uses motor.target variable
  motor.move();

  // user communication
  command.run();
}
