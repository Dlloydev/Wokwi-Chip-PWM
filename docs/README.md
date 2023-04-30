# Wokwi-Chip-PWM
## Description

PWM Signal Generator  **⎍⎍⎍⎍**

-  0.01 - 100000.00 Hz frequency range

- 0- 100.0 % duty range

- Default control settings (below) can be changed in `diagram.json` 

  ```json
  "attrs": { "coefficientHz": "10", "frequencyScale": "3", "duty": "50" }
  ```

  

![image](https://user-images.githubusercontent.com/63488701/235357633-c48347e8-21cd-445f-81f4-9f1c403fcb3a.png)

## Usage

To use this chip in your project, include it as a dependency in your `diagram.json` file:

```json
"dependencies": { "chip-pwm": "github:Dlloydev/Wokwi-Chip-PWM@1.0.4" }
```

Then, add the chip to your circuit by adding a `chip-scope` item to the `parts` section of `diagram.json`:

```json
  "parts": {
    ...,
    {
      "type": "chip-pwm",
      "id": "pwm1",
      "top": -37.38,
      "left": -61.6,
      "attrs": { "coefficientHz": "10", "frequencyScale": "3", "duty": "50" }
    },
```

The actual source code for the chip lives in [src/main.c](https://github.com/Dlloydev/Wokwi-Chip-PWM/blob/main/src/main.c), and the pins are described in [chip.json](https://github.com/Dlloydev/Wokwi-Chip-PWM/blob/main/chip.json).

## Examples

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/354488725362438145) Test PWM Signal Generator with Scope chip.

## License

This project is licensed under the MIT license. See the [LICENSE](https://github.com/Dlloydev/Wokwi-Chip-PWM/blob/main/LICENSE) file for more details.
