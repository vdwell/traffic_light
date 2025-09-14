#include <Arduino.h>

#define J_VRX_PIN A0
#define J_VRY_PIN A1
#define J_SW_PIN 6

#define RED_LED_PIN 5
#define GREEN_LED_PIN 2
#define YELLOW_LED1_PIN 3
#define YELLOW_LED2_PIN 4

#define RADIO_TX_PIN 12
#define BUZZER_PIN 11

#define BLUE_LED_PIN 10

// test leds class
// #include <Blinker.h>
// Blinker blueLed(BLUE_LED_PIN);
// leds buzzer(BUZZER_PIN);
unsigned long previousMillis = 0;

// #define G433_SPEED 3000
// #define G433_FAST
#include <Gyver433.h>
Gyver433_TX<RADIO_TX_PIN> tx; // указали пин

// формат пакета для отправки
struct Data
{
  uint16_t controller_id;    // 1..uint16_t
  uint8_t light_id;          // 1..255
  bool light_action;         // 0..1
  uint8_t light_speed_limit; // 0..2
};
Data data_light1;
Data data_light2;
uint8_t i = 0;
Data remote_lights_state_db[4];
uint32_t myTimer1;

#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

#include "light.h"
light light_indicator(0, RED_LED_PIN, GREEN_LED_PIN, YELLOW_LED1_PIN, YELLOW_LED2_PIN, 0);
uint8_t currentLightId = 0;

#include "button.h"
Button joystickButton(J_SW_PIN);

#include "joystick_as_button.h"
JoystickAsButton joystickGear(J_VRX_PIN, J_VRY_PIN);
int x = 0;
int y = 0;
int xMin = 0;
int xMax = 128;
int yMin = 0;
int yMax = 64;

enum
{
  LIGHT_ID_SELECTION,
  LIGHT_MODE_SELECTION,
} light_configuration_state;

uint8_t lightIdPreSelection = 0;
bool trafficPermitPreSelection = false;
uint8_t speedLimitPreSelection = 0;

uint32_t myTimer2;

void setup()
{
  Serial.begin(9600);

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED1_PIN, OUTPUT);
  pinMode(YELLOW_LED2_PIN, OUTPUT);

  light_configuration_state = LIGHT_ID_SELECTION;

  // remote lights state database
  remote_lights_state_db[0] = (Data){1001, 0, 0, 0};
  remote_lights_state_db[1] = (Data){1001, 1, 1, 0};
  remote_lights_state_db[2] = (Data){1001, 2, 1, 0};
  remote_lights_state_db[3] = (Data){1001, 3, 0, 0};

  // uint8_t lightIdPreSelection = 1;
  // bool trafficPermitPreSelection = remote_lights_state_db[lightIdPreSelection].light_action;
  // uint8_t speedLimitPreSelection = remote_lights_state_db[lightIdPreSelection].light_speed_limit;

  Serial.println((unsigned int)sizeof(remote_lights_state_db));
  Serial.println((unsigned int)sizeof(remote_lights_state_db) / (unsigned int)sizeof(remote_lights_state_db[0]));

  // Serial.println(remote_lights_state_db[0].light_id);
  // Serial.println(remote_lights_state_db[1].light_id);
  // Serial.println(remote_lights_state_db[2].light_id);
  // Serial.println(remote_lights_state_db[3].light_id);

  // currentLightId = 1;
  // light_indicator.setLightId(currentLightId);
  // light_indicator.trafficPermit(remote_lights_state_db[currentLightId].light_action);
  // light_indicator.speedLimit(remote_lights_state_db[currentLightId].light_speed_limit);

  joystickButton.start();

  oled.init();      // инициализация
  oled.clear();     // очистка
  oled.setScale(2); // масштаб текста (1..4)
  oled.home();      // курсор в 0,0
  oled.setCursor(0, 4);
  oled.print("Light?");
  // for (int y = 0; y < 64; y += 16) {
  //   for (int x = 0; x < 128; x += 16) {
  //     oled.rect(x + 2, y + 2, x + 14, y + 14, OLED_STROKE);
  //   }
  // }
}

void loop()
{

  // test with blueLed
  /*
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 2000) {
    previousMillis = currentMillis;
    blueLed.toggle();
  }
  blueLed.turnOffAfterDelay(5000, false);
  blueLed.fadeOn(200);

  blueLed.pulsate(50);

  blueLed.blinkNTimes(10,200,500);
  buzzer.blinkNTimes(3,300,2000);
   */

  // send radio signal with each remote light state (to each remote light) in circle every 100ms from light state database
  // go this code every 100ms without any conditions
  if (millis() - myTimer1 >= 100)
  {
    tx.sendData(remote_lights_state_db[i]);
    myTimer1 = millis();
    if (i >= sizeof(remote_lights_state_db) / sizeof(remote_lights_state_db[0]) - 1)
    {
      i = 0;
    }
    else
    {
      i++;
    }
  }

  // select light id to view its status and configuration
  // go this code only if light_configuration_state is in LIGHT_ID_SELECTION
  if (light_configuration_state == LIGHT_ID_SELECTION)
  {
    // light selection
    switch (joystickButton.checkBTN())
    {
    case 1:
      // short pressing
      Serial.println("short click from joystickButton");
      if (light_configuration_state == LIGHT_ID_SELECTION)
      {
        currentLightId = lightIdPreSelection;
        light_indicator.setLightId(currentLightId);
        trafficPermitPreSelection = remote_lights_state_db[currentLightId].light_action;
        speedLimitPreSelection = remote_lights_state_db[currentLightId].light_speed_limit;
        light_configuration_state = LIGHT_MODE_SELECTION;
        oled.clear();
        oled.setScale(2);
        oled.home();
        oled.setCursor(0, 4);
        oled.print("Inside ");
        oled.print(currentLightId);

        // buzzer.blinkNTimes(3, 300, 2000);
      }
      break;
    case 2:
      // long pressing
      Serial.println("long click from joystickButton. do nothing");
      break;
    }

    // check joystick gear left/right movement and: #1. each movement set/change light id, #2. get db state for selected light
    switch (joystickGear.checkJoystickX())
    {
    case 1:
      Serial.println("joystick moved right");
      if (lightIdPreSelection < sizeof(remote_lights_state_db) / sizeof(remote_lights_state_db[0]) - 1)
      {
        lightIdPreSelection++;
        Serial.print("Selected light id is ");
        Serial.println(lightIdPreSelection);
        light_indicator.trafficPermit(remote_lights_state_db[lightIdPreSelection].light_action);
        light_indicator.speedLimit(remote_lights_state_db[lightIdPreSelection].light_speed_limit);
        oled.clear();
        oled.setScale(2);
        oled.home();
        oled.setCursor(0, 2);
        oled.print("Selection:");
        oled.setCursor(0, 5);
        oled.print(lightIdPreSelection);
      }
      break;
    case -1:
      Serial.println("joystick moved left");
      if (lightIdPreSelection >= 1)
      {
        lightIdPreSelection--;
        Serial.print("Selected light id is ");
        Serial.println(lightIdPreSelection);
        light_indicator.trafficPermit(remote_lights_state_db[lightIdPreSelection].light_action);
        light_indicator.speedLimit(remote_lights_state_db[lightIdPreSelection].light_speed_limit);
        oled.clear();
        oled.setScale(2);
        oled.home();
        oled.setCursor(0, 2);
        oled.print("Selection:");
        oled.setCursor(0, 5);
        oled.print(lightIdPreSelection);
        break;
      }
    }

    // light mode configuration
  }
  else if (light_configuration_state == LIGHT_MODE_SELECTION)
  {

    // light configuration

    switch (joystickButton.checkBTN())
    {
    case 1:
      // short pressing
      Serial.println("short click from joystickButton");
      if (light_configuration_state == LIGHT_MODE_SELECTION)
      {
        remote_lights_state_db[currentLightId].light_action = trafficPermitPreSelection;
        remote_lights_state_db[currentLightId].light_speed_limit = speedLimitPreSelection;
        light_indicator.trafficPermit(remote_lights_state_db[currentLightId].light_action);
        light_indicator.speedLimit(remote_lights_state_db[currentLightId].light_speed_limit);
        light_configuration_state = LIGHT_ID_SELECTION;
        oled.clear();
        oled.setScale(2);
        oled.home();
        oled.setCursor(0, 2);
        oled.print("Selection:");
        oled.setCursor(0, 5);
        oled.print(currentLightId);
      }
      break;
    case 2:
      // long pressing
      Serial.println("long click from joystickButton, do nothing");
      break;
    }

    switch (joystickGear.checkJoystickX())
    {
    case 1:
      Serial.println("joystick moved right");
      trafficPermitPreSelection = true;
      light_indicator.trafficPermit(trafficPermitPreSelection);
      light_indicator.speedLimit(speedLimitPreSelection);
      Serial.println("trafficPermit is true");
      /*
      oled.rect(x + 2, y + 2, x + 14, y + 14, OLED_STROKE);
      if (x < xMax - 16) {
        // limit increase of x
        x += 16;
      }
      oled.rect(x + 2, y + 2, x + 14, y + 14, OLED_FILL);
      // digitalWrite(ledGreen, !digitalRead(ledGreen));
      */
      break;
    case -1:
      Serial.println("joystick moved left");
      trafficPermitPreSelection = false;
      light_indicator.trafficPermit(trafficPermitPreSelection);
      light_indicator.speedLimit(speedLimitPreSelection);
      Serial.println("trafficPermit is false");
      /*
      oled.rect(x + 2, y + 2, x + 14, y + 14, OLED_STROKE);
      if (x >= xMin + 16) {
        // limit decrease of x
        x -= 16;
      }
      oled.rect(x + 2, y + 2, x + 14, y + 14, OLED_FILL);
      // digitalWrite(ledRed, !digitalRead(ledRed));
      */
      break;
    }
  }

  switch (joystickGear.checkJoystickY())
  {
  case -1:
    Serial.println("joystick moved up");
    if (speedLimitPreSelection > 0)
    {
      speedLimitPreSelection--;
    }
    light_indicator.trafficPermit(trafficPermitPreSelection);
    light_indicator.speedLimit(speedLimitPreSelection);
    Serial.print("speedLimitPreSelection is ");
    Serial.println(speedLimitPreSelection);
    /*
    oled.rect(x + 2, y + 2, x + 14, y + 14, OLED_STROKE);
    if (y >= yMin + 16) {
      // limit decrease of y
      y -= 16;
    }
    oled.rect(x + 2, y + 2, x + 14, y + 14, OLED_FILL);
    // digitalWrite(ledRed, !digitalRead(ledRed));
    */
    break;
  case 1:
    Serial.println("joystick moved down");
    if (speedLimitPreSelection < 2)
    {
      speedLimitPreSelection++;
    }
    light_indicator.trafficPermit(trafficPermitPreSelection);
    light_indicator.speedLimit(speedLimitPreSelection);
    Serial.print("speedLimitPreSelection is ");
    Serial.println(speedLimitPreSelection);
    /*
    oled.rect(x + 2, y + 2, x + 14, y + 14, OLED_STROKE);
    if (y < yMax - 16) {
      // limit increase of y
      y += 16;
    }
    oled.rect(x + 2, y + 2, x + 14, y + 14, OLED_FILL);
    // digitalWrite(ledGreen, !digitalRead(ledGreen));
    */
    break;
  }

  /*
  // joystick gear debugging
  if (millis() - myTimer2 >= 1000) {
    myTimer2 = millis();
    Serial.print(analogRead(J_VRX_PIN));
    Serial.print(',');
    Serial.print(map(analogRead(J_VRX_PIN), 0, 1023, -1, 1));
    Serial.print(',');
    Serial.print(analogRead(J_VRY_PIN));
    Serial.print(',');
    Serial.print(map(analogRead(J_VRY_PIN), 0, 1023, -1, 1));
    Serial.print(',');
    Serial.println(digitalRead(J_SW_PIN));
  }
*/
}