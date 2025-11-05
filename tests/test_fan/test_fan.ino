/*
 * Test Program: Fan PWM Control
 * 
 * Tests the fan PWM control functionality
 * - Fan connected to pin D14
 * - Cycles through different fan speeds
 * - Tests temperature-based speed control simulation
 */

#define FAN_PWM_PIN 14

#define PWM_FREQ 25000
#define PWM_CHANNEL 0
#define PWM_RESOLUTION 8

int currentSpeed = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Fan PWM Control Test ===");
  Serial.println("Testing fan on pin D14");
  
  // Initialize PWM
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(FAN_PWM_PIN, PWM_CHANNEL);
  
  // Start with fan off
  ledcWrite(PWM_CHANNEL, 0);
  
  Serial.println("PWM initialized");
  Serial.println("Frequency: 25 kHz");
  Serial.println("Resolution: 8 bits (0-255)");
  delay(1000);
}

void loop() {
  // Test 1: Gradual speed increase
  Serial.println("\nTest 1: Gradual speed increase (0% to 100%)");
  for (int speed = 0; speed <= 255; speed += 5) {
    ledcWrite(PWM_CHANNEL, speed);
    currentSpeed = speed;
    
    if (speed % 25 == 0) {
      Serial.print("  Fan Speed: ");
      Serial.print((speed * 100) / 255);
      Serial.println("%");
    }
    delay(100);
  }
  
  delay(2000);
  
  // Test 2: Gradual speed decrease
  Serial.println("\nTest 2: Gradual speed decrease (100% to 0%)");
  for (int speed = 255; speed >= 0; speed -= 5) {
    ledcWrite(PWM_CHANNEL, speed);
    currentSpeed = speed;
    
    if (speed % 25 == 0) {
      Serial.print("  Fan Speed: ");
      Serial.print((speed * 100) / 255);
      Serial.println("%");
    }
    delay(100);
  }
  
  delay(2000);
  
  // Test 3: Temperature-based speed simulation
  Serial.println("\nTest 3: Temperature-based speed simulation");
  
  float temperatures[] = {25.0, 40.0, 50.0, 60.0, 70.0, 80.0};
  int numTemps = sizeof(temperatures) / sizeof(temperatures[0]);
  
  for (int i = 0; i < numTemps; i++) {
    float simTemp = temperatures[i];
    int fanSpeed = calculateFanSpeed(simTemp);
    
    ledcWrite(PWM_CHANNEL, fanSpeed);
    currentSpeed = fanSpeed;
    
    Serial.print("  Temp: ");
    Serial.print(simTemp);
    Serial.print(" °C -> Fan Speed: ");
    Serial.print((fanSpeed * 100) / 255);
    Serial.println("%");
    
    delay(2000);
  }
  
  // Test 4: Step through specific speeds
  Serial.println("\nTest 4: Specific speed steps");
  int speeds[] = {0, 64, 128, 192, 255};
  String labels[] = {"0% (Off)", "25%", "50%", "75%", "100% (Full)"};
  
  for (int i = 0; i < 5; i++) {
    ledcWrite(PWM_CHANNEL, speeds[i]);
    currentSpeed = speeds[i];
    
    Serial.print("  ");
    Serial.println(labels[i]);
    delay(2000);
  }
  
  Serial.println("\nTest sequence complete. Restarting...\n");
  delay(3000);
}

int calculateFanSpeed(float temperature) {
  // Minimum and maximum speeds
  const int MIN_SPEED = 50;
  const int MAX_SPEED = 255;
  
  // Temperature thresholds
  const float TEMP_LOW = 50.0;
  const float TEMP_HIGH = 80.0;
  
  if (temperature <= TEMP_LOW) {
    return MIN_SPEED;
  } else if (temperature >= TEMP_HIGH) {
    return MAX_SPEED;
  } else {
    // Linear interpolation between thresholds
    float tempRange = TEMP_HIGH - TEMP_LOW;
    float tempDiff = temperature - TEMP_LOW;
    float speedRange = MAX_SPEED - MIN_SPEED;
    
    return MIN_SPEED + (int)((tempDiff / tempRange) * speedRange);
  }
}
