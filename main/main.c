#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "esp_wifi.h"

#define SSID  "NETGEAR98"
#define PASS  "dailymango846"

#define SERVO_MIN_PULSEWIDTH  500  // SG90 has a minimum pulsewidth 1ms/1000us
#define SERVO_MAX_PULSEWIDTH  2000  // SG90 has a maximum pulsewidth 2ms/2000us
#define SERVO_MAX_DEGREE      180   // SG90 can go from 0(1ms) to 180(2ms) deg

static uint32_t pulse_width(uint32_t deg) {
  uint32_t pwm = 0;
  pwm = SERVO_MIN_PULSEWIDTH + (((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * deg ) / SERVO_MAX_DEGREE);
  return pwm;
}

// Needs to be called app_main for ESP-IDF
void app_main(void) {

  uint32_t deg, pw;

  printf("Initializing MCPWM Servo Control GPIO ... \n ");
  // Initialize GPIO as MCPWM
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 32); // Set GPIO32 (Pin 12) as PWM0A

  printf("Setting MCPWM Parameters ... \n" );
  mcpwm_config_t pwm_config;
  pwm_config.frequency = 50;
  pwm_config.cmpr_a = 0;
  pwm_config.cmpr_b = 0;
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

  printf("Sweeping ... ");
  while (1) {
    for (deg = 0; deg < SERVO_MAX_DEGREE; deg++) {
      printf("Angle of rotation: %d \n", deg);
      pw = pulse_width(deg);
      printf("Pulse Width : %d \n", pw);

      mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, pw);
      vTaskDelay(10);

    }
  }
}
