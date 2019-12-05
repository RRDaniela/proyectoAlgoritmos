  
#include <SPI.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <DHT.h>
//#include <WiFi.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
WiFiClient client;
#define BLYNK_PRINT Serial  
#define FIREBASE_HOST "proyectoalgoritmos-d3ba4.firebaseio.com"
#define FIREBASE_AUTH "73Hd45LiGmM3OnxPOUh22Knkgew2wZOcYjNbwMYc"
#define prenderVentilador D6
#define prenderFoco D5
#define DHTPIN 2          // Pin para leer 
#define DHTTYPE DHT11     // DHT 11
#define IP "192.168.1.72"
#define PUERTO 8000

char auth[] = "iabb2XUncClJZXSnx9KcziMrwHustwAP";
char ssid[] = "HOME_WIFI";  //Nombre WIFI
char pass[] = "2439584223";  //Contraseña WIFI
int maxTemperatura= 30;
int maxHumedad=90;
int bandera = 1;
int invernadero = 6;
String host = "192.168.1.72";
String hosts = "192.168.1.72";


DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
void sendSensor()
{
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature(); 
  String temp="";
  String hum="";
  String inv="";
  temp=String(temperatura);
  hum=String(humedad);
  inv=String(invernadero);
  String PostData="id_green"+inv+"&temp"+temp+"&hum"+hum;
  if(client.connect(host,8000)){
    Serial.println("Si entra al post request: ");
    
    client.println("POST /greenBoy/webservices/InvAPI HTTP/1.1");
    client.println("Host : "+hosts);
    client.println("Accept: *");
    client.println("Content-Type: application/json");
    client.println("cache-control: no-cache");
    client.println(PostData.length());
    client.println();
    client.println(PostData);
    long interval = 2000;
    unsigned long currentMillis = millis(), previousMillis = millis();

    while(!client.available()){

      if( (currentMillis - previousMillis) > interval ){

        Serial.println("Timeout");
        client.stop();     
        return;
      }
      currentMillis = millis();
    }

    while (client.connected())
    {
      if ( client.available() )
      {
        char str=client.read();
        Serial.println(str);
      }      
    }
  }else{
    Serial.println("No se conecto ");
  }
  
  Serial.println("PostData: "+PostData);
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

  Blynk.virtualWrite(V5, humedad);  //V5 para humedad
  Blynk.virtualWrite(V6, temperatura);  //V6 para temperatura
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
  
  Serial.begin(9600);
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
  // Llamar a una función cada segundo. 
  timer.setInterval(1000L, sendSensor); 
}

void loop()
{ 
  Blynk.run(); 
  timer.run(); 
  
}
