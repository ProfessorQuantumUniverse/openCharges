# ESP32 Charging Station with Status LEDs

Ein intelligentes Ladesystem mit 6 USB-C Power Delivery Modulen, Status-LEDs, Temperaturüberwachung und Notabschaltung.

## Hardware-Komponenten

### Hauptkomponenten
- **Microcontroller**: ESP32
- **Lademodule**: 6x USB-C Power Delivery Module (max. 45W je Modul)
- **Relais**: 2x Notabschaltungs-Relais (Normally Closed)
- **Stromsensoren**: 6x INA219 Strom/Spannungs-Sensoren
- **LEDs**: 6x NeoPixel LEDs (eine pro Lademodul)
- **Temperatursensor**: DS18B20 Temperatursensor
- **Lüfter**: PWM-gesteuerter Lüfter

### Pin-Belegung

#### Relais (Notabschaltung)
- **Relais 1** (Module 1-3): Pin D27
- **Relais 2** (Module 4-6): Pin D13
- **Typ**: Normally Closed (HIGH = offen/Power ON, LOW = geschlossen/Power OFF)

#### INA219 Sensoren (I2C)
**Gruppe 1** (4 Sensoren, Module 0-3):
- SCL: Pin D35
- SDA: Pin D34
- Adressen: 0x40, 0x41, 0x42, 0x43

**Gruppe 2** (2 Sensoren, Module 4-5):
- SCL: Pin D25
- SDA: Pin D26
- Adressen: 0x40, 0x41

#### Status-LEDs
- **NeoPixel**: Pin D12 (6 LEDs)

#### Temperatur & Lüfter
- **Temperatursensor**: Pin D32
- **Lüfter PWM**: Pin D14

## Funktionen

### 1. Ladestatus-Anzeige (LEDs)
Jede LED zeigt den Status des zugehörigen Lademoduls:
- **Aus (LED OFF)**: Kein Gerät angeschlossen
- **Rot**: Niedriger Ladestand (< 3.5V)
- **Orange**: Mittlerer Ladestand (3.5V - 4.0V)
- **Gelb**: Hoher Ladestand (4.0V - 5.0V)
- **Grün**: Voll geladen (≥ 5.0V oder Strom < 50mA)
- **Blau (blinkend)**: Notabschaltung aktiv

### 2. Temperaturüberwachung
- **Normale Temperatur** (< 50°C): Lüfter läuft mit Mindestgeschwindigkeit
- **Hohe Temperatur** (50°C - 80°C): Lüfter beschleunigt progressiv
- **Kritische Temperatur** (> 80°C): Notabschaltung aktiviert

### 3. Notabschaltung
Automatische Abschaltung bei:
- Temperatur über 80°C
- Spannung über 5.5V an einem beliebigen Modul
- INA219-Sensor reagiert nicht

### 4. Lüftersteuerung
- Läuft immer mit Mindestgeschwindigkeit (≈20%)
- Erhöht Geschwindigkeit basierend auf Temperatur
- PWM-Frequenz: 25 kHz

## Installation

### Erforderliche Arduino-Bibliotheken
1. **Adafruit INA219** - Für Strom/Spannungsmessung
   ```
   Sketch -> Include Library -> Manage Libraries
   Suche: "Adafruit INA219"
   ```

2. **Adafruit NeoPixel** - Für LED-Steuerung
   ```
   Sketch -> Include Library -> Manage Libraries
   Suche: "Adafruit NeoPixel"
   ```

3. **OneWire** - Für DS18B20 Temperatursensor
   ```
   Sketch -> Include Library -> Manage Libraries
   Suche: "OneWire"
   ```

4. **DallasTemperature** - Für DS18B20 Temperatursensor
   ```
   Sketch -> Include Library -> Manage Libraries
   Suche: "DallasTemperature"
   ```

### Board-Einstellungen (Arduino IDE)
1. **Board**: ESP32 Dev Module
2. **Upload Speed**: 115200
3. **CPU Frequency**: 240MHz
4. **Flash Frequency**: 80MHz
5. **Flash Mode**: QIO
6. **Flash Size**: 4MB
7. **Partition Scheme**: Default 4MB with spiffs

## Verwendung

### Hauptprogramm hochladen
1. Öffne `ChargingStation/ChargingStation.ino` in Arduino IDE
2. Wähle den richtigen COM-Port
3. Klicke auf Upload
4. Öffne Serial Monitor (115200 baud) für Status-Ausgaben

### Test-Programme
Im `tests/` Verzeichnis befinden sich separate Test-Programme für jede Komponente:

#### test_relay
Testet die Relais-Funktionalität
- Wechselt alle 3 Sekunden zwischen ON/OFF
- Zeigt Relais-Status im Serial Monitor

#### test_ina219
Testet alle 6 INA219 Sensoren
- Zeigt Spannung, Strom und Leistung
- Aktualisierung jede Sekunde
- Prüft beide I2C-Busse

#### test_neopixel
Testet die LED-Ansteuerung
- Verschiedene Farben und Muster
- Einzelne LED-Tests
- Rainbow-Effekte
- Ladestatus-Simulation

#### test_temperature
Testet den Temperatursensor
- Zeigt Temperatur in °C und °F
- Warnung bei hohen Temperaturen
- Prüft Sensor-Verbindung

#### test_fan
Testet die Lüftersteuerung
- Verschiedene Geschwindigkeiten
- Temperatur-basierte Steuerung
- PWM-Test von 0% bis 100%

## Konfiguration

Konfigurierbare Parameter in `ChargingStation.ino`:

```cpp
#define NUM_MODULES 6                   // Anzahl der Lademodule
#define NUM_LEDS 6                      // Anzahl der LEDs
#define VOLTAGE_THRESHOLD 5.5           // Maximale Spannung (V)
#define TEMP_THRESHOLD_CRITICAL 80.0    // Kritische Temperatur (°C)
#define TEMP_THRESHOLD_HIGH 50.0        // Hohe Temperatur für Lüfter (°C)
#define MIN_FAN_SPEED 50                // Minimale Lüftergeschwindigkeit (0-255)
#define MAX_FAN_SPEED 255               // Maximale Lüftergeschwindigkeit (0-255)
#define CHARGING_VOLTAGE_MIN 0.5        // Mindestspannung für Geräteerkennung (V)
#define FULL_CHARGE_VOLTAGE 5.0         // Vollladespannung (V)
```

## Sicherheitshinweise

1. **Relais sind Normally Closed**: Beim Start werden die Relais geöffnet (HIGH), um Strom zu ermöglichen
2. **Notabschaltung ist permanent**: Nach einer Notabschaltung muss das System neu gestartet werden
3. **Temperaturüberwachung**: Stelle sicher, dass der Temperatursensor korrekt montiert ist
4. **Stromversorgung**: Achte auf ausreichende Stromversorgung für alle 6 Module (bis zu 270W)

## Fehlerbehebung

### LEDs leuchten nicht
- Prüfe Pin D12 Verbindung
- Prüfe NeoPixel Stromversorgung
- Prüfe korrekte LED-Anzahl in der Konfiguration

### INA219 nicht gefunden
- Prüfe I2C-Verbindungen (SDA/SCL)
- Prüfe I2C-Adressen (0x40-0x43)
- Verwende I2C-Scanner zur Adresserkennung

### Temperatursensor zeigt -127°C
- Sensor nicht verbunden oder defekt
- Prüfe OneWire-Verbindung an Pin D32
- Prüfe 4.7kΩ Pull-up Widerstand

### Relais funktionieren nicht
- Prüfe Pins D27 und D13
- Prüfe Relais-Versorgungsspannung
- Beachte: HIGH = offen (Power ON), LOW = geschlossen (Power OFF)

### Lüfter läuft nicht
- Prüfe PWM-Pin D14
- Prüfe Lüfter-Stromversorgung
- Prüfe Mindestgeschwindigkeit (MIN_FAN_SPEED)

## Serial Monitor Ausgaben

Das Hauptprogramm gibt alle 2 Sekunden Status-Informationen aus:
```
=== Charging Station Status ===
Temperature: 25.5 °C
Fan Speed: 20 %
Relays: R1=ON, R2=ON

Module Status:
  Module 1: 5.03 V, 125 mA [Charging]
  Module 2: 5.01 V, 45 mA [Full]
  Module 3: 0.00 V, 0 mA [No Device]
  Module 4: 3.78 V, 850 mA [Charging]
  Module 5: 4.85 V, 210 mA [Charging]
  Module 6: 0.00 V, 0 mA [No Device]
==============================
```

## Lizenz

Siehe LICENSE.md

## Autor

ProfessorQuantumUniverse

## Version

1.0.0 - Erste Version mit allen Grundfunktionen
