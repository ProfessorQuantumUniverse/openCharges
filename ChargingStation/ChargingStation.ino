/*
 * ESP32 Charging Station with Status LEDs
 * 
 * Hardware Configuration:
 * - 6 USB-C PD Charging Modules (max 45W each)
 * - 2 Relays (Normally Closed): Pin D27 (Modules 1-3), Pin D13 (Modules 4-6)
 * - 6 INA219 Current/Voltage Sensors
 *   - Group 1 (4 sensors): SCL D35, SDA D34 (Addresses 0x40-0x43)
 *   - Group 2 (2 sensors): SCL D25, SDA D26 (Addresses 0x40-0x41)
 * - NeoPixel LEDs: Pin D12 (6 LEDs, one per module)
 * - Temperature Sensor: Pin D32
 * - Fan PWM: Pin D14
 */

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_NeoPixel.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin Definitions
#define RELAY1_PIN 27       // Relay for modules 1-3
#define RELAY2_PIN 13       // Relay for modules 4-6
#define NEOPIXEL_PIN 12     // NeoPixel LEDs
#define TEMP_SENSOR_PIN 32  // Temperature sensor
#define FAN_PWM_PIN 14      // Fan PWM control
#define I2C1_SDA 34         // I2C Group 1 SDA
#define I2C1_SCL 35         // I2C Group 1 SCL
#define I2C2_SDA 26         // I2C Group 2 SDA
#define I2C2_SCL 25         // I2C Group 2 SCL

// Configuration
#define NUM_MODULES 6
#define NUM_LEDS 6
#define VOLTAGE_THRESHOLD 5.5       // Max voltage in Volts
#define TEMP_THRESHOLD_CRITICAL 80.0 // Critical temperature in °C
#define TEMP_THRESHOLD_HIGH 50.0     // High temperature for fan speed up
#define MIN_FAN_SPEED 50             // Minimum fan PWM (0-255)
#define MAX_FAN_SPEED 255            // Maximum fan PWM (0-255)
#define CHARGING_VOLTAGE_MIN 0.5     // Minimum voltage to consider device connected
#define FULL_CHARGE_VOLTAGE 5.0      // Voltage considered as full charge

// PWM Settings for Fan
#define PWM_FREQ 25000
#define PWM_CHANNEL 0
#define PWM_RESOLUTION 8

// INA219 Sensors
Adafruit_INA219 ina219_group1[4];  // Group 1: 4 sensors
Adafruit_INA219 ina219_group2[2];  // Group 2: 2 sensors

// NeoPixel
Adafruit_NeoPixel strip(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Temperature Sensor
OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

// I2C Buses
TwoWire I2C_1 = TwoWire(0);
TwoWire I2C_2 = TwoWire(1);

// State Variables
float moduleVoltages[NUM_MODULES] = {0};
float moduleCurrents[NUM_MODULES] = {0};
bool relayStates[2] = {true, true};  // true = relay open (power on)
float currentTemp = 0;
int currentFanSpeed = MIN_FAN_SPEED;
bool emergencyShutdown = false;

// Color Definitions (GRB format)
#define COLOR_OFF       strip.Color(0, 0, 0)
#define COLOR_CHARGING  strip.Color(255, 255, 0)   // Yellow
#define COLOR_LOW       strip.Color(255, 0, 0)     // Red
#define COLOR_MEDIUM    strip.Color(255, 165, 0)   // Orange
#define COLOR_HIGH      strip.Color(255, 255, 0)   // Yellow
#define COLOR_FULL      strip.Color(0, 255, 0)     // Green
#define COLOR_ERROR     strip.Color(0, 0, 255)     // Blue (blinking)

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Charging Station Initializing...");
  
  // Initialize Relay Pins (Normally Closed - must open to allow power)
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, HIGH);  // Open relay (allow power)
  digitalWrite(RELAY2_PIN, HIGH);  // Open relay (allow power)
  Serial.println("Relays opened - Power enabled");
  
  // Initialize Fan PWM
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(FAN_PWM_PIN, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, MIN_FAN_SPEED);
  Serial.println("Fan PWM initialized");
  
  // Initialize I2C Buses
  I2C_1.begin(I2C1_SDA, I2C1_SCL, 100000);
  I2C_2.begin(I2C2_SDA, I2C2_SCL, 100000);
  Serial.println("I2C buses initialized");
  
  // Initialize INA219 Sensors - Group 1 (4 sensors)
  Serial.println("Initializing INA219 sensors - Group 1...");
  for (int i = 0; i < 4; i++) {
    if (!ina219_group1[i].begin(0x40 + i, &I2C_1)) {
      Serial.print("Failed to initialize INA219 #");
      Serial.print(i);
      Serial.println(" on Group 1");
    } else {
      Serial.print("INA219 #");
      Serial.print(i);
      Serial.println(" on Group 1 initialized");
    }
  }
  
  // Initialize INA219 Sensors - Group 2 (2 sensors)
  Serial.println("Initializing INA219 sensors - Group 2...");
  for (int i = 0; i < 2; i++) {
    if (!ina219_group2[i].begin(0x40 + i, &I2C_2)) {
      Serial.print("Failed to initialize INA219 #");
      Serial.print(i);
      Serial.println(" on Group 2");
    } else {
      Serial.print("INA219 #");
      Serial.print(i);
      Serial.println(" on Group 2 initialized");
    }
  }
  
  // Initialize NeoPixels
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.println("NeoPixels initialized");
  
  // Initialize Temperature Sensor
  tempSensor.begin();
  Serial.println("Temperature sensor initialized");
  
  Serial.println("Initialization complete!");
  delay(1000);
}

void loop() {
  // Read all sensors
  readVoltageSensors();
  readTemperature();
  
  // Check for emergency conditions
  checkEmergencyConditions();
  
  // Update LED status
  updateLEDStatus();
  
  // Control fan speed based on temperature
  controlFan();
  
  // Print status every 2 seconds
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 2000) {
    printStatus();
    lastPrint = millis();
  }
  
  delay(100);  // Main loop delay
}

void readVoltageSensors() {
  // Read Group 1 (modules 0-3)
  for (int i = 0; i < 4; i++) {
    float shuntVoltage = ina219_group1[i].getShuntVoltage_mV();
    float busVoltage = ina219_group1[i].getBusVoltage_V();
    float current_mA = ina219_group1[i].getCurrent_mA();
    
    moduleVoltages[i] = busVoltage;
    moduleCurrents[i] = current_mA;
  }
  
  // Read Group 2 (modules 4-5)
  for (int i = 0; i < 2; i++) {
    float shuntVoltage = ina219_group2[i].getShuntVoltage_mV();
    float busVoltage = ina219_group2[i].getBusVoltage_V();
    float current_mA = ina219_group2[i].getCurrent_mA();
    
    moduleVoltages[4 + i] = busVoltage;
    moduleCurrents[4 + i] = current_mA;
  }
}

void readTemperature() {
  tempSensor.requestTemperatures();
  currentTemp = tempSensor.getTempCByIndex(0);
  
  // Check for sensor error
  if (currentTemp == DEVICE_DISCONNECTED_C) {
    currentTemp = 0;
  }
}

void checkEmergencyConditions() {
  bool shouldShutdown = false;
  
  // Check temperature threshold
  if (currentTemp > TEMP_THRESHOLD_CRITICAL) {
    Serial.println("EMERGENCY: Critical temperature exceeded!");
    shouldShutdown = true;
  }
  
  // Check voltage thresholds
  for (int i = 0; i < NUM_MODULES; i++) {
    if (moduleVoltages[i] > VOLTAGE_THRESHOLD) {
      Serial.print("EMERGENCY: Voltage too high on module ");
      Serial.println(i);
      shouldShutdown = true;
    }
  }
  
  // Activate emergency shutdown if needed
  if (shouldShutdown && !emergencyShutdown) {
    emergencyShutdown = true;
    digitalWrite(RELAY1_PIN, LOW);  // Close relay (cut power)
    digitalWrite(RELAY2_PIN, LOW);  // Close relay (cut power)
    relayStates[0] = false;
    relayStates[1] = false;
    Serial.println("EMERGENCY SHUTDOWN ACTIVATED!");
    
    // Flash all LEDs red
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, COLOR_ERROR);
    }
    strip.show();
  }
}

void updateLEDStatus() {
  if (emergencyShutdown) {
    // Blink all LEDs blue in emergency mode
    static unsigned long lastBlink = 0;
    static bool blinkState = false;
    
    if (millis() - lastBlink > 500) {
      blinkState = !blinkState;
      uint32_t color = blinkState ? COLOR_ERROR : COLOR_OFF;
      for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, color);
      }
      strip.show();
      lastBlink = millis();
    }
    return;
  }
  
  // Update each LED based on charging status
  for (int i = 0; i < NUM_MODULES; i++) {
    float voltage = moduleVoltages[i];
    float current = moduleCurrents[i];
    
    uint32_t color;
    
    if (voltage < CHARGING_VOLTAGE_MIN) {
      // No device connected
      color = COLOR_OFF;
    } else if (voltage >= FULL_CHARGE_VOLTAGE || current < 50) {
      // Full charge (voltage high or very low current)
      color = COLOR_FULL;
    } else if (voltage < 3.5) {
      // Low charge
      color = COLOR_LOW;
    } else if (voltage < 4.0) {
      // Medium charge
      color = COLOR_MEDIUM;
    } else {
      // High charge (charging)
      color = COLOR_HIGH;
    }
    
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void controlFan() {
  int targetSpeed;
  
  if (currentTemp > TEMP_THRESHOLD_HIGH) {
    // High temperature - ramp up fan speed
    float tempRange = TEMP_THRESHOLD_CRITICAL - TEMP_THRESHOLD_HIGH;
    float tempDiff = currentTemp - TEMP_THRESHOLD_HIGH;
    float speedRange = MAX_FAN_SPEED - MIN_FAN_SPEED;
    
    targetSpeed = MIN_FAN_SPEED + (tempDiff / tempRange) * speedRange;
    targetSpeed = constrain(targetSpeed, MIN_FAN_SPEED, MAX_FAN_SPEED);
  } else {
    // Normal temperature - minimum speed
    targetSpeed = MIN_FAN_SPEED;
  }
  
  // Smoothly adjust fan speed
  if (targetSpeed != currentFanSpeed) {
    currentFanSpeed = targetSpeed;
    ledcWrite(PWM_CHANNEL, currentFanSpeed);
  }
}

void printStatus() {
  Serial.println("\n=== Charging Station Status ===");
  Serial.print("Temperature: ");
  Serial.print(currentTemp);
  Serial.println(" °C");
  
  Serial.print("Fan Speed: ");
  Serial.print((currentFanSpeed * 100) / 255);
  Serial.println(" %");
  
  Serial.print("Relays: R1=");
  Serial.print(relayStates[0] ? "ON" : "OFF");
  Serial.print(", R2=");
  Serial.println(relayStates[1] ? "ON" : "OFF");
  
  if (emergencyShutdown) {
    Serial.println("*** EMERGENCY SHUTDOWN ACTIVE ***");
  }
  
  Serial.println("\nModule Status:");
  for (int i = 0; i < NUM_MODULES; i++) {
    Serial.print("  Module ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(moduleVoltages[i], 2);
    Serial.print(" V, ");
    Serial.print(moduleCurrents[i], 0);
    Serial.print(" mA");
    
    if (moduleVoltages[i] < CHARGING_VOLTAGE_MIN) {
      Serial.println(" [No Device]");
    } else if (moduleVoltages[i] >= FULL_CHARGE_VOLTAGE) {
      Serial.println(" [Full]");
    } else {
      Serial.println(" [Charging]");
    }
  }
  Serial.println("==============================");
}
