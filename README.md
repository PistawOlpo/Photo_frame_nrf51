# Photo_frame_color_epaper_nrf51822xxaa

Another project of a photo frame with 7-color epaper with nordic nrf51822xxaa chip



### You will need:

nrf51822 board (my is nrf51822-02)

7 color epaper from waveshare with spi, 7.3 inch 



### You will need to install (linux- Ubuntu):

openOCD

nordic sdk-12.3.0

gcc-arm-none-eabi-



### Install:
```
make clean
make
make flash
```


### Connecting to the logger (for viewing the log outputs from the nrf51 chip):
`telnet localhost 9090`




## to do:
- PCB project.
- implementing sleep mode.