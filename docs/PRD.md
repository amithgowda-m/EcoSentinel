# Product Requirements Document (PRD)
## Project: Eco-Sentinel (Random Forest Environmental Monitor)

### 1. Executive Summary
An advanced IoT environmental monitor for Civil Engineering Green Buildings. It uses a **Random Forest Machine Learning model** (trained on 10,000 simulated data points) to classify indoor conditions with high accuracy. It features a real-time web dashboard and **Automated Email Alerts** for critical violations.

### 2. Core Objectives
- **Data Source:** DHT22 Sensor (More accurate than DHT11).
- **ML Engine:** Random Forest Classifier running on-device (Edge AI).
- **Output:** Web Dashboard + Email Notifications.

### 3. The Machine Learning Model
- **Input:** Temperature (Float), Humidity (Float).
- **Classes (Outputs):**
  0. **OPTIMAL:** Conditions meet ASHRAE 55 standards (Energy Saving Mode).
  1. **WARNING_HEAT:** Temp is rising; requires cooling.
  2. **WARNING_HUMIDITY:** Humidity is rising; requires dehumidification.
  3. **CRITICAL:** Severe violation (Mold/Health Risk) -> **TRIGGER EMAIL**.
- **Accuracy Target:** >95% on the test set.

### 4. Features
#### A. Web Dashboard
- Must display: Real-time Temp/Hum, Current ML Classification (e.g., "Critical"), and Confidence Score.
- Visuals: Color-coded cards (Green/Orange/Red).

#### B. Email Alerts
- **Trigger:** When ML Class == 3 (CRITICAL) for more than 10 consecutive seconds (to prevent spam).
- **Content:** "URGENT: Lab Environment Critical. Temp: X, Hum: Y. Immediate HVAC Action Required."

### 5. Dataset Simulation (For the Python Script)
- The system requires a script to generate 10,000 rows of synthetic data based on real physics:
  - Ideal Range: 20-26°C, 30-60% Humidity.
  - Critical Range: Temp > 30°C OR Humidity > 75%.
  - The rest are Warning states.