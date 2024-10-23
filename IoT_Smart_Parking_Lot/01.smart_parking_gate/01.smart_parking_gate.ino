#include <ESP32Servo.h>
#define SERVO_A 12
Servo servo;

int parking_charge = 1000;
int income = 0;
unsigned long previousTime = 0;

#define BUTTON 23
#define PROXSENSOR 39

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_A);
  pinMode(BUTTON, INPUT);
  Serial.println("START PARKING SYSTEM");
}

void loop() {
  entryControl();
  paymentControl();
  gateControl();
  delay(500);
}

void entryControl(){
  int prox_value = analogRead(PROXSENSOR);
  if(prox_value > 2500){
    gateOn();
  }
}

void paymentControl(){
  int btn_value = digitalRead(BUTTON);
  if(btn_value){
    gateOn();
    income = income + parking_charge;
    Serial.print("오늘의 수익 : ");
    Serial.print(income);
    Serial.println(" 원");
  }
}

void gateControl(){
  if(millis() - previousTime > 3000){
    gateOff();
  }
}

void gateOn(){
  servo.write(0);
  previousTime = millis();
}

void gateOff(){
  servo.write(90);
}