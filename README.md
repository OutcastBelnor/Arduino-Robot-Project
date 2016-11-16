# Arduino-Robot-Project
This is a project for the Arduino Uno with custom Shield. It consist of two servo motors, two buttons, three LED diodes (green, red and yellow), infrared transmitter and receiver and three light dependent resistors.

The aim of the assigment was to program your Arduino to move from the origin of the grid to the specified location on the grid (for example (2, 2)), record all the junctions and obstructions along the way and at the destination point send the recorded data through the infrared to another Arduino.

My program will start by prompting the user for the coordinated of the destination point. After that the Arduino should be unplugged and placed with the LDRs above the black line and the right button should be pressed. The robot will conduct a calibration routine then, in which it will store the readings of the LDRs, increased by 40, which will be used to check if the LDRs are above the black line. After 3 seconds it will start the loop() function which will move the robot along the grid to the destination.

At the start of every call of the loop() function, Arduino will take readings of the LDRs and store them in the variables. They will be used to check the position of the robot. First it will be checked if the robot is on the crossing. If no then it will go along a line or try to find a line. Following a line is based on the middle LDR which is checked all the time if it is on the black line. If not then it will check other LDRs and turn accordingly. Otherwise it will turn either way to find the line.

If it detects that it is on the crossing, and this is not the destination crossing it will store the coordinates of this crossing. Then it turns right by 45 degrees and sends infrared waves which will be deflected if there is an obstacle. If the Arduino detects any infrared then it stores info about the obstacle. Then it turns left and does the same.

After storing the crossing data it decides which way to go according to the current position, increments the current coordinates and starts moving. When it reaches the destination it maps the crossing the last time and attempts to send data. For that a second Arduino with infrared equipment is needed. There is a separate program for that.

The transmission routine is quite complex and long. Both of the programs communications start after pressing the buttons. The main robot sends a 'ready' message while the other one is waiting and then it responds. Then the main robot again sends the number of junctions by sending the exact same number of signals. After that it sends all the other data: xpos, ypos, occupiedLeft(an obstacle on the left side) and occupiedRight(an obstacle on the right side) in the same manner. After that the program goes back to the loop() function, sets the boolean variable sent to true and then the Arduino stops completely.

There are two more programs: AssignmentIRLEDReadings.ino and AssignmentLDRreadings.ino which I have used to demonstrate some example data from infrared and the LDRs.
