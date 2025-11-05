# Fehlerbehebung / Troubleshooting Guide

## Allgemeine Diagnose-Schritte

### 1. Serial Monitor prüfen
```
Öffne Serial Monitor bei 115200 Baud
Achte auf:
- Initialisierungsmeldungen
- Fehlermeldungen
- Sensor-Status
- Spannungs-/Stromwerte
```

### 2. Visuelle Inspektion
- LEDs leuchten?
- Relais schalten hörbar?
- Lüfter dreht sich?
- Kabel richtig verbunden?
- Lötverbindungen in Ordnung?

### 3. Multimeter-Messungen
- 3.3V und 5V Versorgung prüfen
- GND-Verbindungen prüfen
- Signal-Pins auf HIGH/LOW prüfen

## Häufige Probleme und Lösungen

### Problem: ESP32 startet nicht

**Symptome:**
- Keine Serial-Ausgabe
- LEDs bleiben dunkel
- Kein USB-Device erkannt

**Lösungen:**
1. **Stromversorgung prüfen**
   ```
   Messe 5V am VIN-Pin
   Messe 3.3V am 3V3-Pin
   Prüfe USB-Kabel
   ```

2. **Boot-Mode prüfen**
   ```
   GPIO0 muss beim Start HIGH sein
   Halte BOOT-Taste beim Reset NICHT gedrückt
   ```

3. **Reset durchführen**
   ```
   Drücke RST/EN Taste
   Trenne Stromversorgung für 10 Sekunden
   ```

### Problem: NeoPixel LEDs leuchten nicht

**Symptome:**
- Alle LEDs aus
- Flackern
- Falsche Farben

**Lösungen:**
1. **Stromversorgung prüfen**
   ```
   NeoPixels brauchen 5V (nicht 3.3V!)
   Messe Spannung direkt am LED-Strip
   Prüfe Stromstärke (max. 60mA pro LED)
   ```

2. **Data-Leitung prüfen**
   ```
   Prüfe Pin D12 Verbindung
   Füge 300-500Ω Widerstand in Data-Leitung ein
   Verwende kurzes Kabel (< 1m)
   ```

3. **Code prüfen**
   ```cpp
   // Überprüfe in ChargingStation.ino:
   #define NEOPIXEL_PIN 12  // Richtiger Pin?
   #define NUM_LEDS 6       // Richtige Anzahl?
   
   // Test mit einfachem Code:
   strip.setPixelColor(0, strip.Color(255, 0, 0));
   strip.show();
   ```

4. **Kondensator hinzufügen**
   ```
   1000µF zwischen 5V und GND
   Nahe am LED-Strip platzieren
   ```

### Problem: INA219 Sensoren nicht gefunden

**Symptome:**
- "Failed to initialize INA219" im Serial Monitor
- Spannung immer 0V
- Keine I2C-Kommunikation

**Lösungen:**
1. **I2C-Verbindungen prüfen**
   ```
   Gruppe 1: SCL=D35, SDA=D34
   Gruppe 2: SCL=D25, SDA=D26
   
   Prüfe mit Multimeter:
   - SDA/SCL sollten HIGH sein (Pull-ups)
   - Bei Kommunikation sollten sie pulsieren
   ```

2. **Pull-up Widerstände hinzufügen**
   ```
   4.7kΩ zwischen SDA und 3.3V
   4.7kΩ zwischen SCL und 3.3V
   Für jeden I2C-Bus separat
   ```

3. **I2C-Scanner verwenden**
   ```cpp
   // Verwende Wire.begin() mit richtigen Pins
   Wire.begin(34, 35);  // SDA, SCL für Gruppe 1
   
   // Scanne nach Devices
   for(byte addr = 1; addr < 127; addr++) {
     Wire.beginTransmission(addr);
     if (Wire.endTransmission() == 0) {
       Serial.print("Device at 0x");
       Serial.println(addr, HEX);
     }
   }
   ```

4. **Adressen überprüfen**
   ```
   INA219 Adressen durch A0/A1 einstellen:
   0x40: A0=GND, A1=GND
   0x41: A0=VCC, A1=GND
   0x42: A0=GND, A1=VCC
   0x43: A0=VCC, A1=VCC
   ```

5. **Spannung prüfen**
   ```
   INA219 braucht 3-5V auf VCC
   Messe am VCC-Pin des Moduls
   ```

### Problem: Temperatursensor zeigt -127°C oder DEVICE_DISCONNECTED

**Symptome:**
- Temperatur = -127°C
- "DEVICE_DISCONNECTED_C" Fehler
- Keine Temperaturwerte

**Lösungen:**
1. **Verkabelung prüfen**
   ```
   DS18B20 Pinout (TO-92):
   Flache Seite zu dir:
   Links:  GND
   Mitte:  Data (Pin D32)
   Rechts: VCC (3.3V)
   ```

2. **Pull-up Widerstand**
   ```
   4.7kΩ zwischen Data und VCC ERFORDERLICH!
   Ohne Pull-up funktioniert OneWire nicht
   ```

3. **Sensor testen**
   ```cpp
   // Teste mit test_temperature.ino
   // Prüfe Anzahl gefundener Sensoren:
   Serial.println(tempSensor.getDeviceCount());
   // Sollte mindestens 1 sein
   ```

4. **Pin-Nummer prüfen**
   ```cpp
   #define TEMP_SENSOR_PIN 32  // Nicht 23!
   ```

### Problem: Relais schalten nicht

**Symptome:**
- Kein hörbares Klicken
- Keine Spannung am Ausgang
- LED am Relais-Modul leuchtet nicht

**Lösungen:**
1. **Pin-Verbindung prüfen**
   ```
   Relais 1: Pin D27
   Relais 2: Pin D13
   
   Messe mit Multimeter:
   HIGH = ~3.3V
   LOW = ~0V
   ```

2. **Relais-Logik prüfen**
   ```cpp
   // WICHTIG: Normally Closed!
   digitalWrite(RELAY1_PIN, HIGH);  // Öffnet Relais (Power ON)
   digitalWrite(RELAY2_PIN, LOW);   // Schließt Relais (Power OFF)
   ```

3. **Stromversorgung prüfen**
   ```
   Relais-Spule braucht 5V
   Prüfe VCC am Relais-Modul
   Strom: typisch 70-80mA pro Relais
   ```

4. **Test mit einfachem Code**
   ```cpp
   // Verwende test_relay.ino
   // Oder direkter Test:
   pinMode(27, OUTPUT);
   digitalWrite(27, HIGH);
   delay(1000);
   digitalWrite(27, LOW);
   // Hörbares Klicken?
   ```

### Problem: Lüfter dreht sich nicht

**Symptome:**
- Lüfter bleibt stehen
- Lüfter läuft immer auf voller Geschwindigkeit
- Keine PWM-Steuerung

**Lösungen:**
1. **Stromversorgung prüfen**
   ```
   Lüfter braucht 12V (nicht 5V!)
   Messe am Lüfter-Anschluss
   Prüfe Stromaufnahme (0.2-0.5A typisch)
   ```

2. **PWM-Verbindung prüfen**
   ```
   Pin D14 an PWM-Eingang des Lüfters
   4-Pin Lüfter benötigt:
   - Schwarz: GND
   - Rot: +12V
   - Gelb: Tacho (optional)
   - Blau: PWM
   ```

3. **PWM-Frequenz**
   ```cpp
   // Lüfter brauchen meist 25kHz
   #define PWM_FREQ 25000
   
   // Manche Lüfter: 21kHz
   // Ältere Lüfter: 100-150Hz
   ```

4. **Minimale Geschwindigkeit**
   ```cpp
   // Zu niedrige Werte: Lüfter startet nicht
   #define MIN_FAN_SPEED 50  // Erhöhe auf 80-100
   ```

5. **Test mit voller Geschwindigkeit**
   ```cpp
   ledcWrite(PWM_CHANNEL, 255);
   // Läuft der Lüfter? -> PWM OK
   // Läuft nicht? -> Hardware-Problem
   ```

### Problem: Notabschaltung wird ständig aktiviert

**Symptome:**
- "EMERGENCY SHUTDOWN ACTIVATED" im Serial
- Relais schalten sofort ab
- Alle LEDs blinken blau

**Lösungen:**
1. **Temperatur prüfen**
   ```
   Ist currentTemp wirklich > 80°C?
   Prüfe Sensor-Kalibrierung
   Erhöhe TEMP_THRESHOLD_CRITICAL falls nötig
   ```

2. **Spannungen prüfen**
   ```
   Zeigt INA219 korrekte Werte?
   Sind Spannungen > 5.5V?
   
   // Erhöhe Schwellwert falls nötig:
   #define VOLTAGE_THRESHOLD 6.0  // War 5.5
   ```

3. **INA219 Fehler**
   ```
   Falsche Messwerte durch:
   - Schlechte I2C-Verbindung
   - Sensor defekt
   - Keine Kalibrierung
   ```

### Problem: Falsche Spannungswerte von INA219

**Symptome:**
- Negative Spannungen
- Zu hohe Werte (>26V)
- Sprunghafte Werte
- Immer 0V

**Lösungen:**
1. **Verdrahtung prüfen**
   ```
   INA219 Anschlüsse:
   VIN+ : Eingang positiv (von Quelle)
   VIN- : Eingang negativ / Ausgang positiv
   GND  : Ground
   VCC  : 3-5V Versorgung
   ```

2. **Shunt-Widerstand**
   ```
   Standard: 0.1Ω, 1%
   Max. Strom: 3.2A (default)
   
   Bei höheren Strömen: Kalibrierung nötig
   ```

3. **Kalibrierung**
   ```cpp
   // 16V, 400mA (default)
   ina219.setCalibration_16V_400mA();
   
   // Oder für höhere Ströme:
   ina219.setCalibration_32V_1A();
   ina219.setCalibration_32V_2A();
   ```

### Problem: LEDs zeigen falsche Farben

**Symptome:**
- Grün statt Rot
- Falsche Farbabfolge
- Nur bestimmte Farben funktionieren

**Lösungen:**
1. **Farbreihenfolge prüfen**
   ```cpp
   // Richtig für WS2812B:
   NEO_GRB + NEO_KHZ800
   
   // Manche Strips brauchen:
   NEO_RGB + NEO_KHZ800
   ```

2. **Farbdefinitionen anpassen**
   ```cpp
   // GRB Format:
   strip.Color(G, R, B)
   
   // RGB Format:
   strip.Color(R, G, B)
   ```

### Problem: System friert ein oder resettet zufällig

**Symptome:**
- Watchdog Timer Reset
- Brownout Reset
- Hängt in Schleife

**Lösungen:**
1. **Stromversorgung stabilisieren**
   ```
   Zu viele Verbraucher an ESP32?
   Separate Netzteile für:
   - ESP32
   - NeoPixels
   - Relais
   - Lüfter
   ```

2. **Kondensatoren hinzufügen**
   ```
   100µF am ESP32 VIN
   1000µF bei NeoPixels
   100nF bei jedem INA219
   ```

3. **Watchdog Timer**
   ```cpp
   // Wenn Code zu lange braucht:
   delay() in setup() reduzieren
   Zeitkritische Operationen optimieren
   ```

4. **Stack Overflow**
   ```cpp
   // Zu viele lokale Variablen?
   // Zu tiefe Rekursion?
   // Verkleinere Arrays falls möglich
   ```

## Debugging-Tipps

### Serial Monitor optimal nutzen
```cpp
// Verbose Output während Entwicklung:
#define DEBUG 1

#ifdef DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif
```

### LED-Blink-Codes einbauen
```cpp
// Onboard LED für Status:
void blinkCode(int times) {
  for(int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
}
```

### I2C-Scanner-Funktion
```cpp
void scanI2C(TwoWire &wire) {
  Serial.println("Scanning I2C bus...");
  for(byte addr = 1; addr < 127; addr++) {
    wire.beginTransmission(addr);
    if(wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      Serial.println(addr, HEX);
    }
  }
}
```

## Support und weitere Hilfe

### Logfiles erstellen
```
1. Öffne Serial Monitor
2. Kopiere komplette Ausgabe
3. Speichere als .txt Datei
4. Teile bei Supportanfrage
```

### Fotos für Support
- Gesamtaufbau
- Verkabelung Nahaufnahme
- Alle Pin-Verbindungen
- Relais-Modul
- INA219 Adress-Jumper

### Weitere Ressourcen
- ESP32 Forum: esp32.com
- Arduino Forum: forum.arduino.cc
- Adafruit Forums: forums.adafruit.com
- GitHub Issues: im Repository Issues erstellen

## Reset auf Werkseinstellungen

Wenn nichts mehr hilft:

1. **Code neu flashen**
   ```
   Tools -> Erase Flash -> All Flash Contents
   Dann Sketch neu hochladen
   ```

2. **Hardware-Reset**
   ```
   Alle Verbindungen trennen
   5 Minuten warten
   Systematisch neu verkabeln und testen
   ```

3. **Komponenten einzeln testen**
   ```
   Verwende die Test-Programme:
   - test_relay.ino
   - test_ina219.ino
   - test_neopixel.ino
   - test_temperature.ino
   - test_fan.ino
   ```
