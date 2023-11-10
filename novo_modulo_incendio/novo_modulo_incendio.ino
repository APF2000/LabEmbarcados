
// #include <BlynkSimpleStream.h>

#include <Servo.h>

#define pinServo1 2
#define pinServo2 4

#define PIN_RX 0
#define PIN_TX 1

#define MIN_GAS 150
#define MIN_CHAMA 950

Servo servo1, servo2;

// SoftwareSerial Serial1(PIN_RX, PIN_TX);

//SENSOR DE CHAMA
const int pin_chama_dig_in = 26, pin_chama_dig_out = 31;
const int pin_chama_anag = A1;
int tem_fogo = 0;
int val_chama_anag = 0;

//SENSOR DE GAS
const int pin_gas_dig_in = 40, pin_gas_dig_out = 49;
const int pin_gas_anag = A2;
int tem_gas = 0;
int val_gas_anag = 0;


#define tam_media 20
int gas_vals[tam_media];
int chama_vals[tam_media];


int sensorChama()
{
  tem_fogo = !digitalRead(pin_chama_dig_in);
  
  val_chama_anag = analogRead(pin_chama_anag);

  if (tem_fogo) digitalWrite(pin_chama_dig_out, HIGH);
  else digitalWrite(pin_chama_dig_out, LOW);

  return val_chama_anag;//tem_fogo;
}

int sensorGas()
{
  
  val_gas_anag = analogRead(pin_gas_anag);
  tem_gas = (val_gas_anag > 160);

  if (tem_gas) digitalWrite(pin_gas_dig_out, HIGH);
  else digitalWrite(pin_gas_dig_out, LOW);
  
  return val_gas_anag;//tem_gas;
}

void abrirJanela(Servo servo, int tem_incendio, int pos_abertura, int pos_fecha)
{
  if(tem_incendio) servo.write(pos_abertura);
  else servo.write(pos_fecha);
}

void girarServo(Servo servo)
{
  int pos = 0;
  for(pos=0; pos <= 180; pos++)
  {
    servo.write(pos);
    delay(15);
  }
  
  for(pos=180; pos >= 0; pos--)
  {
    servo.write(pos);
    delay(15);
  }
}

int media(int *v, int tam)
{
  int m = 0;
  for(int i = 0; i < tam; i++) m += v[i];
  return m / tam;
}

void insert_val(int *v, int tam, int valor)
{
  int aux0 = v[0], aux1;

  v[0] = valor;
  
  for(int i = 1; i < tam; i++)
  {
    aux1 = v[i];
    v[i] = aux0;
    aux0 = aux1;
  }
}


char auth[] = "IInvCN0-af9kV0LiAxvD3V0LlpuZawLU";

// WidgetTerminal terminal(V4);
// BlynkTimer timer;

/*
void blynkAnotherDevice() // Here we will send HIGH or LOW once per second
{
  //Serial.print("blynk another device \n");
  Blynk.virtualWrite(V77, val_gas_anag);
  Blynk.run();
}
*/

void setup()
{
  servo1.attach(pinServo1);
  //servo2.attach(pinServo2);

  for(int i = 0; i < tam_media; i++) gas_vals[i] = 0;
  for(int i = 0; i < tam_media; i++) chama_vals[i] = 0;
  
  //Sensor de Chama
  pinMode(pin_chama_dig_in, INPUT);
  pinMode(pin_chama_anag, INPUT);
  pinMode(pin_chama_dig_out, OUTPUT);

  
  //Sensor de Gás  
  pinMode(pin_gas_dig_in, INPUT);
  pinMode(pin_gas_anag, INPUT);
  pinMode(pin_gas_dig_out, OUTPUT);


  // Conexao WiFi
  // Serial.begin(9600); // passar pra outra placa
  Serial.begin(115200); // printar na tela

  // Blynk.begin(Serial, auth);
  // timer.setInterval(2000L, blynkAnotherDevice);
  
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

  /*Serial.print("\nNivel de gas: ");
  Serial.print(m1);

  Serial.print("\n---------------------\n");*/

  
  int valor_chama = sensorChama();
  insert_val(chama_vals, 20, valor_chama);
  int m2 = media(chama_vals, 20);
  
  //Serial.print("\nNivel de chama: ");
  //Serial.print(m2);

  int tem_gas = (m1 > MIN_GAS);
  int tem_chama = (m2 <= MIN_CHAMA);
  int tem_fogo = tem_gas || tem_chama;

  // Serial1.print("\nTem fogo ? : ");
  // Serial1.print(tem_fogo);
  // Serial1.print("\nTem gas ? : ");
  // Serial1.print(tem_gas);
  // Serial1.print("\nTem chama ? : ");
  // Serial1.print(tem_chama);

  Serial.println("HELLO WORLD FROM ARDUINO UNO");

  char rx_ch = Serial.read();
  Serial.print("RX: ");
  Serial.println(rx_ch);

  delay(1000);
  
  // abrirJanela(servo1, tem_fogo, ang_abre, ang_fecha);
  
  //Blynk.run();
  // timer.run();
}
