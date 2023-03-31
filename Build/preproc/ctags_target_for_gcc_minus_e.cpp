# 1 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino"
# 2 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino" 2
# 3 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino" 2

# 5 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino" 2
# 6 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino" 2

SdExFat sd;
ExFatVolume volume;
ExFile file;







LiquidCrystal_I2C lcd(0x27, 2, 16);
# 26 "C:\\Users\\furka\\source\\repos\\Arduino\\ExFatWrite\\ExFatWrite.ino"
unsigned long FileCounter;
unsigned long writeLinesCount = 1;

void setup()
{
    lcd.begin();

    Initialize();

    while (!sd.begin(10))
    {
        sdLcdErrorMessage();
    }

    pinMode(2, 0x2);

    lcd.backlight();

    FileCounter = EEPROM.get(256, FileCounter);

    if (digitalRead(2) == 0x0)
    {
        FileCounter++;

        EEPROM.put(256, FileCounter);
    }

    FileNameForLcdPrint(2000);

    lcd.clear();

    file = sd.open("/DATA_" + String(FileCounter) + ".DATA", (0X02 /*|< Open for reading and writing.*/ | 0x10 /*|< Create file if it does not exist.*/ | 0X04 /*|< Open at EOF.*/));
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
    static char rString[32 + 1];
    int possible_len = strlen(possible);
    for (int i = 0; i < 32; i++)
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
    lcd.print("/DATA_" + String(FileCounter) + ".DATA");

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
