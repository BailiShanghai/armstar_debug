
# User Guidance

---
## 1 BootROM
*   **bootrom.mk**
    change the version number-'VERSION_MAJOR' 'VERSION_MINOR'
    change the MSP size according to your own demand-'CONFIG_MSP_STACK_SIZE'

---
## 2 BootLoader
*   **bootloader.mk**
    change the primary/recovery bootloader version number-'VERSION_MAJOR' 'VERSION_MINOR'
    change the primary/recovery bootloader MSP size according to your own demand-'CONFIG_MSP_STACK_SIZE'

---
## 3 Config
*   **mk/config.mk**
    when change 'SECURE_BOOT' to 'YES', the app in "apps/secure_boot" will veriry the integrity of all images.
    when change 'SECURE_DBG' to 'YES', the app in "apps/primary_bootloader apps/reocvery_bootloader" will disable the debug feature.

---
## 3 Build flags

*   **CROSS_COMPILE**
    Specifies the compiler prefix in case of cross compile. Default to
    `arm-none-eabi-`. Please make sure path to ${CROSS_COMPILE}gcc is
    included in your local PATH environment.

    The toolchain link:
    https://developer.arm.com/-/media/Files/downloads/gnu-rm/7-2018q2/gcc-arm-none-eabi-7-2018-q2-update-linux.tar.bz2?revision=bc2c96c0-14b5-4bb4-9f18-bceb4050fee7?product=GNU%20Arm%20Embedded%20Toolchain,64-bit,,Linux,7-2018-q2-update

*   **V**
    Boolean flag that is used to turn on verbose build. Default to 0.

*   **DEBUG**
    Boolean flag that is used to turn on debug build type. Default to 0.
