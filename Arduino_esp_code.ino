#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME         "Device"

#define BLYNK_PRINT Serial

#include "WiFiEsp.h"
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>


char ssid[] = "Wifi name";
char pass[] = "Wifi pass";

IPAddress ip(192, 168, 1, 115);

WiFiEspServer server(80);

int led = 9;
String ledStatus="OFF";
int status = WL_IDLE_STATUS;

#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

void setup()
{
  Serial.begin(115200);
  pinMode(led, OUTPUT);

  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(BLYNK_TEMPLATE_ID, wifi, ssid, pass);

  delay(30);
  
  server.begin();
}

void loop()
{
     
  /// Listen for incoming clients
  WiFiEspClient client = server.available();

  if (client) {
    Serial.println("New client");
    
    // An HTTP request ends with a blank line
    boolean currentLineIsBlank = true;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        // If you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          Serial.println("Sending response");

          // Send a standard HTTP response header
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n");
          client.print("<!DOCTYPE HTML>\r\n");
          client.print("<html>\r\n");
          client.print("<head>\r\n");
          client.print("<title>My Arduino</title>\r\n");
          client.print("</head>\r\n");
          client.print("<body>\r\n");
          client.print("<h1>Hello World!</h1>\r\n");
          client.print("<p>We're online!</p>\r\n");
          client.print("</body>\r\n");
          client.print("</html>\r\n");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    
    // Give the web browser time to receive the data
    delay(10);

    // Close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
}
