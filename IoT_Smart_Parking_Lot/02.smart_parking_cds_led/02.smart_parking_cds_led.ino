#include <ESP32Servo.h>
#define SERVO_A 12
Servo servo;

int parking_charge = 1000;
int income = 0;
unsigned long previousTime = 0;

#define BUTTON 23
#define PROXSENSOR 39

// (2) 주차유도시스템 추가
// ***********************************
#define CDS_1F 32
#define CDS_2F 33
#define LED_1F 25
#define LED_2F 26
bool state_1f, state_2f;
// ***********************************


void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_A);
  pinMode(BUTTON, INPUT);
  Serial.println("START PARKING SYSTEM");

// (2) 주차유도시스템 추가
// ***********************************
  pinMode(LED_1F, OUTPUT);
  pinMode(LED_2F, OUTPUT);
// ***********************************

}

void loop() {
  entryControl();
  paymentControl();
  gateControl();

// (2) 주차유도시스템 추가
// ***********************************
  parkingControlSystem();
// ***********************************

  delay(500);
}

// (2) 주차유도시스템 추가
// ***********************************
void parkingControlSystem(){
  int cds_1f_value = analogRead(CDS_1F);
  int cds_2f_value = analogRead(CDS_2F);

  if(cds_1f_value < 200){
    state_1f = true;
  }else{
    state_1f = false;
  }

  if(cds_2f_value < 200){
    state_2f = true;
  }else{
    state_2f = false;
  }

  digitalWrite(LED_1F, state_1f);
  digitalWrite(LED_2F, state_2f);
}
// ***********************************

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