import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
# You might need to install micromlgen: pip install micromlgen
from micromlgen import port 

# 1. SIMULATE 10,000 ROWS OF DATA (ASHRAE STANDARDS)
print("Generating 10,000 simulated data points...")
np.random.seed(42)

# Generate random Temp (15C to 40C) and Hum (20% to 95%)
temps = np.random.uniform(15, 40, 10000)
hums = np.random.uniform(20, 95, 10000)
labels = []

for t, h in zip(temps, hums):
    # Logic for Classification
    # 0 = Optimal (20-26C AND 30-60%)
    if 20 <= t <= 26 and 30 <= h <= 60:
        labels.append(0)
    # 3 = Critical (Very Hot > 32 OR Very Humid > 80)
    elif t > 32 or h > 80:
        labels.append(3)
    # 1 = Warning Temp (Outside ideal temp, but humidity ok)
    elif (t < 20 or t > 26) and 30 <= h <= 60:
        labels.append(1)
    # 2 = Warning Humidity (Temp ok, but humidity bad)
    else:
        labels.append(2)

data = pd.DataFrame({'Temperature': temps, 'Humidity': hums, 'Label': labels})

# 2. TRAIN RANDOM FOREST
print("Training Random Forest...")
X = data[['Temperature', 'Humidity']]
y = data['Label']

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

# Important: Limit depth and estimators for ESP32 RAM!
clf = RandomForestClassifier(n_estimators=10, max_depth=7, random_state=42)
clf.fit(X_train, y_train)

# 3. CHECK ACCURACY
preds = clf.predict(X_test)
acc = accuracy_score(y_test, preds)
print(f"Model Accuracy: {acc * 100:.2f}%")

# 4. EXPORT TO C++ (Using micromlgen logic manually if lib not present, 
# or standard port function)
print("Exporting to C++ header...")
c_code = port(clf, classname="RandomForest")

# Write to file
with open("../firmware/include/RandomForest.h", "w") as f:
    f.write(c_code)

print("Done! 'RandomForest.h' created in firmware/include/")