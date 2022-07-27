// Open loop motor control example
#include <SimpleFOC.h>


// BLDC motor & driver instance
// BLDCMotor(pole pair number, phase resistance [Ohms], KV rating [rpm/V] ...700*1.5)
//BLDCMotor motor = BLDCMotor(6, 0.184, 800); Quad
BLDCMotor motor = BLDCMotor( 5, 0.0027, 38.462 );//Ebike
// BLDCDriver3PWM driver = BLDCDriver3PWM(pwmA, pwmB, pwmC, Enable(optional));
#ifdef __AVR__
BLDCDriver6PWM driver = BLDCDriver6PWM(5, 6, 9, 10, 3, 11);
//BLDCDriver3PWM driver = BLDCDriver3PWM(5, 6, 9);
#else
BLDCDriver6PWM driver = BLDCDriver6PWM(21, 22, 23, 25 , 26, 27);//, 18);
#endif

// instantiate the commander
Commander command = Commander(Serial);
//void doTarget(char* cmd) {
//  command.scalar(&driver.dead_zone, cmd);
//}
//void doLimitVolt(char* cmd) { command.scalar(&motor.voltage_limit, cmd); }
//void doLimitVelocity(char* cmd) { command.scalar(&motor.velocity_limit, cmd); }
void onMotor(char* cmd) {
  command.motor(&motor, cmd);
}
void setup() {

  // driver config
  // power supply voltage [V]
  driver.voltage_power_supply = 24;
  //20khz
  driver.pwm_frequency = 10000;
  driver.dead_zone = 0.001;
  driver.init();
  // link the motor and the driver
  motor.linkDriver(&driver);

  // limiting motor movements
  motor.voltage_limit = 12;   // [V]
  motor.current_limit = 5;   // [A]
  motor.velocity_limit = 350; // [rad/s] cca 50rpm
  // open loop control config
  motor.controller = MotionControlType::velocity_openloop;

  // init motor hardware
  motor.init();
  //motor.initFOC();

  //  add target command T
 // command.add('T', doTarget, "dead zone");
  //  command.add('L', doLimitVolt, "voltage limit");
  //// command.add('V', doLimitVelocity, "velocity limit");
  command.add('A', onMotor, "motor");

  command.verbose = VerboseMode::user_friendly;
  Serial.begin(115200);
  Serial.println("Motor ready!");
  Serial.println("Set target position [rad/s]");
  _delay(1000);
  motor.target = 0;
}

void loop() {
  //motor.loopFOC();
  // open  loop angle movements
  // using motor.voltage_limit and motor.velocity_limit
  motor.move();

  // user communication
  command.run();
}
