float left, right, cds;  // 왼쪽, 오른쪽 라인 센서의 기준값 저장 변수

void setup() {
  Serial.begin(9600);
}

void loop() {
  left = analogRead(32);  // 왼쪽 라인 센서 값 읽기
  right = analogRead(33);  // 오른쪽 라인 센서 값 읽기
  cds = analogRead(34);
  Serial.print("left : ");
  Serial.println(left);
  Serial.print("right : ");
  Serial.println(right);
  Serial.print("cds : ");
  Serial.println(cds);
  delay(500);
}
