// IOT 기능 추가(5) 추가코드
//**********************************Z
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6nU0UxUxm"
#define BLYNK_TEMPLATE_NAME "SMART FARM"
#define BLYNK_AUTH_TOKEN "klysyHVCsoXSWdeQyx-_CA5yFpyblSz9"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "KT_GiGA_2G_makeitnow";  // 5G는 지원이 안됩니다.
char pass[] = "makeitnow1";

BlynkTimer timer;

bool blynk_fan_state, blynk_pump_state;
//**********************************


#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 25    
DHT dht(DHTPIN, DHTTYPE);

#define MOTOR_A1 16 //FAN
#define MOTOR_A2 17 //FAN

// 토양수분관리(2) 추가코드
//**********************************
#define MOTOR_B1 18 //WATER PUMP
#define MOTOR_B2 19 //WATER PUMP
#define SOIL 36
int soil_value;
bool pump_state;
//********************************** 


//식물생장관리(3) 추가코드
//**********************************
#include <Adafruit_NeoPixel.h>
#define PIN        26
#define NUMPIXELS  9 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define BUTTON 23
#define CDS 39
bool led_state; 
//**********************************

//디스플레이(4) 추가코드
//**********************************
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//**********************************

int h, t;
bool fan_state;


// IOT 기능 추가(5) 추가코드
//**********************************
BLYNK_WRITE(V2){
  blynk_fan_state = param.asInt();
}
BLYNK_WRITE(V3){
  blynk_pump_state = param.asInt();
}
BLYNK_WRITE(V4){
  led_state = param.asInt();
}
//**********************************


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

  //식물생장관리(3) 추가코드
  //**********************************
  pinMode(BUTTON, INPUT);
  pixels.begin();
  //**********************************

  //디스플레이(4) 추가코드
  //**********************************
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);
  //**********************************


  // IOT 기능 추가(5) 추가코드
  //**********************************
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, dhtControl);
  timer.setInterval(1000L, fanControl);
  timer.setInterval(1000L, soilControl);
  timer.setInterval(1000L, pumpContorl);
  timer.setInterval(1000L, farmLedControl);
  timer.setInterval(1000L, displayControl);
  //**********************************

}


void loop() {
  // IOT 기능 추가(5) 추가코드
  // 기존 코드들을 모두 Timer로 이동
  //*********************************
  Blynk.run();
  timer.run();
  //*********************************

}

//디스플레이(4) 추가코드
//**********************************
void displayControl(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("IoT SMART FARM");
  display.setCursor(0,10);
  display.print("HUMI : ");
  display.print(h);
  display.print(" %");
  display.setCursor(0,20);
  display.print("TEMP : ");
  display.print(t);
  display.print(" C");
  display.setCursor(0,30);
  display.print("SOIL : ");
  display.print(soil_value);
  display.print(" %");
  display.setCursor(0,40);
  display.print("LED : ");
  display.print(led_state ? "ON" : "OFF");
  display.display();
}
//**********************************


//식물생장관리(3) 추가코드
//**********************************
void farmLedControl(){
  if(digitalRead(BUTTON)){
    led_state = !led_state;
    Blynk.virtualWrite(V4,led_state);   // IOT 기능 추가(5) 추가코드 ********
    delay(100);
  }

  if(analogRead(CDS) < 200){
    led_state = true;
  }

  if(led_state){
    for(int i=0; i<NUMPIXELS; i++) {
      if(i%2 == 0){
        pixels.setPixelColor(i, pixels.Color(150, 0, 0));
      }else{
        pixels.setPixelColor(i, pixels.Color(0, 0, 150));
      }
    }
  }else{
    pixels.clear();
  }
  pixels.show();
}
//**********************************


// 토양수분관리(2) 추가코드
//********************************** 
void soilControl(){
  soil_value = map(analogRead(SOIL), 3600, 1200, 0, 100);
  Serial.print("S : ");
  Serial.print(soil_value);
  Serial.println(" %");
  if(soil_value < 20){
    pump_state = true;
  }else{
    pump_state = false;
  }
  Blynk.virtualWrite(V1, soil_value);
}

void pumpContorl(){
  if(pump_state || blynk_pump_state){   // IOT 기능 추가(5) 추가코드 ********
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
  Blynk.virtualWrite(V0, h); // IOT 기능 추가(5) 추가코드 ********
}

void fanControl(){  
  if(fan_state || blynk_fan_state){
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