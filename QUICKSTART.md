# Quick Start Guide / Schnellstart-Anleitung

## Schnellstart in 5 Schritten

### 1. Hardware vorbereiten
```
✓ ESP32 Board
✓ 6x INA219 Module (Adressen einstellen!)
✓ 2x Relais (Normally Closed)
✓ 6x NeoPixel LEDs
✓ 1x DS18B20 Temperatursensor
✓ 1x PWM Lüfter
✓ Alle Kabel und Widerstände
```

**Wichtig:** Siehe `PARTS_LIST.md` für komplette Teileliste!

### 2. Arduino IDE einrichten

#### ESP32 Board installieren
1. Arduino IDE öffnen
2. File → Preferences
3. Additional Boards Manager URLs:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Tools → Board → Boards Manager
5. Suche "ESP32" und installiere "esp32 by Espressif Systems"

#### Bibliotheken installieren
```
Sketch → Include Library → Manage Libraries

Installiere folgende Bibliotheken:
1. "Adafruit INA219" von Adafruit
2. "Adafruit NeoPixel" von Adafruit
3. "OneWire" von Paul Stoffregen
4. "DallasTemperature" von Miles Burton
```

### 3. Hardware verkabeln

#### INA219 Adressen konfigurieren
```
Gruppe 1 (I2C Bus 1):
- INA219 #0: A0=GND, A1=GND → 0x40
- INA219 #1: A0=VCC, A1=GND → 0x41
- INA219 #2: A0=GND, A1=VCC → 0x42
- INA219 #3: A0=VCC, A1=VCC → 0x43

Gruppe 2 (I2C Bus 2):
- INA219 #4: A0=GND, A1=GND → 0x40
- INA219 #5: A0=VCC, A1=GND → 0x41
```

#### Wichtige Verbindungen
```
Relais 1     → Pin D27
Relais 2     → Pin D13

I2C Bus 1    → SCL: D35, SDA: D34
I2C Bus 2    → SCL: D25, SDA: D26

NeoPixel     → Pin D12 (mit 300Ω Widerstand)
Temperatur   → Pin D32 (mit 4.7kΩ Pull-up)
Lüfter PWM   → Pin D14
```

**Detaillierte Verdrahtung:** Siehe `WIRING.md`

### 4. Test-Programme ausführen

**Teste jede Komponente einzeln, bevor du das Hauptprogramm lädst!**

#### a) Relais Test
```
Öffne: tests/test_relay/test_relay.ino
Upload zum ESP32
Serial Monitor öffnen (115200 baud)

Erwartung:
- Relais schalten alle 3 Sekunden
- Hörbares Klicken
- Status im Serial Monitor
```

#### b) INA219 Test
```
Öffne: tests/test_ina219/test_ina219.ino
Upload zum ESP32

Erwartung:
- Alle 6 Sensoren werden erkannt
- Spannungs- und Stromwerte werden angezeigt
- Keine "FAILED" Meldungen
```

#### c) NeoPixel Test
```
Öffne: tests/test_neopixel/test_neopixel.ino
Upload zum ESP32

Erwartung:
- Alle 6 LEDs leuchten in verschiedenen Farben
- Rainbow-Effekte
- Keine flackernden oder dunklen LEDs
```

#### d) Temperatur Test
```
Öffne: tests/test_temperature/test_temperature.ino
Upload zum ESP32

Erwartung:
- Temperaturwerte in °C und °F
- Keine -127°C Fehler
- Plausible Raumtemperatur (~20-25°C)
```

#### e) Lüfter Test
```
Öffne: tests/test_fan/test_fan.ino
Upload zum ESP32

Erwartung:
- Lüfter startet
- Geschwindigkeit variiert
- PWM-Steuerung funktioniert
```

### 5. Hauptprogramm laden

Wenn alle Tests erfolgreich waren:

```
1. Öffne: ChargingStation/ChargingStation.ino
2. Klicke auf "Upload"
3. Warte bis Upload abgeschlossen ist
4. Öffne Serial Monitor (115200 baud)
```

#### Erwartete Ausgabe beim Start:
```
ESP32 Charging Station Initializing...
Relays opened - Power enabled
Fan PWM initialized
I2C buses initialized
Initializing INA219 sensors - Group 1...
INA219 #0 on Group 1 initialized
INA219 #1 on Group 1 initialized
INA219 #2 on Group 1 initialized
INA219 #3 on Group 1 initialized
Initializing INA219 sensors - Group 2...
INA219 #0 on Group 2 initialized
INA219 #1 on Group 2 initialized
NeoPixels initialized
Temperature sensor initialized
Initialization complete!
```

#### Status-Ausgabe (alle 2 Sekunden):
```
=== Charging Station Status ===
Temperature: 25.5 °C
Fan Speed: 20 %
Relays: R1=ON, R2=ON

Module Status:
  Module 1: 5.03 V, 125 mA [Charging]
  Module 2: 5.01 V, 45 mA [Full]
  Module 3: 0.00 V, 0 mA [No Device]
  ...
==============================
```

## LED-Farben verstehen

| LED Farbe | Status | Bedeutung |
|-----------|--------|-----------|
| **Aus** | Kein Gerät | Keine Spannung erkannt |
| **Rot** | Niedrig | Spannung < 3.5V |
| **Orange** | Mittel | Spannung 3.5V - 4.0V |
| **Gelb** | Hoch | Spannung 4.0V - 5.0V |
| **Grün** | Voll | Spannung ≥ 5.0V oder Strom < 50mA |
| **Blau blinkend** | Notfall | Emergency Shutdown aktiv! |

## Erste Inbetriebnahme Checkliste

- [ ] Alle Komponenten korrekt verkabelt
- [ ] Pull-up Widerstände vorhanden (4.7kΩ)
- [ ] INA219 Adressen richtig konfiguriert
- [ ] Separate Stromversorgung für ESP32, LEDs, Lüfter
- [ ] Alle Test-Programme erfolgreich durchgeführt
- [ ] Serial Monitor zeigt "Initialization complete!"
- [ ] Relais sind beim Start OFFEN (Power ON)
- [ ] Lüfter dreht sich
- [ ] LEDs zeigen korrekten Status

## Probleme beim Start?

### Keine Serial-Ausgabe
- Prüfe USB-Kabel
- Prüfe COM-Port in Arduino IDE (Tools → Port)
- Prüfe Baud-Rate (muss 115200 sein)
- Drücke RST-Taste am ESP32

### INA219 nicht gefunden
- Prüfe I2C-Verbindungen (SDA/SCL)
- Prüfe Pull-up Widerstände (4.7kΩ)
- Prüfe INA219 Stromversorgung (3-5V)
- Verwende test_ina219.ino zum Debuggen

### LEDs leuchten nicht
- Prüfe 5V Stromversorgung für LEDs
- Prüfe Data-Pin (D12)
- Füge 300Ω Widerstand in Data-Leitung ein
- Füge 1000µF Kondensator hinzu
- Verwende test_neopixel.ino zum Debuggen

### Temperatursensor zeigt -127°C
- Prüfe Verkabelung (Pin D32)
- Prüfe 4.7kΩ Pull-up Widerstand
- Prüfe DS18B20 Pinout (siehe WIRING.md)
- Verwende test_temperature.ino zum Debuggen

### Relais schalten nicht
- Prüfe Pins D27 und D13
- Prüfe Relais-Stromversorgung (5V)
- Beachte: HIGH = offen (Power ON), LOW = geschlossen (Power OFF)
- Verwende test_relay.ino zum Debuggen

**Für detaillierte Fehlerbehebung siehe:** `TROUBLESHOOTING.md`

## Konfiguration anpassen

Standardwerte in `ChargingStation.ino` Zeilen 30-38:

```cpp
#define NUM_MODULES 6                   // Anzahl Lademodule
#define NUM_LEDS 6                      // Anzahl LEDs
#define VOLTAGE_THRESHOLD 5.5           // Max Spannung (V)
#define TEMP_THRESHOLD_CRITICAL 80.0    // Kritische Temp (°C)
#define TEMP_THRESHOLD_HIGH 50.0        // Hohe Temp (°C)
#define MIN_FAN_SPEED 50                // Min Lüfter (0-255)
#define MAX_FAN_SPEED 255               // Max Lüfter (0-255)
#define CHARGING_VOLTAGE_MIN 0.5        // Min Spannung Erkennung (V)
#define FULL_CHARGE_VOLTAGE 5.0         // Voll-Spannung (V)
```

Passe diese Werte nach Bedarf an!

## Nächste Schritte

Nach erfolgreicher Inbetriebnahme:

1. **Temperatur-Kalibrierung**
   - Überwache Temperatur unter Last
   - Passe TEMP_THRESHOLD_HIGH an
   - Teste Lüfter-Ansprechverhalten

2. **Spannungs-Kalibrierung**
   - Messe tatsächliche Spannungen mit Multimeter
   - Vergleiche mit INA219-Werten
   - Passe Schwellwerte an

3. **LED-Helligkeit**
   - Standardmäßig: Volle Helligkeit
   - Anpassbar in Code: `strip.setBrightness(50)`
   - Wert von 0-255

4. **Erweiterte Features (optional)**
   - WiFi-Integration für Monitoring
   - MQTT für Smart Home
   - LCD-Display für Stand-alone Betrieb
   - Datenlogging auf SD-Karte

## Support

Bei Problemen:
1. Prüfe `TROUBLESHOOTING.md`
2. Verwende Test-Programme zum Debuggen
3. Erstelle GitHub Issue mit:
   - Serial Monitor Output
   - Fotos der Verkabelung
   - Beschreibung des Problems

## Wichtige Sicherheitshinweise ⚠️

- **Strombelastung**: Max. 270W (6 × 45W) beachten
- **Relais**: Für Gesamtlast ausgelegt? (Min. 10A)
- **Kühlung**: Ausreichende Belüftung sicherstellen
- **Überstromschutz**: Sicherungen verwenden
- **Notabschaltung**: System nach Shutdown neu starten
- **Netzspannung**: Nur von Fachpersonal anschließen!

---

**Viel Erfolg mit deiner Charging Station! 🔌⚡**

Bei Fragen: GitHub Issues oder README.md konsultieren.
