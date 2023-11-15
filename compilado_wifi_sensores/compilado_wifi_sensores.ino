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
const int pin_gas_dig_in = 40; //, pin_gas_dig_out = 49;
// const int pin_gas_anag = A2;
int tem_gas = 0;
// int val_gas_anag = 0;

//SENSOR DE CHAMA
const int pin_chama_dig_in = 26; //, pin_chama_dig_out = 31;
// const int pin_chama_anag = A3;
int tem_fogo = 0;
// int val_chama_anag = 0;

/*

SALAMAKER
Acessando uma página web na rede WiFi com D1 - Wemos - ESP8266 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Configuração de rede e senha */
const char *rede = "FONSECA";  // coloque aqui o SSID / nome da rede WI-FI que deseja se conectar
const char *senha = "VIPDOCTOR33";  // coloque aqui a senha da rede WI-FI que deseja se conectar
// const char *rede = "ArthurWiFi";  // coloque aqui o SSID / nome da rede WI-FI que deseja se conectar
// const char *senha = "12345678";  // coloque aqui a senha da rede WI-FI que deseja se conectar



ESP8266WebServer server(80); //Objeto "servidor" na porta 80(porta HTTP)


//**************************************************************************************************************************************************************
// conferir motivo de tem_fogo receber !digitalRead
//**************************************************************************************************************************************************************

int sensorChama() {
  tem_fogo = digitalRead(pin_chama_dig_in);
  
  val_chama_anag = analogRead(pin_chama_anag);

  if (tem_fogo) digitalWrite(pin_chama_dig_out, HIGH);
  else digitalWrite(pin_chama_dig_out, LOW);

  return tem_fogo; //val_chama_anag;
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
  
  return tem_gas; // val_gas_anag;
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

/////////////////////////////////////////////////////////////////////////////
  // Conexao WiFi
  dht.begin();
  Serial.begin(9600);

  // Serial.begin(115200); // Inicia a serial com velocidade de 115200

  Serial.begin(115200); // printa na tela
  // Serial.begin(9600); // recebe da outra placa


  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(rede);

 
  WiFi.begin(rede, senha);// Inicia a WiFi com com o nome da rede e senha

  while (WiFi.status() != WL_CONNECTED) //Aguarda a conexao
  {
    Serial.print("Estabelecendo conexão com ");
    Serial.println(WiFi.SSID()); //Imprime o nome da rede
    delay(500);
  }
  Serial.print("Conectado a rede! Endereco IP ESP -> ");
  Serial.println(WiFi.localIP()); //Imprime o IP local do ESP

  
  /* Cria a associacao entre endereço HTML as funções que serão utilizadas */
  server.on("/", paginaInicial);
 
  server.begin(); //Inicia o servidor

  
}

void loop() 
{ 
  //sensorChama(); 
  //sensorGas();

  //Serial.print("\n---------------------\n");

  int valor_gas = sensorGas();
  int m1 = media(gas_vals, tam_media);

  //Serial.print("\nTem gas ?(loop): ");
  //Serial.print(tem_gas);

  Serial.print("\nNivel de gas: ");
  Serial.println(m1);

  Serial.print("\n---------------------\n");

  
  int valor_chama = sensorChama();
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

  /////////////////////////////////////////////////////////////

  Serial.println("HELLO WORLD FROM WEMOS D1");
 
  char rx_ch = Serial.read();
  Serial.print("RX: ");
  Serial.println(rx_ch);

  //Analise das solicitacoes via web
  server.handleClient();

  delay(5000);
}
