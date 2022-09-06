///////firebase
#include <Arduino.h>
#include <Firebase.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
int timestamp;

String parentPath;
FirebaseJson json;
// Function that gets current epoch time
unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}

// Database child nodes
String aht10_1_temp = "/temp_sensor_1";
String aht10_1_umid = "/umid_sensor_1";
String aht10_2_temp = "/temp_sensor_2";
String aht10_2_umid = "/umid_sensor_2";
String dsb = "/temp_alimento";
String peso = "/peso";
String nivel = "/nivel";
String tempo = "/tempo";


#define WIFI_SSID "nome_rede"
#define WIFI_PASSWORD "senha"
#define DATABASE_URL xyzajbcbciadbi.firebaseio.com"
#define API_KEY "AIzaSyAIBdqdTiuyudyacbuazZSk56RMsvk68E"

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

//////Balanca
#include <HX711.h>        //Biblioteca do HX711
 
#define DOUT 26           //HX711 DATA OUT = pino D5 do ESP32
#define CLK 33           //HX711 SCK IN = pino D4 do ESP32

HX711 balanca;
long calibration_factor = -10160;

//////boia
int pinosensor = 12;
int pinosensor2 = 15;

/////dsb
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

#define ONE_WIRE_BUS 19

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;

float tempMin = 999;
float tempMax = 0;

/////aht10
#include <Adafruit_AHTX0.h>
#include "Wire.h"
#include "analogWrite.h"
#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) 
    return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}
unsigned long timestart = 0;
const long intervalo = 60000;
unsigned long count_min = 1;
int resist = 27;  

Adafruit_AHTX0 aht;
//////


void setup() {
  
  Serial.begin(115200);

  //config sensores ah10
  while (!Serial);
  delay(1000);

  Wire.begin();
  Serial.println("\nTCAScanner ready!");
    for (uint8_t t=0; t<8; t++) {
      tcaselect(t);
      Serial.print("TCA Port #"); Serial.println(t);

      for (uint8_t addr = 0; addr<=127; addr++) {
        if (addr == TCAADDR) continue;

        Wire.beginTransmission(addr);
        if (!Wire.endTransmission()) {
          Serial.print("Found I2C 0x");  Serial.println(addr,HEX);
        }
      }
    }
    Serial.println("\ndone");
    
  int scl = 22;
  int sda = 21;
  pinMode(scl, OUTPUT);
  pinMode(sda, OUTPUT);
  
  tcaselect(0);
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }

  tcaselect(1);
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");
  ///////////////////
    
  //config dsb
  sensors.begin();
  Serial.println("Localizando sensores DS18B20...");
  Serial.print("Foram encontrados ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  
  //config balanca
  balanca.begin(DOUT, CLK);
  Serial.println();
  Serial.println("HX711 - Calibracao da Balança");             
  Serial.println("Remova o peso da balanca");
  delay(300);
  balanca.set_scale();                                            
  zeraBalanca();  

  //config boia
  pinMode(pinosensor, INPUT);
  pinMode(pinosensor2, INPUT);
  
  //config firebase
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.signer.test_mode = true;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

}
void zeraBalanca ()
{
  Serial.println();                                               // salta uma linha
  balanca.tare();                                                 // zera a Balança
  Serial.println("Balança Zerada ");
}

void loop() {

  Serial.println("escrevendo....");

  //Get current timestamp
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);
    parentPath= "/" + String(timestamp);  //isso faz uma das abas ser o proprio timestamp
    json.set(tempo.c_str(), String(timestamp));

  //aht10 e resistores
  sensors_event_t humidity, temp;
    tcaselect(1);
    aht.getEvent(&humidity, &temp); 

  if(temp.temperature >= 32.0){
      analogWrite(resist, 0, 980, 10, 0);
    }else if(temp.temperature <= 29){
      analogWrite(resist, 600, 980, 10, 0);
    }else{
      analogWrite(resist, 600, 980, 10, 0);
    }
    tcaselect(1);
    aht.getEvent(&humidity, &temp);  
    Serial.print("Temp: ");
    Serial.print(temp.temperature);
    Serial.println(" ºC");
    json.set(aht10_1_temp.c_str(), String(temp.temperature));
    json.set(aht10_1_umid.c_str(), String(humidity.relative_humidity));
  
    tcaselect(0);
    aht.getEvent(&humidity, &temp);
    Serial.print("Temp: ");
    Serial.print(temp.temperature);
    Serial.println(" ºC");
    json.set(aht10_2_temp.c_str(), String(temp.temperature));
    json.set(aht10_2_umid.c_str(), String(humidity.relative_humidity));
    
    Serial.println("");

    //dsb
    // Le a informacao do sensor
    sensors.requestTemperatures();
    float tempC = sensors.getTempC(sensor1);
    // Atualiza temperaturas minima e maxima
  
    // Mostra dados no serial monitor
    Serial.print("Temp C: ");
    Serial.println(tempC);
    json.set(dsb.c_str(), String(tempC));
    
    //balanca
    balanca.set_scale(calibration_factor);                     // ajusta fator de calibração
    Serial.print("Peso: ");                                    // imprime no monitor serial
    Serial.print(balanca.get_units(), 3);                      // imprime peso da balança com 3 casas decimais
    Serial.print(" kg");
    Serial.print("      Fator de Calibração: ");               // imprime no monitor serial
    Serial.println(calibration_factor);                        // imprime fator de calibração
    float peso_arredondado = roundf(balanca.get_units() * 100) / 100;
    json.set(peso.c_str(), String(peso_arredondado));
    
    //boia
    int estado = digitalRead(pinosensor);
    int estado2 = digitalRead(pinosensor2);

    Serial.print("Estado sensor : ");
    Serial.println(estado);
    if(estado==1 && estado2==1){
      json.set(nivel.c_str(), "cheio");
    }else if((estado==1 && estado2!=1) || ( estado!=1 && estado2==1 )){
      json.set(nivel.c_str(), "medio");
    }else{
      json.set(nivel.c_str(), "vazio");
    }

  Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&firebaseData, parentPath.c_str(), &json) ? "ok" : firebaseData.errorReason().c_str());
  

  delay(1800000);
}
