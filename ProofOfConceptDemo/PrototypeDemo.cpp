#include <Stepper.h>

#define STEPS 32


// create an instance of the stepper class using the steps and pins

Stepper stepper(STEPS, 8, 10, 9, 11);


int Pval = 0;

int potVal = 0;

int direction[] = {1, -1};
int dindex = 0;

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
  
  // Print "Analog value:" in serial monitor
  Serial.print("Analog value: ");
  Serial.print(level0);
  Serial.print("\tAnalog value: ");
  Serial.println(level1);
  
  if(level0 > 50 || level1 >50){
    dindex = 1;
  }
  else if (level0 <= 50 && level1 <=50) {
    dindex = 0;
  }

  stepper.step(direction[dindex]*15);


  
  // Turn off all the led initially


  
    // Splitting 1023 into 5 level => 200, 400, 600, 800, 1023
    // Based on the ADC output, LED indicates the level (1 to 5)
  
  
  
}