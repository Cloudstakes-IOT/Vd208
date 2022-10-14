#include <WiFi.h>
#include <ESP32Time.h>
ESP32Time rtc(3600);

const char* WIFI_NAME = "CloudStakes";
const char* WIFI_PASSWORD = "Cloud@1234stakes";
WiFiServer server(80);

String header;

String DEVICE_STATE = "";
String Time = "";

int vibr_pin=13;

void setup() {
  Serial.begin(115200);
  rtc.setTime(00, 34, 12, 30, 9, 2022);
  
  pinMode(vibr_pin,INPUT);


  Serial.print("Connecting to ");
  Serial.println(WIFI_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Trying to connect to Wifi Network");
  }
  Serial.println("");
  Serial.println("Successfully connected to WiFi network");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  int val;
  val = digitalRead(vibr_pin);
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client is requesting web page");
    String current_data_line = "";
    while (client.connected()) {
      if (client.available()) {
        char new_byte = client.read();
        Serial.write(new_byte);
        header += new_byte;
        if (new_byte == '\n') {

          if (current_data_line.length() == 0)
          {

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //            if (header.indexOf("LED0=ON") != -1)
            //            {
            //              Serial.println("GPIO23 LED is ON");
            //              LED_ONE_STATE = "on";
            //              digitalWrite(GPIO_PIN_NUMBER_22, HIGH);
            //            }
            //            if (header.indexOf("LED0=OFF") != -1)
            //            {
            //              Serial.println("GPIO23 LED is OFF");
            //              LED_ONE_STATE = "off";
            //              digitalWrite(GPIO_PIN_NUMBER_22, LOW);
            //            }
            //            if (header.indexOf("LED1=ON") != -1)
            //            {
            //              Serial.println("GPIO23 LED is ON");
            //              LED_TWO_STATE = "on";
            //              digitalWrite(GPIO_PIN_NUMBER_23, HIGH);
            //            }
            //            if (header.indexOf("LED1=OFF") != -1)
            //            {
            //              Serial.println("GPIO23 LED is OFF");
            //              LED_TWO_STATE = "off";
            //              digitalWrite(GPIO_PIN_NUMBER_23, LOW);
            //            }
            //            if (header.indexOf("LED2=ON") != -1)
            //            {
            //              Serial.println("GPIO15 LED is ON");
            //              LED_THREE_STATE = "on";
            //              digitalWrite(GPIO_PIN_NUMBER_15, HIGH);
            //            }
            //            if(header.indexOf("LED2=OFF") != -1) {
            //              Serial.println("GPIO15 LED is OFF");
            //              LED_THREE_STATE = "off";
            //              digitalWrite(GPIO_PIN_NUMBER_15, LOW);
            //            }

            Serial.println(val);
            if (val == 0) {
              DEVICE_STATE = "ON";
              Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
              Time = rtc.getTime("%A, %B %d %Y %H:%M:%S");
              struct tm timeinfo = rtc.getTimeStruct();
            }
            
            else{
              DEVICE_STATE = "OFF";
              Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
              Time = rtc.getTime("%A, %B %d %Y %H:%M:%S");
              struct tm timeinfo = rtc.getTimeStruct();
            }


            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: 2px solid #4CAF50;; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            // Web Page Heading
            client.println("</style></head>");
            client.println("<p>DEVICE is " + DEVICE_STATE + "</p>");
            if(val == 0)
            client.println("<p>DEVICE Start " + Time + "</p>");
            else
            
            
            
            
            client.println("</center></form></body></html>");
            client.println();
            break;
          }
          else
          {
            current_data_line = "";
          }
        }
        else if (new_byte != '\r')
        {
          current_data_line += new_byte;
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
//    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
