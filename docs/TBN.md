# Technical Build Notes (TBN)

### 1. Technology Stack
- **ML Training:** Python (Scikit-Learn, Pandas). Use `micromlgen` porting logic to convert the Random Forest to C++ code.
- **Firmware:** Arduino C++ for ESP32.
- **Sensor:** DHT22 (Pin GPIO 4).
- **Email:** `ESP_Mail_Client` library (SMTP).

### 2. Implementation Steps (Cursor Instructions)
1.  **Step 1 (Python):** Create `ml_training/generate_data.py` to make `dataset.csv` with 10k rows based on ASHRAE logic.
2.  **Step 2 (Python):** Create `ml_training/train_model.py` to train a Random Forest (limit max_depth=10, n_estimators=10 to fit in ESP32 RAM). It must output the C++ code into `firmware/include/RandomForest.h`.
3.  **Step 3 (C++):** Implement `firmware/src/main.cpp`. It should include `RandomForest.h` and run `predict(temp, hum)` in the loop.
4.  **Step 4 (Email):** Implement `email_sender.cpp` using a Gmail SMTP server configuration.

### 3. Constraints
- **Model Size:** The Random Forest must be lightweight. Do not use 100 trees; use approx 10-15 trees.
- **Blocking:** Email sending takes time. Ensure it does not block the Web Server. Use a timer flag (e.g., `sendEmailFlag`) and handle it separately.
- **Web UI:** Store HTML in `firmware/include/index_html.h` as `PROGMEM`.

### 4. Config Variables
- **WiFi:** `YOUR_SSID`, `YOUR_PASS`.
- **Email:** `SENDER_EMAIL`, `SENDER_PASSWORD`, `RECIPIENT_EMAIL`.