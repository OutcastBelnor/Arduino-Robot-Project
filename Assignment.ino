/*
*
*  Assignment:
*
*  Author:
*  Jakub Janas
*
*  Date:
*  02/12/2015
*
*/

//#define DEBUG

#include <Servo.h> // Just a library and the working names for different pins
#define rightLDRPin A0
#define middleLDRPin A1
#define leftLDRPin A2
#define irSensorPin 2
#define irLEDPin 3
#define rightButtonPin 2
#define rightServoPin 5
#define leftServoPin 6
#define redLEDPin 13
#define yellowLEDPin 12
#define greenLEDPin 7

int rightLDRValue; // Variables to hold LDR values
int middleLDRValue;
int leftLDRValue;
int black[3]; // Right, Middle and Left LDRs top black values
int xcur=0, ycur=0; // Robot's current coordinates
int xdest, ydest; // Coordinates of robot's destination
struct GridJunctions // A structure for the junctions in the grid
{
  int xpos; // Position of the junction
  int ypos;
  boolean occupiedLeft; // An obstacle on the left
  boolean occupiedRight; // An obstacle on the right
};
GridJunctions grid[10]; // An array for mapping the junctions
int junction = 0; // A variable that counts the junctions
Servo leftServo;
Servo rightServo;

int input () // A function to take integer input
{
  while (!Serial.available())
  {

    // wait for the user to enter a value
    ;

  }
  return Serial.parseInt();
}

void calibration () // Calibrating function
{
  while (digitalRead (rightButtonPin)) // Calibrating on the button press
  {

    // wait for the user to press the button
    ;
    
  }

  int x = analogRead (rightLDRPin);
  black[0] = x + 40; // Calculating the top value for black
  
  x = analogRead (middleLDRPin);
  black[1] = x + 40;
  
  x = analogRead (leftLDRPin);
  black[2] = x + 40;

  Serial.println("Calibration completed.");
}

void mappingTheGrid () // The function that maps the current junction
{
  grid[junction].xpos = xcur; // Setting the position of the junction
  grid[junction].ypos = ycur;
  
  turnRight(); // Checking if the obstacle is on the right side
  delay(700);
  fullStop();
  tone (irLEDPin, 38000);
  int curTime = millis(); // Helper variable for measuring time
  while (millis() - curTime <= 300)
  {
    if (digitalRead (irSensorPin) == LOW)
    {
      grid[junction].occupiedRight = true;
    }
    else
    {
      grid[junction].occupiedRight = false;
    }
  } 
  delay(100);
  noTone (irLEDPin);
  turnLeft(); // aligning to previous position
  delay(500);
  
  turnLeft(); // Checking if the obstacle is on the right side
  delay(500);
  fullStop();
  tone (irLEDPin, 38000);
  curTime = millis();
  while (millis() - curTime <= 300)
  {
    if (digitalRead (irSensorPin) == LOW)
    {
      grid[junction].occupiedLeft = true;
    }
    else
    {
      grid[junction].occupiedLeft = false;
    }
  }
  delay(100);
  noTone (irLEDPin);
  turnRight(); // aligning to previous position
  delay(700);

  junction++;
  moveStraight();
  delay(200);
}

void sendingData () // The function that sends junction data
{
  while (digitalRead (rightButtonPin))
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

  while (digitalRead(irSensorPin) == HIGH) // Waiting for the second
  {                                        // arduino to respond
    ;
  }
  delay(100);
  
  for (int i = 0; i < 10; i++) // Sending number of junctions
  {
    if (i < junction)
    {
      timePass = millis();
      while (millis() - timePass != 100)
      {
        tone(irLEDPin, 38000);
      }
      noTone(irLEDPin);
    }
    else
    {
      delay(100);
    }
  }

  delay(100);

  for (int i = 0; i < junction; i++) // Sending data about junctions
  {
    for (int j = 0; j < 5; j++) // Sending xpos of the current junction
    {
      if (j < grid[i].xpos)
      {
        timePass = millis();
        while (millis() - timePass != 100)
        {
          tone(irLEDPin, 38000);
        }
        noTone(irLEDPin);
      }
    }

    delay(100);
    
    for (int j = 0; j < 5; j++) // Sending ypos of the current junction
    {
      if (j < grid[i].ypos)
      {
        timePass = millis();
        while (millis() - timePass != 100)
        {
          tone(irLEDPin, 38000);
        }
        noTone(irLEDPin);
      }
    }

    delay(100);

    if (grid[i].occupiedLeft == 1) // Sending if there's an obstacle on the left
    {
      timePass = millis();
      while (millis() - timePass != 100)
      {
        tone(irLEDPin, 38000);
      }
      noTone(irLEDPin);
    }
    else
    {
      delay(100);
    }

    delay(100);

    if (grid[i].occupiedRight == 1) // Sending if there's an obstacle on the right
    {
      timePass = millis();
      while (millis() - timePass != 100)
      {
        tone(irLEDPin, 38000);
      }
      noTone(irLEDPin);
    }
    else
    {
      delay(100);
    }

    delay(100);
  }
}

void moveStraight () // Movement functions
{
  digitalWrite(redLEDPin, LOW);
  digitalWrite(yellowLEDPin, LOW);
  digitalWrite(greenLEDPin, HIGH);
  
  leftServo.write (123);
  rightServo.write (45);
}

void turnRight ()
{
  digitalWrite(redLEDPin, LOW);
  digitalWrite(yellowLEDPin, HIGH);
  digitalWrite(greenLEDPin, LOW);
  
  leftServo.write(175);
  rightServo.write(177);
}

void turnLeft ()
{
  digitalWrite(redLEDPin, HIGH);
  digitalWrite(yellowLEDPin, LOW);
  digitalWrite(greenLEDPin, LOW);
  
  leftServo.write(11);
  rightServo.write(13);
}

void fullStop ()
{
  digitalWrite(redLEDPin, LOW);
  digitalWrite(yellowLEDPin, LOW);
  digitalWrite(greenLEDPin, LOW);
  
  leftServo.write(83);
  rightServo.write(85);
}

void setup() 
{
  Serial.begin(9600); // Setting up a serial port

  leftServo.attach (leftServoPin); // Setting up Servos
  rightServo.attach (rightServoPin);
  fullStop();

  pinMode (rightLDRPin, INPUT); // Setting up LDRs, 
  pinMode (middleLDRPin, INPUT);
  pinMode (leftLDRPin, INPUT);
  pinMode (rightButtonPin, INPUT); // Button,
  pinMode (redLEDPin, OUTPUT); // LEDs,
  pinMode (yellowLEDPin, OUTPUT);
  pinMode (greenLEDPin, OUTPUT);
  pinMode (irLEDPin, OUTPUT); // infrared LED
  pinMode (irSensorPin, INPUT); // and infrared sensor
  
  #ifdef DEBUG
    xdest = 2; // Working destination x value
  #else
  Serial.println("Enter x destination:"); // Setting up destination point
  xdest = input(); // Get x value
  #endif
  #ifdef DEBUG
    ydest = 2; // Working destination y value
  #else
  Serial.println("Enter y destination:");
  ydest = input(); // Get y value
  #endif

  calibration(); // Calibration call
  delay (3000);
}

void loop() 
{
  rightLDRValue = analogRead(rightLDRPin);  // Setting up the LDRs values
  middleLDRValue = analogRead(middleLDRPin);
  leftLDRValue = analogRead(leftLDRPin);

  if (middleLDRValue < black[1]
      && rightLDRValue < black[0]
      && leftLDRValue < black[2])  // Detects if on crossing
  {
    Serial.println("Crossing");
    moveStraight(); // Aligning on the crossing
    delay(200);
    fullStop();
    delay(1000);
    boolean sent = false; // A variable which tells if the data was sent
    
    if (xcur != xdest || ycur != ydest) // Checks if it's still away from
    {                                   // the destination
      mappingTheGrid(); // Mapping the current junction
      delay(1000);
      
      if (ycur < ydest) // If ycur don't have the same value as ydest
      {                 // it still has to move up
        ycur++;
        moveStraight(); // Move out of the junction
        delay (500);
      }
      else if (xcur < xdest) // Else 
      {
        if (xcur == 0)  // Turn if xcur is 0
        {
          turnRight(); // Turn about 90 degrees
          delay (2200);
        }
        xcur++;
        moveStraight(); // Move out of the junction
        delay (500);
      }
    }
    else if (sent == false)
    {
      mappingTheGrid(); // Mapping the last junction
      delay(5000);
      
      sendingData(); // A call to send the data about junctions
      sent = true; // The data was sent
    }
  }
  else    // Following a line
  {
    if (middleLDRValue < black[1])  // If the middle LDR is on black move
    {                               // straight
      moveStraight();
      Serial.println ("Straight");
    }
    else if (rightLDRValue < black[0]) // Else if the right LDR is on
    {                                  // black turn right
      turnRight();
      Serial.println ("Right turn");
    }
    else if (leftLDRValue < black[2]) // Else if the left LDR is on
    {                                 // black turn left
      turnLeft();
      Serial.println ("Left turn");
    }
  }
}
