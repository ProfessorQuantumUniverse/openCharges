# Konfigurationsbeispiele / Configuration Examples

## Standard-Konfiguration (Default)

Dies ist die Standard-Konfiguration wie im Hauptprogramm verwendet:

```cpp
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
#define VOLTAGE_THRESHOLD 5.5
#define TEMP_THRESHOLD_CRITICAL 80.0
#define TEMP_THRESHOLD_HIGH 50.0
#define MIN_FAN_SPEED 50
#define MAX_FAN_SPEED 255
#define CHARGING_VOLTAGE_MIN 0.5
#define FULL_CHARGE_VOLTAGE 5.0
```

## Beispiel 1: Konservative Einstellungen (Mehr Sicherheit)

Für eine konservativere Konfiguration mit höherer Sicherheit:

```cpp
// Niedrigere Temperatur-Schwellwerte
#define TEMP_THRESHOLD_CRITICAL 70.0  // Früher abschalten
#define TEMP_THRESHOLD_HIGH 40.0      // Früher Lüfter hochdrehen

// Niedrigere Spannungs-Schwelle
#define VOLTAGE_THRESHOLD 5.3         // Konservativer

// Höhere Mindest-Lüftergeschwindigkeit
#define MIN_FAN_SPEED 80              // ~31% statt ~20%

// Höhere Volllade-Erkennung
#define FULL_CHARGE_VOLTAGE 4.9       // Früher "voll" anzeigen
```

## Beispiel 2: Leistungsstarke Konfiguration

Für bessere Kühlleistung und höhere Belastbarkeit:

```cpp
// Höhere Temperatur-Toleranz
#define TEMP_THRESHOLD_CRITICAL 85.0  // Höhere Grenze
#define TEMP_THRESHOLD_HIGH 55.0      // Später hochdrehen

// Höhere Spannungs-Toleranz
#define VOLTAGE_THRESHOLD 5.8         // Mehr Spielraum

// Lüfter läuft schneller
#define MIN_FAN_SPEED 100             // ~40% Minimum
#define MAX_FAN_SPEED 255

// Genauere Ladezustands-Erkennung
#define CHARGING_VOLTAGE_MIN 1.0      // Höhere Mindestspannung
#define FULL_CHARGE_VOLTAGE 5.1       // Höhere Vollladespannung
```

## Beispiel 3: Leise Konfiguration (Nachteinsatz)

Für leisen Betrieb mit reduzierter Leistung:

```cpp
// Niedrigere Temperatur-Schwellen
#define TEMP_THRESHOLD_CRITICAL 75.0
#define TEMP_THRESHOLD_HIGH 45.0

// Lüfter läuft langsamer
#define MIN_FAN_SPEED 30              // ~12% - sehr leise
#define MAX_FAN_SPEED 180             // ~70% Maximum

// Konservative Spannungsgrenzen
#define VOLTAGE_THRESHOLD 5.2
```

## Beispiel 4: 4 Module statt 6

Wenn du nur 4 Lademodule verwendest:

```cpp
#define NUM_MODULES 4
#define NUM_LEDS 4

// INA219 Konfiguration anpassen in setup():
void setup() {
  // ...
  
  // Nur 4 Sensoren initialisieren
  for (int i = 0; i < 4; i++) {
    if (!ina219_group1[i].begin(0x40 + i, &I2C_1)) {
      Serial.print("Failed to initialize INA219 #");
      Serial.print(i);
      Serial.println(" on Group 1");
    }
  }
  
  // Zweite Gruppe nicht benötigt - auskommentieren
  
  // ...
}

// readVoltageSensors() anpassen:
void readVoltageSensors() {
  // Nur Group 1 lesen (4 Module)
  for (int i = 0; i < 4; i++) {
    float busVoltage = ina219_group1[i].getBusVoltage_V();
    float current_mA = ina219_group1[i].getCurrent_mA();
    moduleVoltages[i] = busVoltage;
    moduleCurrents[i] = current_mA;
  }
  // Group 2 Code entfernen
}
```

## Beispiel 5: Angepasste LED-Farben

Eigene LED-Farben definieren:

```cpp
// Hellere Farben
#define COLOR_OFF       strip.Color(0, 0, 0)
#define COLOR_LOW       strip.Color(255, 0, 0)     // Rot
#define COLOR_MEDIUM    strip.Color(200, 100, 0)   // Orange-Rot
#define COLOR_HIGH      strip.Color(150, 150, 0)   // Gelb-Grün
#define COLOR_FULL      strip.Color(0, 255, 0)     // Grün
#define COLOR_ERROR     strip.Color(255, 0, 255)   // Magenta

// Oder gedimmte Farben für Nacht
#define COLOR_LOW       strip.Color(80, 0, 0)      // Dunkelrot
#define COLOR_MEDIUM    strip.Color(80, 40, 0)     // Dunkelorange
#define COLOR_HIGH      strip.Color(80, 80, 0)     // Dunkelgelb
#define COLOR_FULL      strip.Color(0, 80, 0)      // Dunkelgrün
```

## Beispiel 6: PWM-Frequenz für spezielle Lüfter

Manche Lüfter benötigen andere PWM-Frequenzen:

```cpp
// Ältere 3-Pin Lüfter
#define PWM_FREQ 100        // Niedrige Frequenz

// Moderne 4-Pin PWM Lüfter
#define PWM_FREQ 25000      // Standard (Default)

// Noctua-Lüfter
#define PWM_FREQ 25000      // Optimal

// Server-Lüfter
#define PWM_FREQ 22500      // Leicht niedriger
```

## Beispiel 7: Erweiterte Spannungserkennung

Genauere Ladezustands-Erkennung:

```cpp
void updateLEDStatus() {
  for (int i = 0; i < NUM_MODULES; i++) {
    float voltage = moduleVoltages[i];
    float current = moduleCurrents[i];
    uint32_t color;
    
    if (voltage < 0.5) {
      color = COLOR_OFF;  // Kein Gerät
    } else if (current < 50 && voltage > 4.5) {
      color = COLOR_FULL;  // Voll geladen
    } else if (voltage < 3.0) {
      color = strip.Color(255, 0, 0);      // Sehr niedrig: Rot
    } else if (voltage < 3.5) {
      color = strip.Color(255, 50, 0);     // Niedrig: Orange-Rot
    } else if (voltage < 4.0) {
      color = strip.Color(255, 165, 0);    // Mittel: Orange
    } else if (voltage < 4.5) {
      color = strip.Color(255, 255, 0);    // Hoch: Gelb
    } else {
      color = strip.Color(128, 255, 0);    // Fast voll: Gelb-Grün
    }
    
    strip.setPixelColor(i, color);
  }
  strip.show();
}
```

## Beispiel 8: Temperaturbasierte Lüfter-Kurve

Alternative Lüfter-Steuerung mit nicht-linearer Kurve:

```cpp
void controlFan() {
  int targetSpeed;
  
  if (currentTemp < 40.0) {
    // Unter 40°C: Minimum
    targetSpeed = MIN_FAN_SPEED;
  } else if (currentTemp < 50.0) {
    // 40-50°C: Langsamer Anstieg (20% → 40%)
    targetSpeed = map(currentTemp * 10, 400, 500, 50, 100);
  } else if (currentTemp < 65.0) {
    // 50-65°C: Mittlerer Anstieg (40% → 70%)
    targetSpeed = map(currentTemp * 10, 500, 650, 100, 180);
  } else if (currentTemp < 80.0) {
    // 65-80°C: Schneller Anstieg (70% → 100%)
    targetSpeed = map(currentTemp * 10, 650, 800, 180, 255);
  } else {
    // Über 80°C: Maximum (vor Notabschaltung)
    targetSpeed = MAX_FAN_SPEED;
  }
  
  targetSpeed = constrain(targetSpeed, MIN_FAN_SPEED, MAX_FAN_SPEED);
  
  if (targetSpeed != currentFanSpeed) {
    currentFanSpeed = targetSpeed;
    ledcWrite(PWM_CHANNEL, currentFanSpeed);
  }
}
```

## Beispiel 9: Sanfte Notabschaltung mit Warnung

Warnung vor Notabschaltung:

```cpp
bool warningActive = false;
unsigned long warningStartTime = 0;
const unsigned long WARNING_DURATION = 10000; // 10 Sekunden Warnung

void checkEmergencyConditions() {
  bool shouldWarn = false;
  bool shouldShutdown = false;
  
  // Prüfe ob Warnung nötig
  if (currentTemp > 75.0 && currentTemp < TEMP_THRESHOLD_CRITICAL) {
    shouldWarn = true;
  }
  
  // Prüfe ob Shutdown nötig
  if (currentTemp > TEMP_THRESHOLD_CRITICAL) {
    shouldShutdown = true;
  }
  
  for (int i = 0; i < NUM_MODULES; i++) {
    if (moduleVoltages[i] > VOLTAGE_THRESHOLD) {
      shouldShutdown = true;
    } else if (moduleVoltages[i] > VOLTAGE_THRESHOLD - 0.3) {
      shouldWarn = true;
    }
  }
  
  // Warnung aktiv?
  if (shouldWarn && !warningActive && !emergencyShutdown) {
    warningActive = true;
    warningStartTime = millis();
    Serial.println("WARNING: System approaching critical values!");
  }
  
  // Warnung vorbei?
  if (!shouldWarn && warningActive) {
    warningActive = false;
    Serial.println("Warning cleared.");
  }
  
  // Nach Warnung automatisch abschalten?
  if (warningActive && (millis() - warningStartTime > WARNING_DURATION)) {
    shouldShutdown = true;
    Serial.println("Warning timeout - initiating shutdown!");
  }
  
  // LED-Warnung
  if (warningActive) {
    // Blinke LEDs gelb
    static unsigned long lastBlink = 0;
    static bool blinkState = false;
    if (millis() - lastBlink > 250) {
      blinkState = !blinkState;
      for (int i = 0; i < NUM_LEDS; i++) {
        if (blinkState) {
          strip.setPixelColor(i, strip.Color(255, 255, 0)); // Gelb
        }
      }
      strip.show();
      lastBlink = millis();
    }
  }
  
  // Notabschaltung
  if (shouldShutdown && !emergencyShutdown) {
    emergencyShutdown = true;
    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, LOW);
    relayStates[0] = false;
    relayStates[1] = false;
    Serial.println("EMERGENCY SHUTDOWN ACTIVATED!");
  }
}
```

## Beispiel 10: Debug-Modus

Ausführlichere Ausgaben für Debugging:

```cpp
#define DEBUG_MODE true  // Setze auf false für Normalbetrieb

void printStatus() {
  Serial.println("\n=== Charging Station Status ===");
  
  #if DEBUG_MODE
  Serial.print("Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println(" seconds");
  
  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  #endif
  
  Serial.print("Temperature: ");
  Serial.print(currentTemp);
  Serial.println(" °C");
  
  #if DEBUG_MODE
  Serial.print("Target Fan Speed: ");
  Serial.print((currentFanSpeed * 100) / 255);
  Serial.println(" %");
  #endif
  
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
    
    #if DEBUG_MODE
    float power = (moduleVoltages[i] * moduleCurrents[i]) / 1000.0;
    Serial.print(", ");
    Serial.print(power, 2);
    Serial.print(" W");
    #endif
    
    if (moduleVoltages[i] < CHARGING_VOLTAGE_MIN) {
      Serial.println(" [No Device]");
    } else if (moduleVoltages[i] >= FULL_CHARGE_VOLTAGE) {
      Serial.println(" [Full]");
    } else {
      Serial.println(" [Charging]");
    }
  }
  
  #if DEBUG_MODE
  // Gesamtleistung
  float totalPower = 0;
  for (int i = 0; i < NUM_MODULES; i++) {
    totalPower += (moduleVoltages[i] * moduleCurrents[i]) / 1000.0;
  }
  Serial.print("\nTotal Power: ");
  Serial.print(totalPower, 2);
  Serial.println(" W");
  #endif
  
  Serial.println("==============================");
}
```

## Hinweise zur Konfiguration

1. **Backup**: Sichere die Original-Werte bevor du änderst
2. **Testen**: Teste neue Konfiguration mit test_*.ino Programmen
3. **Schrittweise**: Ändere immer nur einen Parameter
4. **Dokumentation**: Notiere Änderungen und Gründe
5. **Sicherheit**: Setze kritische Schwellwerte nicht zu hoch

## Empfohlene Test-Prozedur

Nach Änderungen der Konfiguration:

1. Upload neuen Code
2. Prüfe Serial Monitor auf Initialisierung
3. Überwache Temperatur unter Last
4. Prüfe Lüfter-Verhalten
5. Teste Notabschaltung (simuliere hohe Temperatur)
6. Dokumentiere Ergebnisse

Viel Erfolg! 🔧
