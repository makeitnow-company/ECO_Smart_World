#include <Ticker.h>// 타이머인터럽트 헤더파일

//신호등 핀번호 설정(1)
//**********************************
#define RED_LED 4
#define YELLOW_LED 25
#define GREEN_LED 26

Ticker ticker; // Ticker 생성자

int traffic_timer = 0; // 신호등타이머 카운트 변수
bool yellow_state = false; // 노란LED 상태변수
//**********************************

void setup() {
  //신호등 핀을 출력으로 설정(1)
  //**********************************
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  //100ms 간격으로 인터럽트 발생
  ticker.attach(0.1, controlTrafficLight); 
  //**********************************
}

void loop() {
  delay(1000);
}


//신호등의 움직임을 제어하는 함수(1)
//**********************************
void controlTrafficLight(){
  
  if(traffic_timer>110){
    traffic_timer=0;
  }
  if(traffic_timer<50){
    setTrafficLight(1,0,0);
  }
  else if(traffic_timer<100){
    setTrafficLight(0,0,1);
  }
  else{
    yellow_state = !yellow_state;
    setTrafficLight(0,yellow_state,0);
  }
  traffic_timer++;
}
//**********************************

//신호등의 LED를 제어하는 함수(1)
//**********************************
void setTrafficLight(bool red, bool yellow, bool green){
  digitalWrite(RED_LED, red);
  digitalWrite(YELLOW_LED, yellow);
  digitalWrite(GREEN_LED, green);
}
//**********************************