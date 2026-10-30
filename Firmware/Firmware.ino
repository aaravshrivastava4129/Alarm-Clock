#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_SCLK 9
#define TFT_MOSI 10
#define TFT_RST 8
#define TFT_DC 6
#define TFT_CS 7
#define TFT_BL 21

#define SW1 5
#define SW2 4
#define SW3 3
#define SW4 2

#define BUZZER 20

#define BLACK 0x0000
#define DARK_GREY 0x2104
#define GREY 0x8410
#define LIGHT_GREY 0xC618
#define WHITE 0xFFFF


class MyST7789 : public Adafruit_ST7789 {

public:

  MyST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst) : Adafruit_ST7789(cs, dc, mosi, sclk, rst) {}

  void setOffsets(uint8_t col, uint8_t row) {

    _colstart = col;
    _colstart2 = col;
    _rowstart = row;
    _rowstart2 = row;

  }

};

MyST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


int currentHour = 12;
int currentMinute = 0;

int alarmHour = 7;
int alarmMinute = 30;

bool settingMode = false;
bool settingHour = true;
bool alarmMode = false;

bool alarmTriggered = false;
bool alarmRinging = false;
bool alarmSoundOn = false;

bool lastSW1 = HIGH;
bool lastSW2 = HIGH;
bool lastSW3 = HIGH;
bool lastSW4 = HIGH;

unsigned long lastMinute = 0;
unsigned long lastButton = 0;
unsigned long lastAlarmBeep = 0;

const unsigned long debounce = 160;
const unsigned long alarmOn = 300;
const unsigned long alarmOff = 100;

const int timeY = 30;
const int hourX = 91;
const int colonX = 127;
const int minuteX = 145;

const int textSize = 3;
const int charWidth = 18;
const int charHeight = 24;


void beep() {

  ledcWriteTone(BUZZER, 1500);
  delay(60);
  ledcWriteTone(BUZZER, 0);

}


void startAlarmSound() {

  ledcWriteTone(BUZZER, 5000);
  alarmSoundOn = true;
  lastAlarmBeep = millis();

}


void stopAlarm() {

  ledcWriteTone(BUZZER, 0);
  alarmRinging = false;
  alarmSoundOn = false;

}


void updateAlarmSound() {

  if (!alarmRinging)
    return;

  unsigned long now = millis();

  if (alarmSoundOn) {

    if (now - lastAlarmBeep >= alarmOn) {

      ledcWriteTone(BUZZER, 0);
      alarmSoundOn = false;
      lastAlarmBeep = now;
    }
  } else {

    if (now - lastAlarmBeep >= alarmOff) {
      startAlarmSound();
    }

  }
}


void drawCloud(int x, int y, int size) {

  tft.fillCircle(x, y, size, WHITE);
  tft.fillCircle(x + size, y - size / 2, size + 2, WHITE);
  tft.fillCircle(x + size * 2, y, size, WHITE);

  tft.fillRoundRect(x - size / 2, y, size * 3, size, size / 2, WHITE);

}


void drawBackground() {
  tft.fillScreen(BLACK);

  drawCloud(15, 12, 5);
  drawCloud(250, 11, 5);

  tft.fillCircle(45, 68, 2, GREY);
  tft.fillCircle(235, 67, 2, LIGHT_GREY);
  tft.fillCircle(68, 12, 2, GREY);
  tft.fillCircle(216, 28, 2, LIGHT_GREY);
}


void drawHeader() {
  tft.fillRect(0, 0, 284, 18, DARK_GREY);

  tft.setTextColor(WHITE);
  tft.setTextSize(1);

  if (settingMode) {
    tft.setCursor(8, 6);

    if (alarmMode)
      tft.print("SET ALARM");
    else
      tft.print("SET TIME");

  } else if (alarmMode) {
    tft.setCursor(112, 6);
    tft.print("ALARM");

  } else {
    tft.setCursor(108, 6);
    tft.print("BLARE CLOCK");
  }

  tft.drawLine(8, 17, 276, 17, LIGHT_GREY);
}


void drawTimePanel() {
  tft.fillRoundRect(75, 23, 134, 39, 8, DARK_GREY);
  tft.drawRoundRect(75, 23, 134, 39, 8, LIGHT_GREY);
}


void drawNumber(int number, int x, int y) {
  char value[3];
  sprintf(value, "%02d", number);

  tft.fillRect(x, y, charWidth * 2, charHeight + 5, DARK_GREY);

  tft.setTextColor(WHITE);
  tft.setTextSize(textSize);
  tft.setCursor(x, y);
  tft.print(value);
}


void drawColon() {
  tft.fillRect(colonX, timeY, 10, charHeight + 5, DARK_GREY);

  tft.setTextColor(LIGHT_GREY);
  tft.setTextSize(textSize);
  tft.setCursor(colonX, timeY);
  tft.print(":");
}


void drawHourLine(bool visible) {
  tft.drawLine( hourX, timeY + charHeight + 2, hourX + 35, timeY + charHeight + 2, visible ? WHITE : DARK_GREY);
}


void drawMinuteLine(bool visible) {
  tft.drawLine(minuteX, timeY + charHeight + 2, minuteX + 35, timeY + charHeight + 2, visible ? WHITE : DARK_GREY);
}


void drawTime() {
  drawTimePanel();

  if (alarmMode) {
    drawNumber(alarmHour, hourX, timeY);
    drawNumber(alarmMinute, minuteX, timeY);
  } else {
    drawNumber(currentHour, hourX, timeY);
    drawNumber(currentMinute, minuteX, timeY);
  }

  drawColon();
}


void refreshScreen() {
  drawBackground();
  drawHeader();
  drawTime();
}


void changeMode() {
  if (settingMode)
    return;

  alarmMode = !alarmMode;

  beep();
  refreshScreen();
}


void startSetting() {
  settingMode = true;
  settingHour = true;

  beep();

  drawHeader();
  drawHourLine(true);
  drawMinuteLine(false);
}


void finishSetting() {
  settingMode = false;

  drawHourLine(false);
  drawMinuteLine(false);

  drawHeader();
  drawTime();

  lastMinute = millis();

  beep();
}


void changeHour(int amount) {
  if (alarmMode) {
    alarmHour += amount;

    if (alarmHour > 23)
      alarmHour = 0;

    if (alarmHour < 0)
      alarmHour = 23;

    drawNumber(alarmHour, hourX, timeY);

  } else {
    currentHour += amount;

    if (currentHour > 23)
      currentHour = 0;

    if (currentHour < 0)
      currentHour = 23;

    drawNumber(currentHour, hourX, timeY);
  }
}


void changeMinute(int amount) {
  if (alarmMode) {
    alarmMinute += amount;

    if (alarmMinute > 59)
      alarmMinute = 0;

    if (alarmMinute < 0)
      alarmMinute = 59;

    drawNumber(alarmMinute, minuteX, timeY);

  } else {
    currentMinute += amount;

    if (currentMinute > 59)
      currentMinute = 0;

    if (currentMinute < 0)
      currentMinute = 59;

    drawNumber(currentMinute, minuteX, timeY);
  }
}


void handleSettingButtons() {
  bool sw1 = digitalRead(SW1);
  bool sw2 = digitalRead(SW2);
  bool sw3 = digitalRead(SW3);
  bool sw4 = digitalRead(SW4);

  unsigned long now = millis();

  if (now - lastButton < debounce)
    return;

  if (sw1 == LOW && lastSW1 == HIGH) {
    beep();

    if (settingHour)
      drawHourLine(false);
    else
      drawMinuteLine(false);

    settingHour = !settingHour;

    if (settingHour)
      drawHourLine(true);
    else
      drawMinuteLine(true);

    lastButton = now;
  }

  if (sw2 == LOW && lastSW2 == HIGH) {
    beep();

    if (settingHour)
      changeHour(1);
    else
      changeMinute(1);

    lastButton = now;
  }

  if (sw3 == LOW && lastSW3 == HIGH) {
    beep();

    if (settingHour)
      changeHour(-1);
    else
      changeMinute(-1);

    lastButton = now;
  }

  if (sw4 == LOW && lastSW4 == HIGH) {
    finishSetting();
    lastButton = now;
  }

  lastSW1 = sw1;
  lastSW2 = sw2;
  lastSW3 = sw3;
  lastSW4 = sw4;
}


void handleNormalButtons() {
  bool sw1 = digitalRead(SW1);
  bool sw4 = digitalRead(SW4);

  unsigned long now = millis();

  if (now - lastButton < debounce)
    return;

  if (sw4 == LOW && lastSW4 == HIGH) {
    changeMode();
    lastButton = now;
  }

  if (sw1 == LOW && lastSW1 == HIGH) {
    startSetting();
    lastButton = now;
  }

  lastSW1 = sw1;
  lastSW4 = sw4;
}


void handleAlarmButton() {
  bool sw2 = digitalRead(SW2);

  unsigned long now = millis();

  if (now - lastButton < debounce)
    return;

  if (sw2 == LOW && lastSW2 == HIGH) {
    if (alarmRinging)
      stopAlarm();

    lastButton = now;
  }

  lastSW2 = sw2;
}


void updateClock() {
  unsigned long now = millis();

  if (now - lastMinute >= 60000) {
    while (now - lastMinute >= 60000) {
      lastMinute += 60000;

      currentMinute++;

      if (currentMinute >= 60) {
        currentMinute = 0;
        currentHour++;

        if (currentHour >= 24)
          currentHour = 0;

        if (!alarmMode) {
          drawNumber(currentHour, hourX, timeY);
          drawNumber(currentMinute, minuteX, timeY);
        }

      } else if (!alarmMode) {
        drawNumber(currentMinute, minuteX, timeY);
      }
    }
  }

  if (
    currentHour == alarmHour &&
    currentMinute == alarmMinute &&
    !alarmTriggered
  ) {
    alarmTriggered = true;
    alarmRinging = true;
    startAlarmSound();
  }

  if (
    currentHour != alarmHour ||
    currentMinute != alarmMinute
  ) {
    alarmTriggered = false;
  }
}


void startupScreen() {
  tft.fillScreen(BLACK);

  drawCloud(30, 40, 7);
  drawCloud(230, 35, 6);

  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.setCursor(98, 27);
  tft.print("BLARE");

  delay(700);

  beep();

  delay(300);

  tft.fillScreen(BLACK);
}


void setup() {
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);

  ledcAttach(BUZZER, 5000, 8);
  ledcWriteTone(BUZZER, 0);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW);

  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  tft.init(76, 284);
  tft.setOffsets(82, 18);
  tft.invertDisplay(false);
  tft.setRotation(1);

  startupScreen();
  refreshScreen();

  lastMinute = millis();
}


void loop() {
  if (settingMode) {
    handleSettingButtons();
  } else {
    handleNormalButtons();
    updateClock();
    handleAlarmButton();
    updateAlarmSound();
  }

  delay(5);
}