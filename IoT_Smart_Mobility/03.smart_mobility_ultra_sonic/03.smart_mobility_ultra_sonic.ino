// 초음파 센서 핀 정의
#define TRIG_PIN 23  // 초음파 센서의 트리거 핀
#define ECHO_PIN 23  // 초음파 센서의 에코 핀

int duration;  // 초음파 센서로부터 측정된 거리 값 저장 변수
int lightValue;  // 조도센서 값 저장 변수

float black = 0.3;  // 라인 감지 임계값 (검은색 라인 감지 기준)
float left, right;  // 왼쪽, 오른쪽 라인 센서의 기준값 저장 변수

void setup() {
  Serial.begin(9600);  // 시리얼 통신 시작 (9600 baud rate)
  delay(2000);  // 초기화를 위한 2초 대기
  line_detection();  // 라인 센서의 기준값 측정
}

void loop() {
  fw_detection();  // 전방 장애물 감지
  lightValue = analogRead(34);  // 조도센서 값 읽기

  // 조도센서 값이 3000을 초과하면 정지
  if(lightValue > 2500) {  /// 횡단보도 신호등 빨 > 바닥에 불(led) // 어두우면 잘 가다가(?), 불이 들어면 멈춰(?)
    car_stop();
  }
  // 장애물이 300ms 이내에 감지되면 정지
  else if(duration < 300) {
    car_stop();
  }
  // 왼쪽 라인 센서가 검은색 라인을 감지하면 우회전
  else if (analogRead(32) > left * black) {
    car_right();
  }
  // 오른쪽 라인 센서가 검은색 라인을 감지하면 좌회전
  else if (analogRead(33) > right * black) {
    car_left();
  }
  // 그 외의 경우 직진
  else {
    car_go();
  }
  delay(100);  // 100ms 대기
}

// 전방 장애물 감지 함수
void fw_detection() {
  pinMode(TRIG_PIN, OUTPUT);  // 트리거 핀을 출력으로 설정
  digitalWrite(TRIG_PIN, LOW);  // 트리거 핀 LOW로 초기화
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);  // 초음파 발사
  delayMicroseconds(10);  // 10마이크로초 동안 HIGH 유지
  digitalWrite(TRIG_PIN, LOW);  // 트리거 핀 LOW로 설정
  
  pinMode(ECHO_PIN, INPUT);  // 에코 핀을 입력으로 설정

  duration = pulseIn(ECHO_PIN, HIGH);  // 에코 핀의 HIGH 신호 지속 시간 측정
  
  // 측정된 거리 값 시리얼 모니터로 출력
  Serial.print("duration : ");
  Serial.print(duration);
  Serial.println(" ms");
}

// 라인 센서 기준값 측정 함수
void line_detection() {
  left = analogRead(32);  // 왼쪽 라인 센서 값 읽기
  right = analogRead(33);  // 오른쪽 라인 센서 값 읽기
}

// 차량 제어 함수들
// 직진 함수
void car_go() {
  analogWrite(16,0);     // 왼쪽 모터 정방향
  analogWrite(17,500);  // 왼쪽 모터 정방향
  analogWrite(18,500);  // 오른쪽 모터 정방향
  analogWrite(19,0);     // 오른쪽 모터 정방향
}

// 정지 함수
void car_stop() {
  analogWrite(16,0);  // 모든 모터 정지
  analogWrite(17,0);
  analogWrite(18,0);
  analogWrite(19,0);
}

// 우회전 함수
void car_right() {
  analogWrite(16,0);     // 왼쪽 모터 정방향
  analogWrite(17,500);  // 왼쪽 모터 정방향
  analogWrite(18,0);     // 오른쪽 모터 정지
  analogWrite(19,0);     // 오른쪽 모터 정지
}

// 좌회전 함수
void car_left() {
  analogWrite(16,0);     // 왼쪽 모터 정지
  analogWrite(17,0);     // 왼쪽 모터 정지
  analogWrite(18,500);  // 오른쪽 모터 정방향
  analogWrite(19,0);     // 오른쪽 모터 정방향
}