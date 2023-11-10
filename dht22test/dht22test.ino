#include "DHT.h"

DHT dht(A1, DHT22);

float t; //Temperatura
float h; // Umidade

void setup() {
  dht.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("Hello World!");
  // delay(1000);

  h = dht.readHumidity();
  t = dht.readTemperature();

  Serial.println("umi: ");
  Serial.println(h);
  Serial.println("temp: ");
  Serial.println(t);

  delay(1000);
}

/*#include <DHT22.h>
//define pin data
#define data SDA

DHT22 dht22(A2); 
 
void setup() {
  Serial.begin(115200); //1bit=10µs
  Serial.println("\ntest capteur DTH22");
}

void loop() {
  //Serial.println(dht22.debug()); //optionnal

  float t = dht22.getTemperature();
  float h = dht22.getHumidity();

  Serial.println(dht22.getLastError());

  Serial.print("h=");Serial.print(h,1);Serial.print("\t");
  Serial.print("t=");Serial.println(t,1);
  delay(5000);
}*/
