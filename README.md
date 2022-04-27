# esp32-template

A github template for starting projects based on the ESP-IDF.

## Clone

* Use the `--recursive` flag when cloning to pick up the ESP-IDF

## Dependencies

* ESP-IDF Tools
    * `cd esp-idf`
    * `export IDF_GITHUB_ASSETS="dl.espressif.com/github_assets"`
    * `./install.sh`

## Parts and Pinouts

### Parts

* 1x ESP32 board
* 1x INMP441 I2S MEMS microphone
* 1x PCM5102A I2S DAC

### Mic Wiring

| ESP32  | INMP441 |
| ------------- | ------------- |
| 3v3  | VDD  |
| GND  | GND  |
| GPIO 32  | SCK (bit clock)  |
| GPIO 25  | WS (word select)  |
| GPIO 33  | SD (or is it SO?) (data pin)  |
| GND  | L/R (left/right select)  |

### Speaker Wiring

| ESP32  | PCM5102A |
| ------------- | ------------- |
| 3v3  | VIN  |
| GND  | GND  |
| GPIO 4  | BCK (bit clock)  |
| GPIO 5  | LCK (word select)  |
| GPIO 18  | DIN (data pin)  |
| GND  | SCK (left/right select)  |

## Configure

```bash
$ make menuconfig
```

or

```bash
$ idf.py menuconfig
```

## Build

```bash
$ make -j8 # Or pick your favorite parallel number
```

or

```bash
$ idf.py build
```

## Run (and see serial output)

```bash
$ make -j8 flash monitor
```

or

```bash
$ idf.py flash monitor
```

## Monitor Serial Output

```bash
$ make monitor
```

or

```bash
$ idf.py monitor
```

Exit with `Ctrl + ]`.

## Erase Flash

```bash
$ make erase_flash
```

or

```bash
$ idf.py erase-flash
```

## Code Format
-----------

`clang-format` is used for code formatting.

```bash
$ make format
```
