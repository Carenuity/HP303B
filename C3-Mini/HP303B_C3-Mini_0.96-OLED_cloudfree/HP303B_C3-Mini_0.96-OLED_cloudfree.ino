#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LOLIN_HP303B.h>

LOLIN_HP303B HP303B;
// OLED display setup
#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);

  HP303B.begin();  // Initialize sensor (no return value)
  delay(100);      // Short delay for stabilization
}

void loop() {
  // Your existing loop code...
  display.clearDisplay();
  
  int32_t temperature, pressure;
  int16_t oversampling = 3;  // Reduced from 7 for stability
  int16_t ret_temp = HP303B.measureTempOnce(temperature, oversampling);
  int16_t ret_press = HP303B.measurePressureOnce(pressure, oversampling);

  // Display header
  display.setTextSize(1);
  display.setCursor(35, 0);
  display.println("Sensor Data");
  display.drawLine(0, 10, 127, 10, WHITE);

  // Vertical separator
  display.drawLine(63, 12, 63, 63, WHITE);

  // Display temperature (left)
  display.setCursor(5, 15);
  display.print("Temp_ture");
  display.setCursor(10, 25);
  if (ret_temp != 0) {
    display.println("Error");
  } else {
    char tempStr[10];
    dtostrf(temperature, 4, 1, tempStr);  
    display.print(tempStr);
    display.println(" C");
  }

  // Display pressure (right)
  display.setCursor(72, 15);
  display.print("Pressure:");
  display.setCursor(68, 25);
  if (ret_press != 0) {
    display.println("Error");
  } else {
    float pressure_mmHg = pressure / 133.322;
    char pressStr[10];
    dtostrf(pressure_mmHg, 5, 1, pressStr);  // "XXX.X" format
    display.print(pressStr);
    display.println(" mmHg");
  }

  display.display();
  delay(3000);
}
