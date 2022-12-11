#include "Arduino.h"
#include "Arduino_FreeRTOS.h"
#include "RF24.h"
#include "avr/interrupt.h"
#include "semphr.h"

typedef struct
{
  int16_t sensor_id{0};
  int16_t sensor_type{0};
  int16_t value_1{0};
  int16_t value_2{0};
  int16_t value_3{0};
  int16_t value_4{0};
  int16_t value_5{0};
} transmitted_data_t;

void sendBatteryLevelCharge(void *pvParameters);
void sendAlarmStatus(void *pvParameters);
float getBatteryLevelCharge(void);

RF24 radio(9, 10);
TaskHandle_t xSendBatteryLevelCharge;

void setup()
{
  EICRA |= (1 << ISC01) | (1 << ISC00);
  EIMSK |= (1 << INT0);
  ADCSRA &= ~(1 << ADEN);
  radio.begin();
  radio.setChannel(120);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(14);
  radio.setAddressWidth(3);
  radio.setCRCLength(RF24_CRC_8);
  radio.setRetries(15, 15);
  radio.openWritingPipe(0xDDEEFF);
  radio.powerDown();
  xTaskCreate(sendBatteryLevelCharge, "Send Battery Level Charge", configMINIMAL_STACK_SIZE, NULL, 1, &xSendBatteryLevelCharge);
}

void loop()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  portENTER_CRITICAL();
  sleep_enable();
  portEXIT_CRITICAL();
  sleep_cpu();
  sleep_reset();
}

void sendBatteryLevelCharge(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    transmitted_data_t sensor{abs((int16_t)ID), WATER_LEAKAGE};
    sensor.value_1 = getBatteryLevelCharge() * 100;
    sensor.value_2 = DRY;
    radio.powerUp();
    radio.flush_tx();
    radio.write(&sensor, sizeof(transmitted_data_t));
    radio.powerDown();
    vTaskDelay(3600);
  }
  vTaskDelete(NULL);
}

void sendAlarmStatus(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    transmitted_data_t sensor{abs((int16_t)ID), WATER_LEAKAGE};
    sensor.value_2 = ALARM;
    radio.powerUp();
    radio.flush_tx();
    radio.write(&sensor, sizeof(transmitted_data_t));
    radio.powerDown();
    vTaskDelay(60);
  }
  vTaskDelete(NULL);
}

float getBatteryLevelCharge()
{
  ADMUX = (1 << REFS0) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1);
  ADCSRA |= (1 << ADEN);
  delay(10);
  ADCSRA |= (1 << ADSC);
  while (bit_is_set(ADCSRA, ADSC))
    ;
  ADCSRA &= ~(1 << ADEN);
  float value = ((1024 * 1.1) / (ADCL + ADCH * 256));
  return value;
}

ISR(INT0_vect)
{
  cli();
  vTaskDelete(xSendBatteryLevelCharge);
  xTaskCreate(sendAlarmStatus, "Send Alarm Status", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}