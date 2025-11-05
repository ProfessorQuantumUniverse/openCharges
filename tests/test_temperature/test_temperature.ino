/*
 * Test Program: Temperature Sensor
 * 
 * Tests the DS18B20 temperature sensor
 * - Sensor connected to pin D32
 * - Displays temperature readings every second
 * - Shows temperature in Celsius and Fahrenheit
 */

#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMP_SENSOR_PIN 32

OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Temperature Sensor Test ===");
  Serial.println("Testing DS18B20 on pin D32");
  
  // Initialize temperature sensor
  tempSensor.begin();
  
  // Get sensor info
  int deviceCount = tempSensor.getDeviceCount();
  Serial.print("Found ");
  Serial.print(deviceCount);
  Serial.println(" temperature sensor(s)");
  
  if (deviceCount > 0) {
    Serial.println("Sensor initialized successfully");
    
    // Get sensor resolution
    int resolution = tempSensor.getResolution();
    Serial.print("Sensor resolution: ");
    Serial.print(resolution);
    Serial.println(" bits");
  } else {
    Serial.println("ERROR: No temperature sensor found!");
    Serial.println("Check wiring and connections");
  }
  
  Serial.println("\nStarting temperature readings...\n");
  delay(1000);
}

void loop() {
  // Request temperature reading
  tempSensor.requestTemperatures();
  
  // Get temperature in Celsius
  float tempC = tempSensor.getTempCByIndex(0);
  
  // Check if reading is valid
  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("ERROR: Temperature sensor disconnected!");
  } else {
    // Calculate Fahrenheit
    float tempF = tempC * 9.0 / 5.0 + 32.0;
    
    // Display readings
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print(" °C  /  ");
    Serial.print(tempF);
    Serial.println(" °F");
    
    // Warn if temperature is high
    if (tempC > 50.0) {
      Serial.println("  WARNING: High temperature!");
    }
    if (tempC > 80.0) {
      Serial.println("  CRITICAL: Temperature too high!");
    }
  }
  
  delay(1000);
}
