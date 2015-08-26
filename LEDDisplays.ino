void displayHourNumber(int hour)
{
  if (hour < 10)
  {
    hourDisplay.writeDigitAscii(0, ' ');
    hourDisplay.writeDigitAscii(1, hour + 48);
  }
}
void displayHour(int hour)
{
  hourDisplay.writeDigitAscii(3, 'm');
  if (hour >= 12)
  {
    hourDisplay.writeDigitAscii(2, 'p');
  }
  else
  {
    hourDisplay.writeDigitAscii(2, 'a');
  }
  hourDisplay.writeDisplay();
}
void displayDay(int day)
{
  switch (day)
  {
    case 1 :
      {
        dayDisplay.writeDigitAscii(0, 'S');
        dayDisplay.writeDigitAscii(1, 'u');
        dayDisplay.writeDigitAscii(2, 'n');
        dayDisplay.writeDigitAscii(3, ' ');
        break;
      }
    case 2 :
      {
        dayDisplay.writeDigitAscii(0, 'M');
        dayDisplay.writeDigitAscii(1, 'o');
        dayDisplay.writeDigitAscii(2, 'n');
        dayDisplay.writeDigitAscii(3, ' ');
        break;
      }
    case 3 :
      {
        dayDisplay.writeDigitAscii(0, 'T');
        dayDisplay.writeDigitAscii(1, 'u');
        dayDisplay.writeDigitAscii(2, 'e');
        dayDisplay.writeDigitAscii(3, 's');
        break;
      }
    case 4 :
      {
        dayDisplay.writeDigitAscii(0, 'W');
        dayDisplay.writeDigitAscii(1, 'e');
        dayDisplay.writeDigitAscii(2, 'd');
        dayDisplay.writeDigitAscii(3, 's');
        break;
      }
    case 5 :
      {
        dayDisplay.writeDigitAscii(0, 'T');
        dayDisplay.writeDigitAscii(1, 'h');
        dayDisplay.writeDigitAscii(2, 'u');
        dayDisplay.writeDigitAscii(3, 'r');
        break;
      }
    case 6 :
      {
        dayDisplay.writeDigitAscii(0, 'F');
        dayDisplay.writeDigitAscii(1, 'r');
        dayDisplay.writeDigitAscii(2, 'i');
        dayDisplay.writeDigitAscii(3, ' ');
        break;
      }
    case 7 :
      {
        dayDisplay.writeDigitAscii(0, 'S');
        dayDisplay.writeDigitAscii(1, 'a');
        dayDisplay.writeDigitAscii(2, 't');
        dayDisplay.writeDigitAscii(3, ' ');
        break;
      }

  }
  dayDisplay.writeDisplay();
}
