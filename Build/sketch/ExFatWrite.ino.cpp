#include <Arduino.h>
#line 1 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino"
#include <LiquidCrystal_I2C.h>
#include <SdFat.h>

#include <EEPROM.h>
#include <Wire.h>

SdExFat sd;
ExFatVolume volume;
ExFile file;
#define FileCounterLockPin 2
#define CsPin 10

#define LCD_I2C_Address 0x27
#define LCD_I2C_Cols 2
#define LCD_I2C_Rows 16

LiquidCrystal_I2C lcd(LCD_I2C_Address, LCD_I2C_Cols, LCD_I2C_Rows);

#define CounterAddress 256

#define FileContextLength 32
#define FilePrefix "/DATA_"
#define FilePostfix ".DATA"
#define FileName FilePrefix + String(FileCounter) + FilePostfix

unsigned long FileCounter;
unsigned long writeLinesCount = 1;

#line 29 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino"
void setup();
#line 60 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino"
void loop();
#line 86 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino"
const char * generateRandomString();
#line 99 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino"
void SdFileInfoWrite();
#line 108 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino"
void FileNameForLcdPrint(size_t delayMs);
#line 120 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino"
void sdLcdErrorMessage();
#line 140 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino"
void Initialize();
#line 163 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino"
void fileLcdErrorMessage();
#line 29 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino"
void setup()
{
    lcd.begin();

    Initialize();

    while (!sd.begin(CsPin))
    {
        sdLcdErrorMessage();
    }

    pinMode(FileCounterLockPin, INPUT_PULLUP);

    lcd.backlight();

    FileCounter = EEPROM.get(CounterAddress, FileCounter);

    if (digitalRead(FileCounterLockPin) == LOW)
    {
        FileCounter++;

        EEPROM.put(CounterAddress, FileCounter);
    }

    FileNameForLcdPrint(2000);

    lcd.clear();

    file = sd.open(FileName, FILE_WRITE);
}

void loop()
{
    lcd.backlight();

    randomSeed(analogRead(A0));

    if (!file)
    {
        fileLcdErrorMessage();
        return setup();
    }

    for (size_t i = 0; i < 32; i++)
    {
        file.print(generateRandomString());
    }

    file.println();

    SdFileInfoWrite();

    file.close();

    writeLinesCount++;
}

const char *generateRandomString()
{
    const char possible[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static char rString[FileContextLength + 1];
    int possible_len = strlen(possible);
    for (int i = 0; i < FileContextLength; i++)
    {
        int r = random(possible_len);
        rString[i] = possible[r];
    }
    return rString;
}

void SdFileInfoWrite()
{
    lcd.setCursor(0, 0);
    lcd.print("F.S: " + String((float)file.size() / 1048576, 1) + " MB");

    lcd.setCursor(0, 1);
    lcd.print("W.L: " + String(writeLinesCount));
}

void FileNameForLcdPrint(size_t delayMs)
{
    lcd.setCursor(0, 0);
    lcd.print("File Name:");

    lcd.setCursor(0, 1);
    lcd.print(FileName);

    delay(delayMs);
    lcd.clear();
}

void sdLcdErrorMessage()
{
    lcd.setCursor(0, 0);
    lcd.print("Error: Please");
    lcd.setCursor(0, 1);
    lcd.print("Check SD Card!");

    if (lcd.getBacklight())
    {
        lcd.noBacklight();
        delay(500);
    }

    else
    {
        lcd.backlight();
        delay(1500);
    }
}

void Initialize()
{
    lcd.noDisplay();
    lcd.noBacklight();
    lcd.noCursor();

    delay(1000);

    lcd.backlight();
    lcd.display();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Arduino Initial.");

    lcd.setCursor(0, 1);
    lcd.print("Data Collector..");

    delay(2500);

    lcd.clear();
}

void fileLcdErrorMessage()
{
    lcd.setCursor(0, 0);
    lcd.print("Error: file not");
    lcd.setCursor(0, 1);
    lcd.print("Created!!!");
    size_t loop = 0;
    while (loop < 10)
    {
        if (!lcd.getBacklight())
        {
            lcd.backlight();
            delay(1500);
        }

        else
        {
            lcd.noBacklight();
            delay(125);
        }
        loop++;
    }

    lcd.backlight();
}
