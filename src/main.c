/* PWM Breakout by Dlloydev*/

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t pin_out;
  uint32_t Hz_x10000_attr;
  uint32_t Hz_x10000;
  uint32_t Hz_x1000_attr;
  uint32_t Hz_x1000;
  uint32_t Hz_x100_attr;
  uint32_t Hz_x100;
  uint32_t Hz_x10_attr;
  uint32_t Hz_x10;
  uint32_t Hz_x1_attr;
  uint32_t Hz_x1;
  float Hz_x01_attr;
  float Hz_x01;
  uint32_t duty_x10_attr;
  uint32_t duty_x10;
  uint32_t duty_x1_attr;
  uint32_t duty_x1;
  uint32_t dutyUs;
  uint32_t periodUs;
  uint32_t countUs;
  float freq;
  uint8_t tick;
} chip_state_t;

static void chip_timer_event(void *user_data);

void chip_init(void) {
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  chip->pin_out = pin_init("OUT", OUTPUT);
  chip->Hz_x10000_attr = attr_init("Hz_x10000", 0);
  chip->Hz_x1000_attr = attr_init("Hz_x1000", 1000);
  chip->Hz_x100_attr = attr_init("Hz_x100", 0);
  chip->Hz_x10_attr = attr_init("Hz_x10", 0);
  chip->Hz_x1_attr = attr_init("Hz_x1", 0);
  chip->Hz_x01_attr = attr_init_float("Hz_x01", 0.0);
  chip->duty_x10_attr = attr_init("duty_x10", 50); // %
  chip->duty_x1_attr = attr_init("duty_x1", 0); // %

  const timer_config_t timer_config = {
    .callback = chip_timer_event,
    .user_data = chip,
  };
  timer_t timer_id = timer_init(&timer_config);
  timer_start(timer_id, 1, true);
}

static void chip_timer_event(void *user_data) {
  chip_state_t *chip = (chip_state_t*)user_data;

  if (chip->duty_x1 + chip->duty_x10 == 0) pin_write(chip->pin_out, 0);
  else if (chip->periodUs == 99999999) pin_write(chip->pin_out, 0);
  else if (chip->duty_x10 == 100) pin_write(chip->pin_out, 1);
  else if (chip->countUs <= chip->dutyUs) pin_write(chip->pin_out, 1);
  else pin_write(chip->pin_out, 0);
 
  if (chip->countUs >= chip->periodUs) chip->countUs = 0;

  if (chip->tick > 100) {
    chip->tick = 0;
    if (attr_read(chip->Hz_x10000_attr) != chip->Hz_x10000 ||
        attr_read(chip->Hz_x1000_attr) != chip->Hz_x1000 ||
        attr_read(chip->Hz_x100_attr) != chip->Hz_x100 ||
        attr_read(chip->Hz_x10_attr) != chip->Hz_x10 ||
        attr_read(chip->Hz_x1_attr) != chip->Hz_x1 ||
        attr_read_float(chip->Hz_x01_attr) != chip->Hz_x01 ||
        attr_read(chip->duty_x10_attr) != chip->duty_x10 ||
        attr_read(chip->duty_x1_attr) != chip->duty_x1) {
      chip->Hz_x10000 = attr_read(chip->Hz_x10000_attr);
      chip->Hz_x1000 = attr_read(chip->Hz_x1000_attr);
      chip->Hz_x100 = attr_read(chip->Hz_x100_attr);
      chip->Hz_x10 = attr_read(chip->Hz_x10_attr);
      chip->Hz_x1 = attr_read(chip->Hz_x1_attr);
      chip->Hz_x01 = attr_read_float(chip->Hz_x01_attr);
      chip->duty_x10 = attr_read(chip->duty_x10_attr);
      chip->duty_x1 = attr_read(chip->duty_x1_attr);
      if (chip->duty_x10 == 100) chip->duty_x1_attr = attr_init("duty_x1", 0);
      if (chip->Hz_x10000 == 100000) {
        chip->Hz_x1000_attr = attr_init("Hz_x1000", 0);
        chip->Hz_x100_attr = attr_init("Hz_x100", 0);
        chip->Hz_x10_attr = attr_init("Hz_x10", 0);
        chip->Hz_x1_attr = attr_init("Hz_x1", 0);
        chip->Hz_x01_attr = attr_init_float("Hz_x01", 0.0);
      }
      if (chip->duty_x10 == 100) pin_write(chip->pin_out, 1);
      chip->freq = (float)(chip->Hz_x10000) + (float)(chip->Hz_x1000 + chip->Hz_x100 + chip->Hz_x10 + chip->Hz_x1) + chip->Hz_x01;
      if (chip->freq == 0.0f) chip->periodUs = 99999999;
      else {
        chip->periodUs = (uint32_t)(1000000 / chip->freq);
        chip->dutyUs = (((chip->duty_x10 + chip->duty_x1) * chip->periodUs) / 100);
      }
      //printf("periodUs: %d\n", chip->periodUs);
    }
  }
  chip->countUs++;
  chip->tick++;
}
