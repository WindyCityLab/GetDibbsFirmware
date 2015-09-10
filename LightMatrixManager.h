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

typedef struct 
{
  RgbColor color;
  boolean allocated;
  uint8_t day;
  uint8_t hour;
} Reservation;

class LightMatrixManager
{
  public:
    LightMatrixManager();
    void begin(int pin);
    void clearDisplay();
    void update();
    
    int currentWeek, currentHour;

    void clearAllReservations();
    void allocateResource(uint8_t week, uint8_t day, uint8_t hour, RgbColor color);
    
    void displayCursor();
    void refreshDisplay();

    void incrementDay();
    void incrementHour();
    void incrementWeek();
    void decrementDay();
    void decrementHour();
    void decrementWeek();

    void setDay(int day);
    int getDay();
    
  private:
    int _currentDay;
    Reservation _reservations[NUM_COLUMNS * NUM_ROWS];
    NeoPixelBus _pixels = NeoPixelBus(NUMPIXELS,2);
    RgbColor _reservation[MAX_WEEKS][NUM_COLUMNS][NUM_ROWS];
    void setPixel(int day, int hour, RgbColor color);
};

#endif
