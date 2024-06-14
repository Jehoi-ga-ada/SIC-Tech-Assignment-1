#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// Ganti dengan SSID dan password WiFi Anda
const char* ssid = "cestlavie";
const char* password = "eggtarts!";
DHT dht(26, DHT11);

const char* serverName = "http://192.168.101.76:5000/post-data";

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(2000);
  // Menghubungkan ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("Terhubung ke WiFi");
}

void loop() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Menyiapkan data dalam format JSON
    String postData = "{\"humidity\": "+ String(humidity)+ "}";

    // Memulai koneksi HTTP
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Mengirim data melalui POST
    int httpResponseCode = http.POST(postData);

    // Mengecek respon dari server
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }

    // Menutup koneksi
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  // Menunggu 3 detik sebelum mengirim data lagi
  delay(3000);
}
