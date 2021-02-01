#include "config.h"
//#include <SoftwareSerial.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D4 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire); 
#include <LiquidCrystal.h>
//const int RS = D2, EN = D3, d4 = D5, d5 = D6, d6 = D7, d7 = D1;   
LiquidCrystal lcd(D2, D3, D5, D6, D7, D1);
  #define IO_LOOP_DELAY 10000
  unsigned long lastUpdate;
   #define READ_DELAY 8
   int loplus=3,lominus=1;
int ecg1=A0;
int ecg2=0,pressure=0,g=0,h=0;
const byte interruptPin = D8; // Pin to set interrupt to
int interruptCounter = 0; // counter of interrupt executions
int bpm=0,SEC=0;
int buz=10;
int current_Sec=0,last_Sec=0;
  AdafruitIO_Feed *heart_beat = io.feed("BPM");
  AdafruitIO_Feed *temp = io.feed("TEMP");
  AdafruitIO_Feed *ecg = io.feed("ECG");
void setup() 
 {
  lcd.begin(16, 2);
    pinMode(buz, OUTPUT);
 pinMode(loplus, INPUT); // Setup for leads off detection LO +
 pinMode(lominus, INPUT); // Setup for leads off detection LO -
 Serial.begin(9600);
 Serial.print("ECG BPM AND PRESSURE");
 pinMode(interruptPin, INPUT_PULLUP);
Serial.println("Dallas Temperature IC Control Library Demo"); 
 // Start up the library 
 sensors.begin(); 
attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
  lcd.setCursor(0, 0);
   lcd.print(" Turn on your");
     lcd.setCursor(0, 1);
   lcd.print(" WiFi Hotspot");
   delay(3000);
   while (!Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();
   while (io.status() < AIO_CONNECTED)
  {
    Serial.println(io.statusText());
      lcd.clear();
  lcd.setCursor(0, 0);
   lcd.print(" Connecting... ");
   delay(1000);
}
    lcd.clear();
    lcd.print(" Connected");
    delay(500);
  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  lcd.setCursor(0, 0);
   lcd.print("  IoT CARDIAC  ");
     lcd.setCursor(0, 1);
   lcd.print("   MONITORING   ");
   delay(2000);
 }
void loop()
  {
      io.run();
Serial.print(" Requesting temperatures...");
sensors.requestTemperatures(); // Send the command to get temperature readings 
int T= sensors.getTempCByIndex(0); // Send the command to get temperature readings 
int T1= (T*1.8)+32;
 Serial.println("DONE"); 
  Serial.print("Temperature is: "); 
 Serial.print(sensors.getTempCByIndex(0));
  if((digitalRead(loplus) == 1)||(digitalRead(lominus) == 1)){
    Serial.println('!');
  }
  else{
     Serial.println(analogRead(ecg1));
  }
current_Sec=millis();
SEC =current_Sec-last_Sec;
//Serial.print("Time:");
//Serial.println(SEC);
if(SEC>=25200)
{
last_Sec=millis();
  bpm=interruptCounter;
  Serial.print("BPM=");
  Serial.println(bpm);
  interruptCounter = 0;
  pressure=bpm*1.4;
  if((bpm>120)||(T1>102))
  {
    digitalWrite(buz,HIGH);
    delay(300);
    digitalWrite(buz,LOW);
     delay(300);
  }
  else 
  {
    digitalWrite(buz,LOW);
  }
}
ecg2=analogRead(ecg1);
Serial.print("ECG===");
Serial.println(ecg2);
lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Temperature:");
 lcd.setCursor(12, 0);
 lcd.print(T1);
 lcd.setCursor(15, 0);
 lcd.print("F");
 lcd.setCursor(8, 1);
 lcd.print("BPM: ");
 lcd.setCursor(12, 1);
 lcd.print(bpm);
// delay(20);
      temp->save(T1);
      heart_beat->save(bpm);
      ecg->save(ecg2);
   delay(READ_DELAY * 1000);
  
}
void handleInterrupt() 
{
static unsigned long last_interrupt_time = 0;
unsigned long interrupt_time = millis();
// If interrupts come faster than 100ms, assume it's a bounce and ignore
if (interrupt_time - last_interrupt_time > 100)
{
interruptCounter++;
}
last_interrupt_time = interrupt_time;
//Serial.print("TIME:");
//Serial.println(millis()/1000);
}
