#include "LineSensor.h"
#include "Ultrasonic.h"
#include "MotorDriver.h"
#include "BCD.h"

#define DEBUG

#define pin_linesensor_ll   9
#define pin_linesensor_lm   10
#define pin_linesensor_mm   11
#define pin_linesensor_mr   12
#define pin_linesensor_rr   13
#define pin_ultrasonic_echo 2
#define pin_ultrasonic_trig 3
#define pin_motor_e1        5
#define pin_motor_m1        4
#define pin_motor_e2        6
#define pin_motor_m2        7
#define pin_bcd_data        A0
#define pin_bcd_latch       A1
#define pin_bcd_clock       A2

LineSensor lineSensor = LineSensor(pin_linesensor_ll, pin_linesensor_lm, pin_linesensor_mm, pin_linesensor_mr, pin_linesensor_rr);
Ultrasonic ultrasonic = Ultrasonic(pin_ultrasonic_echo, pin_ultrasonic_trig);
MotorDriverPWM motorDriver = MotorDriverPWM(pin_motor_e1, pin_motor_m1, pin_motor_e2, pin_motor_m2);
//MotorDriverPLL motorDriver = MotorDriverPLL(pin_motor_m1, pin_motor_e1, pin_motor_m2, pin_motor_e2);
BCD bcd = BCD(pin_bcd_data, pin_bcd_latch, pin_bcd_clock);

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif

  lineSensor.setup();
  lineSensor.invert(false);

  ultrasonic.setup();

  motorDriver.setup();
  bcd.setup();
  bcd.write("--");
  //  motorDriver.flipLeftMotorDirection();
  //  motorDriver.flipRightMotorDirection();

  // TODO: Remove this, split 5v instead
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);

}

void loop() {
  lineSensor.update();
  //  ultrasonic.update();

  // ============== TEST MOTORS ==============
  delay(1000);

  for (int i = 255; i > 0; i--) {
    motorDriver.setLeftSpeed(i);
    motorDriver.setRightSpeed(i);
    delay(20);
  }

  motorDriver.stop();
  delay(2000);

  for (int i = -255; i < 0; i++) {
    motorDriver.setLeftSpeed(i);
    motorDriver.setRightSpeed(i);
    delay(20);
  }

  motorDriver.stop();
  delay(200000);

  // ============== TEST MOTORS ==============

#ifdef DEBUG
  Serial.print(lineSensor.hasLine());
  Serial.print("\t");
  Serial.print(lineSensor.toString());
  Serial.print("\t");
  Serial.print(lineSensor.toPrettyString());

  Serial.print("\t");
  Serial.print(ultrasonic.distance_cm());
  Serial.print("cm");

  Serial.println();
#endif
}
