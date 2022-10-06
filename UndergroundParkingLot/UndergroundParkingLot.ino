//서보모터, lcd제어
#include <Servo.h>
Servo myservo;
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

//차단바 적외선 센서 설정
#define ir_enter 2
#define ir_back  4

//주차장 적외선 센서 설정
#define ir_car1 5
#define ir_car2 6
#define ir_car3 7
#define ir_car4 8
#define ir_car5 9
#define ir_car6 10

//블루투스 통신 모듈 설정
#include <SoftwareSerial.h>
int RX=12;
int TX=13;
SoftwareSerial bluetooth(TX,RX);

//초기화
int S1=0, S2=0, S3=0, S4=0, S5=0, S6=0;
int flag1=0, flag2=0; 
int slot = 0;
int gslot = 0;
int dslot = 0; 
int eslot = 0;

void setup(){
Serial.begin(9600);
bluetooth.begin(9600);

pinMode(ir_car1, INPUT);
pinMode(ir_car2, INPUT);
pinMode(ir_car3, INPUT);
pinMode(ir_car4, INPUT);
pinMode(ir_car5, INPUT);
pinMode(ir_car6, INPUT);

pinMode(ir_enter, INPUT);
pinMode(ir_back, INPUT);

myservo.attach(3);
myservo.write(90);  

lcd.init();
lcd.backlight();

Read_Sensor();

}

void loop(){

//블루투스 통신 설정
if(bluetooth.available()){
  Serial.write(bluetooth.read());
}
if (Serial.available()){
  bluetooth.write(Serial.read());
}

//잔여자리 설정(slot:전체 잔여, gslot:일반 잔여, dslot: 장애인 주차 구역 잔여, eslot: 전기자 주차 구역 잔여)
int total = S1+S2+S3+S4+S5+S6;
slot = 6 - total;
gslot = 4 -(S3+S4+S5+S6);
dslot = 1 - S2;
eslot = 1 - S1;

Read_Sensor();

//서보모터 제어
if(digitalRead (ir_enter) == 0 && flag1==0){
if(slot>0){flag1=1;
if(flag2==0){myservo.write(180); slot = slot-1;}
}else{
lcd.setCursor (0,0);
lcd.print("No parking");  
delay(1500);
}   
}

if(digitalRead (ir_back) == 0 && flag2==0){flag2=1;
if(flag1==0){myservo.write(180); slot = slot+1;}
}

if(flag1==1 && flag2==1){
delay (1500);
myservo.write(90);
flag1=0, flag2=0;
}

//lcd에 잔여 좌석 표시
lcd.setCursor (0,0);
lcd.print("All Slot: "); 
lcd.print(slot);
lcd.print("");  

lcd.setCursor (0,1);
lcd.print("General Slot: ");
lcd.print(gslot);

lcd.setCursor (0,2);
lcd.print("Disabled Slot: ");
lcd.print(dslot);

lcd.setCursor (0,3);
lcd.print("Electric Slot");
lcd.print(eslot);
lcd.clear();

bluetooth.print("전체 잔여 자리: ");
bluetooth.println(slot);
bluetooth.println("");

bluetooth.print("일반 잔여 자리: ");
bluetooth.println(gslot);
bluetooth.println("");

bluetooth.print("장애인 잔여 자리: ");
bluetooth.println(dslot);
bluetooth.println("");

bluetooth.print("전기차 잔여 자리: ");
bluetooth.println(eslot);
bluetooth.println("");
bluetooth.println("");

if(S1==1){bluetooth.println("1번 자리:주차 중 (*전기차 주차 구역)");}
     else{bluetooth.println("1번 자리:비어있음 (*전기차 주차 구역)");}
bluetooth.println("");

if(S2==1){bluetooth.println("2번 자리:주차 중 (*장애인 주차 구역)");}
     else{bluetooth.println("2번 자리:비어있음 (*장애인 주차 구역)");}
bluetooth.println("");

if(S3==1){bluetooth.println("3번 자리:주차 중");}
     else{bluetooth.println("3번 자리:비어있음");}
bluetooth.println("");

if(S4==1){bluetooth.println("4번 자리:주차 중 ");}
     else{bluetooth.println("4번 자리:비어있음");}
bluetooth.println("");

if(S5==1){bluetooth.println("5번 자리:주차 중 ");}
     else{bluetooth.println("5번 자리:비어있음");}
bluetooth.println("");

if(S6==1){bluetooth.println("6번 자리:주차 중");}
     else{bluetooth.println("6번 자리:비어있음");}
bluetooth.println("");
bluetooth.println("");
bluetooth.println("");

delay(3000);
}

void Read_Sensor(){
S1=0, S2=0, S3=0, S4=0, S5=0, S6=0;

if(digitalRead(ir_car1) == 0){S1=1;}
if(digitalRead(ir_car2) == 0){S2=1;}
if(digitalRead(ir_car3) == 0){S3=1;}
if(digitalRead(ir_car4) == 0){S4=1;}
if(digitalRead(ir_car5) == 0){S5=1;}
if(digitalRead(ir_car6) == 0){S6=1;}  
}