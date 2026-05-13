#include <Arduino.h>
#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>

#ifndef led
int led[8] = { 16, 17, 5, 18, 19, 21, 22, 23 };
#endif
int button[8] = { 13, 12, 14, 27, 15, 4, 33, 32 };

int ledState[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
int buttonState[8];
int lastButtonState[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

unsigned long lastDebounceTime[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned long debounceDelay = 50;
const char *ssid = "huju";
const char *password = "00000000";

int reading[8];

NetworkServer server(80);

void setup() {
  for (int i = 0; i <= 7; i++) {
    pinMode(button[i], INPUT_PULLUP);
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i], ledState[i]);
  }

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  NetworkClient client = server.accept();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("Click <a href=\"/H1\">here</a> to toggle LED1.<br>");
            client.print("<br>");

            client.print("Click <a href=\"/H2\">here</a> to toggle LED2.<br>");
            client.print("<br>");

            client.print("Click <a href=\"/H3\">here</a> to toggle LED3.<br>");
            client.print("<br>");

            client.print("Click <a href=\"/H4\">here</a> to toggle LED4.<br>");
            client.print("<br>");

            client.print("Click <a href=\"/H5\">here</a> to toggle LED5.<br>");
            client.print("<br>");

            client.print("Click <a href=\"/H6\">here</a> to toggle LED6.<br>");
            client.print("<br>");

            client.print("Click <a href=\"/H7\">here</a> to toggle LED7.<br>");
            client.print("<br>");

            client.print("Click <a href=\"/H8\">here</a> to toggle LED8.<br>");
            
            client.println();

            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;  
        }

        if (currentLine.endsWith("GET /H1")) {
          ledState[0] = !ledState[0];
        }
        
        if (currentLine.endsWith("GET /H2")) {
          ledState[1] = !ledState[1];
        }
        

        if (currentLine.endsWith("GET /H3")) {
          ledState[2] = !ledState[2];
        }
        

        if (currentLine.endsWith("GET /H4")) {
          ledState[3] = !ledState[3];
        }
        

        if (currentLine.endsWith("GET /H5")) {
          ledState[4] = !ledState[4];
        }
        

        if (currentLine.endsWith("GET /H6")) {
          ledState[5] = !ledState[5];
        }
        

        if (currentLine.endsWith("GET /H7")) {
          ledState[6] = !ledState[6];
        }
        

        if (currentLine.endsWith("GET /H8")) {
          ledState[7] = !ledState[7];
        }
      }
    }
    
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
  for (int i = 0; i <= 7; i++) {
    reading[i] = digitalRead(button[i]);

    if (reading[i] != lastButtonState[i]) {

      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {

      if (reading[i] != buttonState[i]) {
        buttonState[i] = reading[i];

        if (buttonState[i] == HIGH) {
          ledState[i] = !ledState[i];
        }
      }
    }

    digitalWrite(led[i], ledState[i]);

    lastButtonState[i] = reading[i];
  }
}
