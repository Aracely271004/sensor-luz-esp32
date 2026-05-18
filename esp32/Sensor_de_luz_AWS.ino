#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_sleep.h"

const char* ssid       = "TU_WIFI";
const char* password   = "TU_PASSWORD";
const char* urlDatos   = "http://TU_IP_AWS:4000/datos";
const char* urlEstado  = "http://TU_IP_AWS:4000/estado";
const char* urlComando = "http://TU_IP_AWS:4000/comando";

#define PIN_DO      14
#define PIN_AO      34
#define PIN_POWER   26
#define SLEEP_SEGUNDOS 30

int contadorEstado = 0;
bool sensorActivo = true;
bool modoSleep = false;

void conectarWiFi() {
  Serial.print("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 20) {
    delay(300);
    Serial.print(".");
    intentos++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado!");
  } else {
    Serial.println("\nNo se pudo conectar.");
  }
}

void actualizarEstadoYComando() {
  HTTPClient http;
  http.begin(urlEstado);
  http.setTimeout(1000);
  if (http.GET() == 200) {
    sensorActivo = http.getString().indexOf("pausado") < 0;
  }
  http.end();

  http.begin(urlComando);
  http.setTimeout(1000);
  if (http.GET() == 200) {
    modoSleep = http.getString().indexOf("dormir") >= 0;
  }
  http.end();
}

void entrarDeepSleep() {
  Serial.println("Entrando en Deep Sleep...");
  digitalWrite(PIN_POWER, LOW);
  Serial.flush();
  esp_sleep_enable_timer_wakeup(SLEEP_SEGUNDOS * 1000000ULL);
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_DO, INPUT);
  pinMode(PIN_POWER, OUTPUT);
  digitalWrite(PIN_POWER, LOW);
  conectarWiFi();
  actualizarEstadoYComando();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
    return;
  }

  contadorEstado++;
  if (contadorEstado >= 5) {
    actualizarEstadoYComando();
    contadorEstado = 0;
  }

  if (modoSleep) {
    entrarDeepSleep();
    return;
  }

  if (sensorActivo) {
    digitalWrite(PIN_POWER, HIGH);
    delay(20);

    int digital   = !digitalRead(PIN_DO);
    int analogico = 4095 - analogRead(PIN_AO);

    digitalWrite(PIN_POWER, LOW);

    HTTPClient http;
    http.begin(urlDatos);
    http.setTimeout(1500);
    http.addHeader("Content-Type", "application/json");
    String datos = "{\"digital\":" + String(digital) +
                   ",\"analogico\":" + String(analogico) + "}";
    http.POST(datos);
    http.end();

    Serial.print("Digital: ");
    Serial.print(digital);
    Serial.print("  |  Analógico: ");
    Serial.println(analogico);

  } else {
    digitalWrite(PIN_POWER, LOW);
    Serial.println("Sensor pausado...");
  }

  delay(700);
}
