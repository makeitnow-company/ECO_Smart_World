#define TRIG_PIN 23
#define ECHO_PIN 23

int duration;

float black = 0.4;
float left, right;  // 왼쪽, 오른쪽 센서 값

void setup() {
  Serial.begin(9600);  // 시리얼 모니터를 시작합니다.
  delay(2000);  // 최초 실행 시 2초 대기
  line_detection();  // 왼쪽, 오른쪽 값을 한번만 읽어오기
}

void loop() {
  fw_detection();

  if(duration < 300) {
    car_stop();
  }
  else if (analogRead(32) > left * black) {
    car_right();
  }
  else if (analogRead(33) > right * black) {
    car_left();
  }
  else {
    car_go();
  }
  delay(100);
}

void fw_detection() {
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH); // 트리거 핀에서 신호 보내기
  delayMicroseconds(10);  // 10마이크로초간 HIGH로 유지
  digitalWrite(TRIG_PIN, LOW);
  
  pinMode(ECHO_PIN, INPUT);

  duration = pulseIn(ECHO_PIN, HIGH);  // 에코 핀이 HIGH로 변할 때까지의 시간 측정
  
  // 에코 신호의 지속 시간 출력 (단위: 마이크로초)
  Serial.print("duration : ");
  Serial.print(duration);
  Serial.println(" ms");
}


void line_detection() {
  left = analogRead(32);  // 32번 핀에서 왼쪽 센서 값 읽기
  right = analogRead(33);  // 33번 핀에서 오른쪽 센서 값 읽기
  
  Serial.print("Left : ");
  Serial.println(left);
  Serial.print("Right : ");
  Serial.println(right);
}

// (2) 스마트 모빌리티 제어하기
void car_go() {
  analogWrite(16,0);
  analogWrite(17,1023);
  analogWrite(18,1023);
  analogWrite(19,0);
}

void car_stop() {
  analogWrite(16,0);
  analogWrite(17,0);
  analogWrite(18,0);
  analogWrite(19,0);
}

void car_right() {
  analogWrite(16,0);
  analogWrite(17,1023);
  analogWrite(18,0);
  analogWrite(19,0);
}

void car_left() {
  analogWrite(16,0);
  analogWrite(17,0);
  analogWrite(18,1023);
  analogWrite(19,0);
}