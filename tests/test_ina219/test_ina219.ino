/*
 * Test Program: INA219 Voltage/Current Sensors
 * 
 * Tests all 6 INA219 sensors on two I2C buses
 * - Group 1: 4 sensors on SCL D35, SDA D34 (addresses 0x40-0x43)
 * - Group 2: 2 sensors on SCL D25, SDA D26 (addresses 0x40-0x41)
 * - Displays voltage and current readings every second
 */

#include <Wire.h>
#include <Adafruit_INA219.h>

#define I2C1_SDA 34
#define I2C1_SCL 35
#define I2C2_SDA 26
#define I2C2_SCL 25

// INA219 Sensors
Adafruit_INA219 ina219_group1[4];
Adafruit_INA219 ina219_group2[2];

// I2C Buses
TwoWire I2C_1 = TwoWire(0);
TwoWire I2C_2 = TwoWire(1);

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== INA219 Sensor Test ===");
  
  // Initialize I2C buses
  I2C_1.begin(I2C1_SDA, I2C1_SCL, 100000);
  I2C_2.begin(I2C2_SDA, I2C2_SCL, 100000);
  Serial.println("I2C buses initialized");
  
  // Initialize Group 1 sensors (4 sensors)
  Serial.println("\nInitializing Group 1 (I2C_1):");
  for (int i = 0; i < 4; i++) {
    Serial.print("  Sensor ");
    Serial.print(i);
    Serial.print(" (0x");
    Serial.print(0x40 + i, HEX);
    Serial.print("): ");
    
    if (ina219_group1[i].begin(0x40 + i, &I2C_1)) {
      Serial.println("OK");
    } else {
      Serial.println("FAILED");
    }
  }
  
  // Initialize Group 2 sensors (2 sensors)
  Serial.println("\nInitializing Group 2 (I2C_2):");
  for (int i = 0; i < 2; i++) {
    Serial.print("  Sensor ");
    Serial.print(4 + i);
    Serial.print(" (0x");
    Serial.print(0x40 + i, HEX);
    Serial.print("): ");
    
    if (ina219_group2[i].begin(0x40 + i, &I2C_2)) {
      Serial.println("OK");
    } else {
      Serial.println("FAILED");
    }
  }
  
  Serial.println("\nStarting measurements...\n");
  delay(1000);
}

void loop() {
  Serial.println("=== Sensor Readings ===");
  
  // Read Group 1 sensors
  Serial.println("Group 1:");
  for (int i = 0; i < 4; i++) {
    float busVoltage = ina219_group1[i].getBusVoltage_V();
    float current_mA = ina219_group1[i].getCurrent_mA();
    float power_mW = ina219_group1[i].getPower_mW();
    
    Serial.print("  Module ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(busVoltage, 2);
    Serial.print("V, ");
    Serial.print(current_mA, 0);
    Serial.print("mA, ");
    Serial.print(power_mW, 0);
    Serial.println("mW");
  }
  
  // Read Group 2 sensors
  Serial.println("Group 2:");
  for (int i = 0; i < 2; i++) {
    float busVoltage = ina219_group2[i].getBusVoltage_V();
    float current_mA = ina219_group2[i].getCurrent_mA();
    float power_mW = ina219_group2[i].getPower_mW();
    
    Serial.print("  Module ");
    Serial.print(4 + i);
    Serial.print(": ");
    Serial.print(busVoltage, 2);
    Serial.print("V, ");
    Serial.print(current_mA, 0);
    Serial.print("mA, ");
    Serial.print(power_mW, 0);
    Serial.println("mW");
  }
  
  Serial.println();
  delay(1000);
}
