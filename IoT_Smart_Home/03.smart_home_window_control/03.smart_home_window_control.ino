#include <ESP32Servo.h>
Servo myservo;
#define SERVO_A 5
#define BUZZER 26
#define BUTTON 23
bool door_state;

// (2) 층별 LED제어 하기
// ******************************************
#define LED_PIN_1F 4
#define LED_PIN_2F 25
#define CDS 39
bool led_state_1f, led_state_2f;
bool night = true;
// ******************************************

// (3) 출입감지 시스템
// ******************************************
#define PHOTOSENSOR 27
// ******************************************

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
}

void loop() {
  // (2) 층별 LED제어 하기
  // ******************************************
  floorLedControl();
  // ******************************************

  // (3) 출입감지 시스템
  // ******************************************
  // securitySystem();
  // ******************************************
  
  doorControl();
  delay(500);
}

// (3) 출입감지 시스템
// ******************************************
void securitySystem(){
  if(!digitalRead(PHOTOSENSOR)){
    playSiren();
  }
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
  if(analogRead(CDS) < 200 && night==true){
    led_state_1f = true;
    led_state_2f = true;
    night = false;
  }

  if(analogRead(CDS) >= 200 && night==false){
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
    myservo.attach(SERVO_A);
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