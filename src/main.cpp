#include <Arduino.h>
#include <Wire.h>
#include <VL53L1X.h>
#include <Bounce2.h>
#include <vector>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "LCD.h"
#include "TOF400.h"
#include "Sample.h"
#include "Solenoid.h"
#include "Magnet.h"

enum state {
  IDLE,
  SETUP,
  RUNNING,
};

enum mode {
  AUTO,
  MANUAL,
};


using namespace std;
hw_timer_t *My_timer = NULL;

const char* ssid = "Home coming_2.4G";
const char* password = "33333333";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
 
LCD lcd(17, 16, 4); 
Magnet magnet(25, 33, 32);
TOF400 tof;
Solenoid solenoid(26);

#define BTN1 36 // IDLE
#define BTN2 39 // SETUP
#define BTN3 34 // START
#define BTN4 35 // SWAP MODE
#define BTN5 27 // HIT

Bounce2::Button btn1 = Bounce2::Button();
Bounce2::Button btn2 = Bounce2::Button();
Bounce2::Button btn3 = Bounce2::Button();
Bounce2::Button btn4 = Bounce2::Button();

state currentState;
state previousState;
mode currentMode;
unsigned int hitTime = 534249.0 - 80000.0;

unsigned long idleStartTime = 0; 
const unsigned long IDLE_TIMEOUT = 20000; // 20 วินาที (20,000 ms)

volatile unsigned long solenoidStartTime = 0;
volatile bool solenoidActive = false;
volatile bool hited = false;

void setup_wifi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected!");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setupButtons_1_4() {
  btn1.attach(BTN1, INPUT);
  btn2.attach(BTN2, INPUT);
  btn3.attach(BTN3, INPUT);
  btn4.attach(BTN4, INPUT);

  btn1.interval(50);
  btn2.interval(50);
  btn3.interval(50);
  btn4.interval(50);

  btn1.setPressedState(LOW);
  btn2.setPressedState(LOW);
  btn3.setPressedState(LOW);
  btn4.setPressedState(LOW);
}

void updateButtons() {
  btn1.update();
  btn2.update();
  btn3.update();
  btn4.update();
}

void updateSolenoid() {
  if (solenoidActive && (millis() - solenoidStartTime >= 300)) {
    solenoid.off();
    solenoidActive = false;
  }
}

void enterDeepSleep() {
  Serial.println("Entering Deep Sleep due to inactivity...");
  
  lcd.clearScreen();
  lcd.setCursor(20, 70);
  lcd.setColor(150, 150, 150);
  lcd.print("SLEEPING...");
  delay(3000);
  lcd.clearScreen();

  solenoid.off();
  magnet.off();

  // ตั้งค่าให้ตื่นเมื่อ Pin 36 (BTN1) มีสถานะเป็น LOW (0)
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, 0);

  // สั่งเข้า Deep Sleep
  esp_deep_sleep_start();
}

void IRAM_ATTR HIT_BALL() {
  if (solenoidActive && hited) return; // ป้องกันการกดซ้ำขณะที่โซลินอยด์ยังทำงานอยู่

  solenoid.on();                
  solenoidStartTime = millis(); 
  solenoidActive = true;
  hited = true;
  detachInterrupt(digitalPinToInterrupt(BTN5));
}

void setup()
{
  Serial.begin(9600);

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("Woke up from Deep Sleep!");
  }

  lcd.begin();
  tof.begin();
  solenoid.begin();
  magnet.begin();
  
  setupButtons_1_4();
  pinMode(BTN5, INPUT);

  currentState = IDLE;
  previousState = IDLE;
  currentMode = MANUAL;

  lcd.clearScreen();
  lcd.setCursor(20, 70);
  lcd.setColor(255, 255, 255);
  lcd.print("State: IDLE");

  solenoid.off();
  magnet.off();

  My_timer = timerBegin(0, 80, true);         // set counter frequency to 1MHz
  timerAttachInterrupt(My_timer, &HIT_BALL, true); // point to the ISR
  timerAlarmWrite(My_timer, hitTime, true);

  hited = false;

  idleStartTime = millis();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setBufferSize(2048);

  Serial.println("Setup complete.");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  updateButtons();
  updateSolenoid();

  switch (currentState) {
  case state::IDLE:
    if (previousState != currentState) {
      lcd.clearScreen();
      lcd.setCursor(20, 70);
      lcd.setColor(255, 255, 255);
      lcd.print("State: IDLE");
      hited = false;
      idleStartTime = millis();
      previousState = currentState;
    }

    if (millis() - idleStartTime >= IDLE_TIMEOUT) {
      enterDeepSleep();
    }

    // หากมีการกดปุ่มใดๆ ในหน้า IDLE (เช่น เผลอกดปุ่มอื่น) ให้รีเซ็ตเวลาใหม่จะได้ไม่หลับ
    if (btn1.fell() || btn3.fell() || btn4.fell()) {
      idleStartTime = millis();
    }
    
    solenoid.off();
    magnet.off();

    if (btn2.fell()) {
      currentState = SETUP;
    }
    break;

  case state::SETUP:
    if (previousState != currentState) {
      lcd.clearScreen();

      for (int power = 0; power <= 255; power += 5) {
        magnet.on(power);
        lcd.clearScreen();
        lcd.setColor(255-power, power, 0);
        lcd.setCursor(20, 40);
        lcd.print("SETTING UP...");
        lcd.setCursor(0, 70);
        lcd.print("MAGNET POWER: ");
        lcd.setCursor(40, 100);
        lcd.print(String(power) + " / 255");
        delay(20);
        
        if (power == 255) delay(1000);
      }

      lcd.clearScreen();
      lcd.setColor(0, 200, 0);
      lcd.setCursor(5, 20);
      lcd.print("MAGNET READY!");

      lcd.setColor(255, 255, 255);
      lcd.setCursor(5, 50);
      lcd.print("MODE: " + String((currentMode == AUTO) ? "AUTO" : "MANUAL"));
      
      lcd.setColor(200, 200, 0);
      lcd.setCursor(20, 100);
      lcd.print("PRESS START");
      lcd.setCursor(30, 120);
      lcd.print("TO BEGIN");

      previousState = currentState;
    }

    if (btn1.fell()) {
      currentState = IDLE;
    }

    if (btn3.fell()) {
      currentState = RUNNING;
    }

    if (btn4.fell()) {
      currentMode = (currentMode == AUTO) ? MANUAL : AUTO;
      lcd.clearScreen();
      lcd.setColor(0, 200, 0);
      lcd.setCursor(5, 20);
      lcd.print("MAGNET READY!");

      lcd.setColor(255, 255, 255);
      lcd.setCursor(5, 50);
      lcd.print("MODE: " + String((currentMode == AUTO) ? "AUTO" : "MANUAL"));
      
      lcd.setColor(200, 200, 0);
      lcd.setCursor(20, 100);
      lcd.print("PRESS START");
      lcd.setCursor(30, 120);
      lcd.print("TO BEGIN");
    } 
    
    break;

  case state::RUNNING:
    if (previousState != currentState) {
      for (int i = 3; i > 0; i--) {
        lcd.clearScreen();
        lcd.setColor(255, 0, 0);
        lcd.setCursor(30, 50);
        lcd.print("STARTING IN...");
        lcd.setCursor(50, 100);
        lcd.print(String(i));
        delay(1000);
      }

      lcd.clearScreen();
      lcd.setColor(0, 255, 0);
      lcd.setCursor(20, 70);
      lcd.print("State: RUNNING");

      vector<int> distanceReadings;
      vector<unsigned long> timeStamps;
      int Distance = tof.getDistance();

      timerWrite(My_timer, 0);
      if (currentMode == mode::MANUAL) attachInterrupt(digitalPinToInterrupt(BTN5), HIT_BALL, RISING);

      magnet.off();
      if (currentMode == mode::AUTO) timerAlarmEnable(My_timer);
      unsigned long dropStartTime = millis();

      while (Distance < 1300) {
        delay(1);
        Distance = tof.getDistance();
        distanceReadings.push_back(Distance);
        timeStamps.push_back(millis());

        updateSolenoid();

        if (millis() - dropStartTime > 2000) break;
      }

      // รอโซลินอยด์ทำงาน (เฉพาะโหมด AUTO)
      if (currentMode == mode::AUTO) {
        // รอจนกว่าจะมีการตี หรือหมดเวลา (เผื่อระบบรวนจะได้ไม่ค้าง)
        while (!solenoidActive && (millis() - dropStartTime < 2000)) {
          delay(1);
        }
      }

      DynamicJsonDocument doc(2048); 
      JsonArray tArray = doc.createNestedArray("t"); 
      JsonArray dArray = doc.createNestedArray("d"); 

      unsigned long t0 = timeStamps[0];

      for (int i = 0; i < distanceReadings.size(); i++) {
        tArray.add((timeStamps[i] - t0));
        dArray.add(distanceReadings[i]);
      }

      String payload;
      serializeJson(doc, payload);

      if (!client.connected()) {
        Serial.println("MQTT disconnected! Reconnecting before publish...");
        reconnect();
      }

      if (client.publish("testtopic/MQTT/CPE1", payload.c_str())) {
        Serial.println("Already published data to MQTT:");
      } else {
        Serial.println("Publish FAILED! (Payload might be too large)");
      }

      solenoid.off();
      solenoidActive = false;
      
      // ปิด Interrupt และ Timer เมื่อทำงานเสร็จ เพื่อไม่ให้กวนโหมด IDLE
      if (currentMode == AUTO) timerAlarmDisable(My_timer); 
      else detachInterrupt(digitalPinToInterrupt(BTN5));

      previousState = currentState;
      currentState = IDLE;
    }
    
    break;
  }
}