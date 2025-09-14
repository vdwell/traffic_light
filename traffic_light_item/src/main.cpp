#include <Arduino.h>

// light1
#define RADIO_RX_PIN 12
#define BUZZER_PIN 6
#define RED_LED_PIN 2
#define GREEN_LED_PIN 3
#define YELLOW1_LED_PIN 4
#define YELLOW2_LED_PIN 5
#define LIGHT_ID 2 //// next is 2, previos is 4
// #define G433_SPEED 1000
// #define G433_SLOW         // отправляю раз в секунду на SYN480R
// #define SAMPLE_AVERAGE 3  // количество чтений для усреднения

#include "light.h"
light mylight(LIGHT_ID, RED_LED_PIN, GREEN_LED_PIN, YELLOW1_LED_PIN, YELLOW2_LED_PIN, BUZZER_PIN);

#include <Gyver433.h>
// Gyver433_TX<RADIO_TX_PIN> tx;  // указали пин
Gyver433_RX<RADIO_RX_PIN, 4> rx; // указали пин

// формат пакета для отправки
struct Data
{
  uint8_t controller_id;     // 1..255
  uint8_t light_id;          // 1..255
  bool light_action;         // 0..1
  uint8_t light_speed_limit; // 0..2
};
Data data;

int tmr;

enum speed
{
  NO_LIMIT = 0,
  RESTRICTED = 1,
  SLOW = 2
} speed_limitation;

void setup()
{
  Serial.begin(9600);
  speed_limitation = SLOW;
  Serial.println("Started");
  Serial.println(speed_limitation);
  mylight.speedLimit(speed_limitation);
}

void loop()
{

  // приём по радио
  if (rx.tick())
  {
    Data data;
    // если чтение без ошибок
    if (rx.readData(data))
    {
      Serial.println(data.light_id);
      if (data.light_id == LIGHT_ID)
      {
        tmr = millis(); // сброс таймаута
        mylight.trafficPermit(data.light_action);
        mylight.speedLimit(data.light_speed_limit);

        // для отладки
        Serial.println(data.light_action);
      }
    }
  }
}