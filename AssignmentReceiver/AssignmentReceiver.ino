/*
*
*  Assignment:
*  IR Receiver
*
*  Author:
*  Jakub Janas
*
*  Date:
*  04/12/2015
*
*/

#define irSensorPin 2
#define irLEDPin 3
#define leftButtonPin 4

struct GridJunctions // A structure for the junctions in the grid
{
  int xpos; // Position of the junction
  int ypos;
  boolean occupiedLeft; // An obstacle on the left
  boolean occupiedRight; // An obstacle on the right
};
GridJunctions grid[10]; // An array for the received data about junctions
int junction = 0; // Number of received junction data

void setup()
{
  Serial.begin (9600);

  pinMode (irLEDPin, OUTPUT);
  pinMode (irSensorPin, INPUT);
  pinMode (leftButtonPin, INPUT);
  
  while (digitalRead(irSensorPin) == HIGH) // Waiting for the "start"
  {                                        // message
    ;
  }
  delay(100);

  while (digitalRead (leftButtonPin))
  {
    // wait for the user to press the button
    ;
  }

  int timePass = millis();
  while (millis() - timePass != 100) // Sending "ready" message
  {
    tone(irLEDPin, 38000);
  }
  noTone(irLEDPin);

  for (int i = 0; i < 10; i++) // Receiving number of junctions
  {
    delay(25);
    timePass = millis();
    while (millis() - timePass != 50)
    {
      if (digitalRead(irSensorPin) == LOW)
      {
        junction++;
        delay(50 - (millis() - timePass));
        break;
      }
    }
    delay(25);
  }

  delay(100);

  for (int i = 0; i < junction; i++) // Receiving data about junctions
  {
    grid[i].xpos = 0; // Setting the values of the junction position to zero
    grid[i].ypos = 0;
    grid[i].occupiedLeft = false;
    grid[i].occupiedRight = false;
    
    for (int j = 0; j < 5; j++) // Receiving xpos of current junction
    {
      delay(25);
      timePass = millis();
      while (millis() - timePass != 50)
      {
        if (digitalRead(irSensorPin) == LOW)
        {
          grid[i].xpos++;
          delay(50 - (millis() - timePass));
          break;
        }
      }
      delay(25);
    }

    delay(100);
    
    for (int j = 0; j < 5; j++) // Receiving ypos of the current junction
    {
      delay(25);
      timePass = millis();
      while (millis() - timePass != 50)
      {
        if (digitalRead(irSensorPin) == LOW)
        {
          grid[i].ypos++;
          delay(50 - (millis() - timePass));
          break;
        }
      }
      delay(25);
    }

    delay(125);
    
    timePass = millis();
    while (millis() - timePass != 50) // Receiving comfirmation of the obstacle on the left
    {
      if (digitalRead(irSensorPin) == LOW)
      {
        grid[i].occupiedLeft = true;
      }
    }

    delay(150);

    timePass = millis();
    while (millis() - timePass != 50) // Receiving comfirmation of the obstacle on the right
    {
      if (digitalRead(irSensorPin) == LOW)
      {
        grid[i].occupiedRight = true;
      }
    }
    
    delay(125);
  }

  for (int i = 0; i < junction; i++) // Printing out the data in the monitor.
  {
    Serial.print("Junction ");
    Serial.println(i);
    Serial.print("xpos: ");
    Serial.print(grid[i].xpos);
    Serial.print(" ypos: ");
    Serial.println(grid[i].ypos);
    Serial.print("occupiedLeft: ");
    Serial.println(grid[i].occupiedLeft);
    Serial.print("occupiedRight: ");
    Serial.println(grid[i].occupiedRight);
    Serial.println("");
  }

}

void loop()
{
  ;
}
