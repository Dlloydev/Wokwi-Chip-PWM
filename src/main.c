// PWM Chip v1.0.6 by David Lloyd

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
  pin_t pin_out;
  uint32_t timerUs;

  uint32_t CoefficientHz_attr; // control
  uint32_t coefficientHz_attr; // diagram.json
  float coefficientHz;

  uint32_t ExponentHz_attr; // control
  uint32_t exponentHz_attr; // diagram.json
  float exponentHz;

  uint32_t Duty_attr; // control
  uint32_t duty_attr; // diagram.json
  float duty;

  float Hz;
  uint32_t dutyUs;
  uint32_t periodUs;
  uint32_t countUs;
  uint8_t tick;
  bool firstRun;
} chip_state_t;

static void chip_timer_event(void *user_data);

void chip_init(void) {
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  chip->pin_out = pin_init("OUT", OUTPUT);
  chip->timerUs = 10;
  chip->firstRun = true;

  chip->coefficientHz_attr = attr_init_float("coefficientHz", 1);
  chip->coefficientHz = attr_read_float(chip->coefficientHz_attr);
  if (chip->coefficientHz < 0) {
    printf("coefficientHz range is 0.0-10.0\n");
    chip->coefficientHz = 0;
  }
  else if (chip->coefficientHz > 10) {
    printf("coefficientHz range is 0.0-10.0\n");
    chip->coefficientHz = 10;
  }
  chip->CoefficientHz_attr = attr_init_float("CoefficientHz", chip->coefficientHz);

  chip->exponentHz_attr = attr_init_float("exponentHz", 1.0);
  chip->exponentHz = attr_read_float(chip->exponentHz_attr);
  if (chip->exponentHz < 0.01) {
    printf("exponentHz options are 0.01, 0.1, 1, 2, 3, 4\n");
    chip->exponentHz = 0.01;
  }
  else if (chip->exponentHz > 4) {
    printf("exponentHz options are 0.01, 0.1, 1, 2, 3, 4\n");
    chip->exponentHz = 4;
  }
  chip->ExponentHz_attr = attr_init_float("ExponentHz", chip->exponentHz);

  chip->duty_attr = attr_init_float("duty", 100.0);
  chip->duty = attr_read_float(chip->duty_attr);
  if (chip->duty < 0) {
    printf("duty range is 0-100\n");
    chip->duty = 0;
  }
  else if (chip->duty > 100) {
    printf("duty range is 0-100\n");
    chip->duty = 100;
  }
  chip->Duty_attr = attr_init_float("Duty", chip->duty);

  const timer_config_t timer_config = {
    .callback = chip_timer_event,
    .user_data = chip,
  };
  timer_t timer_id = timer_init(&timer_config);
  timer_start(timer_id, chip->timerUs, true);
}

static void chip_timer_event(void *user_data) {
  chip_state_t *chip = (chip_state_t*)user_data;

  if (chip->duty == 0) pin_write(chip->pin_out, 0);
  else if (chip->periodUs == 99999999) pin_write(chip->pin_out, 0);
  else if (chip->duty == 100) pin_write(chip->pin_out, 1);
  else if (chip->countUs <= chip->dutyUs) pin_write(chip->pin_out, 1);
  else pin_write(chip->pin_out, 0);
 
  if (chip->countUs >= chip->periodUs) chip->countUs = 0;

  if (chip->tick > 100) {
    chip->tick = 0;
    if (chip->firstRun || attr_read_float(chip->CoefficientHz_attr) != chip->coefficientHz ||
        attr_read_float(chip->ExponentHz_attr) != chip->exponentHz ||
        attr_read_float(chip->Duty_attr) != chip->duty) {
        chip->coefficientHz = attr_read_float(chip->CoefficientHz_attr);
        chip->exponentHz = attr_read_float(chip->ExponentHz_attr);
        chip->duty = attr_read_float(chip->Duty_attr);
        chip->Hz = chip->coefficientHz * pow(10, chip->exponentHz);
      if (chip->duty == 100) pin_write(chip->pin_out, 1);
      if (chip->Hz == 0.0f) chip->periodUs = 99999999;
      else {
        chip->periodUs = (uint32_t)(1000000 / chip->Hz);
        chip->dutyUs = ((chip->duty * chip->periodUs) / 100);
      }
       chip->firstRun = false;
    }
  }
  chip->countUs += chip->timerUs;
  chip->tick++;
}
