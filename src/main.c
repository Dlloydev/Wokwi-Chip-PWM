/* PWM Breakout by Dlloydev*/

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t pin_out;
  uint32_t coarseHz_attr;
  uint32_t coarseHz;
  uint32_t fineHz_attr;
  uint32_t fineHz;
  uint32_t duty_attr;
  uint32_t duty;
  uint32_t dutyUs;
  uint32_t periodUs;
  uint32_t countUs;
} chip_state_t;

static void chip_timer_event(void *user_data);

void chip_init(void) {
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  chip->pin_out = pin_init("OUT", OUTPUT);
  chip->coarseHz_attr = attr_init("coarseHz", 0); // Hz
  chip->fineHz_attr = attr_init("fineHz", 0); // Hz
  chip->duty_attr = attr_init("duty", 50); // %

  const timer_config_t timer_config = {
    .callback = chip_timer_event,
    .user_data = chip,
  };
  timer_t timer_id = timer_init(&timer_config);
  timer_start(timer_id, 1, true);
}

static void chip_timer_event(void *user_data) {
  chip_state_t *chip = (chip_state_t*)user_data;
  if (chip->countUs >= chip->periodUs || !(chip->coarseHz + chip->fineHz)) chip->countUs = 0;
  if (chip->duty < 100) {
    if (chip->countUs >= chip->dutyUs || !chip->countUs) pin_write(chip->pin_out, 0);
    else  pin_write(chip->pin_out, 1);
  }
  if (attr_read(chip->coarseHz_attr) != chip->coarseHz ||
      attr_read(chip->fineHz_attr) != chip->fineHz ||
      attr_read(chip->duty_attr) != chip->duty) {
    chip->coarseHz = attr_read(chip->coarseHz_attr);
    chip->fineHz = attr_read(chip->fineHz_attr);
    chip->duty = attr_read(chip->duty_attr);
    if (chip->duty == 100) pin_write(chip->pin_out, 1);
    if (chip->coarseHz || chip->fineHz) {
      chip->periodUs = 1000000UL / (chip->coarseHz + chip->fineHz);
      chip->dutyUs = ((chip->duty * chip->periodUs) / 100);
    }
  }
  chip->countUs++;
}
