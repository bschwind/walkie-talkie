# esp32-template

A github template for starting projects based on the ESP-IDF.

## Clone

* Use the `--recursive` flag when cloning to pick up the ESP-IDF

## Dependencies

* ESP-IDF Tools
    * `cd esp-idf`
    * `export IDF_GITHUB_ASSETS="dl.espressif.com/github_assets"`
    * `./install.sh`

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
