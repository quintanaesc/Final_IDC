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


//selecionamos pines
int anlgTemp = A0;
int digtLuz = 5;

int venti= 12;
int lamp = 14;

//rango de temperatura de operacion del sensor(°C)
int optTempMin = 0;
int optpTempMax = 150;
int optTempRang = optpTempMax - optTempMin;

//rango de temperatura de control del sensor(°C)
int ctrTempMin = 15;
int ctrpTempMax = 20;

// valores de contro de luz
int ctrLuzEnsen = 0;

//contro automatico 1,0 (S/N)
int controlAuto = 0;

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
  if(lecturaDigital == ctrLuzEnsen){
    iluminacion = "Habitacion iluminada";
  }else{
    iluminacion = "Habitacion no iluminada";
  }

  //imprimimos el estado del invernadero 
  Serial.print("temperatura actual (C°): ");
  Serial.println(temperatura);
  Serial.println(iluminacion);

  //realisamos el post al php de la base de datos
  //para el estado del ivernadero
  postBD(temperatura,iluminacion);

  //realizamos un get para encender o apagar la lampara
  //getEstado();

  delay(10000);
}

void postBD(int temperatura, String iluminacion){
  String sEnviado = "temperatura=" + String(temperatura) + "&iluminacion="+iluminacion;
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
