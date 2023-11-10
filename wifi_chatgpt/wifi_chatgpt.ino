#include <ESP8266WiFi.h>

const char* ssid = "FONSECA";
const char* password = "VIPDOCTOR33";

void setup() {
  Serial.begin(115200); // Configura a taxa de transmissão para 115200
  delay(10);

  // Conecta-se à rede Wi-Fi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi!");
}

void loop() {
  // Seu código aqui
  Serial.println("loop");
}
