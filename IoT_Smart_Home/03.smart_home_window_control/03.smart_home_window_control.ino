// IOT 기능 추가(4) 추가코드
//**********************************
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL64zbvSq8Y"
#define BLYNK_TEMPLATE_NAME "SMARTHOME 2F"
#define BLYNK_AUTH_TOKEN "oHUaQBdnY57_DCqGF3i-shD9W431QLdk"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "KT_GiGA_2G_makeitnow";  // 5G는 지원이 안됩니다.
char pass[] = "makeitnow1";

BlynkTimer timer;

bool securityMode;
//**********************************

#include <ESP32Servo.h>
Servo myservo;
#define SERVO_A 12
#define BUZZER 26
#define BUTTON 23
bool door_state;

// (2) 층별 LED제어 하기
// ******************************************
#define LED_PIN_1F 4
#define LED_PIN_2F 25
#define CDS 39
bool led_state_1f, led_state_2f;
// ******************************************

// (3) 출입감지 시스템
// ******************************************
#define PHOTOSENSOR 27
// ******************************************

// IOT 기능 추가(4) 추가코드
//**********************************
BLYNK_WRITE(V0){
  led_state_1f = param.asInt();
}
BLYNK_WRITE(V1){
  led_state_2f = param.asInt();
}
BLYNK_WRITE(V2){
  securityMode = param.asInt();
}
BLYNK_WRITE(V3){
  door_state = param.asInt();
}
//**********************************

void setup() {
  Serial.begin(9600);
	myservo.attach(SERVO_A);
  delay(100);
  myservo.write(0);
  pinMode(BUTTON, INPUT);
  pinMode(BUZZER, OUTPUT);

  // (2) 층별 LED제어 하기
  // ******************************************
  pinMode(LED_PIN_1F, OUTPUT);
  pinMode(LED_PIN_2F, OUTPUT);
  // ******************************************

  // (3) 출입감지 시스템
  // ******************************************
  pinMode(PHOTOSENSOR, INPUT);
  // ******************************************

    // IOT 기능 추가(4) 추가코드
  //**********************************
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(500L, doorControl);
  timer.setInterval(500L, floorLedControl);
  timer.setInterval(500L, securitySystem);
  //**********************************
}

void loop() {
  // IOT 기능 추가(4) 추가코드
  //**********************************
  Blynk.run();
  timer.run();
  //**********************************
}

// (3) 출입감지 시스템
// ******************************************
void securitySystem(){

  // IOT 기능 추가(4) 추가코드
  //**********************************
  if(securityMode){
    if(!digitalRead(PHOTOSENSOR)){
      playSiren();
      Blynk.virtualWrite(V4, "창문열림감지!!");
    }else{
      Blynk.virtualWrite(V4, "보안모드");
    }
  }else{
    Blynk.virtualWrite(V4, "일반모드");
  }
  //**********************************

}

void playSiren(){
  for(int i=0; i<5; i++){
    tone(BUZZER, 784, 200);
    delay(200);
    tone(BUZZER, 1047, 200);
    delay(200);
  }
  noTone(BUZZER); // 벨 소리 끝내기
}
// ******************************************

// (2) 층별 LED제어 하기
// ******************************************
void floorLedControl(){
  if(analogRead(CDS) < 200 && night==TRUE){
    led_state_1f = true;
    led_state_2f = true;
    night = false;
  }

  if(analogRead(CDS) >= 200 && night==FALSE){
    led_state_1f = false;
    led_state_2f = false;
    night = true;
  }

  
  if(led_state_1f){
    digitalWrite(LED_PIN_1F, HIGH);
  }else{
    digitalWrite(LED_PIN_1F, LOW);
  }

  if(led_state_2f){
    digitalWrite(LED_PIN_2F, HIGH);
  }else{
    digitalWrite(LED_PIN_2F, LOW);
  }
}
// ******************************************


void doorControl(){
  if(digitalRead(BUTTON)){
    playDoorBell();
    delay(300);
    door_state = true;
  }

  if(door_state){
    myservo.write(90);
    delay(3000);
    door_state = false;
  }else{
    myservo.write(0);
  }
}

void playDoorBell() {
  int melody[] = { 262, 330, 392, 523 }; // 도, 미, 솔, 높은 도
  int noteDuration = 300; // 각 음의 지속 시간

  for (int thisNote = 0; thisNote < 4; thisNote++) {
    tone(BUZZER, melody[thisNote], noteDuration);
    delay(noteDuration);  // 음이 끝난 후 약간의 지연을 추가
  }
  noTone(BUZZER); // 벨 소리 끝내기
}

