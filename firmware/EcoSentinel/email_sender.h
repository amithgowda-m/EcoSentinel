#ifndef EMAIL_SENDER_H
#define EMAIL_SENDER_H

#include <Arduino.h>

// Email configuration - Update these with your credentials
#define SENDER_EMAIL "amithgowda0071@gmail.com"
#define SENDER_PASSWORD "11221122wwss"
#define RECIPIENT_EMAIL "gowdaamithm@gmail.com"
#define SMTP_SERVER "smtp.gmail.com"
#define SMTP_PORT 465

// Function declarations
bool initEmailSender();
bool sendCriticalAlert(float temperature, float humidity);

#endif
