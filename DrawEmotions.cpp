
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Config WiFi
const char* ssid = "*****";
const char* password = "*********";       
const char* serverIP = "192.168.1.72";  // IP del servidor socket
const uint16_t serverPort = 9080;       // Puerto del servidor

WiFiClient client;

// Sensor de proximidad
const int proximitySensorPin = 19;

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define OLED_SDA 21
#define OLED_SCL 22
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Ojos
const int eyeRadius = 15;
const int leftEyeX = SCREEN_WIDTH / 4;
const int rightEyeX = 3 * SCREEN_WIDTH / 4;
const int eyeY = SCREEN_HEIGHT / 2;

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" conectado!");
}

void connectToServer() {
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Conectado al servidor socket!");
  } else {
    Serial.println("No se pudo conectar al servidor.");
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(proximitySensorPin, INPUT);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();

  connectToWiFi();
  connectToServer();
}

// --- Animaciones ---
void drawNormal() {
  display.clearDisplay();
  display.fillCircle(leftEyeX, eyeY, eyeRadius, SSD1306_WHITE);
  display.fillCircle(rightEyeX, eyeY, eyeRadius, SSD1306_WHITE);
  display.display();
}

void drawFeliz() {
  display.clearDisplay();
  display.fillCircle(leftEyeX, eyeY + 5, 10, SSD1306_WHITE);
  display.fillCircle(leftEyeX, eyeY + 5, 5, SSD1306_BLACK);
  display.fillCircle(rightEyeX, eyeY + 5, 10, SSD1306_WHITE);
  display.fillCircle(rightEyeX, eyeY + 5, 5, SSD1306_BLACK);
  display.fillRect(leftEyeX - 10, eyeY + 5, 20, 10, SSD1306_BLACK);
  display.fillRect(rightEyeX - 10, eyeY + 5, 20, 10, SSD1306_BLACK);
  display.display();
}

void drawTriste() {
  display.clearDisplay();
  display.fillCircle(leftEyeX, eyeY - 5, 10, SSD1306_WHITE);
  display.fillCircle(leftEyeX, eyeY - 5, 5, SSD1306_BLACK);
  display.fillCircle(rightEyeX, eyeY - 5, 10, SSD1306_WHITE);
  display.fillCircle(rightEyeX, eyeY - 5, 5, SSD1306_BLACK);
  display.fillRect(leftEyeX - 10, eyeY - 15, 20, 10, SSD1306_BLACK);
  display.fillRect(rightEyeX - 10, eyeY - 15, 20, 10, SSD1306_BLACK);
  display.display();
}

void drawCurioso() {
  display.clearDisplay();
  display.fillCircle(leftEyeX, eyeY, eyeRadius, SSD1306_WHITE);
  display.fillCircle(rightEyeX, eyeY, eyeRadius, SSD1306_WHITE);
  display.display();
}

void drawConfundido() {
  display.clearDisplay();
  display.fillCircle(leftEyeX, eyeY, eyeRadius, SSD1306_WHITE);
  display.fillCircle(rightEyeX, eyeY, 10, SSD1306_WHITE);
  display.display();
}

void drawDormido() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(SCREEN_WIDTH / 2 - 5, 5);
  display.print("Z");
  display.setCursor(SCREEN_WIDTH / 2 - 3, 15);
  display.print("Z");
  display.setCursor(SCREEN_WIDTH / 2 - 1, 25);
  display.print("Z");
  display.display();
}

void drawEnojado() {
  display.clearDisplay();
  display.fillCircle(leftEyeX, eyeY, eyeRadius - 5, SSD1306_WHITE);
  display.fillCircle(rightEyeX, eyeY, eyeRadius - 5, SSD1306_WHITE);
  for (int i = 0; i < 10; i++) {
    display.drawLine(leftEyeX - 10 + i, eyeY - 15, leftEyeX + 5 + i, eyeY - 5, SSD1306_WHITE);
    display.drawLine(rightEyeX + 10 - i, eyeY - 15, rightEyeX - 5 - i, eyeY - 5, SSD1306_WHITE);
  }
  display.display();
}

void drawParpadeo() {
  display.clearDisplay();
  display.drawLine(leftEyeX - 10, eyeY, leftEyeX + 10, eyeY, SSD1306_WHITE);
  display.drawLine(rightEyeX - 10, eyeY, rightEyeX + 10, eyeY, SSD1306_WHITE);
  display.display();
}


void loop() {
  if (client.connected() && client.available()) {
    String msg = client.readStringUntil('\n');
    msg.trim();
    Serial.print("Recibido: ");
    Serial.println(msg);
    
    if (msg == "feliz") drawFeliz();
    else if (msg == "triste") drawTriste();
    else if (msg == "enojado") drawEnojado();
    else if (msg == "curioso") drawCurioso();
    else if (msg == "confundido") drawConfundido();
    else if (msg == "dormido") drawDormido();
    else if (msg == "normal") drawNormal();
    else if (msg == "parpadeo") drawParpadeo();
    else Serial.println("Emoción no reconocida");
  }
}
