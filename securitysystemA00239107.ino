#include <DHT.h> //library for dht sensor
#include <IRremote.h>//same here for remote
//----------------------------------------------sensor module pins---------------------------------
#define LASER_PIN 9//laser
#define PHOTO_PIN A3//photore an pin
#define FLAME_PIN A2//flame sens pin Ana
#define DHT_PIN 7//dht11 data
#define IR_PIN 11//receiver pin for IR 
#define BUZZER_PIN 8//buzzer
#define BUTTON_PIN 4//button

#define RED_PIN 3//all three rgb below
#define GREEN_PIN 5
#define BLUE_PIN 6

#define JOY_X A0//all three joystick, x,y and button below
#define JOY_Y A1
#define JOY_SW 2
//---------------------------------------create-------------------------------
DHT dht(DHT_PIN, DHT11);
IRrecv irrecv(IR_PIN);
decode_results results;
//-------------------------------armed/not armed----------------------------------
bool armed = false;
bool alarmTriggered = false;
//----------------------------------RGB color function----------------------------
void rgb(int r, int g, int b) 
{
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}
//------------------------------------------alarm-----------------------------
void melody1() 
{
  tone(BUZZER_PIN, 1000, 200);
  delay(250);
  tone(BUZZER_PIN, 1500, 200);
  delay(250);
}
//-----------------------------------------alarm2---------------------------------
void melody2() 
{
  tone(BUZZER_PIN, 800, 300);
  delay(350);
  tone(BUZZER_PIN, 1200, 300);
  delay(350);
}
//--------------------------------------setup---------------------------------------
void setup() 
{
  Serial.begin(9600);//monitor

  pinMode(LASER_PIN, OUTPUT);//output for laser
  pinMode(BUTTON_PIN, INPUT_PULLUP);// pullup button
  pinMode(JOY_SW, INPUT_PULLUP);//joystick pullup

  pinMode(RED_PIN, OUTPUT);//rgb
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  dht.begin();//dht11 starts
  irrecv.enableIRIn();//IR starts

  digitalWrite(LASER_PIN, HIGH);//laser is on
  rgb(0, 0, 255);//off aka blue
}
//----------------------------main loop----------------------------------------
void loop() {
//check input for IR 
  if (irrecv.decode(&results)) 
  {
    if (results.value == 0xFFA25D) 
    {
      armed = !armed;//is armed
      alarmTriggered = false;//reset
    }
    irrecv.resume();
  }
//button
  if (digitalRead(BUTTON_PIN) == LOW) 
  {
    armed = !armed;//armed button
    alarmTriggered = false;//unarmed 
    delay(300);
  }
//joystick press down = print values of all sensors
  int joySW = digitalRead(JOY_SW);
  if (joySW == LOW) 
  {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    int flame = analogRead(FLAME_PIN);
    int light = analogRead(PHOTO_PIN);

    Serial.print("Temp: "); Serial.print(t);//text to label what is being read and where
    Serial.print("C  Humidity: "); Serial.print(h);
    Serial.print("%  Flame: "); Serial.print(flame);
    Serial.print("  Light: "); Serial.println(light);
    delay(500);
  }
//read
  int lightValue = analogRead(PHOTO_PIN);
  int flameValue = analogRead(FLAME_PIN);
  float temp = dht.readTemperature();
//system armed check for sensors
  if (armed) 
  {
    rgb(255, 0, 0);

    if (lightValue < 300) alarmTriggered = true;//light
    if (flameValue < 400) alarmTriggered = true;//flame
    if (temp > 35) alarmTriggered = true;//temp

    if (alarmTriggered) 
    {
      rgb(255, 0, 0);
      melody1();
      melody2();
    }
  }
//blue
  else 
  {
    rgb(0, 0, 255);
  }

  delay(50);
}

