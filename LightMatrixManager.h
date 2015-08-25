#ifndef LIGHTMATRIX
#define LIGHTMATRIX

#include "../NeoPixelBus/NeoPixelBus.h"

#define NUM_COLUMNS 8
#define NUM_ROWS 16
#define MAX_CLIENTS 255
#define MAX_WEEKS 8

#define INITIAL_HOUR 7

#define NUMPIXELS NUM_COLUMNS * NUM_ROWS

#define CURSOR_COLOR RgbColor(50,50,50)

class LightMatrixManager
{
  public:
    LightMatrixManager();
    void begin(int pin);
    void clearDisplay();

    int currentWeek, currentHour;
    void setClientColor(uint8_t clientID, RgbColor color);
    
    void allocateResource(uint8_t week, uint8_t day, uint8_t hour, uint8_t client);
    void allocateResource(uint8_t client);
    void toggleAllocation(uint8_t client);
    
    void displayCursor();
    void refreshDisplay();

    void incrementDay();
    void incrementHour();
    void incrementWeek();
    void decrementDay();
    void decrementHour();
    void decrementWeek();

    void setDay(int day);
    
  private:
    int _currentDay;
    NeoPixelBus _pixels = NeoPixelBus(NUMPIXELS,2);
    int _reservation[MAX_WEEKS][NUM_COLUMNS][NUM_ROWS]; // Client number goes in the cell, color is looked up from there
    RgbColor _clientColor[MAX_CLIENTS]; // Client colors
    void setPixel(int day, int hour, RgbColor color);
};

#endif
