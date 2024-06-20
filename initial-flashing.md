# Notes on initial flashing

Initial flashing required some (unexpected) steps to succeed.


## ❌ What did not work (for me)

Connecting the Powermeter directly to the **RPi running ESPHome**:
- Flashing directly from ESPHome via USB-Serial failed, as plugging in the Powermeter crashed the Pi (potentially because of power consumption, but not sure).

Connecting the Powermeter to a **laptop** and generating an image in ESPHome:
- Flashing via [ESPHome Web] (using WebSerial): was able to connect, but upon writing always encountered a timeout.
- Flashing via espressif's [`esptool.py`] CLI: was able to write the image, but the Powermeter wasn't able to boot afterwards.


## ✅ What did work (for me)

With the Powermeter connected to the laptop, using `esptool.py`:
1. Flashing official firmware
    - Putting the Powermeter in **Flash mode**
    - Flashing the [`smart-powermeter-offline.bin`] image
    - → The Powermeter should now be able to boot and display stuff on the display.
2. Flashing the ESPHome firmware
    - Putting the Powermeter in **operation(!) mode** (i.e. running)
    - Flashing ESPHome-generated 'modern' image
    - → The Powermeter should now boot the custom ESPHome firmware.

### `esptool.py` command used

#### Testing connection:
```shell
esptool.py --port </path/to/device> chip_id
```

#### Writing an image:
```shell
esptool.py --port </path/to/device> --chip esp32s2 --no-stub write_flash --flash_freq 40m --flash_mode dio --spi-connection SPI 0x0 </path/to/firmware.bin>
```


[ESPHome Web]: https://web.esphome.io/
[`esptool.py`]: https://docs.espressif.com/projects/esptool/en/latest/esp32/
[`smart-powermeter-offline.bin`]: https://github.com/JGAguado/Smart_Powermeter/blob/bca3334e352407ce5b41c6b4450c7d4ce1809eb8/docs/source/files/smart-powermeter-offline.bin
