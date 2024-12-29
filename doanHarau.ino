#define BLYNK_TEMPLATE_ID "TMPL6V-7yC4vv"
#define BLYNK_TEMPLATE_NAME "Do an Ha Tuyen Phong"
#define BLYNK_AUTH_TOKEN "ImfyLEKe8rFqksZSu-Xnuw_J5vrsIDXl"

//nhanh moi
// Uncomment the line below to enable debug prints
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Initialize LCD with address 0x27, 20 columns, 4 rows
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Blynk authentication token
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials
char ssid[] = "Thai";
char pass[] = "12345678";

// Initialize DHT sensor
DHT dht(16, DHT11); 

// Define pin numbers for sensors and actuators
#define MQ2 34   
#define IR1 27
#define IR2 13
#define RAIN 14
#define LED1 17
#define LED2 5
#define COI1 2
#define COI2 18
#define button1_pin 26
#define button2_pin 25
#define button3_pin 33
#define button4_pin 32
#define relay1_pin 23   
#define relay2_pin 12
#define relay3_pin 4
#define relay4_pin 19

// Define virtual pins for Blynk
#define button1_vpin    V4
#define button2_vpin    V5
#define button3_vpin    V6 
#define button4_vpin    V7

int relay1_state = 0;
int relay2_state = 0;
int relay3_state = 0;
int relay4_state = 0;

BlynkTimer timer;

void setup() {
  // Debug console
  Serial.begin(115200);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize DHT sensor
  dht.begin();

  // Initialize pins
  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(button2_pin, INPUT_PULLUP);
  pinMode(button3_pin, INPUT_PULLUP);
  pinMode(button4_pin, INPUT_PULLUP);
  pinMode(MQ2, INPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(RAIN, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(COI1, OUTPUT);
  pinMode(COI2, OUTPUT);
  pinMode(relay1_pin, OUTPUT);
  pinMode(relay2_pin, OUTPUT);
  pinMode(relay3_pin, OUTPUT);
  pinMode(relay4_pin, OUTPUT);

  // Turn off all relays initially
  digitalWrite(relay1_pin, HIGH);
  digitalWrite(relay2_pin, HIGH);M
  digitalWrite(relay3_pin, HIGH);
  digitalWrite(relay4_pin, HIGH);

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);

  // Set up timers
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, sendUptime);
  timer.setInterval(1000L, sendUptime2);
  timer.setInterval(1000L, sendUptime3);
  timer.setInterval(1000L, sendUptime4);
}

// Blynk connected callback
BLYNK_CONNECTED() { 
  Blynk.syncVirtual(button1_vpin);
  Blynk.syncVirtual(button2_vpin);
  Blynk.syncVirtual(button3_vpin);
  Blynk.syncVirtual(button4_vpin);
}

// Blynk button handlers
BLYNK_WRITE(button1_vpin) {
  relay1_state = param.asInt();
  digitalWrite(relay1_pin, relay1_state);
}

BLYNK_WRITE(button2_vpin) {
  relay2_state = param.asInt();
  digitalWrite(relay2_pin, relay2_state);
}

BLYNK_WRITE(button3_vpin) {
  relay3_state = param.asInt();
  digitalWrite(relay3_pin, relay3_state);
}

BLYNK_WRITE(button4_vpin) {
  relay4_state = param.asInt();
  digitalWrite(relay4_pin, relay4_state);
}

// Send sensor data to Blynk and LCD
void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("Temp : ");
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Humidity : ");
  lcd.print(h);

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
}

// Read MQ2 sensor and control LED
void sendUptime() {
 int sensorValue= map(analogRead(MQ2), 0, 4095, 0, 100);
  Blynk.virtualWrite(V2, sensorValue);
  if(sensorValue>70){
    Blynk.logEvent("gas_nguy_hiem","ro ri gas");
     digitalWrite(LED1,HIGH);
     delay(1000);
    }
     else
  {
   digitalWrite(LED1,LOW);
   delay(1000);
  }
  Serial.println(sensorValue);
} 
// Detect movement using IR1 sensor
void sendUptime2() { //HANHLANG
  int sensorValue1 = digitalRead(IR1);
  WidgetLED LED_ON_APP(V3);

  if (sensorValue1 == LOW) {
    digitalWrite(COI1, HIGH);
    LED_ON_APP.on();
  } else {
    digitalWrite(COI1, LOW);
    LED_ON_APP.off();
  }

  Serial.println(sensorValue1 == LOW ? "chuyen dong" : "BT");
}

// Detect movement using IR2 sensor
void sendUptime3() { //NHAXE
  int sensorValue2 = digitalRead(IR2);
  WidgetLED LED_ON_APP(V8);

  if (sensorValue2 == LOW) {
    Blynk.logEvent("co_vat_den_gan", "Warning");
    digitalWrite(COI2, HIGH);
    LED_ON_APP.on();
  } else {
    digitalWrite(COI2, LOW);
    LED_ON_APP.off();
  }

  Serial.println(sensorValue2 == LOW ? "chuyen dong" : "BT");
}
void sendUptime4() {
  int sensorValue3 = digitalRead(RAIN);
  WidgetLED LED_ON_APP(V9);

  if (sensorValue3 == LOW) {
    Blynk.logEvent("troi_mua_kia","Raining");
    digitalWrite(LED2, HIGH);
       Serial.println("mua");
    LED_ON_APP.on();
         delay(1000);

  } else {
    digitalWrite(LED2, LOW);
        Serial.println("Khong mua");
    LED_ON_APP.off();
         delay(1000);

  }

  Serial.println(sensorValue3 == LOW ? "cO MUA" : "BT");
}

// Main loop
void loop() {
  Blynk.run();
  timer.run();
  listenPushButtons();
}

// Handle button presses
void listenPushButtons() {
  if (digitalRead(button1_pin) == LOW) {
    delay(200); 
    controlRelay(1);
    Blynk.virtualWrite(button1_vpin, relay1_state);
  } else if (digitalRead(button2_pin) == LOW) {
    delay(200);
    controlRelay(2);
    Blynk.virtualWrite(button2_vpin, relay2_state);
  } else if (digitalRead(button3_pin) == LOW) {
    delay(200);
    controlRelay(3);
    Blynk.virtualWrite(button3_vpin, relay3_state);
  } else if (digitalRead(button4_pin) == LOW) {
    delay(200);
    controlRelay(4);
    Blynk.virtualWrite(button4_vpin, relay4_state);
  }
}

// Control relay state
void controlRelay(int relay) {
  if (relay == 1) {
    relay1_state = !relay1_state;
    digitalWrite(relay1_pin, relay1_state);
  } else if (relay == 2) {
    relay2_state = !relay2_state;
    digitalWrite(relay2_pin, relay2_state);
  } else if (relay == 3) {
    relay3_state = !relay3_state;
    digitalWrite(relay3_pin, relay3_state);
  } else if (relay == 4) {
    relay4_state = !relay4_state;
    digitalWrite(relay4_pin, relay4_state);
  }

  Serial.print("Relay");
  Serial.print(relay);
  Serial.print(" State = ");
  Serial.println(relay == 1 ? relay1_state : (relay == 2 ? relay2_state : (relay == 3 ? relay3_state : relay4_state)));
}
