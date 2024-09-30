#include <DHT.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define DHTPIN D2  // Define the digital pin to which DHT11 is connected
#define DHTTYPE DHT11  // Define the type of DHT sensor (DHT11 or DHT22)

DHT dht(DHTPIN, DHTTYPE);


#define DATABASE_URL "babylog-68a4a-default-rtdb.europe-west1.firebasedatabase.app" // Without http:// or https:// schemes
#define API_KEY "AIzaSyA7c-29lTcqP7UEXUPltcU1S6n3W_4Uh70"
#define WIFI_SSID "Fuiorea"
#define WIFI_PASSWORD "fuiorea66"
#define USER_EMAIL "mynewuser@gmail.com"
#define USER_PASSWORD "arduinowifisensor"

// Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

const int NUM_OF_READINGS = 30;

float tempReadings[NUM_OF_READINGS];
float humidityReadings[NUM_OF_READINGS];

typedef struct struct_message
{
  float temp;
  float humidity;
  unsigned long milis;
} struct_message;


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

 while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  // /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // /* Assign the RTDB URL (required) */
  // config.database_url = DATABASE_URL;

  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = API_KEY;

  Firebase.begin(&config, &auth);

  delay(2000);  // Delay to let system boot
  Serial.println("DHT11 Test");
  dht.begin();
}

void storeCurrentVals(){
  for(int i = 0; i < NUM_OF_READINGS; i++){

    // Read temperature as Celsius
    float temperature = dht.readTemperature();

    // Read humidity
    float humidity = dht.readHumidity();

    // Check if any reads failed and exit early (to try again).
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    tempReadings[i] = temperature;
    humidityReadings[i] = humidity;

    delay(2000);
  }

}

float getAvgVal(float val[]){
  float totalVal = 0;
  int arraySize = sizeof(val) / sizeof(val[0]);
  for(int i = 0; i < arraySize; i++){
    totalVal += val[i];
  }
  return totalVal / arraySize;
}

void resetStoredVals(){
  for(int i = 0; i < NUM_OF_READINGS; i++){
    tempReadings[i] = 0;
    humidityReadings[i] = 0;
  }
}

void loop() {
  resetStoredVals();
  storeCurrentVals();

  float avgTemp = getAvgVal(tempReadings);
  float avgHumidity = getAvgVal(humidityReadings);

  FirebaseJson json;

  String path = "/FirebaseIOT/sensor_interior/values";
  String latestDataPath = "/FirebaseIOT/sensor_interior/latestData";

  json.set("temperature", avgTemp);  
  json.set("humidity", avgHumidity);
  // now we will set the timestamp value at Ts
  json.set("timestamp/.sv", "timestamp"); // .sv is the required place holder for sever value which currently supports only string "timestamp" as a value

  Serial.printf("Add data to values... %s\n", Firebase.pushJSON(firebaseData, path, json) ? firebaseData.to<FirebaseJson>().raw() : firebaseData.errorReason().c_str());
  Serial.printf("Add data latest value... %s\n", Firebase.setJSON(firebaseData, latestDataPath, json) ? firebaseData.to<FirebaseJson>().raw() : firebaseData.errorReason().c_str());

  // Print the temperature and humidity values to the Serial Monitor.
  Serial.print("Temperature: ");
  Serial.print(avgTemp);
  Serial.print(" °C, Humidity: ");
  Serial.print(avgHumidity);
  Serial.println("%");
  delay(2000);
}