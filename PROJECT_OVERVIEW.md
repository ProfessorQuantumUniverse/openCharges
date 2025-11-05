# openCharges - ESP32 Charging Station

Ein intelligentes 6-Port USB-C Power Delivery Ladesystem mit LED-Statusanzeige, Temperaturüberwachung und automatischer Notabschaltung.

## 🚀 Features

- **6 USB-C PD Ladeanschlüsse** mit je bis zu 45W Leistung
- **Intelligente LED-Statusanzeige** mit farbcodierten Ladezuständen
- **Temperaturüberwachung** mit automatischer Lüftersteuerung
- **Notabschaltung** bei Überhitzung oder Überspannung
- **Echtzeit-Monitoring** von Spannung und Strom pro Port
- **Dual-Relais Sicherheitssystem** (Normally Closed)

## 📁 Projekt-Struktur

```
openCharges/
├── ChargingStation/
│   └── ChargingStation.ino          # Hauptprogramm
├── tests/
│   ├── test_relay/                  # Relais-Test
│   ├── test_ina219/                 # INA219 Sensor-Test
│   ├── test_neopixel/               # LED-Test
│   ├── test_temperature/            # Temperatur-Test
│   └── test_fan/                    # Lüfter-Test
├── README.md                         # Komplette Anleitung
├── QUICKSTART.md                     # Schnellstart-Anleitung
├── WIRING.md                         # Verdrahtungsplan
├── PARTS_LIST.md                     # Teileliste
├── TROUBLESHOOTING.md                # Fehlerbehebung
├── CONFIGURATION_EXAMPLES.md         # Konfigurations-Beispiele
└── platformio.ini                    # PlatformIO-Konfiguration
```

## 🎨 LED-Statusanzeige

| Farbe | Status | Bedeutung |
|-------|--------|-----------|
| 🔴 **Rot** | Niedrig | Ladestand < 3.5V |
| 🟠 **Orange** | Mittel | Ladestand 3.5V - 4.0V |
| 🟡 **Gelb** | Hoch | Ladestand 4.0V - 5.0V |
| 🟢 **Grün** | Voll | Vollständig geladen |
| ⚫ **Aus** | Kein Gerät | Kein Gerät angeschlossen |
| 🔵 **Blau (blinkend)** | Notfall | Notabschaltung aktiv |

## 🔧 Hardware-Anforderungen

### Hauptkomponenten
- ESP32 Development Board
- 6× USB-C Power Delivery Module (max. 45W)
- 2× Relais Module (Normally Closed)
- 6× Adafruit INA219 Stromsensoren
- 6× WS2812B NeoPixel LEDs
- 1× DS18B20 Temperatursensor
- 1× PWM-Lüfter (12V)

### Kosten
Geschätzte Gesamtkosten: **~260€**

Siehe [PARTS_LIST.md](PARTS_LIST.md) für Details

## ⚡ Quick Start

### 1. Arduino IDE einrichten
```
1. ESP32 Board Support installieren
2. Bibliotheken installieren:
   - Adafruit INA219
   - Adafruit NeoPixel
   - OneWire
   - DallasTemperature
```

### 2. Hardware verdrahten
Siehe [WIRING.md](WIRING.md) für detaillierte Anschluss-Diagramme

### 3. Test-Programme ausführen
```
1. test_relay.ino        → Relais testen
2. test_ina219.ino       → Sensoren testen
3. test_neopixel.ino     → LEDs testen
4. test_temperature.ino  → Temperatur testen
5. test_fan.ino          → Lüfter testen
```

### 4. Hauptprogramm laden
```
1. ChargingStation.ino öffnen
2. Auf ESP32 hochladen
3. Serial Monitor (115200 baud) öffnen
```

Ausführliche Anleitung: [QUICKSTART.md](QUICKSTART.md)

## 📖 Dokumentation

| Dokument | Beschreibung |
|----------|--------------|
| [README.md](README.md) | Vollständige Anleitung |
| [QUICKSTART.md](QUICKSTART.md) | 5-Schritte Schnellstart |
| [WIRING.md](WIRING.md) | Hardware-Verdrahtung |
| [PARTS_LIST.md](PARTS_LIST.md) | Teileliste mit Preisen |
| [TROUBLESHOOTING.md](TROUBLESHOOTING.md) | Problemlösungen |
| [CONFIGURATION_EXAMPLES.md](CONFIGURATION_EXAMPLES.md) | Konfigurations-Beispiele |

## 🔒 Sicherheitsfeatures

### Automatische Notabschaltung bei:
- Temperatur > 80°C
- Spannung > 5.5V an einem Port
- Sensor-Ausfall

### Temperaturgesteuerte Kühlung:
- < 50°C: Minimale Lüftergeschwindigkeit (~20%)
- 50-80°C: Progressive Beschleunigung
- > 80°C: Notabschaltung

## 📊 Technische Spezifikationen

| Parameter | Wert |
|-----------|------|
| Anzahl Ports | 6 |
| Max. Leistung/Port | 45W |
| Gesamtleistung | 270W |
| Spannungsüberwachung | 0-26V |
| Stromüberwachung | 0-3.2A |
| Temperaturbereich | -55°C bis +125°C |
| PWM-Frequenz | 25 kHz |
| Update-Rate | 100ms |

## 🔌 Pin-Belegung

```
Relais:
  D27 → Relais 1 (Module 1-3)
  D13 → Relais 2 (Module 4-6)

I2C Bus 1 (Module 1-4):
  D35 → SCL
  D34 → SDA

I2C Bus 2 (Module 5-6):
  D25 → SCL
  D26 → SDA

LEDs & Sensoren:
  D12 → NeoPixel LEDs
  D32 → Temperatursensor
  D14 → Lüfter PWM
```

## 🐛 Fehlersuche

Häufige Probleme und Lösungen:

### LEDs leuchten nicht
- Prüfe 5V Stromversorgung
- Füge 300Ω Widerstand hinzu
- Prüfe Pin D12

### INA219 nicht gefunden
- Prüfe I2C-Verbindungen
- Füge 4.7kΩ Pull-ups hinzu
- Prüfe Adressen (0x40-0x43)

### Temperatur zeigt -127°C
- Prüfe Verkabelung Pin D32
- Füge 4.7kΩ Pull-up hinzu
- Prüfe DS18B20 Pinout

Vollständige Anleitung: [TROUBLESHOOTING.md](TROUBLESHOOTING.md)

## 📺 Serial Monitor Ausgabe

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

## 🛠️ Entwicklung

### PlatformIO
```bash
# Projekt bauen
pio run

# Hochladen
pio run --target upload

# Serial Monitor
pio device monitor
```

### Arduino IDE
```
1. Datei → Öffnen → ChargingStation/ChargingStation.ino
2. Tools → Board → ESP32 Dev Module
3. Tools → Port → [COM Port auswählen]
4. Hochladen-Button klicken
```

## 🤝 Beitragen

Verbesserungsvorschläge und Bug-Reports sind willkommen!

1. Fork das Repository
2. Erstelle einen Feature Branch
3. Committe deine Änderungen
4. Push zum Branch
5. Öffne einen Pull Request

## 📝 Lizenz

Siehe [LICENSE.md](LICENSE.md)

## 👨‍💻 Autor

**ProfessorQuantumUniverse**

## 🌟 Features (Roadmap)

Mögliche zukünftige Erweiterungen:

- [ ] WiFi-Webinterface für Monitoring
- [ ] MQTT-Integration für Smart Home
- [ ] LCD-Display für Stand-alone Betrieb
- [ ] SD-Karte für Datenlogging
- [ ] Smartphone App
- [ ] Ladeprofil-Verwaltung
- [ ] Energieverbrauch-Statistiken

## ⚠️ Wichtige Hinweise

1. **Relais sind Normally Closed (NC)**
   - HIGH = Relais offen = Power ON (Normal)
   - LOW = Relais geschlossen = Power OFF (Notfall)

2. **Notabschaltung ist permanent**
   - Nach Notabschaltung: System neu starten
   - Ursache beseitigen vor Neustart

3. **Stromversorgung**
   - Separate Netzteile verwenden
   - Max. 270W Gesamtlast beachten
   - Sicherungen einbauen

4. **Nur von Fachpersonal installieren**
   - Netzspannung kann lebensgefährlich sein
   - Relais für Gesamtlast auslegen
   - Ausreichende Kühlung sicherstellen

## 📞 Support

Bei Fragen oder Problemen:
1. Konsultiere [TROUBLESHOOTING.md](TROUBLESHOOTING.md)
2. Verwende die Test-Programme zum Debuggen
3. Öffne ein GitHub Issue mit:
   - Serial Monitor Ausgabe
   - Fotos der Verkabelung
   - Problembeschreibung

---

**Made with ❤️ for safe and efficient charging**

**Version:** 1.0.0
