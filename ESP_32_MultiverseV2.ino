#include <WiFi.h>
#include <WiFiClient.h>
#include <Artnet.h>
//#define FASTLED_ALLOW_INTERRUPTS 0
//#define INTERRUPT_THRESHOLD 1
#include "FastLED.h"
FASTLED_USING_NAMESPACE
#define FASTLED_SHOW_CORE 1
#define LED_PANEL_WIDTH 34
#define LED_PANEL_HEIGHT 20
#define NUM_PANELS 4
#define NUM_LEDS_PANEL LED_PANEL_WIDTH*LED_PANEL_HEIGHT
#define NUM_LEDS NUM_LEDS_PANEL*NUM_PANELS
#define UNIVERSE_SIZE 170
CRGB leds[NUM_LEDS];

Artnet artnet;

static TaskHandle_t FastLEDshowTaskHandle2 = 0;
static TaskHandle_t userTaskHandle = 0;

void FastLEDshowESP322()
{
    if (userTaskHandle == 0) {
        const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );
        userTaskHandle = xTaskGetCurrentTaskHandle();
        xTaskNotifyGive(FastLEDshowTaskHandle2);
    }
}

void FastLEDshowTask2(void *pvParameters)
{
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
     for(;;) {
         ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
         memcpy(leds,artnet.getframe(),NUM_LEDS*sizeof(CRGB));
         FastLED.show();
         userTaskHandle=0;
     }
}

void setup() {
 Serial.begin(9600);
   xTaskCreatePinnedToCore(FastLEDshowTask2, "FastLEDshowTask2", 1000, NULL,3, &FastLEDshowTaskHandle2, FASTLED_SHOW_CORE);
   WiFi.mode(WIFI_STA);
   Serial.printf("Connecting ");
   WiFi.begin("LED_PANEL", "ledpanel2019");
   while (WiFi.status() != WL_CONNECTED) {
     Serial.println(WiFi.status());
     delay(500);
     Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    FastLED.addLeds<NEOPIXEL, 23>(leds, 0, NUM_LEDS_PANEL); //tell FastLED there's 600 NEOPIXEL leds on pin 23, starting at index 0 in the led array
    FastLED.addLeds<NEOPIXEL, 13>(leds, NUM_LEDS_PANEL, NUM_LEDS_PANEL);//tell FastLED there's 600 NEOPIXEL leds on pin 13, starting at index 600 in the led array
    FastLED.addLeds<NEOPIXEL, 12>(leds, 2 * NUM_LEDS_PANEL, NUM_LEDS_PANEL);//tell FastLED there's 600 NEOPIXEL leds on pin 12, starting at index 1200 in the led array
    FastLED.addLeds<NEOPIXEL, 14>(leds, 3 * NUM_LEDS_PANEL, NUM_LEDS_PANEL); // tell FastLED there's 600 NEOPIXEL leds on pin 14, starting at index 1800 in the led array
   //FastLED.addLeds<NEOPIXEL, 27>(leds, 4 * NUM_LEDS_PANEL, NUM_LEDS_PANEL); // tell FastLED there's 600 NEOPIXEL leds on pin 27, starting at index 2400 in the led array
   //FastLED.addLeds<NEOPIXEL, 26>(leds, 5 * NUM_LEDS_PANEL, NUM_LEDS_PANEL); // tell FastLED there's 600 NEOPIXEL leds on pin 26, starting at index 3000 in the led array
   //FastLED.addLeds<NEOPIXEL, 25>(leds, 6 * NUM_LEDS_PANEL, NUM_LEDS_PANEL); // tell FastLED there's 600 NEOPIXEL leds on pin 25, starting at index 3600 in the led array
   //FastLED.addLeds<NEOPIXEL, 33>(leds, 7 * NUM_LEDS_PANEL, NUM_LEDS_PANEL); // tell FastLED there's 600 NEOPIXEL leds on pin 33, starting at index 4200 in the led array
   //FastLED.addLeds<NEOPIXEL, 32>(leds, 8 * NUM_LEDS_PANEL, NUM_LEDS_PANEL); // tell FastLED there's 600 NEOPIXEL leds on pin 32, starting at index 4800 in the led array
    artnet.begin(NUM_LEDS,UNIVERSE_SIZE,1); 
}

void loop() {
  artnet.read();
  FastLEDshowESP322();
  artnet.resetsync();
 
}
