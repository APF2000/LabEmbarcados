#include "DHT.h"

// limites dos parâmetros
#define MIN_GAS 150
#define MIN_CHAMA 950
#define MIN_TEMPERATURA 50
#define MIN_UMIDADE 50

//SENSOR TEMPERATURA E UMIDADE
DHT dht(A1, DHT22);
float t = 0; //Temperatura
float h = 0; // Umidade

//SENSOR DE GAS
//const int pin_gas_dig_in = 40, pin_gas_dig_out = 49;
const int pin_gas_anag = A2;
int tem_gas = 0;
int val_gas_anag = 0;

//SENSOR DE CHAMA
//const int pin_chama_dig_in = 26, pin_chama_dig_out = 31;
const int pin_chama_anag = A3;
int tem_fogo = 0;
int val_chama_anag = 0;

/*
#define tam_media 20
int gas_vals[tam_media];
int chama_vals[tam_media];
*/

int sensorChama() {
  //tem_fogo = !digitalRead(pin_chama_dig_in);
  
  val_chama_anag = analogRead(pin_chama_anag);

  //if (tem_fogo) digitalWrite(pin_chama_dig_out, HIGH);
  //else digitalWrite(pin_chama_dig_out, LOW);

  return val_chama_anag;//tem_fogo;
}

int sensorGas() {
  val_gas_anag = analogRead(pin_gas_anag);
  tem_gas = (val_gas_anag > 160);

  //if (tem_gas) digitalWrite(pin_gas_dig_out, HIGH);
  //else digitalWrite(pin_gas_dig_out, LOW);
  
  return val_gas_anag;//tem_gas;
}
/*
int media(int *v, int tam) {
  int m = 0;
  for(int i = 0; i < tam; i++) m += v[i];
  return m / tam;
}
*/
/*
void insert_val(int *v, int tam, int valor) {
  int aux0 = v[0], aux1;

  v[0] = valor;
  
  for(int i = 1; i < tam; i++)
  {
    aux1 = v[i];
    v[i] = aux0;
    aux0 = aux1;
  }
}
*/


void setup() {
  dht.begin();
  Serial.begin(9600);

  //for(int i = 0; i < tam_media; i++) gas_vals[i] = 0;
  //for(int i = 0; i < tam_media; i++) chama_vals[i] = 0;
  
  //Sensor de Chama
  //pinMode(pin_chama_dig_in, INPUT);
  pinMode(pin_chama_anag, INPUT);
  //pinMode(pin_chama_dig_out, OUTPUT);
  
  //Sensor de Gás  
  //pinMode(pin_gas_dig_in, INPUT);
  pinMode(pin_gas_anag, INPUT);
  //pinMode(pin_gas_dig_out, OUTPUT);

}

void loop() {
  h = dht.readHumidity();
  t = dht.readTemperature();

  Serial.println("umi: ");
  Serial.println(h);
  Serial.println("temp: ");
  Serial.println(t);

  delay(1000);

  //sensorChama(); 
  //sensorGas();
  //int ang_abre = 150, ang_fecha = 90;

  //Serial.print("\n---------------------\n");

  int valor_gas = sensorGas();
  //insert_val(gas_vals, tam_media, valor_gas);
  //int m1 = media(gas_vals, tam_media);

  //Serial.print("\nTem gas ?(loop): ");
  //Serial.print(tem_gas);

  /*Serial.print("\nNivel de gas: ");
  Serial.print(m1);

  Serial.print("\n---------------------\n");*/

  
  int valor_chama = sensorChama();
  //insert_val(chama_vals, 20, valor_chama);
  //int m2 = media(chama_vals, 20);
  
  //Serial.print("\nNivel de chama: ");
  //Serial.print(m2);

  //int tem_gas = (m1 > MIN_GAS);
  //int tem_chama = (m2 <= MIN_CHAMA);
  //int tem_fogo = tem_gas || tem_chama;

}
