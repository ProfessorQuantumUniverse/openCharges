# Hardware-Verdrahtungsplan

## ESP32 Pin-Zuordnung

### Relais (Notabschaltung)
```
ESP32 Pin D27  -->  Relais 1 (Steuert Module 1-3)
ESP32 Pin D13  -->  Relais 2 (Steuert Module 4-6)

Relais-Typ: Normally Closed (NC)
- HIGH Signal = Relais offen = Strom fließt (Normal-Betrieb)
- LOW Signal = Relais geschlossen = Strom unterbrochen (Notabschaltung)
```

### INA219 Sensoren - Gruppe 1 (Module 0-3)
```
ESP32 Pin D35  -->  SCL (Clock)
ESP32 Pin D34  -->  SDA (Data)

INA219 #0: Adresse 0x40  -->  Modul 1
INA219 #1: Adresse 0x41  -->  Modul 2
INA219 #2: Adresse 0x42  -->  Modul 3
INA219 #3: Adresse 0x43  -->  Modul 4

Hinweis: 4.7kΩ Pull-up Widerstände für SDA und SCL
```

### INA219 Sensoren - Gruppe 2 (Module 4-5)
```
ESP32 Pin D25  -->  SCL (Clock)
ESP32 Pin D26  -->  SDA (Data)

INA219 #4: Adresse 0x40  -->  Modul 5
INA219 #5: Adresse 0x41  -->  Modul 6

Hinweis: 4.7kΩ Pull-up Widerstände für SDA und SCL
```

### NeoPixel LEDs
```
ESP32 Pin D12  -->  NeoPixel Data In (DIN)

LED #0  -->  Modul 1 Status
LED #1  -->  Modul 2 Status
LED #2  -->  Modul 3 Status
LED #3  -->  Modul 4 Status
LED #4  -->  Modul 5 Status
LED #5  -->  Modul 6 Status

Hinweis: 
- Separate 5V Stromversorgung für LEDs empfohlen
- 300-500Ω Widerstand zwischen ESP32 und DIN
- 1000µF Kondensator zwischen 5V und GND
```

### Temperatursensor (DS18B20)
```
ESP32 Pin D32  -->  DS18B20 Data Pin

Anschlüsse:
- VCC  -->  3.3V
- GND  -->  GND
- Data -->  Pin D32

Hinweis: 4.7kΩ Pull-up Widerstand zwischen Data und VCC
```

### Lüfter (PWM-Steuerung)
```
ESP32 Pin D14  -->  Lüfter PWM-Eingang

PWM-Konfiguration:
- Frequenz: 25 kHz
- Auflösung: 8 Bit (0-255)
- Mindestgeschwindigkeit: 50/255 (≈20%)
- Maximalgeschwindigkeit: 255/255 (100%)
```

## Stromversorgung

### ESP32
```
Eingangsspannung: 5V via USB oder VIN (5-12V)
Stromaufnahme: ca. 500mA (mit allen Peripheriegeräten)
```

### USB-C PD Module
```
Eingangsspannung: Typischerweise 12-20V
Max. Leistung pro Modul: 45W
Gesamtleistung: bis zu 270W
```

### NeoPixel LEDs
```
Spannung: 5V
Stromaufnahme: max. 60mA pro LED bei voller Helligkeit
Gesamt: max. 360mA für 6 LEDs
Empfehlung: Separate 5V Versorgung
```

### INA219 Module
```
Spannung: 3.3V oder 5V
Stromaufnahme: ca. 1mA pro Modul
```

### Relais
```
Spulenspannung: 5V (typisch)
Schaltstrom: Abhängig vom Relais-Typ
Empfohlene Relais: 10A bei 250V AC
```

### Lüfter
```
Spannung: 12V (typisch)
Stromaufnahme: 100-500mA (abhängig vom Lüfter)
```

## INA219 Adress-Konfiguration

Die INA219 Module haben eine Basis-Adresse von 0x40. Durch Verbindung der A0/A1 Pins können bis zu 4 verschiedene Adressen eingestellt werden:

```
A1    A0    Adresse
GND   GND   0x40
GND   VCC   0x41
VCC   GND   0x42
VCC   VCC   0x43
```

### Gruppe 1 (I2C Bus 1):
- INA219 #0: A1=GND, A0=GND --> 0x40
- INA219 #1: A1=GND, A0=VCC --> 0x41
- INA219 #2: A1=VCC, A0=GND --> 0x42
- INA219 #3: A1=VCC, A0=VCC --> 0x43

### Gruppe 2 (I2C Bus 2):
- INA219 #4: A1=GND, A0=GND --> 0x40
- INA219 #5: A1=GND, A0=VCC --> 0x41

## Verdrahtungsdiagramm (Textdarstellung)

```
                         ESP32
                    ┌──────────────┐
                    │              │
         D27 ───────┤              │
         D13 ───────┤              │
                    │              │
         D35 ───────┤  (SCL1)      │
         D34 ───────┤  (SDA1)      │
                    │              │
         D25 ───────┤  (SCL2)      │
         D26 ───────┤  (SDA2)      │
                    │              │
         D12 ───────┤              │
         D32 ───────┤              │
         D14 ───────┤              │
                    │              │
                    └──────────────┘

Relais 1 (D27)      Relais 2 (D13)
    │                   │
    ├─ Modul 1          ├─ Modul 4
    ├─ Modul 2          ├─ Modul 5
    └─ Modul 3          └─ Modul 6

I2C Bus 1 (D35/D34)    I2C Bus 2 (D25/D26)
    │                      │
    ├─ INA219 #0 (0x40)    ├─ INA219 #4 (0x40)
    ├─ INA219 #1 (0x41)    └─ INA219 #5 (0x41)
    ├─ INA219 #2 (0x42)
    └─ INA219 #3 (0x43)

NeoPixel (D12)
    │
    ├─ LED #0 (Modul 1)
    ├─ LED #1 (Modul 2)
    ├─ LED #2 (Modul 3)
    ├─ LED #3 (Modul 4)
    ├─ LED #4 (Modul 5)
    └─ LED #5 (Modul 6)

Temp.Sensor (D32)
    └─ DS18B20

Lüfter (D14)
    └─ PWM Lüfter
```

## Sicherheitshinweise

1. **Relais-Dimensionierung**: Stelle sicher, dass die Relais für die Gesamtlast ausgelegt sind (max. 270W)
2. **Stromversorgung**: Verwende separate Netzteile für ESP32 und Lademodule
3. **Erdung**: Alle Komponenten sollten eine gemeinsame Masse (GND) haben
4. **Überstromschutz**: Installiere Sicherungen vor den USB-C Modulen
5. **Wärmeableitung**: Stelle ausreichende Kühlung für alle Komponenten sicher
6. **Kabelquerschnitt**: Verwende ausreichend dicke Kabel für hohe Ströme (mindestens 1.5mm² für 10A)

## Testpunkte

Für Debugging und Wartung sollten folgende Testpunkte zugänglich sein:
- 3.3V und GND
- 5V und GND
- Alle I2C-Leitungen (SDA/SCL)
- Relais-Ausgänge
- Temperatursensor-Leitung
- PWM-Ausgang

## Montageempfehlungen

1. **ESP32**: In Sockel montieren für einfachen Austausch
2. **INA219**: Nahe an den USB-C Modulen montieren
3. **Temperatursensor**: An der wärmsten Stelle im Gehäuse
4. **Lüfter**: Für optimalen Luftstrom positionieren
5. **LEDs**: Sichtbar an der Gehäusefront
6. **Relais**: Leicht zugänglich für manuelle Überbrückung im Notfall
