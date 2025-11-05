/*
 * Test Program: Relay Control
 * 
 * Tests the relay functionality for emergency shutdown
 * - Tests relay pins D27 and D13
 * - Toggles relays on/off every 3 seconds
 * - Shows current relay state on serial monitor
 */

#define RELAY1_PIN 27  // Relay for modules 1-3
#define RELAY2_PIN 13  // Relay for modules 4-6

bool relay1State = true;  // true = open (power on)
bool relay2State = true;

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Relay Control Test ===");
  Serial.println("Testing relays on pins D27 and D13");
  Serial.println("Relays are NORMALLY CLOSED");
  Serial.println("HIGH = Open (power ON), LOW = Closed (power OFF)");
  Serial.println();
  
  // Initialize relay pins
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  
  // Start with relays open (power on)
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  
  Serial.println("Relays initialized - Both OPEN (power ON)");
}

void loop() {
  static unsigned long lastToggle = 0;
  
  if (millis() - lastToggle > 3000) {
    // Toggle both relays
    relay1State = !relay1State;
    relay2State = !relay2State;
    
    digitalWrite(RELAY1_PIN, relay1State ? HIGH : LOW);
    digitalWrite(RELAY2_PIN, relay2State ? HIGH : LOW);
    
    Serial.println("\n--- Relay Status Update ---");
    Serial.print("Relay 1 (D27): ");
    Serial.println(relay1State ? "OPEN (Power ON)" : "CLOSED (Power OFF)");
    Serial.print("Relay 2 (D13): ");
    Serial.println(relay2State ? "OPEN (Power ON)" : "CLOSED (Power OFF)");
    
    lastToggle = millis();
  }
}
