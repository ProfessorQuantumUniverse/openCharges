/*
 * Test Program: NeoPixel LED Control
 * 
 * Tests the NeoPixel LED strip for charging status display
 * - 6 NeoPixels on pin D12
 * - Cycles through different colors and patterns
 * - Tests individual LED control
 */

#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN 12
#define NUM_LEDS 6

Adafruit_NeoPixel strip(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Color definitions
#define COLOR_OFF    strip.Color(0, 0, 0)
#define COLOR_RED    strip.Color(255, 0, 0)
#define COLOR_GREEN  strip.Color(0, 255, 0)
#define COLOR_BLUE   strip.Color(0, 0, 255)
#define COLOR_YELLOW strip.Color(255, 255, 0)
#define COLOR_ORANGE strip.Color(255, 165, 0)
#define COLOR_WHITE  strip.Color(255, 255, 255)

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== NeoPixel LED Test ===");
  Serial.println("Testing 6 NeoPixels on pin D12");
  
  strip.begin();
  strip.setBrightness(50); // Set to 50% brightness for testing
  strip.show(); // Initialize all pixels to 'off'
  
  Serial.println("NeoPixels initialized");
  delay(1000);
}

void loop() {
  // Test 1: All LEDs Red
  Serial.println("Test 1: All LEDs Red");
  setAllLEDs(COLOR_RED);
  delay(1000);
  
  // Test 2: All LEDs Green
  Serial.println("Test 2: All LEDs Green");
  setAllLEDs(COLOR_GREEN);
  delay(1000);
  
  // Test 3: All LEDs Blue
  Serial.println("Test 3: All LEDs Blue");
  setAllLEDs(COLOR_BLUE);
  delay(1000);
  
  // Test 4: All LEDs Yellow
  Serial.println("Test 4: All LEDs Yellow");
  setAllLEDs(COLOR_YELLOW);
  delay(1000);
  
  // Test 5: All LEDs Orange
  Serial.println("Test 5: All LEDs Orange");
  setAllLEDs(COLOR_ORANGE);
  delay(1000);
  
  // Test 6: Individual LED test
  Serial.println("Test 6: Individual LED test (one at a time)");
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.clear();
    strip.setPixelColor(i, COLOR_WHITE);
    strip.show();
    Serial.print("  LED ");
    Serial.print(i);
    Serial.println(" ON");
    delay(500);
  }
  
  // Test 7: Rainbow wave
  Serial.println("Test 7: Rainbow wave");
  rainbowCycle(5);
  
  // Test 8: Charging status simulation
  Serial.println("Test 8: Charging status simulation");
  simulateChargingStatus();
  
  // Test 9: Emergency blink
  Serial.println("Test 9: Emergency blink (blue)");
  for (int i = 0; i < 6; i++) {
    setAllLEDs(COLOR_BLUE);
    delay(250);
    setAllLEDs(COLOR_OFF);
    delay(250);
  }
  
  Serial.println("\nTest sequence complete. Restarting...\n");
  delay(2000);
}

void setAllLEDs(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void rainbowCycle(int cycles) {
  for (int j = 0; j < 256 * cycles; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / NUM_LEDS) + j) & 255));
    }
    strip.show();
    delay(5);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void simulateChargingStatus() {
  // Simulate different charging states
  uint32_t states[] = {COLOR_OFF, COLOR_RED, COLOR_ORANGE, COLOR_YELLOW, COLOR_GREEN, COLOR_GREEN};
  String stateNames[] = {"No Device", "Low Charge", "Medium", "High", "Full", "Full"};
  
  for (int state = 0; state < 6; state++) {
    Serial.print("  Simulating: ");
    Serial.println(stateNames[state]);
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, states[state]);
    }
    strip.show();
    delay(1000);
  }
}
