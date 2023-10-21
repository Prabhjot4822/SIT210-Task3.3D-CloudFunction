#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>
#include <Firebase_Arduino_WiFiNINA.h>

// Create instances of required objects
BH1750 lightMeter;
WiFiSSLClient client;

// Wi-Fi credentials
char WIFI_SSID[] = "MONGA PG 3.1";
char WIFI_PASSWORD[] = "MongaBhai@1";

// Firebase database information
#define FIREBASE_HOST "buddy-4de95-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "5oxWbwhxU2EtCJmxtPxL5h976AVstQQdu2vBtMEm"

FirebaseData firebaseData; // Firebase data object

void setup() 
{
  Serial.begin(9600);
  while (!Serial);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Wire.begin();
  lightMeter.begin();

  // Initialize Firebase connection
  initializeFirebase(); 
}

void loop() 
{
  float lux = lightMeter.readLightLevel();

  if (lux < 50) 
  {
    while (lux < 50)
    {  
      delay(2000); //Wait for 2 sec to check if it is long enough to be wave
      lux = lightMeter.readLightLevel();

      if (lux < 50)
      {
        delay(4000); //Respond after 4 sec as its a wave response

        Serial.print("Light: ");
        Serial.print(lux);
        Serial.println(" lx");

        Serial.println("Published : WAVE !!!");

        // Send Firebase data
        Firebase.setString(firebaseData, "/Light/Wave", "on");
        Firebase.setString(firebaseData, "/Light/Pat", "off");

        lux = lightMeter.readLightLevel();
      }
      else
      {
        Serial.print("Light: ");
        Serial.print(lux);
        Serial.println(" lx");

        Serial.println("Published : PAT !!!");

        // Send Firebase data
        Firebase.setString(firebaseData, "/Light/Wave", "off");
        Firebase.setString(firebaseData, "/Light/Pat", "on");
      }

      delay(1000);
      Serial.println("");
    }
  }

  // Reset Firebase data when not needed
  Firebase.setString(firebaseData, "/Light/Wave", "off");
  Firebase.setString(firebaseData, "/Light/Pat", "off");
}

void initializeFirebase()
{
  // Initialize Firebase connection
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);
}