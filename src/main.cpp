
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);


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
}

void loop()
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.printf("%04d RPM", (int)(millis() / 1000));
    display.display();
    delay(1000);
}
