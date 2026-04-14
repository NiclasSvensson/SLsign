# SLSIGN

This is code for a RaspberryPi project that attempts to mimic the subway (Stockholm public transportation, SL) sign for upcoming departures on the current platform.

https://github.com/user-attachments/assets/497651d5-554d-483c-ac5e-83148a0a90fd

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

## Hardware

This is how the Sign is connected.

```text
                AC MAINS INPUT
                  230V / 50 Hz
                        │
                        ▼
        ┌──────────────────────────────┐
        │        POWER SUPPLY          │
        │          5V / 22A            │
        └──────────────┬───────────────┘
                       │
        ┌──────────────┼────────────────────────────────┐
        │              │                                │
        ▼              ▼                                ▼
┌──────────────┐  ┌──────────────────────┐   ┌──────────────────────┐
│ Raspberry Pi │  │   LED Matrix #1      │──►│   LED Matrix #2      │
│  (GPIO Ctrl) │  │  (Panel 1 - Input)   │   │  (Panel 2 - Chained) │
└──────┬───────┘  └──────────────────────┘   └──────────────────────┘
       │                      ▲
       │                      │
       └──────────────────────┘
         GPIO (Data/Control)

   5V POWER DISTRIBUTION:
   Power Supply ───────► Raspberry Pi
   Power Supply ───────► LED Matrix #1
   Power Supply ───────► LED Matrix #2

   GND: All components share common ground
   ```
