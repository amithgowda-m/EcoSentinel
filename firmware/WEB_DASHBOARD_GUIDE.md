# How to Access the Eco-Sentinel Web Dashboard

## Quick Start

The web server runs automatically on your ESP32 once the code is uploaded. Follow these steps:

## Step 1: Upload Code to ESP32

1. Open `EcoSentinel.ino` in Arduino IDE
2. Make sure WiFi credentials are set in the code (lines 10-11)
3. Upload the code to your ESP32
4. Keep the ESP32 connected via USB

## Step 2: Find Your ESP32 IP Address

### Method 1: Serial Monitor (Easiest)

1. Open **Tools → Serial Monitor** in Arduino IDE
2. Set baud rate to **115200**
3. Look for this message:
   ```
   WiFi connected!
   IP address: 192.168.1.XXX
   ```
4. **Copy the IP address** (e.g., `192.168.1.105`)

### Method 2: Router Admin Panel

1. Log into your router's admin panel (usually `192.168.1.1` or `192.168.0.1`)
2. Look for connected devices
3. Find "ESP32" or check MAC address

### Method 3: Network Scanner

Use a network scanning app on your phone or computer to find the ESP32 device.

## Step 3: Access the Dashboard

1. **Open any web browser** on a device connected to the same WiFi network
2. **Type the IP address** in the address bar:
   ```
   http://192.168.1.XXX
   ```
   (Replace XXX with your actual IP address)

3. **Press Enter**

You should see the **Eco-Sentinel Dashboard** with:
- Real-time temperature display
- Real-time humidity display
- Current status (Optimal/Warning/Critical)
- Confidence score
- Auto-updating every 2 seconds

## Example

If your Serial Monitor shows:
```
WiFi connected!
IP address: 192.168.1.105
```

Then open in browser:
```
http://192.168.1.105
```

## Dashboard Features

### Real-time Updates
- The dashboard **automatically refreshes** every 2 seconds
- No need to manually reload the page
- Shows "Last updated" timestamp

### Status Indicators

| Status | Color | Meaning |
|--------|-------|---------|
| **OPTIMAL** | 🟢 Green | Conditions meet ASHRAE standards |
| **WARNING: HEAT** | 🟠 Orange | Temperature rising, needs cooling |
| **WARNING: HUMIDITY** | 🟡 Yellow | Humidity rising, needs dehumidification |
| **CRITICAL** | 🔴 Red | Severe violation - Email alert triggered |

### Data Display
- **Temperature**: Displayed in °C
- **Humidity**: Displayed in %
- **Confidence**: ML model confidence score (0-100%)

## Troubleshooting

### "Can't Connect" or "Site Can't Be Reached"

**Problem**: Browser can't find the ESP32

**Solutions**:
1. ✅ Check ESP32 is connected to WiFi (check Serial Monitor)
2. ✅ Verify you're on the **same WiFi network** as ESP32
3. ✅ Double-check the IP address is correct
4. ✅ Make sure ESP32 is powered on and code is running
5. ✅ Try `http://` not `https://`

### Dashboard Shows "--" or No Data

**Problem**: Sensor not reading or web server not responding

**Solutions**:
1. ✅ Check Serial Monitor for errors
2. ✅ Verify DHT22 is wired correctly (see WIRING_GUIDE.md)
3. ✅ Check if you see sensor readings in Serial Monitor:
   ```
   Temp: XX.X°C, Hum: XX.X%, Status: X, Confidence: X.XX
   ```
4. ✅ Restart ESP32 (unplug and replug USB)

### IP Address Changes After Reboot

**Problem**: ESP32 gets a new IP address each time

**Solutions**:
1. **Check Serial Monitor** each time you restart to get new IP
2. **Set Static IP** (advanced - see below)

### Dashboard Not Auto-Updating

**Problem**: Data stays the same

**Solutions**:
1. ✅ Refresh the browser page (F5)
2. ✅ Check browser console for JavaScript errors (F12)
3. ✅ Verify ESP32 is still connected (check Serial Monitor)

## Accessing from Different Devices

### Same WiFi Network Required

You can access the dashboard from:
- ✅ **Computer** (Windows/Mac/Linux) - Same WiFi
- ✅ **Phone** (Android/iOS) - Same WiFi
- ✅ **Tablet** - Same WiFi
- ❌ **Different network** - Won't work (ESP32 is on local network only)

### Mobile Access Example

1. Connect your phone to the same WiFi as ESP32
2. Open mobile browser (Chrome, Safari, etc.)
3. Type: `http://192.168.1.XXX`
4. Dashboard will work on mobile (responsive design)

## Advanced: Setting Static IP (Optional)

If you want the ESP32 to always have the same IP address, add this to `setup()` in `EcoSentinel.ino`:

```cpp
// Add after WiFi.mode(WIFI_STA);
IPAddress local_IP(192, 168, 1, 100);  // Choose an IP
IPAddress gateway(192, 168, 1, 1);     // Your router IP
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);

if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
    Serial.println("STA Failed to configure");
}
```

Then use `192.168.1.100` (or your chosen IP) to access the dashboard.

## API Endpoint

The dashboard uses a JSON API endpoint:

**URL**: `http://[ESP32_IP]/data`

**Response**:
```json
{
  "temperature": 23.5,
  "humidity": 45.2,
  "status": 0,
  "confidence": 0.95
}
```

You can use this endpoint for custom integrations or monitoring scripts.

## Status Codes Reference

| Code | Status Name | Description |
|------|-------------|-------------|
| 0 | OPTIMAL | 20-26°C AND 30-60% humidity |
| 1 | WARNING_HEAT | Temperature outside ideal range |
| 2 | WARNING_HUMIDITY | Humidity outside ideal range |
| 3 | CRITICAL | Temp >32°C OR Humidity >80% (triggers email) |

## Quick Checklist

Before accessing dashboard:
- [ ] Code uploaded to ESP32
- [ ] WiFi credentials configured correctly
- [ ] ESP32 connected to WiFi (check Serial Monitor)
- [ ] IP address noted from Serial Monitor
- [ ] Device (computer/phone) on same WiFi network
- [ ] Browser opened with `http://[IP_ADDRESS]`

## Still Having Issues?

1. **Check Serial Monitor** - Look for error messages
2. **Verify WiFi** - Make sure ESP32 shows "WiFi connected!"
3. **Test Connection** - Try pinging the IP address:
   - Windows: `ping 192.168.1.XXX`
   - Mac/Linux: `ping 192.168.1.XXX`
4. **Restart Everything** - Unplug ESP32, wait 5 seconds, plug back in

The web server starts automatically when the ESP32 boots up - no additional setup needed!
