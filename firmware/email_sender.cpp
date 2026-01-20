#include "email_sender.h"
#include <ESP_Mail_Client.h>

SMTPSession smtp;

bool initEmailSender() {
    // Just verify configuration, don't connect yet
    // Connection will be established when sending email
    Serial.println("Email sender configuration ready");
    return true;
}

bool sendCriticalAlert(float temperature, float humidity) {
    // Configure SMTP session
    smtp.debug.enable(false);
    
    ESP_Mail_Session session;
    session.server.host_name = SMTP_SERVER;
    session.server.port = SMTP_PORT;
    session.login.email = SENDER_EMAIL;
    session.login.password = SENDER_PASSWORD;
    session.login.user_domain = "";
    
    // Set SSL/TLS
    session.time.ntp_server = "pool.ntp.org";
    session.time.gmt_offset = 0;
    session.time.day_light_offset = 0;
    
    // Connect to SMTP server
    if (!smtp.connect(&session)) {
        Serial.println("Failed to connect to SMTP server: " + smtp.errorReason());
        return false;
    }
    
    // Create email message
    SMTP_Message message;
    
    message.sender.name = "Eco-Sentinel";
    message.sender.email = SENDER_EMAIL;
    message.subject = "URGENT: Lab Environment Critical";
    message.addRecipient("Recipient", RECIPIENT_EMAIL);
    
    // HTML email content
    String htmlMsg = "<html><body>";
    htmlMsg += "<h2 style='color: red;'>URGENT: Lab Environment Critical</h2>";
    htmlMsg += "<p><strong>Immediate HVAC Action Required</strong></p>";
    htmlMsg += "<p>Temperature: <strong>" + String(temperature, 1) + "°C</strong></p>";
    htmlMsg += "<p>Humidity: <strong>" + String(humidity, 1) + "%</strong></p>";
    htmlMsg += "<p>The environmental conditions have exceeded safe thresholds.</p>";
    htmlMsg += "<p>This is an automated alert from the Eco-Sentinel monitoring system.</p>";
    htmlMsg += "</body></html>";
    
    message.html.content = htmlMsg.c_str();
    message.text.charSet = "us-ascii";
    message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
    
    // Send email
    if (!MailClient.sendMail(&smtp, &message)) {
        Serial.println("Error sending email: " + smtp.errorReason());
        smtp.closeSession();
        return false;
    }
    
    Serial.println("Critical alert email sent successfully");
    smtp.closeSession();
    return true;
}
