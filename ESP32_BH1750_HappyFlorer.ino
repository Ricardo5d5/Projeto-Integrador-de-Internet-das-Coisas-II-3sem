/*******************************************************************************
  Projeto HappyFlowers (v1.0)

** Copyright 2021 HappyFlowers.
  Escrito por Ricardo SS (12/11/2021).

*******************************************************************************/
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <BH1750.h>

int tempoExposicao = 0;
const int limitInferior = 14825;
const int limitSuperior = 20000;

BH1750 lightMeter;

//Put your SSID & Password
//const char* ssid = "Cristo Salva";
//const char* password = "4736351761";
const char* ssid = "OTT";
const char* password = "12345678";




void setup() {
  Serial.begin(9600);  //Inicializa a porta serial somente para debug
  delay(100);
  Wire.begin(); //Inicializa O I2C BUS
  lightMeter.begin(); //Inicializa a medição de luminosidade


  // Connect to Wi-Fi

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

}

void loop() {
  //Leitura do Sensor GY30 BH1750
  float lux = lightMeter.readLightLevel(); //Variavel para receber o valor de luminosidade lido
  Serial.print("Luminosidade: ");
  Serial.print(lux);
  Serial.println(" lux");

  if (lux > limitInferior) {
    tempoExposicao = 1;
  }
  else {
    tempoExposicao = 0;
  }

  String insert_API = "https://happyflowers-api.herokuapp.com/insert/sensor/";
  insert_API = insert_API + lux + "/" + tempoExposicao;

  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

    HTTPClient http;

    //http.begin("http://jsonplaceholder.typicode.com/comments?id=10"); //Specify the URL
    Serial.println(insert_API);
    //http.begin("https://happyflowers-api.herokuapp.com/insert/sensor/27600/10");
    http.begin(insert_API);
    int httpCode = http.GET();  //Make the request

    if (httpCode > 0) { //Check for the returning code

      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }

    else {
      Serial.println("Error on HTTP request");
    }

    http.end(); //Free the resources
  }
  delay(60000);

}
