#include <Servo.h>

// For wio-terminal
// #define servo_base_pin D7
// #define servo_door_pin D8

// For arduino-uno
int servo_base_pin = 8;  // analog pin used to connect the potentiometer
int servo_door_pin = 9;  // analog pin used to connect the potentiometer

Servo myservo_base, myservo_door;  // create servo object to control a servo

int val;    // variable to read the value from the analog pin
String msg = "";

void setup() {
  Serial.begin(9600);
  myservo_base.attach(servo_base_pin);  // attaches the servo on pin 9 to the servo object
  myservo_door.attach(servo_door_pin);  // attaches the servo on pin 9 to the servo object

  moveServo(myservo_base, 0); // base bottle
  moveServo(myservo_door, 0); // door closed

  // Serial comunication
  //Serial.setTimeout(1);
}

void loop() {

  // Waiting for serial communciation result
  while (!Serial.available());
  msg = Serial.readString();
  
  // Testing on Serial monitor
  // msg = String(random(0, 3));
  // delay(5000);
  // Serial.println("the random value is: ");
  // Serial.println(msg);

  if (msg == "1"){
    moveServoCan(myservo_base);
    delay(2000);
    open_close_door(myservo_door);
  }

  if (msg == "2"){
    moveServoBottle(myservo_base);
    delay(2000);
    open_close_door(myservo_door);
  }
}

void moveServoCan(Servo myservo_base){
  moveServo(myservo_base, 0);
}

void moveServoBottle(Servo myservo_base){
  moveServo(myservo_base, 180);
}

void open_close_door(Servo myservo_door){
  // open door
  moveServo(myservo_door, 180);
  
  // close door
  moveServo(myservo_door, 0);

}

void moveServo(Servo myservo, int newpos){
  int pos = myservo.read();

  if (pos < newpos){
    for (pos; pos <= newpos; pos += 1)
    {
      myservo.write(pos);
      delay(5);      
    }
  }
  else if (pos > newpos) {
    for (pos; pos >= newpos; pos -= 1)
    {
      myservo.write(pos);
      delay(5);      
    }
  }
}