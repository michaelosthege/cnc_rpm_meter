
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

volatile uint16_t pulseCount = 0;
volatile unsigned long previousPulse;
uint16_t interval = 200;
unsigned long lastUpdate = 0;

const uint8_t bufferSize = 64;
volatile unsigned long buffer[bufferSize];
volatile uint8_t bufferIdx = 0;

void IRAM_ATTR handlePulseInterrupt()
{
    static unsigned long now;

    pulseCount++;
    // measure time since last pulse into rolling buffer
    now = micros();
    buffer[bufferIdx] = now - previousPulse;
    bufferIdx = (bufferIdx + 1) % bufferSize;
    previousPulse = now;
}

float getMeanPulsewidth()
{
    static unsigned long sum;
    sum = 0;
    for (size_t i = 0; i < bufferSize; i++)
    {
        sum += buffer[i];
    }
    return (float)sum / (float)bufferSize;
}

void setup()
{
    Serial.begin(115200);

    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.cp437(true);
    
    display.setCursor(0, 0);
    display.println("0123456789");
    display.setCursor(0, 8);
    display.println("ABCDEFGHIJ");
    display.setCursor(0, 16);
    display.println("KLMNOPQRST");
    display.setCursor(0, 24);
    display.println("UVWX  YZ_!");
    display.setCursor(0, 32);
    display.println("123456789A");
    display.setCursor(0, 40);
    display.println(1234567890);
    display.display();
    delay(2000);

    pinMode(D7, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(D7), handlePulseInterrupt, FALLING);
}

void loop()
{
    static unsigned long elapsed;
    static float meanPulsewidth;
    static float rpm;

    elapsed = millis() - lastUpdate;
    if (elapsed < interval)
        return;

    // 2 pulses per revolution!
    meanPulsewidth = getMeanPulsewidth();
    if (pulseCount < 4)
        rpm = 0;
    else
        rpm = 60000000.0f / 2 / meanPulsewidth;

    Serial.printf(">pulses:%i\n", pulseCount);
    Serial.printf(">pulseWidth_µS:%.1f\n", meanPulsewidth);
    Serial.printf(">RPM:%.1f\n", rpm);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.printf("%05d RPM", (int)rpm);
    display.display();

    pulseCount = 0;
    lastUpdate = millis();
}
