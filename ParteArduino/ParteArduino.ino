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
#define leerVentilador  2
#define escribirVentilador 3
#define leerFoco  4
#define escribirFoco  5

void setup() {
  Serial.begin(9600);
  pinMode(prender, OUTPUT);
  pinMode(foco,OUTPUT);
  pinMode(leer,INPUT);
  pinMode(electro,OUTPUT);
  pinMode(leerVentilador,INPUT);
  pinMode(leerFoco, INPUT);
  pinMode(escribirVentilador,OUTPUT);
  pinMode(escribirFoco, OUTPUT);
}

void loop() {
  //temperatura=dht.readTemperature();
  int chk = DHT.read11(DHT11_PIN);
  temperatura= DHT.temperature;  
  humedad=DHT.humidity;
  int valor = digitalRead(leer);
  int valorFoco = digitalRead(leerFoco);
  int valorVentilador = digitalRead(leerVentilador);

  if(valor == HIGH){
    digitalWrite(electro,HIGH);
  }
  else if(valor ==LOW){
    digitalWrite(electro,LOW);
  }

  if(valorFoco ==HIGH){
    digitalWrite(escribirFoco,HIGH);
    digitalWrite(escribirVentilador,HIGH);
  }
  else if(valorFoco==LOW){
    digitalWrite(escribirFoco,LOW);
    digitalWrite(escribirVentilador,LOW);
  }
  

  delay(2000);
}
