# Arduino IDE Setup Guide for EcoSentinel

## Step 1: Install ESP32 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. In "Additional Board Manager URLs", add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Click **OK**
5. Go to **Tools → Board → Boards Manager**
6. Search for "**esp32**"
7. Install "**esp32 by Espressif Systems**" (latest version)
8. Click **Close**

## Step 2: Install Required Libraries

Go to **Sketch → Include Library → Manage Libraries** and install:

1. **DHT sensor library** by Adafruit
   - This will also install "Adafruit Unified Sensor" automatically

2. **ESP Mail Client** by Mobizt
   - Search for "ESP Mail Client"
   - Install version 4.0.0 or later

## Step 3: Select Board and Port

1. Go to **Tools → Board → ESP32 Arduino → ESP32 Dev Module**
2. Go to **Tools → Port** and select your ESP32 COM port
3. Set **Upload Speed** to **115200** (Tools → Upload Speed)

## Step 4: Configure WiFi and Email

Before uploading, edit these files:

### In `EcoSentinel.ino`:
- Line 10: Change `"YOUR_SSID"` to your WiFi network name
- Line 11: Change `"YOUR_PASS"` to your WiFi password

### In `email_sender.h`:
- Line 7: Change `"your_email@gmail.com"` to your Gmail address
- Line 8: Change `"your_app_password"` to your Gmail App Password
  - **Note:** You need to generate an App Password in your Google Account settings
- Line 9: Change `"recipient@gmail.com"` to the recipient email address

## Step 5: Upload Code

1. Open `EcoSentinel.ino` in Arduino IDE
2. Click the **Upload** button (→ arrow icon)
3. Wait for compilation and upload to complete
4. Open **Tools → Serial Monitor** (set to 115200 baud) to see output

## Step 6: Access Web Dashboard

1. Check Serial Monitor for the ESP32 IP address
2. Open a web browser and go to: `http://[ESP32_IP_ADDRESS]`
3. You should see the Eco-Sentinel dashboard!

## Troubleshooting

- **WiFi not connecting**: Check SSID and password are correct
- **Email not sending**: 
  - Make sure you're using a Gmail App Password (not regular password)
  - Enable "Less secure app access" or use 2FA with App Password
- **DHT22 not reading**: Check wiring (VCC, GND, Data pin to GPIO 4)
- **Compilation errors**: Make sure all libraries are installed correctly

## File Structure

All these files should be in the same folder (`firmware/`):
- `EcoSentinel.ino` (main sketch file)
- `email_sender.cpp`
- `email_sender.h`
- `index_html.h`
- `RandomForest.h`

Arduino IDE will automatically compile all `.cpp` files in the same folder.
