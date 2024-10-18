#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 25    
DHT dht(DHTPIN, DHTTYPE);

#define MOTOR_A1 2 //FAN
#define MOTOR_A2 5 //FAN

// 토양수분관리(2) 추가코드
//**********************************
#define MOTOR_B1 14 //WATER PUMP
#define MOTOR_B2 15 //WATER PUMP
#define SOIL 35
int soil_value;
bool pump_state;
//********************************** 


int h, t;
bool fan_state;

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);

  dht.begin();

  // 토양수분관리(2) 추가코드
  //********************************** 
  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
  //**********************************
}


void loop() {
  dhtControl();
  fanControl();

  // 토양수분관리(2) 추가코드
  //********************************** 
  soilControl();
  pumpContorl();
  //**********************************

  delay(2000); // 2초 딜레이
}


// 토양수분관리(2) 추가코드
//********************************** 
void soilControl(){
  soil_value = map(analogRead(SOIL), 3600, 1200, 0, 100);
  if(soil_value < 20){
    pump_state = true;
  }else{
    pump_state = false;
  }
}

void pumpContorl(){
  if(pump_state){
    water_pump_on();
  }else{
    water_pump_stop();
  }
}
//**********************************


void dhtControl(){
  h = dht.readHumidity();
  t = dht.readTemperature();
  Serial.print("H : ");
  Serial.print(h);
  Serial.print(" % , ");
  Serial.print("T : ");
  Serial.print(t);
  Serial.println(" °C");
  if(h > 70){
    fan_state = true;
  }else{
    fan_state = false;
  }
}

void fanControl(){  
  if(fan_state){
    fan_on();
  }else{
    fan_off();
  }
}

void fan_on(){
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
}

void fan_off(){
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
}

// 토양수분관리(2) 추가코드
//********************************** 
void water_pump_on(){
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}

void water_pump_stop(){
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);
}
//********************************** 