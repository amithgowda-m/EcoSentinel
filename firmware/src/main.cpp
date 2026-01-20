#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include "RandomForest.h"
#include "email_sender.h"
#include "index_html.h"

// WiFi credentials - UPDATE THESE
const char* ssid = "";
const char* password = "";

// DHT22 sensor configuration
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Web server
WebServer server(80);

// Email alert state
unsigned long criticalStartTime = 0;
const unsigned long CRITICAL_DURATION_MS = 10000; // 10 seconds
bool emailSent = false;
bool emailInitialized = false;

// Sensor data
struct SensorData {
    float temperature;
    float humidity;
    int status;
    float confidence;
} currentData;

// Function declarations
void handleRoot();
void handleData();
void updateSensorData();
void checkCriticalState(int predictedStatus);

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("Eco-Sentinel Starting...");
    
    // Initialize DHT22 sensor
    dht.begin();
    Serial.println("DHT22 sensor initialized");
    
    // Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nWiFi connection failed!");
    }
    
    // Initialize email sender
    if (initEmailSender()) {
        emailInitialized = true;
        Serial.println("Email sender initialized");
    } else {
        Serial.println("Email sender initialization failed");
    }
    
    // Setup web server routes
    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.begin();
    Serial.println("Web server started");
    
    // Initialize sensor data
    currentData.temperature = 0.0;
    currentData.humidity = 0.0;
    currentData.status = 0;
    currentData.confidence = 0.0;
    
    Serial.println("Eco-Sentinel Ready!");
}

void loop() {
    server.handleClient();
    
    // Read sensor and update predictions every 2 seconds
    static unsigned long lastSensorRead = 0;
    if (millis() - lastSensorRead >= 2000) {
        updateSensorData();
        lastSensorRead = millis();
    }
    
    // Check critical state and trigger email if needed
    checkCriticalState(currentData.status);
}

void handleRoot() {
    server.send_P(200, "text/html", index_html);
}

void handleData() {
    // Return JSON data
    String json = "{";
    json += "\"temperature\":" + String(currentData.temperature, 1) + ",";
    json += "\"humidity\":" + String(currentData.humidity, 1) + ",";
    json += "\"status\":" + String(currentData.status) + ",";
    json += "\"confidence\":" + String(currentData.confidence, 3);
    json += "}";
    
    server.send(200, "application/json", json);
}

void updateSensorData() {
    // Read temperature and humidity from DHT22
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    
    // Check if readings are valid
    if (isnan(temp) || isnan(hum)) {
        Serial.println("Failed to read from DHT22 sensor!");
        return;
    }
    
    currentData.temperature = temp;
    currentData.humidity = hum;
    
    // Use RandomForest to predict status
    // RandomForest expects an array: [temperature, humidity]
    float features[2] = {temp, hum};
    Eloquent::ML::Port::RandomForest rf;
    int predictedStatus = rf.predict(features);
    currentData.status = predictedStatus;
    
    // For confidence, we'll use a simple heuristic based on how far from thresholds
    // In a real implementation, you might want to modify RandomForest to return probabilities
    float confidence = 0.85; // Default confidence
    if (predictedStatus == 0) {
        // Optimal: high confidence if well within range
        if (temp >= 22 && temp <= 24 && hum >= 40 && hum <= 50) {
            confidence = 0.95;
        }
    } else if (predictedStatus == 3) {
        // Critical: high confidence if very extreme
        if (temp > 35 || hum > 85) {
            confidence = 0.98;
        }
    }
    currentData.confidence = confidence;
    
    // Print to serial for debugging
    Serial.print("Temp: ");
    Serial.print(temp, 1);
    Serial.print("°C, Hum: ");
    Serial.print(hum, 1);
    Serial.print("%, Status: ");
    Serial.print(predictedStatus);
    Serial.print(", Confidence: ");
    Serial.println(confidence, 2);
}

void checkCriticalState(int predictedStatus) {
    if (predictedStatus == 3) { // CRITICAL
        if (criticalStartTime == 0) {
            // Start timing critical state
            criticalStartTime = millis();
            emailSent = false;
        } else {
            // Check if critical state has persisted for more than 10 seconds
            unsigned long criticalDuration = millis() - criticalStartTime;
            if (criticalDuration >= CRITICAL_DURATION_MS && !emailSent) {
                // Trigger email alert
                if (emailInitialized) {
                    Serial.println("Critical state detected for >10s, sending email...");
                    if (sendCriticalAlert(currentData.temperature, currentData.humidity)) {
                        emailSent = true;
                        Serial.println("Email alert sent successfully");
                    } else {
                        Serial.println("Failed to send email alert");
                    }
                } else {
                    Serial.println("Email not initialized, cannot send alert");
                }
            }
        }
    } else {
        // Reset critical state tracking
        criticalStartTime = 0;
        emailSent = false;
    }
}
