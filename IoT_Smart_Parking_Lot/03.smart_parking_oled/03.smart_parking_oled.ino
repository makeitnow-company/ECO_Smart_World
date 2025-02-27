#include <ESP32Servo.h>
#define SERVO_A 5
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

// (3) 디스플레이
// ***********************************
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
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

// (3) 디스플레이
// ***********************************
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.clearDisplay();
  display.display();
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

// (3) 디스플레이
// ***********************************
  displayControl();
// ***********************************

  delay(500);
}

// (3) 디스플레이
// ***********************************
void displayControl(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("SMART PARKING");
  display.setCursor(0,15);
  display.print("No 1 : ");
  display.print(state_1f ? "X" : "O");
  display.setCursor(0,30);
  display.print("No 2 : ");
  display.print(state_2f ? "X" : "O");
  display.display();
}
// ***********************************


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