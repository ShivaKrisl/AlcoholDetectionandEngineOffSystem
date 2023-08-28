#include<SoftwareSerial.h>
#include<TinyGPS.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(13,A1,A2,A3,A4,A5);
float gpslat, gpslon;
TinyGPS gps;
SoftwareSerial sgps(2, 3);//gps rx=2, tx=3
SoftwareSerial sgsm(10, 11);//gsm rx=10,tx=11
void setup()
{
 Serial.begin(9600);
 sgsm.begin(9600);
 sgps.begin(9600);
 lcd.begin(16,2);
 pinMode(A0,INPUT);//mq2 sensor
 pinMode(4,OUTPUT);//MOTOR 1
 pinMode(5,OUTPUT);//MOTOR 2
 pinMode(7,OUTPUT);//MOTOR 3
 pinMode(8,OUTPUT);//MOTOR 4
 pinMode(12,OUTPUT);////BUZZER
}
void loop()
{
int alcohol=analogRead(A0);
Serial.println(alcohol);
sgps.listen();
while(sgps.available())
 {
 int c=sgps.read();
 if(gps.encode(c))
 {
 gps.f_get_position(&gpslat,&gpslon);
 }
 }
if(alcohol>200)
 {
 sgsm.listen();
 sgsm.print("\r");
 delay(1000);
 sgsm.print("AT+CMGF=1\r");
 delay(1000);
 sgsm.print("AT+CMGS=\"+919398325300\"\r");
 delay(1000);
28
 sgsm.print("https://www.google.com/maps/?q=");
 sgsm.print(gpslat, 6);
 sgsm.print(",");
 sgsm.print(gpslon, 6);
 delay(1000);
 sgsm.write(0x1A);
 delay(1000);
 lcd.setCursor(0,0);
 lcd.print("alcoholtaken");
 digitalWrite(12,HIGH);//BUZZER ON
 lcd.setCursor(0,1);
 lcd.print("sending sms");
 digitalWrite(4,LOW);//MOTOR 1 OFF
 digitalWrite(5,LOW);//MOTOR 2 OFF
 digitalWrite(7,LOW);//MOTOR 3 OFF
 digitalWrite(8,LOW);//MOTOR 4 OFF
 delay(1000);
 }
 else
 {
 digitalWrite(12,LOW);//BUZZER OFF
 lcd.setCursor(0,0);
 lcd.print("alcoholnottaken");
 lcd.setCursor(0,1);
 lcd.print("engine running");
 digitalWrite(4,HIGH);//MOTOR 1 ON
 digitalWrite(5,HIGH);//MOTOR 2 ON
 digitalWrite(7,HIGH);//MOTOR 3 ON
 digitalWrite(8,HIGH);//MOTOR 4 ON
 }
 delay(1000);
}
