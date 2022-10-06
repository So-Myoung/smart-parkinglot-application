//lcd 제어
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

//블루투스 통신 제어
#include <SoftwareSerial.h>
int RX=12;
int TX=13;
SoftwareSerial bluetooth(RX,TX);

//주차장 초음파 센서
#define echoPin1 4
#define trigPin1 5
#define echoPin2 8
#define trigPin2 9
#define echoPin3 6
#define trigPin3 7

long duration1, distance1;
long duration2, distance2;
long duration3, distance3;

int all_count=0;
int disabled_count=0;
int general_count=0;

// allSlot = 전체 잔여자리, generalSlot = 일반 잔여자리, disabledSlot = 장애인주차구역 잔여자리
int allSlot = 0;
int generalSlot = 0;
int disabledSlot = 0;

void setup() { 
Serial.begin (9600);
bluetooth.begin(9600);

//주차장 센서 설정
pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);
pinMode(trigPin3, OUTPUT);
pinMode(echoPin3, INPUT);

//lcd 초기 설정
lcd.init();
lcd.backlight();
}

void loop() {

//블루투스 통신 모듈
if(bluetooth.available()){
  Serial.write(bluetooth.read());
}
if (Serial.available()){
  bluetooth.write(Serial.read());
}

//주차장 초음파 센서 제어
digitalWrite(trigPin1, LOW); 
delayMicroseconds(2); 
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10); 
digitalWrite(trigPin1, LOW);
duration1 = pulseIn(echoPin1, HIGH);

digitalWrite(trigPin2, LOW);
delayMicroseconds(2); 
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10); 
digitalWrite(trigPin2, LOW);
duration2 = pulseIn(echoPin2, HIGH);
 
digitalWrite(trigPin3, LOW);
delayMicroseconds(2); 
digitalWrite(trigPin3, HIGH);
delayMicroseconds(10); 
digitalWrite(trigPin3, LOW);
duration3 = pulseIn(echoPin3, HIGH);

// 거리 계산
distance1 = duration1 / 58.2;
if(distance1 < 10)
  distance1 = 1;  
else distance1 = 0;
 
distance2 = duration2 / 58.2;
if(distance2 < 10)
  distance2 = 1;
else distance2 = 0;
 
distance3 = duration3 / 58.2;
if(distance3 < 10)
  distance3 = 1;
else distance3 = 0;

all_count = distance1 + distance2 + distance3;
general_count = distance1 + distance2;
disabled_count = distance3;

// allSlot(전체 잔여자리) = total slot - total car
allSlot = 3 - all_count;

// disabledSlot(장애인주차구역 잔여자리)
disabledSlot = 1 - disabled_count;

// generalSlot(일반 잔여자리)
generalSlot = 2 - general_count;

//아두이노 시리얼 모니터에 잔여자리 표시
Serial.print("전체 잔여:");
Serial.print(allSlot);
Serial.println("자리");

Serial.print("일반 잔여:");
Serial.print(generalSlot);
Serial.println("자리");

Serial.print("장애인 주차 구역 잔여:");
Serial.print(disabledSlot);
Serial.println("자리");

if(distance1==1){Serial.println("1번 자리:주차 중 ");}
     else{Serial.println("1번 자리:비어있음");}

if(distance2==1){Serial.println("2번 자리:주차 중 ");}
     else{Serial.println("2번 자리:비어있음");}

if(distance3==1){Serial.println("3번 자리:주차 중 (*장애인 주차 구역)");}
     else{Serial.println("3번 자리:비어있음 (*장애인 주차 구역)");}
Serial.println("");

//lcd로 주차 자리 표시
lcd.begin(20, 4);
lcd.setCursor (0,0);
lcd.print("Outdoor Parking Lot");
lcd.setCursor (0,1);
lcd.print("   All Slot: "); 
lcd.print(allSlot);
lcd.setCursor (0,2);
lcd.print("   General Slot: "); 
lcd.print(generalSlot);
lcd.setCursor (0,3);
lcd.print("   Disabled Slot: "); 
lcd.print(disabledSlot);

//블루투스 통신으로 앱에 주차 자리 표시
bluetooth.print("전체 잔여 자리: ");
bluetooth.println(allSlot);
bluetooth.println("");

bluetooth.print("일반 잔여 자리: ");
bluetooth.println(generalSlot);
bluetooth.println("");

bluetooth.print("장애인 주차 구역 잔여 자리: ");
bluetooth.println(disabledSlot);
bluetooth.println("");
bluetooth.println("");

if(distance1==1){bluetooth.println("1번 자리:주차 중 ");}
     else{bluetooth.println("1번 자리:비어있음");}
bluetooth.println("");

if(distance2==1){bluetooth.println("2번 자리:주차 중 ");}
     else{bluetooth.println("2번 자리:비어있음");}
bluetooth.println("");

if(distance3==1){bluetooth.println("3번 자리:주차 중 (*장애인 주차 구역)");}
     else{bluetooth.println("3번 자리:비어있음 (*장애인 주차 구역)");}
bluetooth.println("");
     
//지연 및 초기화
delay(5000);
allSlot = 0;
disabledSlot = 0;
generalSlot = 0;
distance1 = 0;
distance2 = 0;
distance3 = 0;
}