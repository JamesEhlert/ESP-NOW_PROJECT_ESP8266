# ESP-NOW_PROJECT_ESP8266
Sistema de comunicação sem fio entre dois módulos ESP8266 utilizando o protocolo ESPNOW para controle de LEDs em tempo real. O transmissor envia o estado de um botão e o receptor controla LEDs com baixa latência.


# Projeto ESP8266 com Protocolo ESPNOW - Controle Sem Fio de LEDs

## 📋 Descrição do Projeto
Sistema de comunicação sem fio entre dois módulos ESP8266 utilizando o protocolo ESPNOW para controle de LEDs em tempo real. O transmissor envia o estado de um botão e o receptor controla LEDs com baixa latência.

## 🎯 Objetivo
Criar um link de comunicação confiável e de baixa latência entre microcontroladores ESP8266 para aplicações que exigem resposta rápida, como controle de dispositivos em tempo real.

## 📦 Componentes Necessários
- 2x Módulos ESP8266 (NodeMCU, Wemos D1 Mini, ou similar)
- 1x Botão push-button
- 1x LED externo (para o receptor)
- 1x Display OLED SSD1306 128x64 (I2C)
- Resistores: 330Ω para o LED externo
- Protoboard e fios para conexões

## 🛠️ Configuração do Hardware

### Transmissor
- Botão: Conectado entre GPIO5 (D1) e GND
- Display OLED: 
  - SCL → GPIO12 (D6)
  - SDA → GPIO14 (D5)
  - VCC → 3.3V
  - GND → GND

### Receptor
- LED Interno: Já presente na placa (LED_BUILTIN)
- LED Externo: 
  - Ânodo (+) → GPIO14 (D5) com resistor de 330Ω em série
  - Cátodo (-) → GND

## 🔧 Configuração do Software

### Pré-requisitos
- IDE Arduino ou PlatformIO
- Biblioteca ESP8266 (versão 2.7.4 ou superior)
- Biblioteca U8g2 para o display OLED

### Instalação das Bibliotecas
1. Na IDE Arduino, vá em Sketch → Include Library → Manage Libraries
2. Instale as bibliotecas:
   - "ESP8266WiFi" by ESP8266 Community
   - "U8g2" by oliver.
