#include <LiquidCrystal.h>
#include<Stepper.h>
//define steps for stepper motor
#define STEPS 32
//define pins
#define echoPin A2
#define trigPin A1
#define LEDPin 7
#define IN1  11
#define IN2  10
#define IN3   9
#define  IN4   8
#define  joystick  A0
#define buzzerPin 2 
// initialize libraries
Stepper stepper(STEPS, IN4, IN2,  IN3, IN1);
LiquidCrystal lcd(4, 6, 3, 5, 12, 13);
//define variables 
long duration;
long dist = 0;
long angle = 0;
long dir = 1;
int minRange = 5;
int maxRange = 200;
void setup() {
  lcd.begin(16,2); // start display
  lcd.print("Distance: "); //Ensure distance is always printed and is constant
  //declare pinMode for all pins that need it
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

}

void loop() {
  delay(333); //set delay of display displaying information to make it readable
  //get distance and set off alarm if closer than 10cm
  int distance = GetDistance();
  if (distance <= 20 && distance >= 15) {
    tone(buzzerPin, 2000, 100);
  }
  else if (distance < 15 && distance >= 12) {
    tone(buzzerPin, 3000, 75);
    tone(buzzerPin, 3000, 75);
  }
  else if (distance < 12 && distance != -1) {
    tone(buzzerPin, 4000, 50);
    tone(buzzerPin, 4000, 50);
  }
  else if (distance == -1) {
    tone(buzzerPin, 1000, 100);
    tone(buzzerPin, 1000, 100);
    tone(buzzerPin, 1000, 100);
  }
  else {
    noTone(buzzerPin);
  }
  //display distance on LCD screen
  lcd.print(distance);
  lcd.print(" cm ");
  //read joystick input and output power to motor accordingly
  int val = analogRead(joystick);
  if(  (val > 500)  && (val < 523) )
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2,  LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
 
  else
  {
    //move motor in "forward" direction
    while (val  >= 523)
    {
      int speed_  = map(val, 523, 1023, 5, 500);
      stepper.setSpeed(speed_);  
      stepper.step(1);
      val  = analogRead(joystick);
    }
    // move motor in "bacckward" direction
    while (val <= 500)
    {
      int speed_ = map(val, 500, 0, 5, 500);
      stepper.setSpeed(speed_);
      stepper.step(-1);
      val  = analogRead(joystick);
    }
 
  }
}

//function to get raw input data from ultrasonic sensor and convert it into centimeters
int GetDistance() {
  lcd.setCursor(0,1);
  //reset ultrasonic sensor measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); //measure distance
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  dist = duration / 58.2; //calculate distance
  //base case if distance measured is less than minimum range sensor supports measuring
  if (dist >= maxRange || dist <= minRange) {
    return -1;
  }
  else {
    return dist;
  }
}