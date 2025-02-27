#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 25    
DHT dht(DHTPIN, DHTTYPE);

#define MOTOR_A1 16 //FAN
#define MOTOR_A2 17 //FAN

int h, t;
bool fan_state;

void setup() {
  Serial.begin(9600);

  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);

  dht.begin();
}


void loop() {
  dhtControl();
  fanControl();
  delay(2000); // 2초 딜레이
}


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