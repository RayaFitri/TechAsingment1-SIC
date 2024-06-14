#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>

#define DHT11PIN 5

WiFiClient wifi;
DHT dht(DHT11PIN, DHT11);

void setup() {
  Serial.begin(115200);
  Serial.println("Halo, ESP32!");

  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin("Raya", "qwertyuiop");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Menghubungkan");
  }
  Serial.println("WiFi terhubung! Alamat IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  HTTPClient http;
  http.begin("http://192.168.138.26:5000/dht/data");
  http.addHeader("Content-Type", "application/json");

  float kelembapan = dht.readHumidity();
  float suhu = dht.readTemperature();
  long waktu = millis();

  String data = "{"
                "\"kelembapan\": " + String(kelembapan, 2) + ","
                "\"suhu\": " + String(suhu, 2) + ","
                "\"waktu\": " + String(waktu) +
                "}";

  int httpCode = http.POST(data);

  Serial.println(data);
  if (httpCode > 0) { 
    Serial.println("Kode Respon HTTP: " + String(httpCode));
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Data berhasil dikirim");
    } else {
      Serial.println("Kode HTTP tak terduga: " + String(httpCode));
    }
  } else {
    Serial.println("Kesalahan saat mengirim POST: " + String(http.errorToString(httpCode).c_str()));
  }

  http.end();
  delay(5000);
}
