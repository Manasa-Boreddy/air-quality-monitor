# Air Quality Monitor
<p></p>

## Architecture

![Architecture](architecture.svg)

## 🧠 Architecture Overview

This system is designed to monitor environmental air quality using multiple sensors, ESP32-based microcontrollers, and MQTT protocol.

### Components:
- **Sensors**: 
  - PM2.5 (PMS5003)
  - CO₂ (MH-Z19)
  - VOC (MP503)
  - Ozone (MQ-131)
  - Temperature & Humidity (DHT22)
- **Microcontrollers**:
  - ESP32 (for sensor data collection and MQTT publishing)
  - ESP32-S3 (for displaying data via MQTT subscription)
- **MQTT Broker**: Mosquitto (for message communication between devices)

### Data Flow:
1. Sensors collect environmental data.
2. ESP32 reads and publishes sensor data to the MQTT broker.
3. ESP32-S3 subscribes to the broker to receive and display live data.

---

## 📊 Features

- Real-time monitoring of:
  - PM2.5
  - CO₂
  - VOC
  - Ozone
  - Temperature
  - Humidity
- Historical data logging for long-term trend analysis.
- Modular design for easy upgrades and maintenance.

---

## 🛠️ Tools & Technologies

- **Hardware**: Arduino Pro Mini, ESP32, ESP32-S3
- **Sensors**: PMS5003, MH-Z19, MP503, MQ-131, DHT22
- **Communication**: MQTT (Mosquitto)
- **Software**: Python, MySQL

---
