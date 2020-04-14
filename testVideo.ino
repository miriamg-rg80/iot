#include <ESP32_Servo.h>
#include <analogWrite.h>
#include <FirebaseESP32.h>
#include <FirebaseESP32HTTPClient.h>
#include <FirebaseJson.h>
#include <jsmn.h>
#include <DHT.h>
#include <WiFi.h>

//Pin sensor humedad y temperatura, tipo dht11
#define DHTPIN 18
#define DHTTYPE DHT11
#include "FirebaseESP32.h"

// Definimos la direccion host de nuestro servidor
#define FIREBASE_HOST "mdhomeapp.firebaseio.com"
#define FIREBASE_AUTH "S3k5i0NzOi9dZ2gc108BiqOkR3ZOdueA76fmOum2"

// Definimos la conexion a punto de acceso wifi
#define WIFI_SSID "INFINITUM7458A9"
#define WIFI_PASSWORD "ECED6592BE"

WiFiClient client;
FirebaseData firebaseDataLed1;
FirebaseData firebaseDataLed2;
FirebaseData firebaseDataLed3;
FirebaseData firebaseDataTem;
FirebaseData firebaseDataHum;
FirebaseData firebaseDataVent;
FirebaseData firebaseDataPuerta;
//Servo se inicializa
Servo servoMotor;
//Sensor se inicializa
DHT dht(DHTPIN, DHTTYPE);

//Pines de salida
const int led1 = 2;
const int led2 = 5;
const int led3 = 23;
const int vent = 4;
const int puerta = 19;

void setup() {
  Serial.begin(9600);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".NO CONECTA ");
  }

  Serial.println ("");
  Serial.println ("Se conectó al wifi!");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //Leds
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  //Ventilador
  pinMode(vent, OUTPUT);
  //Sensor dht11
  dht.begin();
  //puerta
  servoMotor.attach(19);
}

void loop() {
  //Luces
  Firebase.getInt(firebaseDataLed1, "/led");
  Firebase.getInt(firebaseDataLed2, "/led2");
  Firebase.getInt(firebaseDataLed3, "/led3");
  int dato1 = firebaseDataLed1.intData();
  int dato2 = firebaseDataLed2.intData();
  int dato3 = firebaseDataLed3.intData();
  analogWrite(led1, dato1);
  analogWrite(led2, dato2);
  analogWrite(led3, dato3);
  Serial.print(dato1);
  Serial.print(dato2);
  Serial.print(dato3);
  //Clima
  float t = dht.readTemperature();
  int h = dht.readHumidity();
  Firebase.setFloat(firebaseDataTem, "temperature", t);
  Firebase.setInt(firebaseDataHum, "humidity", h);
  //Ventilador
  Firebase.getInt(firebaseDataVent, "/ventilador");
  int datoVen = firebaseDataVent.intData();
  analogWrite(vent, datoVen);
  //Puerta
  Firebase.getInt(firebaseDataPuerta, "/puerta");
  int door = firebaseDataPuerta.intData();
  servoMotor.write(door);
  Serial.println(door);
}
