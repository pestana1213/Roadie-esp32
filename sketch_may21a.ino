#include <BluetoothSerial.h>
#include <TFT_eSPI.h>
#include "Base64.h" 
#include <Arduino.h>

BluetoothSerial SerialBT;
TFT_eSPI tft = TFT_eSPI();
#define ICON_SIZE 32
#define SCREEN_WIDTH 128  // Adjust based on your display
#define SCREEN_HEIGHT 128 // Adjust based on your display


int decode_base64(const char *input, uint8_t *output, int outputLen) {
    const char lookup[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int buffer = 0, bitsCollected = 0, outIndex = 0;

    for (const char *p = input; *p; ++p) {
        char ch = *p;
        if (ch == '=') break;

        const char *lookupPos = strchr(lookup, ch);
        if (!lookupPos) continue; // Ignore non-base64 characters

        buffer = (buffer << 6) | (lookupPos - lookup);
        bitsCollected += 6;
        if (bitsCollected >= 8) {
            bitsCollected -= 8;
            if (outIndex < outputLen) {
                output[outIndex++] = (buffer >> bitsCollected) & 0xFF;
            } else {
                break; // Prevent overflow
            }
        }
    }
    return outIndex;
}

void drawWrappedText(String text, int x, int y) {
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    // Clear text area with correct width
    tft.fillRect(0, 0, SCREEN_WIDTH, 50, TFT_BLACK);

    int currentX = x;
    int currentY = y;

    int start = 0;
    while (start < text.length()) {
        int end = text.indexOf(' ', start);
        if (end == -1) end = text.length();

        String word = text.substring(start, end);
        int16_t wordWidth = tft.textWidth(word + " ");

        if (currentX + wordWidth > SCREEN_WIDTH) {
            currentX = x;
            currentY += tft.fontHeight();
            if (currentY > 50) break;
        }

        tft.setCursor(currentX, currentY);
        tft.print(word + " ");
        currentX += wordWidth;

        start = end + 1;
    }
}

void displayIcon(const String &base64Data) {
    int iconX = (SCREEN_WIDTH - ICON_SIZE) / 2;
    int iconY = 50;

    // Base64 decode
    int expectedLen = ICON_SIZE * ICON_SIZE / 8; // 1024 bits = 128 bytes
    uint8_t decoded[expectedLen];
    int actualLen = decode_base64(base64Data.c_str(), decoded, sizeof(decoded));

    tft.startWrite();
    for (int y = 0; y < ICON_SIZE; y++) {
        for (int x = 0; x < ICON_SIZE; x++) {
            int bitIndex = y * ICON_SIZE + x;
            int byteIndex = bitIndex / 8;
            int bitInByte = 7 - (bitIndex % 8); // MSB first

            if (byteIndex < actualLen && (decoded[byteIndex] & (1 << bitInByte))) {
                tft.drawPixel(iconX + x, iconY + y, TFT_WHITE);
            }
        }
    }
    tft.endWrite();
}

void processNavigationData(String payload) {
    int textIdx = payload.indexOf("TEXT:");
    int iconIdx = payload.indexOf("|ICON:");
    
    if (textIdx != -1 && iconIdx != -1) {
        String text = payload.substring(textIdx + 5, iconIdx);
        String iconData = payload.substring(iconIdx + 6);
        
        // Clear screen (except maybe status bar area)
        tft.fillScreen(TFT_BLACK);
        
        // Display wrapped text at top
        drawWrappedText(text, 4, 4); // Small margin from top-left
        
        // Display icon if available
        displayIcon(iconData);
        
    }
}

void setup() {
    Serial.begin(115200);
    SerialBT.begin("ESP32_NAV");
    
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextWrap(false); // We handle wrapping ourselves
    drawWrappedText("Welcome to Roadie", 4, 4); // Small margin from top-left
}

void loop() {
    if (SerialBT.available()) {
        String payload = SerialBT.readStringUntil('\n');
        processNavigationData(payload);
    }
    delay(100);
}