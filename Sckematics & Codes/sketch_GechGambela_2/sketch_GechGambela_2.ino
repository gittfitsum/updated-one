//Code for Implementation
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12,11,10,7,6,5);
//declare variables 
#define trigPin 2
#define echoPin 8
float duration, FLevel, OLevel; 
int analogInput = A1;
float vout = 0.0; 
float vin = 0.0; 
float R1 = 10000; 
float R2 = 10000; 
int value = 0;
int LED = 9; // the pin where LED is found
int Relay = 13;// the pin where Relay is found 
int Buzzer=4;// the pin where Speaker is found 
int voltPin=A1;
int FLevelPin = A0;
int tempPin = A2; 
int fan = 3;
int fuelPin = A0; 
int OLevelPin = A4;
float treshold = 4.3; 
String textForSMS; 
char incoming_char = 0; 
int temp;
int val = 0;
int tempMin = 40; 
int tempMax = 100; 
int voltMin = 220; 
int voltMax = 400; 
void setup() {
  
  pinMode(fan, OUTPUT); 
  pinMode(Buzzer, OUTPUT);
  pinMode(Relay, OUTPUT); 
  pinMode(LED, OUTPUT); 
  lcd.begin(16, 2); 
  Serial.begin(9600); 
  SIM900.begin(9600); 
  analogReference(DEFAULT);
  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A4, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
void loop() {
  //For fuel level sensor 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(5); 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  duration = pulseIn(echoPin, HIGH);
  FLevel = (duration / 2) * 0.343;
  Serial.print("FLevel=");// Flevel is indicated that Fuel Level
  Serial.print(FLevel);
  Serial.println("cm");
  delay(500); //lcd display
  lcd.setCursor(0, 1);
  lcd.print("FLevel= "); 
  lcd.print(FLevel, 1);
  lcd.print("cm"); delay(500); 
  if (FLevel > 40) {
    Serial.println("FUEL IS LOW ");
    digitalWrite(4, HIGH); 
    digitalWrite(9, HIGH); 
    delay(500);
  }
  else {
    digitalWrite(4, LOW);
    digitalWrite(9, LOW);
    delay(500);
    Serial.println("FUEL IS NORMAL");
    delay(500);
  }
  //For Oil sensor
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(5); 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(5);
  duration = pulseIn(echoPin, HIGH);
  OLevel = (duration / 2) * 0.343;
  Serial.print("OLevel=");// Olevel is indicated that Oil level
  Serial.print(OLevel, 1);
  Serial.println("cm");
  delay(500); //lcd display
  lcd.setCursor(0, 1);
  lcd.print("FLevel= "); 
  lcd.print(FLevel, 1); 
  lcd.print("cm"); 
  delay(500); 
  if (FLevel > 40) {
    Serial.println("FUEL IS LOW ");
    digitalWrite(4, HIGH); 
    digitalWrite(9, HIGH); 
    delay(500);
  }
  else {
    digitalWrite(4, LOW);
    digitalWrite(9, LOW);
    delay(500);
    Serial.println("FUEL IS NORMAL"); 
    delay(500);
  }
  //For Temperature sensor 
  temp = analogRead(tempPin); 
  temp = temp * 0.48828125/10;
  Serial.print("T = "); 
  Serial.print(temp); 
  Serial.print("C"); 
  Serial.println();
  delay(1000); //lcd display
  lcd.setCursor(0, 0); 
  lcd.print("T= "); 
  lcd.print(temp, 1); 
  lcd.print("C");
  lcd.clear();
  delay(5);
  if ((temp > tempMin)) { // if temperature is higher than minimum temp 
    Serial.println("TEMPERATURE IS HIGH ");
    digitalWrite(4, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(fan, HIGH);
    delay(500);
  }
  else
  {
    digitalWrite(4, LOW);
    digitalWrite(9, LOW); 
    delay(5);
    Serial.println("TEMPERATURE IS NORMAL");
    delay(5);
  }
  //For Voltage sensor
  value = analogRead(analogInput); 
  vout = (value * 5.0) / 1023;
  vin = (vout * 10) / (R2 / (R1 + R2)); //by voltage division rule
  Serial.print("vout=");
  Serial.print((vin));
  Serial.print("V");
  Serial.println();
  delay(1000); 
  lcd.setCursor(0, 0);
  lcd.print("vout="); 
  lcd.print((vin)); 
  lcd.print("V"); 
  delay(500);
  if ((60 < vout && vout < 100)) {
    delay(500); 
    Serial.println("Normal voltage");
    delay(500);
    digitalWrite(Relay, HIGH);
    delay(500);
  }
  else {
    digitalWrite(Relay, LOW);
    digitalWrite(9, HIGH); 
  delay(500);
    Serial.println ("Unnormal voltage");
    delay(500);
    Serial.println("FAULT IS HAPPEN");
    delay(500);
  }
  //For GSM communication 
  Serial.println("AT+CMGF=1\r"); 
  delay(1000);
  Serial.println("AT+CMGS=\"+251916186459\"\r");
  delay(1000);
  if ((temp > tempMin)) { // if temperature is higher than minimum temp 
    Serial.println("TEMPERATURE IS HIGH ");
  }
  if (FLevel > 40) {
    Serial.println("FUEL IS LOW ");
    if ((220 < vout && vout < 400)) {
    Serial.println ("Unnormal voltage");
    delay(1000);
    Serial.println (char(26));//the ASCII code of the ctrl+z is 26
    }
  }
}
