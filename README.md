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
   - "U8g2" by oliver
   - 

⚠️ ATENÇÃO: Passo Mais Importante!
Antes de tudo, você DEVE descobrir o endereço MAC do seu receptor!

Cada ESP8266 tem um endereço MAC único, como uma "digital" que o identifica na rede. Se você não configurar o transmissor com o MAC correto do receptor, a comunicação não funcionará!

Como Descobrir o MAC do Seu ESP8266
Carregue este código simples no ESP que será o RECEPTOR:

#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // Nada aqui
}

Passo a passo:

Conecte o ESP8266 no computador

Cole este código na IDE Arduino

Selecione a placa e porta corretas

Faça upload e abra o Serial Monitor (115200 baud)

Anote o MAC Address que aparecer - será algo como: AA:BB:CC:DD:EE:FF

Agora substitua no código do transmissor:

// Cole o MAC do seu receptor aqui:
uint8_t receiverMAC[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
🛠️ Montagem Rápida
Receptor (LED)
Pino ESP8266	Componente
GPIO14 (D5)	LED (+)
GND	LED (-) com resistor
Transmissor (Botão)
Pino ESP8266	Componente
GPIO5 (D1)	Botão (um terminal)
GND	Botão (outro terminal)



📥 Instalação
Instale as bibliotecas (Sketch > Include Library > Manage Libraries):

ESP8266WiFi

U8g2 (para o display OLED)

Carregue os códigos:

Primeiro o receptor (com o LED)

Depois o transmissor (com o botão)

Lembre-se: Cole o MAC address correto no transmissor!


🔧 Como Funciona
Transmissor verifica o botão 50 vezes por segundo

Receptor recebe o estado do botão e acende/apaga os LEDs

Se perder sinal por 300ms, desliga os LEDs automaticamente


🚀 Dicas Importantes
Distância: Funciona até ~50m em área aberta

Paredes: Sinal enfraquece através de obstáculos

Alimentação: Use fonte 5V estável para melhor performance


💡 Ideias para Expandir
Adicione mais LEDs de cores diferentes

Controle relays ou motores

Crie uma rede com vários dispositivos

Adicione bateria para projeto sem fio total


📞 Precisando de Ajuda?
Se encontrar problemas:

Verifique o MAC address novamente

Confirme as conexões dos componentes

Teste cada ESP8266 individualmente

Lembre-se: 90% dos problemas são causados pelo MAC address incorreto!

Agora é com você! Este projeto é só o começo do que você pode fazer com ESP8266 e comunicação sem fio. 🚀

Divirta-se programando! 😊
