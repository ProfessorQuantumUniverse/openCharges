# Teileliste / Parts List

## Hauptkomponenten

### Microcontroller
- **1x ESP32 Development Board**
  - Modell: ESP32-DevKitC oder kompatibel
  - GPIO Pins: Mindestens 30
  - Flash: 4MB empfohlen

### USB-C PD Lademodule
- **6x USB-C Power Delivery Module**
  - Max. Leistung: 45W pro Modul
  - Ausgangsspannung: 5V, 9V, 12V, 15V, 20V (PD)
  - Max. Strom: 3A (bei 15V) bis 2.25A (bei 20V)
  - Beispiel: IP2721 oder ähnliche PD-Controller

### Relais
- **2x Relais (Normally Closed)**
  - Spulenspannung: 5V DC
  - Schaltstrom: Min. 10A bei 250V AC
  - Typ: SPDT (Single Pole Double Throw) mit NC-Kontakt
  - Beispiel: SRD-05VDC-SL-C oder Omron G5V-2

### Stromsensoren
- **6x Adafruit INA219 Breakout Board**
  - Messbereich: ±3.2A
  - Spannung: 0-26V
  - I2C-Interface
  - Konfigurierbare Adresse (0x40-0x43)

### LED-Anzeige
- **6x NeoPixel RGB LEDs (WS2812B)**
  - Alternative: 1x NeoPixel Strip mit 6 LEDs
  - Spannung: 5V
  - Datenprotokoll: WS2812B
  - Beispiel: Adafruit NeoPixel Stick

### Temperatursensor
- **1x DS18B20 Temperatursensor**
  - Messbereich: -55°C bis +125°C
  - Genauigkeit: ±0.5°C
  - Interface: OneWire
  - Gehäuse: TO-92 oder wasserdicht

### Lüfter
- **1x PWM-Lüfter**
  - Spannung: 12V DC
  - Stromaufnahme: Typisch 0.2-0.5A
  - Größe: 80mm oder 120mm empfohlen
  - PWM-Signal: 25 kHz
  - 4-Pin Anschluss mit PWM-Steuerung

## Elektronische Bauteile

### Widerstände
- **6x 4.7kΩ Widerstand**
  - Für I2C Pull-ups (2x für jeden Bus)
  - Für DS18B20 Pull-up (1x)
  - Leistung: 1/4W
  
- **1x 300-500Ω Widerstand**
  - Für NeoPixel Data-Leitung
  - Leistung: 1/4W

### Kondensatoren
- **1x 1000µF Elektrolytkondensator**
  - Spannung: 16V
  - Für NeoPixel Spannungsversorgung

- **4x 100nF Keramikkondensator**
  - Für INA219 Entkopplung
  - Spannungsfestigkeit: 16V

### Dioden
- **2x 1N4007 Freilaufdiode**
  - Für Relais-Spulen
  - Optional: bereits auf Relais-Modul vorhanden

## Stromversorgung

### Netzteile
- **1x 5V Netzteil für ESP32 und Peripherie**
  - Spannung: 5V DC
  - Strom: Min. 2A (empfohlen 3A)
  
- **1x Netzteil für USB-C Module**
  - Spannung: Abhängig von PD-Modulen (typisch 12-20V)
  - Leistung: Min. 300W (für alle 6 Module)
  
- **1x 12V Netzteil für Lüfter**
  - Spannung: 12V DC
  - Strom: Min. 0.5A

### Verkabelung
- **Jumper Wires / Dupont Kabel**
  - 20x Male-Female für Breadboard-Verbindungen
  - 10x Female-Female
  
- **Stromkabel**
  - 2m 1.5mm² für hohe Ströme (USB-C Module)
  - 2m 0.5mm² für niedrige Ströme (Signale)

## Mechanische Komponenten

### Gehäuse
- **1x Kunststoff- oder Metallgehäuse**
  - Abmessungen: Abhängig von Layout (empfohlen 30x20x10cm)
  - Mit Lüftungsöffnungen
  - Montage für 6 USB-C Buchsen an der Front

### Befestigung
- **Abstandsbolzen für ESP32**
  - 4x M3 Abstandsbolzen, 10mm
  - 8x M3 Schrauben
  
- **Lüftergitter**
  - 1x Schutzgitter für Lüfter
  - Passend zu Lüftergröße

### Verkabelung & Anschlüsse
- **6x USB-C Buchsen**
  - Einbau-Buchsen für Gehäuse
  - Mit PD-Unterstützung

- **1x Stromeingangs-Buchse**
  - DC-Barrel Jack oder IEC C14

## Optionale Komponenten

### Zusätzliche Features
- **1x Taster für Reset**
  - Momentary Push Button
  - NO (Normally Open)

- **1x Hauptschalter**
  - Beleuchtet
  - 10A Schaltleistung

- **1x Display** (Optional)
  - 0.96" OLED Display (I2C)
  - Für Status-Anzeige ohne Computer

- **1x Summer/Buzzer** (Optional)
  - Für akustische Alarme
  - 5V DC

### Sicherheit
- **6x Sicherungen**
  - Typ: Flink, 5A
  - Für jeden USB-C Ausgang

- **1x Hauptsicherung**
  - Typ: Flink, 16A
  - Für Netzeingang

## Werkzeug (benötigt)

- Lötkolben und Lötzinn
- Abisolierzange
- Seitenschneider
- Schraubendreher (Kreuz und Schlitz)
- Multimeter
- Heißklebepistole (optional)
- Kabelbinder

## Geschätzte Gesamtkosten

| Komponente | Stückpreis | Anzahl | Gesamt |
|------------|-----------|--------|--------|
| ESP32 | 8€ | 1 | 8€ |
| USB-C PD Module | 10€ | 6 | 60€ |
| Relais | 3€ | 2 | 6€ |
| INA219 | 5€ | 6 | 30€ |
| NeoPixel Strip | 12€ | 1 | 12€ |
| DS18B20 | 3€ | 1 | 3€ |
| Lüfter | 10€ | 1 | 10€ |
| Netzteile | 30€ | 3 | 90€ |
| Gehäuse | 20€ | 1 | 20€ |
| Kleinteile | - | - | 20€ |
| **Gesamt** | | | **≈260€** |

*Preise sind Schätzungen und können variieren*

## Bezugsquellen

### Online-Händler (Deutschland/Europa)
- **Reichelt Elektronik** (www.reichelt.de)
- **Conrad Electronic** (www.conrad.de)
- **Mouser Electronics** (www.mouser.de)
- **AZ-Delivery** (www.az-delivery.de) - Spezialisiert auf Arduino/ESP32
- **Adafruit** (www.adafruit.com) - Original Adafruit Produkte
- **Berrybase** (www.berrybase.de)

### Alternativen
- **AliExpress** - Günstigere Preise, längere Lieferzeit
- **Amazon** - Schnelle Lieferung, oft teurer
- **eBay** - Gebrauchte oder günstige Komponenten

## Hinweise zur Beschaffung

1. **ESP32**: Achte auf ausreichend GPIO-Pins (mindestens 30)
2. **INA219**: Adafruit Original oder kompatible Clone (z.B. GY-INA219)
3. **NeoPixel**: WS2812B kompatibel, 5V Version
4. **USB-C Module**: Prüfe PD-Protokoll-Unterstützung
5. **Relais**: Unbedingt "Normally Closed" Kontakte!
6. **Temperatursensor**: DS18B20 (nicht DHT22 oder ähnliches)
7. **Lüfter**: PWM-fähig (4-Pin), nicht nur 2-Pin

## Erweiterungsmöglichkeiten

Für zukünftige Verbesserungen könntest du hinzufügen:
- WiFi-Monitoring (ESP32 hat WiFi integriert)
- MQTT-Integration für Home Automation
- SD-Karte für Datenlogging
- LC-Display für Stand-alone Betrieb
- Zusätzliche Temperatursensoren pro Modul
- Strommessung am Eingang
