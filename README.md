# ExFatWrite

This repo contains a code for creating ExFat formatted files on SD card with Arduino.

## Code Explanation

This code is a simple example of how to use the SdFat library to write data to an ExFat formatted SD card. The code uses a LiquidCrystal_I2C library to control an LCD screen with I2C interface. The code also uses the EEPROM library to store and retrieve the file counter value. The code generates a random string using the possible characters array and the randomSeed function. The code writes the string to the file using the print and println methods. The code also displays the file size and the number of lines written on the LCD screen using the size and setCursor methods. The code closes the file after each write operation using the close method.

## File Name Format

The file name uses a format that starts with DATA_ and ends with .DATA and has a number in between. The file number is stored in EEPROM and incremented when FileCounterLockPin is low.

## Code Location

The final version of the code is located in the Build folder as ExFatWrite.ino.hex.
