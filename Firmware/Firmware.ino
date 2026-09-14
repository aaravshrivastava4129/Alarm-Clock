#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>


// Button Pins

#define SW1 3
#define SW2 2
#define SW3 1
#define SW4 0


// TFT Pins

#define TFT_SCLK 9
#define TFT_MOSI 10
#define TFT_RST 8
#define TFT_DC 4
#define TFT_CS 5
#define TFT_BL 6


// Buzzer

#define BUZZER 7


// TFT Display

class MyST7789 : public Adafruit_ST7789 {
public:

  MyST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst) : Adafruit_ST7789(cs, dc, mosi, sclk, rst) {}

  void setOffsets(uint8_t col, uint8_t row) {
    _colstart = _colstart2 = col;
    _rowstart = _rowstart2 = row;
  }
};


MyST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


// Timer

int currentHour = 0;
int currentMinute = 0;
int currentSecond = 0;

unsigned long lastSecond = 0;

int displayedHour = -1;
int displayedMinute = -1;

bool timerRunning = false;
unsigned long timerStart = 0;


// Setup

void setup() {

  Serial.begin(115200);


  // Buttons

  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);


  // Buzzer

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);


  // TFT Backlight

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);


  // Initialize TFT

  tft.init(76, 284);
  tft.setOffsets(82, 18);
  tft.invertDisplay(false);
  tft.setRotation(1);


  // Clear screen

  tft.fillScreen(ST77XX_WHITE);

  Serial.println("BLARE Alarm Clock initialized!");
}


// Button Helper

bool buttonPressed(int pin) {

  static bool lastSW1 = HIGH;
  static bool lastSW2 = HIGH;
  static bool lastSW3 = HIGH;
  static bool lastSW4 = HIGH;

  bool currentState = digitalRead(pin);
  bool pressed = false;

  if (pin == SW1) {

    if (lastSW1 == HIGH && currentState == LOW) {
      pressed = true;
    }

    lastSW1 = currentState;
  }

  else if (pin == SW2) {

    if (lastSW2 == HIGH && currentState == LOW) {
      pressed = true;
    }

    lastSW2 = currentState;
  }

  else if (pin == SW3) {

    if (lastSW3 == HIGH && currentState == LOW) {
      pressed = true;
    }

    lastSW3 = currentState;
  }

  else if (pin == SW4) {

    if (lastSW4 == HIGH && currentState == LOW) {
      pressed = true;
    }

    lastSW4 = currentState;
  }

  return pressed;
}


// Timer Button

void handleTimerButton() {

  if (buttonPressed(SW1)) {

    if (timerRunning == false) {

      // Reset timer
      currentHour = 0;
      currentMinute = 0;
      currentSecond = 0;

      displayedHour = -1;
      displayedMinute = -1;

      // Start timer
      timerRunning = true;
      timerStart = millis();
      lastSecond = millis();

      displayTimer();

      Serial.println("Timer started!");

    } else {

      // Stop timer
      timerRunning = false;

      Serial.println("Timer stopped!");
    }
  }
}


// Timer Update

void updateTimer() {

  if (timerRunning == true) {

    if (millis() - lastSecond >= 1000) {

      lastSecond = millis();

      currentSecond++;


      if (currentSecond >= 60) {

        currentSecond = 0;
        currentMinute++;
      }


      if (currentMinute >= 60) {

        currentMinute = 0;
        currentHour++;
      }


      if (currentHour >= 24) {

        currentHour = 0;
      }
    }
  }
}


// Display Hour and Minute

void displayHourMinute() {

  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(4);

  tft.setCursor(20, 50);


  if (currentHour < 10) {
    tft.print("0");
  }

  tft.print(currentHour);
  tft.print(":");


  if (currentMinute < 10) {
    tft.print("0");
  }

  tft.print(currentMinute);

  tft.print(":");
}


// Display Seconds

void displaySeconds() {

  tft.fillRect(150, 45, 70, 40, ST77XX_WHITE);

  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(4);

  tft.setCursor(150, 50);


  if (currentSecond < 10) {
    tft.print("0");
  }

  tft.print(currentSecond);
}


// Display Timer

void displayTimer() {

  tft.fillScreen(ST77XX_WHITE);

  displayHourMinute();
  displaySeconds();
}


// Main Loop

void loop() {

  handleTimerButton();

  if (timerRunning == true) {

    updateTimer();

    if (
      currentHour != displayedHour ||
      currentMinute != displayedMinute
    ) {

      displayedHour = currentHour;
      displayedMinute = currentMinute;

      displayTimer();
    }

    displaySeconds();
  }
}
