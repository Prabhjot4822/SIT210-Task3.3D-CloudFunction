#include <Firebase_ESP_Client.h>
#include <WiFi.h>

// Wi-Fi credentials
#define WIFI_SSID "MONGA PG 3.1"
#define WIFI_PASSWORD "MongaBhai@1"

// Pin definitions for LEDs
const int WaveLED = 22;
const int PatLED = 23;

// Firebase configuration
#define API_KEY "AIzaSyBMCHEF8Lw97XKi68Ow_guWtiOQmlS88vQ"
#define DATABASE_URL "https://buddy-4de95-default-rtdb.firebaseio.com/"

FirebaseData firebaseDatabase;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

void setup() 
{
  Serial.begin(9600);

  pinMode(WaveLED, OUTPUT);
  pinMode(PatLED, OUTPUT);

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Configure Firebase API and database URL
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Attempt to sign up with Firebase (if successful, set signupOK flag)
  if (Firebase.signUp(&config, &auth, "", "")) 
  {
    Serial.println("Firebase sign up successful");
    signupOK = true;
  }

  // Initialize Firebase connection and enable WiFi reconnection
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() 
{
  if (Firebase.ready() && signupOK) 
  {
    // Check the status of the "Wave" light
    if (Firebase.RTDB.getString(&firebaseDatabase, "/Light/Wave")) 
    {
      String waveStatus = firebaseDatabase.stringData();
      if (waveStatus == "on") 
      {
        // Turn on the WaveLED
        digitalWrite(WaveLED, HIGH);
        Serial.println("WAVE !!!");
        delay(1000);
        digitalWrite(WaveLED, LOW);
      }
    }

    // Check the status of the "Pat" light
    if (Firebase.RTDB.getString(&firebaseDatabase, "/Light/Pat")) 
    {
      String patStatus = firebaseDatabase.stringData();
      if (patStatus == "on") 
      {
        // Turn on the PatLED
        digitalWrite(PatLED, HIGH);
        Serial.println("PAT !!!");
        delay(1000);
        digitalWrite(PatLED, LOW);
      }
    }
  }
}