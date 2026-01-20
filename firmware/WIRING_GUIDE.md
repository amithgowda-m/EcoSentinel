# ESP32 and DHT22 Wiring Guide

## Pin Connections

Connect the DHT22 sensor to your ESP32 as follows:

| DHT22 Pin | ESP32 Pin | Description |
|-----------|-----------|-------------|
| **VCC** (Pin 1) | **3.3V** | Power supply (3.3V) |
| **DATA** (Pin 2) | **GPIO 4** | Data signal (with pull-up resistor) |
| **GND** (Pin 4) | **GND** | Ground |

**Note:** DHT22 has 4 pins. Pin 3 (NC) is not connected.

## Detailed Connection Diagram

```
DHT22 Sensor          ESP32 Dev Module
┌──────────┐         ┌──────────────┐
│          │         │              │
│  VCC (1) ├────────►│  3.3V        │
│          │         │              │
│  DATA(2) ├───┬────►│  GPIO 4      │
│          │   │     │              │
│   NC (3) │   │     │              │
│          │   │     │              │
│  GND (4) ├───┼────►│  GND         │
│          │   │     │              │
└──────────┘   │     └──────────────┘
               │
               │
           4.7kΩ - 10kΩ
         Pull-up Resistor
         (to 3.3V)
```

## Pull-up Resistor

**IMPORTANT:** The DHT22 data line requires a **pull-up resistor** (4.7kΩ to 10kΩ) between the DATA pin and 3.3V.

### Option 1: External Resistor (Recommended)
- Connect a **4.7kΩ or 10kΩ resistor** between:
  - DHT22 DATA pin (Pin 2) and ESP32 3.3V pin

### Option 2: Internal Pull-up (May work, less reliable)
- The code uses `DHT dht(DHTPIN, DHTTYPE)` which may enable internal pull-up
- If readings are unstable, add external resistor

## Pin Layout Reference

### DHT22 Pinout (front view, pins facing you):
```
    ┌─────────┐
    │  DHT22  │
    │         │
    │  ┌───┐  │
    │  │   │  │
    └──┼───┼──┘
       │   │
    ┌──┘   └──┐
    │         │
   VCC      DATA
  (Pin 1)  (Pin 2)
  
    ┌──┘   └──┐
    │         │
   NC       GND
  (Pin 3)  (Pin 4)
```

### ESP32 Dev Module Common Pins:
- **3.3V**: Power output (pin varies by board, usually labeled)
- **GND**: Ground (multiple GND pins available)
- **GPIO 4**: Digital pin 4 (used in code)

## Wiring Steps

1. **Power Off**: Make sure ESP32 is powered off before wiring

2. **Connect VCC**: 
   - DHT22 Pin 1 (VCC) → ESP32 3.3V

3. **Connect GND**: 
   - DHT22 Pin 4 (GND) → ESP32 GND

4. **Connect DATA with Pull-up**:
   - DHT22 Pin 2 (DATA) → ESP32 GPIO 4
   - Add 4.7kΩ resistor between DATA pin and 3.3V

5. **Double-check**: Verify all connections are secure

6. **Power On**: Connect ESP32 to USB for power

## Testing the Connection

After uploading the code:

1. Open Serial Monitor (115200 baud)
2. You should see:
   ```
   Eco-Sentinel Starting...
   DHT22 sensor initialized
   Temp: XX.X°C, Hum: XX.X%, Status: X, Confidence: X.XX
   ```
3. If you see "Failed to read from DHT22 sensor!", check:
   - Wiring connections
   - Pull-up resistor is connected
   - Power supply is stable
   - GPIO pin number matches code (GPIO 4)

## Changing the GPIO Pin

If you need to use a different GPIO pin:

1. Edit `EcoSentinel.ino` line 14:
   ```cpp
   #define DHTPIN 4  // Change 4 to your desired GPIO pin
   ```

2. **Avoid these pins** (they have special functions):
   - GPIO 0: Boot mode (may cause issues)
   - GPIO 2: Built-in LED (can use but may interfere)
   - GPIO 6-11: Used for flash memory (DO NOT USE)
   - GPIO 34-39: Input only (no pull-up, avoid)

3. **Good alternative pins**: GPIO 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33

## Power Requirements

- **DHT22**: Operates on 3.3V to 5V (we use 3.3V to match ESP32)
- **Current**: ~1-2.5mA when reading
- **ESP32**: Can supply enough power via 3.3V pin

## Troubleshooting

| Problem | Solution |
|---------|----------|
| No readings / "Failed to read" | Check pull-up resistor, verify GPIO 4 connection |
| Erratic readings | Add external 4.7kΩ pull-up resistor, check power supply |
| Sensor gets hot | Wrong power connection - check VCC is 3.3V, not 5V |
| Wrong values | Verify DHT22 (not DHT11) - they have different accuracy |

## Visual Wiring Diagram (Text)

```
ESP32 Dev Module:
┌─────────────────────────┐
│  [USB]                  │
│                         │
│  3.3V ──────┐           │
│             │           │
│  GND  ──────┼───┐       │
│             │   │       │
│  GPIO4 ─────┼───┼───┐   │
│             │   │   │   │
└─────────────┘   │   │   │
                  │   │   │
         ┌────────┘   │   │
         │            │   │
      [4.7kΩ]        │   │
         │            │   │
         └────────────┘   │
                          │
DHT22:                    │
┌──────────┐              │
│ VCC (1)──┼──────────────┘
│ DATA(2)──┼──────────────┐
│  NC (3)  │              │
│ GND (4)──┼──────────────┘
└──────────┘
```

## Safety Notes

- ⚠️ **Never connect DHT22 to 5V** if using 3.3V logic ESP32 (most ESP32 boards)
- ⚠️ **Double-check pin numbers** - DHT22 pins can vary by manufacturer
- ⚠️ **Use proper breadboard** or soldering for stable connections
- ⚠️ **Keep wires short** to reduce interference
