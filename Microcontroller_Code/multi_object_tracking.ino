#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Cloud endpoint
const char* serverURL = "https://your-backend.com/updateLocation";

// IR sensor pins
const int sensors[] = {14, 27, 26, 25};
const char* zones[] = {"Top Left", "Top Right", "Bottom Left", "Bottom Right"};
const int numSensors = 4;

int sensorStates[numSensors] = {0, 0, 0, 0};

void setup() {
  Serial.begin(115200);

  for(int i=0; i<numSensors; i++){
    pinMode(sensors[i], INPUT);
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void loop() {
  String detectedZones = "";
  for(int i=0; i<numSensors; i++){
    sensorStates[i] = digitalRead(sensors[i]);
    if(sensorStates[i] == HIGH){
      if(detectedZones.length() > 0) detectedZones += ", ";
      detectedZones += zones[i];
    }
  }

  if(detectedZones.length() > 0){
    Serial.println("Detected in zones: " + detectedZones);
    sendLocationToCloud(detectedZones);
  }

  delay(1000);
}

void sendLocationToCloud(String zoneList){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");

    String payload = "{"zones":"" + zoneList + "","timestamp":"" + String(millis()) + ""}";
    int httpResponseCode = http.POST(payload);
    if(httpResponseCode>0){
      Serial.println("Cloud updated successfully");
    } else {
      Serial.println("Error sending data: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("WiFi disconnected");
  }
}
