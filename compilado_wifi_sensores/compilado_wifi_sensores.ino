#include "DHT.h"

// definição dos limites dos parâmetros
#define MIN_UMIDADE 60

// pinagem dos sensores
#define PIN_SENSOR_CHAMA 2
#define PIN_SENSOR_GAS 4
#define PIN_SENSOR_UMIDADE 7

//SENSOR DE TEMPERATURA E UMIDADE
DHT dht(A0, DHT22);

//SENSOR DE GAS
#define PIN_GAS_DIG_IN 40
int tem_gas = 0;

//SENSOR DE CHAMA
#define PIN_CHAMA_DIG_IN 26
int tem_fogo = 0;

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
  tem_fogo = digitalRead(PIN_CHAMA_DIG_IN);

  return tem_fogo;
}

// Faz a leitura do sensor de gas, compara esse valor com o parâmetro mínimo definido "MIN_GAS" e altera o led de acordo para indicar se têm gás ou não
int sensorGas() {
  tem_fogo = digitalRead(PIN_GAS_DIG_IN);
  
  return tem_fogo;
}

void paginaInicial()
{

  String dados = "<p>HELLO WORLD</p><h1>TESTE H1</h1>";

  server.send(200, "text/html", dados); //Retorna resposta HTTP
  // server.send(200, "text/plain", dados); //Retorna resposta HTTP
}

void setup()
{
  // sensor de cham,a
  pinMode(PIN_CHAMA_DIG_IN, INPUT);
  
  // sensor de gás
  pinMode(PIN_GAS_DIG_IN, INPUT);

  // sensor temperatura e umidade
  dht.begin();

/////////////////////////////////////////////////////////////////////////////
  // Conexao WiFi
  Serial.begin(115200); // printa na tela

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
  sensorChama(); 
  sensorGas();

  float valor_temperatura; //Temperatura
  float valor_umidade; // Umidade
  valor_umidade = dht.readHumidity();
  valor_temperatura = dht.readTemperature();

  int n_tem_umidade = (valor_umidade <= MIN_UMIDADE);

  int tem_fogo = tem_gas || tem_fogo;

  Serial.print("\nTem fogo ? : ");
  Serial.print(tem_fogo);
  Serial.print("\nTem gas ? : ");
  Serial.print(tem_gas);
  Serial.print("\nTem chama ? : ");
  Serial.print(tem_fogo);

  digitalWrite(PIN_SENSOR_CHAMA, tem_fogo);
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
