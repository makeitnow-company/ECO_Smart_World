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
}

void loop() {
  // (2) 층별 LED제어 하기
  // ******************************************
  floorLedControl();
  // ******************************************
  doorControl();
  delay(500);
}

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