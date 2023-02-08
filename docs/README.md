# Wokwi PWM Chip

This custom chip for [Wokwi](https://wokwi.com/) implements a simple PWM Signal Generator.

## Pin names

| Name | Description                                                  |
| ---- | ------------------------------------------------------------ |
| OUT  | PWM output,  0-100,000 Hz (1000 Hz default), 0-100 % duty (50% duty default) |

## Usage

- Setting the top slider `Hz x 10000` to max (100000) will automatically clear the lower Hz sliders.
- Setting the `% Duty x 10` slider to max (100) will automatically clear the `% Duty x 1`  slider.

To use this chip in your project, include it as a dependency in your `diagram.json` file:

```json
  "dependencies": {
    "chip-pwm": "github:Dlloydev/Wokwi-Chip-PWM@1.0.3"
  }
```

Then, add the chip to your circuit by adding a `chip-pwm` item to the `parts` section of `diagram.json`:

```json
  "parts": {
    ...,
    { "type": "chip-pwm", "id": "pwm1" }
  },
```

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/354488725362438145) Test PWM Signal Generator with logic analyzer.

- For a complete example, see [the inverter chip test project](https://wokwi.com/projects/350946636543820370).

