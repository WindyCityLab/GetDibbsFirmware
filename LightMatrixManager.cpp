#include "LightMatrixManager.h"
#include "../NeoPixelBus/NeoPixelBus.h"

LightMatrixManager::LightMatrixManager()
{
  for (uint8_t week = 0; week < MAX_WEEKS; week++)
  {
    for (uint8_t day = 0; day < NUM_COLUMNS; day++)
    {
      for (uint8_t hour = 0; hour < NUM_ROWS; hour++)
      {
        _reservation[week][day][hour] = RgbColor(0,0,0);
      }
    }
  }
}

void LightMatrixManager::setDay(int day)
{
  if (day == 1)
  {
    _currentDay = 6;
  }
  else
  {
    _currentDay = day - 2;
  }
}
void LightMatrixManager::clearDisplay()
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    _pixels.SetPixelColor(i, RgbColor(0, 0, 0));
  }
}
void LightMatrixManager::begin(int pin)
{
  _pixels.Begin();
  clearDisplay();
  _pixels.Show();
}

void LightMatrixManager::update()
{
  _pixels.Show();
}

void LightMatrixManager::setPixel(int day, int hour, RgbColor color)
{
  uint8_t column = day + 1;
  uint8_t row = hour + 1;
  uint8_t pixel = row * NUM_COLUMNS + column;
  _pixels.SetPixelColor(pixel, color);
}

void LightMatrixManager::displayCursor()
{
  //Draw a horizontal line matching at the current Hour
  setPixel(-1, currentHour - INITIAL_HOUR, CURSOR_COLOR);
  setPixel(_currentDay - 1, currentHour - INITIAL_HOUR, CURSOR_COLOR);
  setPixel(_currentDay + 1, currentHour - INITIAL_HOUR, CURSOR_COLOR);

  setPixel(_currentDay, -1, CURSOR_COLOR);
  setPixel(_currentDay, currentHour - INITIAL_HOUR - 1, CURSOR_COLOR);
  setPixel(_currentDay, currentHour - INITIAL_HOUR + 1, CURSOR_COLOR);
}

void LightMatrixManager::refreshDisplay()
{
  clearDisplay();
  for (int day = 0; day < NUM_COLUMNS; day++)
  {
    for (int hour = 0; hour < NUM_ROWS; hour++)
    {
      setPixel(day, hour, _reservation[currentWeek][day][hour]);
    }
  }
  displayCursor();
  _pixels.Show();
}
void LightMatrixManager::allocateResource(uint8_t week, uint8_t day, uint8_t hour, RgbColor color)
{
  for (int i=0; i<NUM_ROWS * NUM_COLUMNS; i++)
  {
    if (!_reservations[i].allocated)
    {
      _reservations[i].allocated = true;
      _reservations[i].color = color;
      _reservations[i].day = day;
      _reservations[i].hour = hour;
      break;
    }
  }
}
void LightMatrixManager::clearAllReservations()
{
  for (int i=0; i<NUM_ROWS * NUM_COLUMNS; i++)
  {
    _reservations[i].allocated = false;
  }
}
//void LightMatrixManager::allocateResource(RgbColor color)
//{
//  _reservation[currentWeek][_currentDay][currentHour - INITIAL_HOUR] = color;
//}
//void LightMatrixManager::toggleAllocation(int color)
//{
//  if (_reservation[currentWeek][_currentDay][currentHour - INITIAL_HOUR] == -1)
//  {
//    _reservation[currentWeek][_currentDay][currentHour - INITIAL_HOUR] = color;
//  }
//  else
//  {
//    if (color == _reservation[currentWeek][_currentDay][currentHour - INITIAL_HOUR])
//    {
//      _reservation[currentWeek][_currentDay][currentHour - INITIAL_HOUR] = -1;
//    }
//  }
//}
void LightMatrixManager::incrementDay()
{
  _currentDay++;
  if (_currentDay > 6) // we start counting weeddays at 0
  {
    incrementWeek();
    _currentDay = 0;
  }
}
void LightMatrixManager::incrementHour()
{
  currentHour++;
  if (currentHour > INITIAL_HOUR + NUM_ROWS - 1)
  {
    currentHour = INITIAL_HOUR;
    incrementDay();
  }
}
void LightMatrixManager::incrementWeek()
{
  currentWeek = (currentWeek + 1) % MAX_WEEKS;
}

void LightMatrixManager::decrementDay()
{
  _currentDay--;
  if (_currentDay < 0)
  {
    _currentDay = 6;
    decrementWeek();
  }
}
void LightMatrixManager::decrementHour()
{
  currentHour--;
  if (currentHour < INITIAL_HOUR)
  {
    currentHour = INITIAL_HOUR + NUM_ROWS - 1;
    decrementDay();
  }
}
void LightMatrixManager::decrementWeek()
{
  currentWeek--;
  if (currentWeek < 0)
  {
    currentWeek = MAX_WEEKS - 1;
  }
}
int LightMatrixManager::getDay()
{
  if (_currentDay == 6)
  {
    return 1;
  }
  return _currentDay + 2;
}

