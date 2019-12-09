/*
  ReadAnalogVoltage
  The ESP8266EX(Which IC D1 board used)  integrates a generic purpose 10-bit analog ADC. The ADC range is from 0V to 1.0V
  And on the  board, we use following circuit:
  -----A0
  |
 220K
  |--- ADC
 100K
  |
 GND
so,the input voltage(A0) can be 3.2V, and the A0=3.2*ADC
 */



// Defines pins numbers
const int stepPin = 1; //D3
const int dirPin = 2;  //D4

int customDelay,customDelayMapped; // Defines variables

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);

  digitalWrite(dirPin,HIGH); //Enables the motor to move in a particular direction

  Serial.begin(115200);
}


void loop() {

  customDelayMapped = speedUp(); // Gets custom delay values from the custom speedUp function
  // Makes pules with custom delay, depending on the Potentiometer, from which the speed of the motor depends
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(customDelayMapped);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(customDelayMapped);
}

// Function for reading the Potentiometer
int speedUp() {

  int customDelay = analogRead(A0); // Reads the potentiometer
  int newCustom = map(customDelay, 0, 1023, 300,4000); // Convrests the read values of the potentiometer from 0 to 1023 into desireded delay values (300 to 4000)

  return newCustom;

}


class OSStatitics { 
    public:
       OSStatitics(); 
       ~OSStatitics();
       void updateStep(int delayUs);

    private:
      
      unsigned long nSecondsUsed;
      unsigned long nMicroSeconds;

      unsigned long nRevs;
      unsigned int nSteps;

      bool dirty;
};

OSStatitics::OSStatitics()
{
    dirty = false;

    nSecondsUsed = 0;
    nMicroSeconds = 0;

    nRevs = 0;
    nSteps = 0;
}

void OSStatitics::updateStep(int delayUs)
{
    if(delayUs == 0)
    {
        if (dirty)
        {

            //Read flash, update, write back
#if 0
            EEPROM.read(address);

            Update ....
            Serial.println("Non volative memory read and write");

            EEPROM.write(address, value);
            EEPROM.commit();
#endif             
            dirty = false;
        }
    }
    else 
    {
        dirty = true;
        nMicroSeconds++;
        if (nMicroSeconds >= 1000000)
        {
            nSecondsUsed++;
            nMicroSeconds = 0;
        }
        nSteps++;
        if (nSteps >= 200)
        {
          nRevs++;
          nSteps = 0;
        }
    }
}
