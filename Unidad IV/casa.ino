#include "thingProperties.h"
#include <ESP32Servo.h>
#include <DHT.h>

Servo myServo;
const int sensorPin = 2; // Pin al que está conectado el sensor PIR
const float luz = 4; 
bool movimientoDetectado = false;
bool ledInterior = false;
const int ledInteriorPin = 13;
const int ventiladorPin = 12;
DHT dht(16, DHT11);


void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  ////////////////////////////////////////
  pinMode(sensorPin, INPUT);
  pinMode(ledInteriorPin, OUTPUT);
  pinMode(luz, OUTPUT);
  pinMode(16, INPUT);
  myServo.attach(ventiladorPin);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();


}

void loop() {
  ArduinoCloud.update();

  int LecMovimiento = digitalRead(sensorPin);

  if (LecMovimiento == HIGH) {
    if (!movimientoDetectado) {
      movimiento = true;
      Serial.println("¡Movimiento detectado!");
      movimientoDetectado = true;
      delay(500);
      digitalWrite(ledInteriorPin, HIGH);
      digitalWrite(luz, HIGH);
      myServo.write(180);
      delay(2000);
    }
  } else {
    movimiento = false;
    movimientoDetectado = false;
    delay(500);
    digitalWrite(ledInteriorPin, LOW);
    digitalWrite(luz, LOW);
    myServo.write(0);
    delay(500);
  }

  delay(100); // Pequeña pausa para estabilidad
float humidity = dht.readHumidity();
  
 
float temperature = dht.readTemperature();

if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error al leer el sensor DHT!");
}else{
  temperatura = temperature;
  humedad = humidity;
  Serial.println("Humedad: ");
  Serial.print(humidity);
  Serial.print("%");
  Serial.println("Temperatura: ");
  Serial.print(temperature);
  Serial.print("ºC");
}
}

void onMovimientoChange()  {
  
}


void onLuzChange()  {
 
}
void onTemperaturaChange()  {
}

void onHumedadChange()  {
 
}