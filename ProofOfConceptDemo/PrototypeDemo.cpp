#include <Stepper.h>

#define STEPS 32


// create an instance of the stepper class using the steps and pins

Stepper stepper(STEPS, 8, 10, 9, 11);


int Pval = 0;

int potVal = 0;

int direction[] = {1, -1};
int dindex = 0;


int state = 1;
int prevState = 0;
bool StateInit = state != prevState;

bool cmdGrip = true;
int numSteps;
int timeCounter = 0;

void setup() {
  // Set the serial monitor baudrate to 9600
  Serial.begin(9600);
  stepper.setSpeed(200);

  
}

void loop() {
  
  // Variable to store ADC value ( 0 to 1023 )
  int level0;
  int level1;
  // analogRead function returns the integer 10 bit integer (0 to 1023)
  level0 = analogRead(0);
  level1 = analogRead(1);

  switch(state){
    1: // Wait for grip command
      if(StateInit) {
        numSteps = 0;
        break;
      }
      if(cmdGrip) {
        state = 2;
        numSteps = 15;
        dindex = 1;
        cmdGrip = false; //temp for code test
        break;
      }

    2: // Check sensors to ensure object is gripped
      if(level0 > 50 || level1 >50){
        // Print "Analog value:" in serial monitor
        Serial.print("Analog value: ");
        Serial.print(level0);
        Serial.print("\tAnalog value: ");
        Serial.println(level1);
        numSteps = 0;
        break;
      }

    3: // Wait a few seconds
      if (StateInit) {
        timeCounter = 0;
        break;
      }
      timeCounter += 1;
      if timeCounter >= 1000 {
        state = 4;
        break;
      }

    4: // Release object state
      numSteps = 15;
      dindex = -1;
      timeCounter += 1;
      if timeCounter >= 1000 {
        state = 5;
        break;
      }

    5:
      if (StateInit) {
        timeCounter = 0;
        break;
      }
      timeCounter += 1;
      if timeCounter >= 1000 {
        state = 1;
        cmdGrip = true;
        break;
      }

  }
  
  StateInit = state != prevState;
  prevState = state;

  stepper.step(direction[dindex]*numSteps);
  
  // Turn off all the led initially


  
    // Splitting 1023 into 5 level => 200, 400, 600, 800, 1023
    // Based on the ADC output, LED indicates the level (1 to 5)
  
  
  
}