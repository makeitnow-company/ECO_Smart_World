#include <ESP32Servo.h>
Servo myservo;
#define SERVO_A 5
#define BUZZER 26
#define BUTTON 23
bool door_state;

void setup() {
  Serial.begin(9600);

	myservo.attach(SERVO_A);
  delay(100);
  myservo.write(0);

  pinMode(BUTTON, INPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  doorControl();
  delay(500);
}


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