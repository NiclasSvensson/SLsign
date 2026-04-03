# SLSIGN

This is code for a RaspberryPi project that attempts to mimic the subway (Stockholm public transportation, SL) sign for upcoming departures on the current platform.

## Compile

```
git clone git@github.com:NiclasSvensson/SLsign.git
cd SLsign
git clone https://github.com/hzeller/rpi-rgb-led-matrix.git
cd rpi-rgb-led-matrix/
make
cd ..
mkdir build
cd build
cmake ..
make
```

## Run

```
sudo ./sl_app --led-no-hardware-pulse --led-rows=32 --led-cols=128
```