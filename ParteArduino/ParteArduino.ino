#include <dht.h>

dht DHT;
#define DHT11_PIN A0
#define prender 9
#define foco 10
float temperatura=0;
float humedad=0;
float tempHum[2];
#define leer  11
#define electro 12
int data;


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
  tempHum[0]={temperatura};
  tempHum[1]={humedad};
  int valor = digitalRead(leer);

  for (int i=0; i<2; i++){
    Serial.println(tempHum[i]);
  }

  while(Serial.available()){
    data = Serial.read();
  }
  if(data=='1')
  {
  digitalWrite(prender, HIGH);
  digitalWrite(foco,HIGH);
  }
  else if(data=='0')
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
