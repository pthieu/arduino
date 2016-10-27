/*
 * Connect to WIFI router and POST to route
 */

#include <ESP8266WiFi.h>

void send_event(const char *event);

const char* ssid = "LINKSYSE4200";
const char* password = "TKRUWIFI";

// IFTTT setup
const char *host = "maker.ifttt.com";
const char *privateKey = "cH7bUKjquRCKvGaanH0JFL";

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  pinMode(0, OUTPUT);
  digitalWrite(0, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(1000);
  send_event("hcsr04_garage");
}

void send_event(const char *event)
{
  // set the LED on whle we are sending the event

  Serial.print("Connecting to ");
  Serial.println(host);
  String postData = "value1=" + String(millis()/1000) + String("&") + "value2=" + random(5);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/trigger/";
  url += event;
  url += "/with/key/";
  url += privateKey;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.println(String("POST ") + url + " HTTP/1.1");
  client.println("Host: " + String(host));
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(postData.length());
  client.println();
  client.println(postData);
  client.println("Connection: close\r\n\r\n");

  // Read all the lines of the reply from server and print them to Serial,
  // the connection will close when the server has sent all the data.
  while(client.connected())
  {
    if(client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    } else {
      // No data yet, wait a bit
      delay(50);
    };
  }
  
  // All done
  Serial.println();
  Serial.println("closing connection");

  client.stop();
}
