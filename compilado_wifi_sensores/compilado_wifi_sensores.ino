#include "DHT.h"

// definição dos limites dos parâmetros
#define MIN_UMID 60
#define MAX_TEMP 40

// sensor de temperatura e umidade (dht)
#define PIN_TEMP_UMID_ANAG_IN D2
#define PIN_TEMP_DIG_OUT D6
#define PIN_UMID_DIG_OUT D7

DHT dht(PIN_TEMP_UMID_ANAG_IN, DHT22);
int umid_ok = 1;
int temp_ok = 1;

// semsor de gas
#define PIN_GAS_DIG_IN D3
int tem_gas = 0;

// sensor de chama
#define PIN_CHAMA_DIG_IN D5
int tem_chama = 0;


/*
SALAMAKER
Acessando uma página web na rede WiFi com D1 - Wemos - ESP8266 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

#include <WiFiClientSecureBearSSL.h>


/* Configuração de rede e senha */
// const char *rede = "FONSECA";  // coloque aqui o SSID / nome da rede WI-FI que deseja se conectar
// const char *senha = "VIPDOCTOR33";  // coloque aqui a senha da rede WI-FI que deseja se conectar
const char *rede = "ArthurWiFi";  // coloque aqui o SSID / nome da rede WI-FI que deseja se conectar
const char *senha = "12345678";  // coloque aqui a senha da rede WI-FI que deseja se conectar

ESP8266WebServer server(80); //Objeto "servidor" na porta 80(porta HTTP)

int sensorChama() {
  tem_chama = digitalRead(PIN_CHAMA_DIG_IN);

  return tem_chama;
}

// Faz a leitura do sensor de gas, compara esse valor com o parâmetro mínimo definido "MIN_GAS" e altera o led de acordo para indicar se têm gás ou não
int sensorGas() {
  tem_gas = digitalRead(PIN_GAS_DIG_IN);
  
  return tem_gas;
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
  pinMode(PIN_TEMP_DIG_OUT, OUTPUT);
  pinMode(PIN_UMID_DIG_OUT, OUTPUT);

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

  int tem_fogo = tem_gas || tem_chama;

  Serial.print("\n------------------------\n");

  Serial.print("\nTem fogo ? : ");
  Serial.print(tem_chama);
  Serial.print("\nTem gas ? : ");
  Serial.print(tem_gas);
  Serial.print("\nTem chama ? : ");
  Serial.print(tem_fogo);

  digitalWrite(PIN_CHAMA_DIG_IN, tem_chama);
  digitalWrite(PIN_GAS_DIG_IN, tem_gas);

  Serial.print("\n------------------------\n");

  float valor_temperatura; //Temperatura
  float valor_umidade; // Umidade
  valor_umidade = dht.readHumidity();
  valor_temperatura = dht.readTemperature();

  umid_ok = (valor_umidade >= MIN_UMID);
  temp_ok = (valor_temperatura <= MAX_TEMP);

  digitalWrite(PIN_TEMP_DIG_OUT, !umid_ok);
  digitalWrite(PIN_UMID_DIG_OUT, !temp_ok);

  Serial.println("umi: ");
  Serial.println(valor_umidade);
  Serial.println("temp: ");
  Serial.println(valor_temperatura);
  Serial.print("umid_ok ? : ");
  Serial.println(umid_ok);
  Serial.print("temp_ok ? : ");
  Serial.println(temp_ok);

  /////////////////////////////////////////////////////////////

  Serial.println("HELLO WORLD FROM WEMOS D1");
 
  char rx_ch = Serial.read();
  Serial.print("RX: ");
  Serial.println(rx_ch);

  //Analise das solicitacoes via web
  server.handleClient();

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient https; 

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setInsecure();

    char *url = "https://api.tago.io/data";

    if (https.begin(*client, url)) {  
        String httpRequestData = "";
        httpRequestData.concat("[{ \"variable\": \"temperatura\", \"value\": \"");
        httpRequestData.concat(valor_temperatura);
        httpRequestData.concat("\", \"unit\": \"C\"}");

        httpRequestData.concat(", { \"variable\": \"umidade\", \"value\": \"");
        httpRequestData.concat(valor_umidade);
        httpRequestData.concat("\", \"unit\": \"%\"}]");

        Serial.print("httpRequestData ");
        Serial.println(httpRequestData);

        https.addHeader("Content-Type", "application/json");
        // https.addHeader("data-raw", httpRequestData);
        https.addHeader("Authorization", "4bb77716-ed20-48e8-9824-f4fdf667cba4");  
        Serial.print("[HTTPS] POST...\n");
        // start connection and send HTTP header

        // Ignore SSL certificate validation
        client->setInsecure();


        int httpCode = https.POST(httpRequestData);
        
        Serial.print("httpCode");
        Serial.println(httpCode);
      
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
 

    https.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }

  delay(1000);
}
