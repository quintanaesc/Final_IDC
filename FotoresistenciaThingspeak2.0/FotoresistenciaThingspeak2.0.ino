#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <ESP8266HTTPClient.h>

//credenciales escuela
const char *ssid = "PC_PUMA";
const char *password = "";

//credenciales casa
//const char* ssid = "IZZI-883B";
//const char* password =  "F0AF853B883B";

//inicializar wifi como cliente
WiFiClient client;

//preparacion del server
const char *servidor = "api.thingspeak.com";
//canal Indicador de iluminación
String apiKey = "SK7DGIKYLDTZZQ61";
//canal Interruptor de lampara
String apiKeyLamp = "UHFOFV4P7H4897B3";
unsigned long channel = 2321294;
unsigned int field1 = 1;

//selecionamos pines
int Anlg = A0;
int lamp = 5;

// valores de contro de luz
int minLuz = 40;
int maxLuz = 60;
int controlAuto = 0;

//valor maximo que da la lectura directa del sensor cuando no hay nada de luz
//!FAVOR DE CALIBRAR!
int maxSensor = 350;

void setup() {
  // Inicia la comunicación serial
  Serial.begin(9600);
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);

  //comprobamos internet
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConexión WiFi establecida!");

  //definimos pines
  pinMode(Anlg, INPUT);
  pinMode(lamp, OUTPUT);
}

void loop() {
  Serial.println("\n------------------------------");

  // Lee el valor analógico y calcula la iluminación
  int lecturaAnalogica = analogRead(Anlg);
  int preiluminacion = lecturaAnalogica * 100 / maxSensor;
  int iluminacion = 100 - preiluminacion;

  //imprimimos la iluminacion en seria
  Serial.print("Iluminación actual: ");
  Serial.println(iluminacion);

  //realisamos el post para la iluminacion
  postIluminacion(iluminacion);

  //realizamos el apagado o encendido automatico
  automatico(iluminacion);

  //realizamos un get para encender o apagar la lampara
  getLampara();
}

//funcion enviar datos al servidor donde se mide la iluminacion
void postIluminacion(int iluminacion) {
  if (client.connect(servidor, 80)) {
    String postStr = "field1=" + String(iluminacion);

    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println("X-THINGSPEAKAPIKEY:" + apiKey);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postStr.length());
    client.println();
    client.print(postStr);

    Serial.print("Iluminación: ");
    Serial.print(iluminacion);
    Serial.println(" Enviado a ThingSpeak");

    delay(10000); // Espera 15 segundos antes de enviar el siguiente dato
    client.stop(); // Cierra la conexión
  }
}

void automatico(int iluminacion) {
  if (client.connect(servidor, 80)) {
    if (iluminacion < minLuz) {
      Serial.println("iluminacion demaciado baja, encendiendo la luz");
      controlAuto = 1;
    } else if (iluminacion > maxLuz) {
      Serial.println("iluminacion demaciado Alta, apagando la luz");
      controlAuto = 0;
    }
    String postStr = "field1=" + String(controlAuto);

    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println("X-THINGSPEAKAPIKEY:" + apiKeyLamp);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postStr.length());
    client.println();
    client.print(postStr);
    Serial.println(" Control enviado a ThingSpeak");

    delay(10000); // Espera 15 segundos antes de enviar el siguiente dato
    client.stop(); // Cierra la conexión
  }
}

void getLampara() {
  HTTPClient http;
  String url = "http://api.thingspeak.com/channels/2321294/fields/1/last.json?api_key=YIFS5N23NKEK9VZW";
  http.begin(client, url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int iCodigoRespuesta = http.GET();
  String respuesta = http.getString();
  int field1 = valorLampara(respuesta);
  if (field1 == 1) {
    digitalWrite(lamp, HIGH);
    Serial.println("lampara encendida");
  }
  else if (field1 == 0) {
    digitalWrite(lamp, LOW);
    Serial.println("lampara apagada");
  }
  delay(10000);
}
int valorLampara(String respuesta) {
  Serial.println("Respuesta server:" + respuesta);
  int field1Pos = respuesta.indexOf("\"field1\"");
  field1Pos = respuesta.indexOf("\"field1\"", field1Pos);
  if (field1Pos != -1) {
    int valuePos = respuesta.indexOf(":", field1Pos) + 2;
    int endPos = respuesta.indexOf("\"", valuePos);

    if (valuePos != -1 && endPos != -1) {
      String field1Value = respuesta.substring(valuePos, endPos);
      Serial.println("Valor de control de la lampara: ");
      Serial.println(field1Value);
      return field1Value.toInt();
    } else {
      Serial.println("No se pudo encontrar el valor de field1");
    }
  } else {
    Serial.println("No se encontró la clave 'field1' en el JSON.");
  }
  return 999;
}
