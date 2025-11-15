
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

volatile uint16_t pulseCount = 0;
uint16_t interval = 200;
unsigned long lastUpdate = 0;


void ICACHE_RAM_ATTR countPulse()
{
    pulseCount++;
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
    attachInterrupt(digitalPinToInterrupt(D7), countPulse, FALLING);
}

void loop()
{
    static unsigned long elapsed;
    static float rpm;

    elapsed = millis() - lastUpdate;
    if (elapsed < interval)
        return;

    // 2 pulses per revolution!
    rpm = ((float)pulseCount / 2.0) * (60000.0 / elapsed);

    Serial.printf(">pulses:%i\n", pulseCount);
    Serial.printf(">RPM:%.1f\n", rpm);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.printf("%05d RPM", (int)rpm);
    display.display();

    pulseCount = 0;
    lastUpdate = millis();
}
