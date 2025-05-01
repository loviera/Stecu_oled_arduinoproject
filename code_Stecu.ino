#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BUTTON_PIN 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* words[] = {
  "ADUH\nABANG",
  "BUKAN\nMAKSUDKU",
  "BEGITU",
  "MASALAH\nSTECU",
  "BUKAN\nBERARTI",
  "TAK MAU",
  "JUAL\nMAHAL",
  "DIKIT\nKAN BISA",
  "COBA\nKASE",
  "EFFORTNYA\nSAJA",
  "KALO\nMEMANG",
  "COCOK",
  "BISA\nDATANG",
  "KE RUMAH",
  "STECU"
};

int wordIndex = 0;
bool showText = true;
unsigned long lastBlink = 0;
const unsigned long blinkInterval = 200;

bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// Untuk efek goyang
int wobbleOffset = 0;
int wobbleDirection = 1;
unsigned long lastWobbleTime = 0;
const unsigned long wobbleInterval = 10;  // Lebih cepat goyangnya

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);  // Ukuran teks diperbesar (2x)
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop() {
  // Debounce tombol
  bool currentButtonState = digitalRead(BUTTON_PIN);
  if (currentButtonState != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentButtonState == LOW) {
      wordIndex = (wordIndex + 1) % (sizeof(words) / sizeof(words[0]));
      while (digitalRead(BUTTON_PIN) == LOW);  // Tunggu sampai dilepas
    }
  }
  lastButtonState = currentButtonState;

  // Blink handler
  if (millis() - lastBlink > blinkInterval) {
    showText = !showText;
    lastBlink = millis();
  }

  // Wobble effect handler
  if (millis() - lastWobbleTime > wobbleInterval) {
    wobbleOffset += wobbleDirection;
    if (wobbleOffset > 3 || wobbleOffset < -3) {
      wobbleDirection *= -1;
    }
    lastWobbleTime = millis();
  }

  display.clearDisplay();

  if (showText) {
    char buffer[32];
    strcpy(buffer, words[wordIndex]);

    char* upperLine = strtok(buffer, "\n");
    char* lowerLine = strtok(NULL, "\n");

    if (strcmp(words[wordIndex], "STECU") == 0) {
      // Efek khusus STECU
      int16_t x1, y1;
      uint16_t w, h;
      display.getTextBounds("STECU", 0, 0, &x1, &y1, &w, &h);
      int xPos = (SCREEN_WIDTH - w) / 2;
      int yPos = (SCREEN_HEIGHT - h) / 2 + wobbleOffset;
      display.setCursor(xPos, yPos);
      display.print("STECU");
    }
    else if (lowerLine == NULL) {
      // Jika satu kata, posisikan di tengah
      int16_t x1, y1;
      uint16_t w, h;
      display.getTextBounds(upperLine, 0, 0, &x1, &y1, &w, &h);
      int xPos = (SCREEN_WIDTH - w) / 2;
      int yPos = (SCREEN_HEIGHT - h) / 2;
      display.setCursor(xPos, yPos);
      display.print(upperLine);
    }
    else {
      // Dua baris teks
      int16_t x1, y1;
      uint16_t w, h;

      display.getTextBounds(upperLine, 0, 0, &x1, &y1, &w, &h);
      int yPosUpper = (SCREEN_HEIGHT - h * 2) / 2 - 5;
      display.setCursor((SCREEN_WIDTH - w) / 2, yPosUpper);
      display.print(upperLine);

      display.getTextBounds(lowerLine, 0, 0, &x1, &y1, &w, &h);
      int yPosLower = (SCREEN_HEIGHT - h * 2) / 2 + 15;
      display.setCursor((SCREEN_WIDTH - w) / 2, yPosLower);
      display.print(lowerLine);
    }
  }

  display.display();
}
