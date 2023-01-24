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
  uint32_t duty_x10_attr;
  uint32_t duty_x10;
  uint32_t duty_x1_attr;
  uint32_t duty_x1;
  uint32_t dutyUs;
  uint32_t periodUs;
  uint32_t countUs;
} chip_state_t;

static void chip_timer_event(void *user_data);

void chip_init(void) {
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  chip->pin_out = pin_init("OUT", OUTPUT);
  chip->Hz_x10000_attr = attr_init("Hz_x10000", 0);
  chip->Hz_x1000_attr = attr_init("Hz_x1000", 0);
  chip->Hz_x100_attr = attr_init("Hz_x100", 0);
  chip->Hz_x10_attr = attr_init("Hz_x10", 0);
  chip->Hz_x1_attr = attr_init("Hz_x1", 0);
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
  if (chip->countUs >= chip->periodUs || !(chip->Hz_x10000 + chip->Hz_x1000 + chip->Hz_x100 + chip->Hz_x10 + chip->Hz_x1)) chip->countUs = 0;
  if (chip->duty_x10 < 100) {
    if (chip->countUs >= chip->dutyUs || !chip->countUs) pin_write(chip->pin_out, 0);
    else  pin_write(chip->pin_out, 1);
  }
  if (attr_read(chip->Hz_x10000_attr) != chip->Hz_x10000 ||
      attr_read(chip->Hz_x1000_attr) != chip->Hz_x1000 ||
      attr_read(chip->Hz_x100_attr) != chip->Hz_x100 ||
      attr_read(chip->Hz_x10_attr) != chip->Hz_x10 ||
      attr_read(chip->Hz_x1_attr) != chip->Hz_x1 ||
      attr_read(chip->duty_x10_attr) != chip->duty_x10 ||
      attr_read(chip->duty_x1_attr) != chip->duty_x1) {
    chip->Hz_x10000 = attr_read(chip->Hz_x10000_attr);
    chip->Hz_x1000 = attr_read(chip->Hz_x1000_attr);
    chip->Hz_x100 = attr_read(chip->Hz_x100_attr);
    chip->Hz_x10 = attr_read(chip->Hz_x10_attr);
    chip->Hz_x1 = attr_read(chip->Hz_x1_attr);
    chip->duty_x10 = attr_read(chip->duty_x10_attr);
    chip->duty_x1 = attr_read(chip->duty_x1_attr);
    if (chip->duty_x10 == 100) chip->duty_x1_attr = attr_init("duty_x1", 0);
    if (chip->Hz_x10000 == 100000) {
      chip->Hz_x1000_attr = attr_init("Hz_x1000", 0);
      chip->Hz_x100_attr = attr_init("Hz_x100", 0);
      chip->Hz_x10_attr = attr_init("Hz_x10", 0);
      chip->Hz_x1_attr = attr_init("Hz_x1", 0);
    }
    if (chip->duty_x10 == 100) pin_write(chip->pin_out, 1);
    if (chip->Hz_x10000 || chip->Hz_x1000 || chip->Hz_x100 || chip->Hz_x10 || chip->Hz_x1) {
      chip->periodUs = 1000000UL / (chip->Hz_x10000 + chip->Hz_x1000 + chip->Hz_x100 + chip->Hz_x10 + chip->Hz_x1);
      chip->dutyUs = (((chip->duty_x10 + chip->duty_x1) * chip->periodUs) / 100);
    }
  }
  chip->countUs++;
}