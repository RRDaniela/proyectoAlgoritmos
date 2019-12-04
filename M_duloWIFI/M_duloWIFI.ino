#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "iabb2XUncClJZXSnx9KcziMrwHustwAP"; //Enter the Auth code which was send by Blink
#define FIREBASE_HOST "proyectoalgoritmos-d3ba4.firebaseio.com"
#define FIREBASE_AUTH "73Hd45LiGmM3OnxPOUh22Knkgew2wZOcYjNbwMYc"
#define prenderVentilador D6
#define prenderFoco D5
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "HOME_WIFI";  //Enter your WIFI Name
char pass[] = "2439584223";  //Enter your WIFI Password
int maxTemperatura= 30;
int maxHumedad=90;
int bandera = 1;

#define DHTPIN 2          // Digital pin 

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  
  if(temperatura>=maxTemperatura)
  {
  digitalWrite(prenderVentilador, HIGH);
  digitalWrite(prenderFoco,HIGH);
  Serial.println("Ventilador");
  }
  else if(temperatura<=maxTemperatura)
  {
  digitalWrite(prenderVentilador, LOW);
  digitalWrite(prenderFoco,LOW);
  Serial.println("Foco");
  }

  if (isnan(humedad) || isnan(temperatura)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, humedad);  //V5 is for Humidity
  Blynk.virtualWrite(V6, temperatura);  //V6 is for Temperature
  Firebase.setString("Proyecto/Temperatura",String(temperatura));
  Firebase.setString("Proyecto/Humedad",String(humedad));
  if (humedad<maxHumedad && bandera==1){
    Blynk.notify("¡La humedad no es óptima para tus plantas! Aprieta el botón para regar.");
    bandera=0;
  }
  else{
    bandera=1;
  }
}

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth, ssid, pass);

  pinMode(D7, OUTPUT);
  pinMode(prenderFoco,OUTPUT);
  pinMode(prenderVentilador,OUTPUT);
  dht.begin();
  WiFi.begin(ssid, pass);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
      {
    Serial.print(".");
    delay(500);
      }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{ 
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}
