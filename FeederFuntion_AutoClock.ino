#include "arduino_secrets.h"

//This adds the preexisting libraries
#include <Stepper.h>
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;
char t[32];


//This is the number of steps in one revolution of the motor. The 28BYJ-48 motor has 2048 steps per revolution. Make sure to find the number of steps of your motor.
const int stepsPerRevolution = 2048; //136.6 ideal

// initialize the stepper library on pins:
Stepper myStepper1BOB(stepsPerRevolution, 8, 9, 10, 11);
Stepper myStepper2JEFF(stepsPerRevolution, 2, 3, 4, 5);

// Define variables to store the last feeding timestamp for BOB and JEFF
unsigned long lastFeedTimeBOB = 0;
unsigned long lastFeedTimeJEFF = 0;

// Define variables to count the number of feedings in the past 24 hours
int feedCountBOB = 0;
int feedCountJEFF = 0;

void setup() {
  
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
 rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set the RTC to the date & time this sketch was compiled


  pinMode(6, OUTPUT); //LED BOB
  pinMode(7,OUTPUT); //LED JEFF
  
  myStepper1BOB.setSpeed(4);
  myStepper2JEFF.setSpeed(4);
}

void printCurrentTime() {
  DateTime now = rtc.now();
  Serial.print("Current time: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
}

void feedBOB() {
  Serial.println("Calling Bob...");
  digitalWrite(6,HIGH); //Tell bOb its snak time
  delay(5000);
  digitalWrite(6,LOW);
  
  Serial.println("Feeding Bob...");
  myStepper1BOB.step(138); //Feed bOb
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  Serial.println("Bob had snaks");
  
  lastFeedTimeBOB = millis();
  feedCountBOB++;

}

void feedJEFF() {
  Serial.println("Calling Jeff...");
  digitalWrite(7,HIGH); //Tell Jeff its snak time
  delay(5000);
  digitalWrite(7,LOW);
  
  Serial.println("Feeding Jeff...");
  myStepper2JEFF.step(138); //Feed Jeff
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  Serial.println("Jeff had snaks");
  
  lastFeedTimeJEFF = millis();
  feedCountJEFF++;
}

//With Clock
void loop() {
  
      //Check RTC module
  if (! rtc.begin())
  {
    Serial.println(" RTC Module not Present");
    while (1);
  }
  
  // Check if BOB hasn't been fed in the past 24 hours
  if (millis() - lastFeedTimeBOB >= 24 * 60 * 60 * 1000UL) {
    Serial.println("Bob hasn't eaten in 24h...");
    feedBOB();
    Serial.println("Bob has been fed automatically!");
  }

  // Check if JEFF hasn't been fed in the past 24 hours
  if (millis() - lastFeedTimeJEFF >= 24 * 60 * 60 * 1000UL) {
    Serial.println("Jeff hasn't eaten in 24h...");
    feedJEFF();
    Serial.println("Jeff has been fed automatically!");
  }

  // Handle other commands from the Serial Monitor
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // Read the input until a newline character is encountered

    if (input == "Feed Bob" || input == "Feed Jeff") {
      DateTime now = rtc.now();
      int currentHour = now.hour();

      // Check if the current time is within the feeding window (10h00 to 18h00)
      if (currentHour >= 10 && currentHour < 18) {
        if (input == "Feed Bob") {
          feedBOB();
        } else if (input == "Feed Jeff") {
          feedJEFF();
        }
      } else {
        Serial.println("Oh no! It's past bedtime :( Waiting to feed automatically at 10am tomorrow...");
      }

    } else if (input == "Check Feed Status") {
      // Check how many times BOB has been fed in the past 24 hours
      Serial.print("Bob has been fed ");
      Serial.print(feedCountBOB);
      Serial.print(" times in the past 24 hours.");

      // Check how many times JEFF has been fed in the past 24 hours
      Serial.print("Jeff has been fed ");
      Serial.print(feedCountJEFF);
      Serial.print(" times in the past 24 hours.");

    } else if (input == "Time?") {
      printCurrentTime();

    } else if (input == "Connect") {
      Serial.println("Listening...");
      
    } else {
      Serial.println("I don't understand >:{");
    }
  }

  // Optional delay
  delay(100);
}


