#include <dht.h>

dht DHT;
#define DHT11_PIN A0
#define prender 9
#define foco 10
float temperatura=0;
float humedad=0;
#define leer  11
#define electro 12
int data;
int temperaturaMax=30;

void setup() {
  Serial.begin(9600);
  pinMode(prender, OUTPUT);
  pinMode(foco,OUTPUT);
  pinMode(leer,INPUT);
  pinMode(electro,OUTPUT);
}

void loop() {
  //temperatura=dht.readTemperature();
  int chk = DHT.read11(DHT11_PIN);
  temperatura= DHT.temperature;  
  humedad=DHT.humidity;
  int valor = digitalRead(leer);
  
  if(temperatura>=temperaturaMax)
  {
  digitalWrite(prender, HIGH);
  digitalWrite(foco,HIGH);
  }
  else if(temperatura<=temperaturaMax)
  {
  digitalWrite(prender, LOW);
  digitalWrite(foco,LOW);
  }
  if(valor == HIGH){
    digitalWrite(electro,HIGH);
  }
  else if(valor ==LOW){
    digitalWrite(electro,LOW);
  }
  
  delay(2000);
}
