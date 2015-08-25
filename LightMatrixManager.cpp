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
        _reservation[week][day][hour] = -1;
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
  clearDisplay();

  //Draw a horizontal line matching at the current Hour
  for (int d = -1; d < NUM_COLUMNS - 1; d++)
  {
    setPixel(d, currentHour - INITIAL_HOUR, CURSOR_COLOR);
  }

  //Draw a vertical line matching the current day
  for (int h = -1; h < NUM_ROWS - 1; h++)
  {
    setPixel(_currentDay, h, CURSOR_COLOR);
  }
}

void LightMatrixManager::refreshDisplay()
{
  clearDisplay();
  displayCursor();
  for (int day = 0; day < NUM_COLUMNS; day++)
  {
    for (int hour = 0; hour < NUM_ROWS; hour++)
    {
      if (_reservation[currentWeek][day][hour] > -1)
      {
        int clientID = _reservation[currentWeek][day][hour];
        setPixel(day, hour, _clientColor[clientID]);
      }
    }
  }
  _pixels.Show();
}
void LightMatrixManager::setClientColor(uint8_t clientID, RgbColor color)
{
  _clientColor[clientID] = color;
}
void LightMatrixManager::allocateResource(uint8_t week, uint8_t day, uint8_t hour, uint8_t client)
{
  _reservation[week][day][hour] = client;
}
void LightMatrixManager::allocateResource(uint8_t client)
{
  _reservation[currentWeek][_currentDay][currentHour - INITIAL_HOUR] = client;
}
void LightMatrixManager::toggleAllocation(uint8_t client)
{
  if (_reservation[currentWeek][_currentDay][currentHour - INITIAL_HOUR] == -1)
  {
    _reservation[currentWeek][_currentDay][currentHour - INITIAL_HOUR] = client;
  }
  else
  {
    if (client == _reservation[currentWeek][_currentDay][currentHour - INITIAL_HOUR])
    {
      _reservation[currentWeek][_currentDay][currentHour - INITIAL_HOUR] = -1;
    }
  }
}
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

