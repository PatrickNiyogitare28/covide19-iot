#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
WiFiClient wifiClient;
int redPin = 14; // Red LED pin
int greenPin = 16; // Green LED pin
int buzzer = 12; // Buzzer Pin
LiquidCrystal_I2C lcd(0x27, 16, 2);
const char* host = "192.168.1.167";
void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(redPin, OUTPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(greenPin, OUTPUT);
    lcd.begin();
    lcd.backlight();

    WiFi.begin("RCA-WiFi", "rca@2019");
}
void loop() {
    // continous checking of temperature
    int rawData = analogRead(A0);
    float vcc = 5.0;
    float voltage = rawData * (vcc / 1024.0);
    float temperature = (voltage * 100);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");
    delay(500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(temperature);
    if(temperature > 35){
        digitalWrite(greenPin,LOW);
        digitalWrite(redPin,HIGH);
        digitalWrite(buzzer, HIGH);   // turn the buzzer on (HIGH is the voltage level)
        delay(100);
        String mData="";
        String device_name = "340722SPE0002022"; // My Index number
        mData = "device="+device_name+"&temperature="+(String) temperature;
        
        // Send to a remote server
        sendData(80, "192.168.1.150", "/iot/" , mData);
        
         // Send to a local server
        String request = "POST /iot?device="+device_name+"&temperature="+(String)temperature+" HTTP/1.1";
        wifiClient.connect(host, 8000);
        wifiClient.println(request);
        wifiClient.println("Host: 192.168.1.167");
        wifiClient.println("User-Agent: ESP8266/1.0");
        wifiClient.println("C: ESP8266/1.0");
        wifiClient.println();
        Serial.println("Response : "+wifiClient.readStringUntil('\n'));
        delay(1000);
        //end
   }
    else{
      digitalWrite(greenPin,HIGH);
      digitalWrite(redPin,LOW);
      digitalWrite(buzzer, LOW);    // turn the buzzer off by making the voltage LOW
      delay(1000);
        }
   delay(500);
}
void sendData(const int Port, const char* host,const char* filepath , String data){
  wifiClient.connect(host, Port);
  wifiClient.println("POST "+(String)filepath+data+" HTTP/1.1");
  wifiClient.println("Host: " + (String)host);
  wifiClient.println("User-Agent: ESP8266/1.0");
  wifiClient.print(data);
  Serial.println("Response: " + wifiClient.readStringUntil('\n'));
}
