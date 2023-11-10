#include "DHT.h"

// definição dos limites dos parâmetros
#define MIN_GAS 300
#define MIN_CHAMA 950
#define MIN_UMIDADE 60

// pinagem dos sensores
#define PIN_SENSOR_CHAMA 2
#define PIN_SENSOR_GAS 4
#define PIN_SENSOR_UMIDADE 7

//SENSOR DE TEMPERATURA E UMIDADE
DHT dht(A1, DHT22);

//SENSOR DE GAS
const int pin_gas_dig_in = 40, pin_gas_dig_out = 49;
const int pin_gas_anag = A2;
int tem_gas = 0;
int val_gas_anag = 0;

//SENSOR DE CHAMA
const int pin_chama_dig_in = 26, pin_chama_dig_out = 31;
const int pin_chama_anag = A3;
int tem_fogo = 0;
int val_chama_anag = 0;


#define tam_media 20
int gas_vals[tam_media];
int chama_vals[tam_media];

//**************************************************************************************************************************************************************
// conferir motivo de tem_fogo receber !digitalRead
//**************************************************************************************************************************************************************

int sensorChama() {
  tem_fogo = !digitalRead(pin_chama_dig_in);
  
  val_chama_anag = analogRead(pin_chama_anag);

  if (tem_fogo) digitalWrite(pin_chama_dig_out, HIGH);
  else digitalWrite(pin_chama_dig_out, LOW);

  return val_chama_anag;//tem_fogo;
}

// Faz a leitura do sensor de gas, compara esse valor com o parâmetro mínimo definido "MIN_GAS" e altera o led de acordo para indicar se têm gás ou não
int sensorGas() {
  val_gas_anag = analogRead(pin_gas_anag);

  // LEDs para debug
  tem_gas = (val_gas_anag > MIN_GAS);
  if (tem_gas) 
    digitalWrite(pin_gas_dig_out, HIGH);
  else 
    digitalWrite(pin_gas_dig_out, LOW);
  
  return val_gas_anag;
}


int media(int *v, int tam) {
  int m = 0;
  for(int i = 0; i < tam; i++) m += v[i];
  return m / tam;
}

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


void setup()
{

  for(int i = 0; i < tam_media; i++) gas_vals[i] = 0;
  for(int i = 0; i < tam_media; i++) chama_vals[i] = 0;
  
  //Serial.begin(9600);
  
  //Sensor de Chama
  pinMode(pin_chama_dig_in, INPUT);
  pinMode(pin_chama_anag, INPUT);
  pinMode(pin_chama_dig_out, OUTPUT);

  
  //Sensor de Gás  
  pinMode(pin_gas_dig_in, INPUT);
  pinMode(pin_gas_anag, INPUT);
  pinMode(pin_gas_dig_out, OUTPUT);

  pinMode(PIN_SENSOR_CHAMA, OUTPUT);  // chama
  pinMode(PIN_SENSOR_GAS, OUTPUT);  // gas
  pinMode(PIN_SENSOR_UMIDADE, OUTPUT);  // umidade


  // Conexao WiFi
  dht.begin();
  Serial.begin(9600);
  
}

void loop() 
{ 
  //sensorChama(); 
  //sensorGas();

  //Serial.print("\n---------------------\n");

  int valor_gas = sensorGas();
  insert_val(gas_vals, tam_media, valor_gas);
  int m1 = media(gas_vals, tam_media);

  //Serial.print("\nTem gas ?(loop): ");
  //Serial.print(tem_gas);

  Serial.print("\nNivel de gas: ");
  Serial.println(m1);

  Serial.print("\n---------------------\n");

  
  int valor_chama = sensorChama();
  insert_val(chama_vals, 20, valor_chama);
  int m2 = media(chama_vals, 20);
  
  Serial.print("\nNivel de chama: ");
  Serial.println(m2);


  float valor_temperatura; //Temperatura
  float valor_umidade; // Umidade
  valor_umidade = dht.readHumidity();
  valor_temperatura = dht.readTemperature();

  int tem_gas = (m1 > MIN_GAS);
  int tem_chama = (m2 <= MIN_CHAMA);
  int n_tem_umidade = (valor_umidade <= MIN_UMIDADE);

  int tem_fogo = tem_gas || tem_chama;

  Serial.print("\nTem fogo ? : ");
  Serial.print(tem_fogo);
  Serial.print("\nTem gas ? : ");
  Serial.print(tem_gas);
  Serial.print("\nTem chama ? : ");
  Serial.print(tem_chama);

  digitalWrite(PIN_SENSOR_CHAMA, tem_chama);
  digitalWrite(PIN_SENSOR_GAS, tem_gas);
  digitalWrite(PIN_SENSOR_UMIDADE, n_tem_umidade);


  Serial.println("umi: ");
  Serial.println(valor_umidade);
  Serial.println("temp: ");
  Serial.println(valor_temperatura);

  delay(5000);
}
