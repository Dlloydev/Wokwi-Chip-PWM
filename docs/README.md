# Wokwi PWM Chip

This custom chip for [Wokwi](https://wokwi.com/) implements a simple PWM Signal Generator.

## Pin names

| Name | Description                            |
| ---- | -------------------------------------- |
| OUT  | PWM output,  0-10,100 Hz, 0-100 % duty |
| GND  | Ground                                 |
| VCC  | Supply voltage                         |

## Usage

To use this chip in your project, include it as a dependency in your `diagram.json` file:

```json
  "dependencies": {
    "chip-pwm": "github:Dlloydev/Wokwi-Chip-PWM@1.0.0"
  }
```

Then, add the chip to your circuit by adding a `chip-pwm` item to the `parts` section of `diagram.json`:

```json
  "parts": {
    ...,
    { "type": "chip-pwm", "id": "chip1" }
  },
```

For a complete example, see [the inverter chip test project](https://wokwi.com/projects/350946636543820370).

