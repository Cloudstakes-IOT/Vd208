
#include <WiFi.h>
#include <HTTPClient.h>

#include <Wire.h>


// Replace with your network credentials
const char* ssid     = "CloudStakes";
const char* password = "Cloud@1234stakes";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://40.77.64.56/dbvd208.php";


String apiKeyValue = "tPmAT5Ab3j7F9";
String DEVICE_STATE = "";

int vibr_pin=13;

void setup() {
  Serial.begin(115200);
  pinMode(vibr_pin,INPUT);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int val;
  val = digitalRead(vibr_pin);
  if (val == 0) {
  DEVICE_STATE = "ON";
  }
  else{
    DEVICE_STATE = "OFF";
  }
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(DEVICE_STATE);
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
//  delay(30000);  
}
