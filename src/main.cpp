#define WIDTH 16
#define HEIGHT 16

#define LED_PIN 7
#define LED_NUM WIDTH*HEIGHT

#include <Arduino.h>
#include "NeoMatrix.h"
#include "FastLED.h"
#include "GParser.h"
CRGB leds[LED_NUM];
Matrix<WIDTH, HEIGHT> matrix(FastLED, leds);

/*
 * "протокол" с мордочки на сервер
 *   /?fill&col=цвет - залить
 *   /?set&x = int, x &y = int, y &col=цвет, 4 байта - поставить пиксель
 *   /?clear - очистить
 * 
 * "протокол" с сервер на дуину, терминатор ;
 *   0, x, y, цвет red, цвет green, цвет blue; - поставить пиксель
 *   1, цвет red, цвет green, цвет blue; - заливка
 *   2; - очистка
*/

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(50);
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 3000);
    leds[0] = CRGB::Red;
    FastLED.show();
}

void loop() {
    if (Serial.available()){
        char buffer[30];
        int am = Serial.readBytesUntil(';', buffer, 30);
        buffer[am] = '\0';
        GParser parser(buffer);
        int32_t data[6];
        parser.parseLong(data);
        switch(data[0]){
            case 0:
                matrix.setPixel(data[1], data[2], CRGB(data[3], data[4], data[5]));
                break;
            case 1:
                matrix.fillAll(CRGB(data[1], data[2], data[3]));
                break;
            case 2:
                matrix.fillAll(CRGB::Black);
                break;
        }
        FastLED.show();
    }
}