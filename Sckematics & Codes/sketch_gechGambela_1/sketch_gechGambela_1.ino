#include <LiquidCrystal.h> 
#include <SoftwareSerial.h> 
SoftwareSerial SIM900(7, 8);
// initialize the library with the numbers of the interface pins 
LiquidCrystal lcd(12,11,10,7,6,5);
//declare variables 
float fuelOut; 
float OL;
float fuel;
float oil; 
float D;
float VoltOut; 
float tempC;
float Volt; 
float T;
float V;
int LED = 9;
int Relay = 13;// the pin where Relay is found 
int Speaker=4;// the pin where Speaker is found
int fuelPin = A0;
int voltPin = A1; 
int tempPin = A2; 
int oilPin = A4;
int fan = 3; // the pin where fan is found 
int val=0;
float treshold = 4.3; 
String textForSMS; 
char incoming_char = 0; 
int temp;
int tempMin = 40; // the temperature to start the fan
int tempMax = 100; // the maximum temperature when fan is at 100% 
int fanSpeed;
int voltMin = 220; 
int voltMax = 380;
void setup() {
  // set up the LCD's number of columns and rows:
  pinMode(fan, OUTPUT);//Defining the pin 3 of the Arduino as output 
  pinMode(Speaker, OUTPUT);//Defining the pin 8 of the Arduino as output 
  pinMode(Relay, OUTPUT);//Defining the pin 13 of the Arduino as output 
  pinMode(LED, OUTPUT);//Defining the pin 9 of the Arduino as output
  lcd.begin(20, 4); 
  Serial.print("T="); 
  lcd.print("T="); 
  lcd.setCursor(0, 1); 
  Serial.println("F=");
  lcd.print("F="); 
  lcd.setCursor(0, 2); 
  lcd.print("O="); 
  Serial.begin(9600);
  SIM900.begin(9600); 
  analogReference(DEFAULT); 
  pinMode(3, OUTPUT); 
  pinMode(4, OUTPUT); 
  pinMode(9, OUTPUT); 
  pinMode(13, OUTPUT); 
  pinMode(A0, INPUT);
  pinMode(A1, INPUT); 
  pinMode(A2, INPUT); 
  pinMode(A4, INPUT);

}
void loop()
{
  int mytemp [5]; 
  float avrtemp = 0;
  for (int i = 0; i <= 4; i++)
  {
    mytemp [i] = analogRead(tempPin); 
    delay(5);
  }
  avrtemp = (mytemp [0] + mytemp [1] + mytemp [2] + mytemp [3] + mytemp [4]) / 5; 
  temp = (5 * avrtemp * 100) / 1023;
  lcd.setCursor(0, 0); 
  lcd.print("T= ");
  lcd.print(temp, 1); 
  lcd.print("C"); 
  delay(5);
  if (temp < tempMin) { // if temp is lower than minimum temp 
    digitalWrite(Speaker,LOW);
    fanSpeed = 0; // fan is not spinning 
    digitalWrite(fan, LOW);
    analogWrite(fan, fanSpeed); // spin the fan at the fanS peed 
    Serial.print(" - ");
    delay(5);
    Serial.println("TEMPERATURE IS NORMAL");
    delay(500);
  }
  if (temp >= tempMin) { // if temperature is higher than minimum temp
    digitalWrite(Speaker,HIGH);
    digitalWrite(LED, HIGH);
    fanSpeed = map(temp, tempMin, tempMax, 32, 255); // the actual speed of fan 
    digitalWrite(fan, HIGH);
    analogWrite(fan, fanSpeed); // spin the fan at the fanSpeed  speed 
    Serial.print(" , ");
    delay(5);
    Serial.println("TEMPERATURE IS HIGH "); 
    delay(500);
  }
  T = analogRead(tempPin); //read the value from the sensor 
  V = analogRead(voltPin);
  int myVolt [5];
  float avrVolt = 0;
  for (int i = 0; i <= 4; i++)
  {
    myVolt [i] = analogRead(voltPin); 
    delay(5);
  }
  avrVolt = (myVolt [0] + myVolt [1] + myVolt [2] + myVolt [3] + myVolt [4]) / 5;
  VoltOut = (5 * avrVolt * 80) / 1023;
  lcd.setCursor(0, 3); 
  lcd.print("V= "); 
  lcd.print(VoltOut, 1);
  lcd.print("V"); 
  delay(5);
  int myfuel [5];
  float avrfuel = 0;
  for (int i = 0; i <= 4; i++)
  {
    myfuel [i] = analogRead(fuelPin);
    delay(5);
  }
  avrfuel = (myfuel [0] + myfuel [1] + myfuel [2] + myfuel[3] + myfuel [4]) / 5; 
  fuelOut = (5 * avrfuel * 46) / 1023;
  lcd.setCursor(0, 1);
  lcd.print("F= ");
  lcd.print(fuelOut, 1);
  lcd.print("cm");
  delay(5);
  OL = analogRead(oilPin); 
  oil = (5 * OL * 15) / 1023;
  D = (5 * OL) / 1023;
  lcd.setCursor(0, 2);
  lcd.print("O= ");
  lcd.print(oil, 1);
  lcd.print("cm"); 
  delay(5);
  SIM900.print("AT+CMGF=1\r");
  delay(5);
  SIM900.print("AT+CMGS=\"+251977665840\"\r");
  delay(10);
  if (fuelOut < 40) {
    Serial.println("FUEL IS LOW "); 
    digitalWrite(Speaker, HIGH);
    digitalWrite(LED, HIGH); 
    delay(5);
  }
  else {
    digitalWrite(Speaker, LOW);
    digitalWrite(LED, LOW);
    delay(5);
    Serial.println("FUEL IS NORMAL"); 
    delay(500);
  }
  if (oil < 20) {
    Serial.println("OIL IS LOW "); 
    digitalWrite(Speaker, HIGH); 
    digitalWrite(LED, HIGH);
    delay(500);
  }
  else {
    digitalWrite(Speaker, LOW);
    digitalWrite(LED, LOW);
    delay (5);
    Serial.println ("OIL IS NORMAL");
    delay (500);
  }
  if ((220 < VoltOut && VoltOut < 380)) {
    delay(5);
    Serial.println("NORMAL VOLTAGE"); 
    delay(5);
    digitalWrite(Relay, HIGH); 
    digitalWrite(LED, LOW);
    delay(5);
  }
  else {
    digitalWrite(Relay, LOW); 
    digitalWrite(LED, HIGH); 
    delay(5);
    Serial.println ("UNNORMAL VOLTAGE"); 
    delay(500);
    Serial.println("FAULT IS HAPPEN");
    delay(500);
  }
}
