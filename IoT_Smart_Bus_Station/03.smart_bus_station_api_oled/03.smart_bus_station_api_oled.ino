#include <Ticker.h>// 타이머인터럽트 헤더파일

//신호등 핀번호 설정(1)
//**********************************
#define RED_LED 4
#define YELLOW_LED 25
#define GREEN_LED 26

Ticker ticker; // Ticker 생성자
Ticker ticker2;

int traffic_timer = 0; // 신호등타이머 카운트 변수
bool yellow_state = false; // 노란LED 상태변수
//**********************************

//CDS 및 네오픽셀 설정(2)추가코드
//**********************************
#include <Adafruit_NeoPixel.h>//네오픽셀 헤더파일
#define PIN          27
#define NUMPIXELS    1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define CDS 39
//**********************************

// WiFi 및 API 설정(3)
//**********************************
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi 설정
char* ssid = "KT_GiGA_2G_makeitnow";
char* password = "makeitnow1";

// API URL
String serviceKey = "Ix2k8uPptp%2B2wxT6OAoD%2FAFXinE8TJ00ZL%2BlbW3tsaum6domZHCRrJZS1LvxGqta%2F2vvpPhg1xFCpl30UicBxw%3D%3D";
String cityCode = "31020";
String nodeId = "GGB206000532";
String basicUrl = "http://apis.data.go.kr/1613000/ArvlInfoInqireService/getSttnAcctoArvlPrearngeInfoList";

// 버스번호와 시간을 저장할 구조체
struct BusInfo {
  String busNumber;
  int arrivalTime;
};

// 최대 10개의 버스를 저장할 수 있는 배열
BusInfo busList[10];
int bus_count = 0;

String bus_routeno;
int bus_arrtime;
//**********************************

// OLED 설정(3)
//**********************************
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int bus_time=0;
int minute=0;
int second=0;

BusInfo printList[10];
int print_count = 0;

bool newData = false;
//**********************************


void setup() {
    
  //신호등 핀을 출력으로 설정(1)
  //**********************************
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  //**********************************

    //네오픽셀 초기화(2)추가코드
  //**********************************
  pixels.begin();
  pixels.clear();
  //**********************************

  //100ms 간격으로 인터럽트 발생
  ticker.attach(0.1, controlTrafficLight);
  ticker2.attach(0.5, controlNeopixel);

  Serial.begin(9600);

  // OLED 설정(3)추가코드
  //**********************************
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(1);
  display.setCursor(0, 0);
  display.println("Connecting...");
  display.display();
  //**********************************

  // WiFi 및 API 설정(3)추가코드
  //**********************************
  connectToWifi();
  //**********************************
}

void loop() {
  getApiData();
  displayBusInfo();
  delay(30000);
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

//네오픽셀 제어 함수(2) 추가코드
//**********************************
void controlNeopixel(){
  int CDS_value = analogRead(CDS);
  if(CDS_value<1200){
    pixels.setPixelColor(0, pixels.Color(70, 70, 70));
    pixels.show();
  }
  else{
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
  }
}
//**********************************

// WiFi 연결 함수(3)
//**********************************
void connectToWifi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
}
//**********************************

// API 데이터 가져오기 함수(3)
//**********************************
void getApiData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient Http; //HTTP 생성자
    //URL생성
    String apiURL = basicUrl + "?serviceKey=" + serviceKey 
                    + "&cityCode=" + cityCode + "&nodeId=" + nodeId + "&_type=json";
    Http.begin(apiURL);
    int httpResponseCode = Http.GET();
    
    if(httpResponseCode == 200) {
      Serial.println("URL Connection");
      String response = Http.getString(); //JSON타입의 원시데이터 저장
      readBusData(response);
      sortBusData();
    } else {
      Serial.println("Error on HTTP request: " + String(httpResponseCode));
    }
    Http.end();
  }
}
//**********************************

// API데이터를 정리하는 함수(3)
//**********************************
void readBusData(String response) {
  DynamicJsonDocument doc(2048);  // 2048 바이트의 동적 메모리 할당
  // JSON 데이터 파싱
  DeserializationError error = deserializeJson(doc, response);
  // 에러발생시 에러처리
  if (error) {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return;
  }

  // JSON 배열에 버스 데이터를 추출
  JsonArray items = doc["response"]["body"]["items"]["item"];
  bus_count = 0;
  // for 문을 이용해 items 배열에 인덱스로 접근
  for (int i = 0; i < items.size(); i++) {
    JsonObject data = items[i];  // items 배열의 i번째 요소 접근
    bus_routeno = String(data["routeno"]);   // 버스 번호
    bus_arrtime = data["arrtime"];   // 도착 시간
    if(bus_routeno.toInt()==0){
      continue;
    }
    // 버스 정보를 배열에 저장
    busList[bus_count].busNumber = bus_routeno;
    busList[bus_count].arrivalTime = bus_arrtime;
    bus_count++;
  }
}
//**********************************

// 도착 시간을 기준으로 버스 데이터를 정렬하는 함수(3)
//**********************************
void sortBusData() {
  // 버블 정렬로 도착 시간을 기준으로 정렬
  for (int i = 0; i < bus_count - 1; i++) {
    for (int j = 0; j < bus_count - i - 1; j++) {
      if (busList[j].arrivalTime > busList[j + 1].arrivalTime) {
        // 두 버스 정보를 교환
        BusInfo temp = busList[j];
        busList[j] = busList[j + 1];
        busList[j + 1] = temp;
      }
    }
  }

  print_count = 0;  // printList에 저장된 버스 수
  for (int i = 0; i < bus_count; i++) {
    newData = false;
    // 이전에 이미 추가된 버스 번호인지 확인
    for (int j = 0; j < print_count; j++) {
      if (busList[i].busNumber == printList[j].busNumber) {
        newData = true;
        break;
      }
    }
    // 중복되지 않은 버스 번호만 printList에 추가
    if (!newData) {
      printList[print_count] = busList[i];
      print_count++;
    }
  }
}
//**********************************

//OLED 출력
//**********************************
void displayBusInfo() {
  display.clearDisplay();
  display.drawRect(1, 1, 127, 14, 1);
  display.setCursor(31,4);
  display.println("BUS STATION");

  int y =20;
  for (int i = 0; i < print_count; i++) {
    bus_time = printList[i].arrivalTime;
    minute = bus_time / 60;
    second = bus_time % 60;

    display.setCursor(1 , y);
    display.println(printList[i].busNumber);
    display.setCursor(60 , y);
    display.println(String(minute) + "m " + String(second) + "s");
    y += 9;
  }
  display.display();
}
//**********************************