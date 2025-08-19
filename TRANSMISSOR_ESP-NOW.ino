/*
  ESP8266 - TRANSMISSOR (Botão + OLED U8g2) via ESP-NOW (API do ESP8266)
  - Botão no GPIO5 (D1) com pull-up interno.
  - Envia o estado do botão:
      * Sempre que mudar (transição imediata)
      * Além disso, keepalive a cada 20 ms (baixo delay e robusto a perdas)
  - OLED (U8g2) em SW-I2C: clock=GPIO12, data=GPIO14
  - MAC do receptor (LED): 10:52:1C:02:C4:BC
  - WiFi sem sleep para reduzir latência.
*/

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <U8g2lib.h>

// ===== OLED (U8g2) em SW-I2C: clock=GPIO12, data=GPIO14 =====
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(
  U8G2_R0, /*clock=*/12, /*data=*/14, U8X8_PIN_NONE
);

// ===== PINOS =====
#define BUTTON_PIN 5  // GPIO5 (D1). Botão entre este pino e GND.

// ===== Estrutura do payload =====
typedef struct __attribute__((packed)) {
  uint8_t buttonState; // 0=solto, 1=pressionado
} msg_t;

msg_t txMsg;

// ===== MAC do receptor (LED) =====
uint8_t receiverMAC[6] = {0x10, 0x52, 0x1C, 0x02, 0xC4, 0xBC};

// ===== Timings (otimizados p/ baixa latência) =====
const unsigned long DEBOUNCE_MS   = 12;   // filtro rápido
const unsigned long KEEPALIVE_MS  = 20;   // 50 Hz de atualização
const unsigned long UI_REFRESH_MS = 120;  // não travar loop com OLED

bool lastStablePressed = false;
bool lastRaw           = true;            // pull-up: HIGH = solto
unsigned long lastChangeMs   = 0;
unsigned long lastSendMs     = 0;
unsigned long lastUiMs       = 0;

// ===== Callback de envio (minimalista p/ não atrasar loop) =====
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  // Mantido “silencioso” para não impactar latência.
  // Se quiser logar, descomente:
  // Serial.println(sendStatus == 0 ? "[TX] SUCESSO" : "[TX] FALHA");
}

void drawOLED(const char* line1, const char* line2 = nullptr) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_7x14B_tr);

  // Mantém as mensagens mais para baixo (afasta da faixa amarela)
  u8g2.setCursor(0, 28);
  u8g2.print(line1);
  if (line2) {
    u8g2.setCursor(0, 48);
    u8g2.print(line2);
  }
  u8g2.sendBuffer();
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // OLED
  u8g2.begin();
  drawOLED("Transmissor", "Iniciando...");

  // Botão
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  lastRaw = digitalRead(BUTTON_PIN);

  // WiFi + ESP-NOW (sem sleep para reduzir latência)
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("[ERRO] esp_now_init()");
    drawOLED("ERRO:", "ESP-NOW init");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  if (esp_now_add_peer(receiverMAC, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != 0) {
    Serial.println("[ERRO] esp_now_add_peer()");
    drawOLED("ERRO:", "Add peer");
    return;
  }

  Serial.print("[INFO] MAC (TX): ");
  Serial.println(WiFi.macAddress());
  drawOLED("Pronto", "Aguardando...");
}

void loop() {
  // Debounce não-bloqueante
  bool raw = digitalRead(BUTTON_PIN); // HIGH=solto, LOW=pressionado
  if (raw != lastRaw) {
    lastChangeMs = millis();
    lastRaw = raw;
  }

  bool pressed = lastStablePressed;
  if (millis() - lastChangeMs > DEBOUNCE_MS) {
    pressed = (digitalRead(BUTTON_PIN) == LOW);
  }

  unsigned long now = millis();

  // Critério de envio:
  // - Sempre que houver transição (imediato)
  // - Ou a cada KEEPALIVE_MS (garantia de atualização contínua)
  bool transition = (pressed != lastStablePressed);
  bool timeToSend = (now - lastSendMs >= KEEPALIVE_MS);

  if (transition || timeToSend) {
    txMsg.buttonState = pressed ? 1 : 0;
    esp_now_send(receiverMAC, (uint8_t*)&txMsg, sizeof(txMsg));
    lastSendMs = now;

    // Logs enxutos (pode comentar se quiser máximo desempenho)
    if (transition) {
      Serial.println(pressed ? "[TX] Pressionado (transicao)" : "[TX] Solto (transicao)");
    }
  }

  // OLED: atualiza no máx. a cada UI_REFRESH_MS para não travar o loop
  if (now - lastUiMs >= UI_REFRESH_MS) {
    drawOLED(pressed ? "Pressionado!" : "Aguardando...", pressed ? "Enviando..." : "Botao solto");
    lastUiMs = now;
  }

  lastStablePressed = pressed;

  // Zero delay: mantém loop muito responsivo
  // yield(); // opcional: o core já faz isso quando necessário
}
