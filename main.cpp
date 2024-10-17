#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 33
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.print("Conectando-se ao Wi-Fi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Conectado!");
  
  dht.begin();
}

void loop() {
  // Lê a umidade e a temperatura
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Verifique se as leituras falharam
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Falha ao ler do DHT22!");
    return;
  }

  // Verifique se está conectado
  if (WiFi.status() == WL_CONNECTED) {
    sendData(humidity, temperature);
  }

  delay(10000); // Atraso para o próximo loop
}

// Função para enviar os dados
void sendData(float humidity, float temperature) {
  // Dados JSON para enviar
  String jsonData = String("{\"humidity\":") + humidity + String(",\"temperature\":") + temperature + String("}");

  // Enviar a requisição
  HTTPClient http;
  http.begin("https://weathermonitorapi.onrender.com/weather"); // URL da API
  http.addHeader("Content-Type", "application/json");

  // Envie a solicitação POST
  int httpResponseCode = http.POST(jsonData);

  // Verifique a resposta
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(httpResponseCode); // Código de status HTTP
    Serial.println(response); // Resposta da API
  } else {
    Serial.print("Erro na chamada: ");
    Serial.println(httpResponseCode);
  }

  http.end(); // Libere recursos
}
