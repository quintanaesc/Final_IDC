#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

//credenciales escuela
//const char *ssid = "PC_PUMA";
//const char *password = "";

//credenciales casa
const char* ssid = "IZZI-883B";
const char* password =  "F0AF853B883B";

//inicializar wifi como cliente
WiFiClient client;

//Servidor php para enviar a la base de datos
String servidor = "http://internetdelacosa.000webhostapp.com/sensores_BD.php";
String servidorFinal = "http://internetdelacosa.000webhostapp.com/node_Final.php";


//selecionamos pines
int anlgTemp = A0;
int digtLuz = 5;

int venti = 12;
int lamp = 14;

//rango de temperatura de operacion del sensor(°C)
int optTempMin = 0;
int optpTempMax = 150;
int optTempRang = optpTempMax - optTempMin;

//valores de conrtrol
int ctrpTempMax = 15;
String ctrpLuzEnc = "Habitacion iluminada";

// Variables en el server
String temperaturaServ;
String iluminacionServ;
String estadoLamparaServ;
String estadoVentiladorServ;


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
  pinMode(anlgTemp, INPUT);
  pinMode(digtLuz, INPUT);
  pinMode(venti, OUTPUT);
  pinMode(lamp, OUTPUT);
}

void loop() {
  Serial.println("\n------------------------------");
  // Lee el valor analógico y calcula la Temperatura
  int lecturaAnalogica = analogRead(anlgTemp);
  int preTemperatura = lecturaAnalogica * optTempRang / 1024;
  int temperatura = preTemperatura + optTempMin;

  // Lee el valor digital y calcula la iluminación
  int lecturaDigital = digitalRead(digtLuz);
  String iluminacion;
  if (lecturaDigital == 1) {
    iluminacion = "Habitacion iluminada";
  } else {
    iluminacion = "Habitacion no iluminada";
  }

  //imprimimos el estado del invernadero
  Serial.print("temperatura actual (C°): ");
  Serial.println(temperatura);
  Serial.println(iluminacion);

  //realisamos el post al php de la base de datos
  //para el estado del ivernadero
  postBD(temperatura, iluminacion);

  //realizamos un get optener las variables del server
  getEstado();

  //desidimos si encender los actuadores
  actuadores();

  //se envia el reporte final
  postFinal();
  delay(10000);
}

void postBD(int temperatura, String iluminacion) {
  String sEnviado = "temperatura=" + String(temperatura) + "&iluminacion=" + iluminacion;
  HTTPClient http;
  http.begin(client, servidor);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  //envio de post cpm esra de respuesta
  int iCodigoRespuesta = http.POST(sEnviado);

  if (iCodigoRespuesta > 0) {
    Serial.println("Codigo de respuesta: : " + String(iCodigoRespuesta));
    if (iCodigoRespuesta == 200) {
      String sRecibido = http.getString();
      Serial.println("Respuesta del servidor: ");
      Serial.println("->" + sRecibido);
    }
    http.end(); // Termina la conexión
  }
}

void getEstado() {
  HTTPClient http;
  String url = "http://internetdelacosa.000webhostapp.com/server_Actua.php";
  http.begin(client, url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int iCodigoRespuesta = http.GET();
  String respuesta = http.getString();
  Serial.println(respuesta);

  temperaturaServ = dividirResp(respuesta, "temperatura");
  iluminacionServ = dividirResp(respuesta, "iluminacion");
  estadoLamparaServ = dividirResp(respuesta, "estadoLampara");
  estadoVentiladorServ = dividirResp(respuesta, "estadoVentilador");
  http.end(); // Termina la conexión
}

String dividirResp(String respuesta, String  identificador) {
  int field1Pos = respuesta.indexOf(identificador);
  field1Pos = respuesta.indexOf(identificador, field1Pos);
  int valuePos;
  int endPos;
  if (field1Pos != -1) {
    if (identificador == "temperatura") {
      valuePos = respuesta.indexOf(":", field1Pos) + 1;
      endPos = respuesta.indexOf(",", valuePos);
    } else if (identificador == "estadoVentilador") {
      valuePos = respuesta.indexOf(":", field1Pos) + 2;
      endPos = respuesta.indexOf("}", valuePos) - 1;
    } else {
      valuePos = respuesta.indexOf(":", field1Pos) + 2;
      endPos = respuesta.indexOf(",", valuePos) - 1;
    }

    if (valuePos != -1 && endPos != -1) {
      String field1Value = respuesta.substring(valuePos, endPos);
      Serial.print("Valor de control ");
      Serial.println(identificador);
      Serial.println(field1Value);
      return field1Value;
    } else {
      Serial.println("No se pudo encontrar el valor de field1");
    }
  } else {
    Serial.println("No se encontró la clave 'field1' en el JSON.");
  }
  return "S/n";
}
void actuadores() {
  //ventilador
  if (estadoVentiladorServ == "Automatico") {
    if (temperaturaServ.toInt() > ctrpTempMax) {
      digitalWrite(venti, HIGH);
      Serial.println("Encendiendo Ventilador");
    } else {
      digitalWrite(venti, LOW);
      Serial.println("Apagando Ventilador");
    }
  } else if (estadoVentiladorServ == "Apagado") {
    digitalWrite(venti, LOW);
    Serial.println("Apagando Ventilador");
  } else if (estadoVentiladorServ == "Encendido") {
    digitalWrite(venti, HIGH);
    Serial.println("Encendiendo Ventilador");

  }
  //lampara
  if (estadoLamparaServ == "Automatico") {
    if (iluminacionServ == ctrpLuzEnc) {
      digitalWrite(lamp, HIGH);
      Serial.println("Encendiendo Lampara");
    } else {
      digitalWrite(lamp, LOW);
      Serial.println("Apagando Lampara");
    }
  } else if (estadoLamparaServ == "Apagado") {
    digitalWrite(lamp, LOW);
    Serial.println("Apagando Lampara");
  } else if (estadoLamparaServ == "Encendido") {
    digitalWrite(lamp, HIGH);
    Serial.println("Encendiendo Lampara");
  }
}
void postFinal(){
  String sEnviado = "temperatura=" + temperaturaServ + "&iluminacion=" + iluminacionServ + "&lampara=" + estadoLamparaServ + "&ventilador=" + estadoVentiladorServ;
  HTTPClient http;
  http.begin(client, servidorFinal);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  //envio de post cpm esra de respuesta
  int iCodigoRespuesta = http.POST(sEnviado);
  if (iCodigoRespuesta > 0) {
    Serial.println("Codigo de respuesta: : " + String(iCodigoRespuesta));
    if (iCodigoRespuesta == 200) {
      String sRecibido = http.getString();
      Serial.println("Respuesta del servidor final: ");
      Serial.println("->" + sRecibido);
    }
    http.end(); // Termina la conexión
  }
}
