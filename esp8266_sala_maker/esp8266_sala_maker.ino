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


void setup() {

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

void loop() {


  Serial.println("HELLO WORLD FROM WEMOS D1");

  char rx_ch = Serial.read();
  Serial.print("RX: ");
  Serial.println(rx_ch);

  //Analise das solicitacoes via web
  server.handleClient();
}

void paginaInicial()
{

  String dados = "<p>HELLO WORLD</p><h1>TESTE H1</h1>";

  server.send(200, "text/html", dados); //Retorna resposta HTTP
  // server.send(200, "text/plain", dados); //Retorna resposta HTTP
}
