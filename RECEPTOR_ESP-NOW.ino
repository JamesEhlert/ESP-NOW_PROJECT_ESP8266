/*
  ESP8266 - RECEPTOR (LED_BUILTIN + LED externo) via ESP-NOW
  - Recebe buttonState do transmissor.
  - Se buttonState=1: LED_BUILTIN e LED externo acendem.
  - Se buttonState=0: ambos apagam.
  - "Link UP": quando chega o primeiro pacote.
  - "Link DOWN": se ficar >300 ms sem receber pacotes (apaga LEDs rápido).
  - Serial exibe MAC de quem enviou e o estado do botão.
*/

#include <ESP8266WiFi.h>
#include <espnow.h>

// ===== Estrutura do payload =====
typedef struct __attribute__((packed)) {
  uint8_t buttonState; // 0=solto, 1=pressionado
} msg_t;

volatile msg_t rxMsg;

// ===== LEDs =====
#define LED_PIN     LED_BUILTIN  // LED interno (ativo em LOW)
#define LED_EXTERN  14           // GPIO14 = D5 no NodeMCU

// ===== Link supervision (curto p/ baixar latência de "solto") =====
const unsigned long LINK_TIMEOUT_MS = 300; // se não chega pacote, considera link down rapidamente
unsigned long lastPacketMs = 0;
bool linkUp = false;

// ===== Callback de recepção =====
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  if (len < sizeof(msg_t)) return;
  memcpy((void*)&rxMsg, incomingData, sizeof(msg_t));

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  lastPacketMs = millis();

  if (!linkUp) {
    linkUp = true;
    Serial.print("[RX] Link UP - primeiro pacote de ");
    Serial.println(macStr);
  }

  // Atualiza LEDs conforme estado do botão
  if (rxMsg.buttonState == 1) {
    digitalWrite(LED_PIN, LOW);    // LED interno aceso (ativo em LOW)
    digitalWrite(LED_EXTERN, HIGH); // LED externo aceso
    Serial.println("[RX] Botao PRESSIONADO -> LEDs ACESOS");
  } else {
    digitalWrite(LED_PIN, HIGH);   // LED interno apagado
    digitalWrite(LED_EXTERN, LOW); // LED externo apagado
    Serial.println("[RX] Botao SOLTO -> LEDs APAGADOS");
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // Configura LEDs
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // começa apagado

  pinMode(LED_EXTERN, OUTPUT);
  digitalWrite(LED_EXTERN, LOW); // começa apagado

  // WiFi + ESP-NOW (sem sleep para reduzir latência)
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("[ERRO] esp_now_init()");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  Serial.print("[INFO] MAC (RX): ");
  Serial.println(WiFi.macAddress());
  Serial.println("[INFO] Aguardando pacotes ESP-NOW...");
}

void loop() {
  unsigned long now = millis();

  // Verifica se link caiu (apaga LEDs rápido se transmissor parar de enviar)
  if (linkUp && (now - lastPacketMs > LINK_TIMEOUT_MS)) {
    linkUp = false;
    digitalWrite(LED_PIN, HIGH);   // apaga LED interno
    digitalWrite(LED_EXTERN, LOW); // apaga LED externo
    Serial.println("[RX] Link DOWN - timeout sem pacotes");
  }

  // Sem delay para máxima responsividade
}
