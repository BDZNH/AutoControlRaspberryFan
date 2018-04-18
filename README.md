# AutoControlRaspberryFan
## Usage
Auto control raspberry fan with 5V. Turn the fan when the tempreture is high than 45°C, turn off fan when the CPU tempreture is lower than 40°C.
## Build
compile with g++.
```bash
g++ main.cpp -lwiringPi -lpthread
```
## GPIO
![引脚定义](rpi-pins-40-0.png)(http://shumeipai.nxez.com/raspberry-pi-pins-version-40)
