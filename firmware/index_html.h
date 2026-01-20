#ifndef INDEX_HTML_H
#define INDEX_HTML_H

#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Eco-Sentinel Dashboard</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
        }
        h1 {
            color: white;
            text-align: center;
            margin-bottom: 30px;
            font-size: 2.5em;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
        }
        .dashboard {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 20px;
            margin-bottom: 20px;
        }
        .card {
            background: white;
            border-radius: 15px;
            padding: 25px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.2);
            transition: transform 0.3s ease;
        }
        .card:hover {
            transform: translateY(-5px);
        }
        .card-header {
            font-size: 1.2em;
            font-weight: bold;
            margin-bottom: 15px;
            color: #333;
        }
        .card-value {
            font-size: 2.5em;
            font-weight: bold;
            margin-bottom: 10px;
        }
        .card-unit {
            font-size: 1em;
            color: #666;
        }
        .status-card {
            text-align: center;
        }
        .status-badge {
            display: inline-block;
            padding: 15px 30px;
            border-radius: 25px;
            font-size: 1.5em;
            font-weight: bold;
            color: white;
            margin: 10px 0;
        }
        .status-optimal {
            background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%);
        }
        .status-warning-heat {
            background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
        }
        .status-warning-humidity {
            background: linear-gradient(135deg, #ffecd2 0%, #fcb69f 100%);
        }
        .status-critical {
            background: linear-gradient(135deg, #ff0000 0%, #cc0000 100%);
            animation: pulse 2s infinite;
        }
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.7; }
        }
        .confidence {
            font-size: 1em;
            color: #666;
            margin-top: 10px;
        }
        .last-update {
            text-align: center;
            color: white;
            margin-top: 20px;
            font-size: 0.9em;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🌿 Eco-Sentinel Environmental Monitor</h1>
        <div class="dashboard">
            <div class="card">
                <div class="card-header">Temperature</div>
                <div class="card-value" id="temperature">--</div>
                <div class="card-unit">°C</div>
            </div>
            <div class="card">
                <div class="card-header">Humidity</div>
                <div class="card-value" id="humidity">--</div>
                <div class="card-unit">%</div>
            </div>
            <div class="card status-card">
                <div class="card-header">Current Status</div>
                <div class="status-badge" id="status-badge">Loading...</div>
                <div class="confidence" id="confidence">Confidence: --</div>
            </div>
        </div>
        <div class="last-update" id="last-update">Last updated: --</div>
    </div>
    <script>
        function updateDashboard() {
            fetch('/data')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('temperature').textContent = data.temperature.toFixed(1);
                    document.getElementById('humidity').textContent = data.humidity.toFixed(1);
                    
                    const statusBadge = document.getElementById('status-badge');
                    const confidence = document.getElementById('confidence');
                    const statusNames = {
                        0: { name: 'OPTIMAL', class: 'status-optimal' },
                        1: { name: 'WARNING: HEAT', class: 'status-warning-heat' },
                        2: { name: 'WARNING: HUMIDITY', class: 'status-warning-humidity' },
                        3: { name: 'CRITICAL', class: 'status-critical' }
                    };
                    
                    const status = statusNames[data.status] || statusNames[0];
                    statusBadge.textContent = status.name;
                    statusBadge.className = 'status-badge ' + status.class;
                    
                    confidence.textContent = `Confidence: ${(data.confidence * 100).toFixed(1)}%`;
                    
                    const now = new Date();
                    document.getElementById('last-update').textContent = 
                        'Last updated: ' + now.toLocaleTimeString();
                })
                .catch(error => {
                    console.error('Error fetching data:', error);
                });
        }
        
        // Update immediately and then every 2 seconds
        updateDashboard();
        setInterval(updateDashboard, 2000);
    </script>
</body>
</html>
)rawliteral";

#endif
