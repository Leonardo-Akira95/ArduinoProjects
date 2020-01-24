
#include "DHT.h"

#define DHTPIN 2

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  
}

void loop() {
  // Delay de 2 segundos, o sensor de leitura é um pouco lento
  delay(2000);

  // Ler temperatura e umidade leva 250ms
  float h = dht.readHumidity();
  // Ler temperatura em Celsius
  float t = dht.readTemperature();
  // Ler temperatura em fahrenheit, se o parâmetro for true, ele retorna em fahrenheit
  float f = dht.readTemperature(true);

  // Checa se o valor de leitura é válido
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  float hif = dht.computeHeatIndex(f, h);
  
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  
}
