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
    if(delayUs <= 340)
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
            Serial.println("Non volative memory read and write");
            
            dirty = false;
            nMicroSeconds = 0;
            nSteps = 0;
            nRevs = 0;
            nSecondsUsed = 0;
        }
    }
    else if (delayUs > 350)
    {
        dirty = true;
        nMicroSeconds += (delayUs * 2);
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



// Defines pins numbers
const int stepPin = D4; //D3
const int dirPin = D3;  //D4

OSStatitics* osStatitics_p;

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);

  digitalWrite(dirPin,HIGH); //Enables the motor to move in a particular direction

  Serial.begin(115200);
  osStatitics_p = new OSStatitics();
}


void loop() {

  int customDelayMapped = speedUp(); // Gets custom delay values from the custom speedUp function
  // Makes pules with custom delay, depending on the Potentiometer, from which the speed of the motor depends
  digitalWrite(stepPin, HIGH);
  delay(customDelayMapped);
  digitalWrite(stepPin, LOW);
  delay(customDelayMapped);

  osStatitics_p->updateStep(customDelayMapped);
  Serial.print("customDelayMapped ");
  Serial.print(customDelayMapped, DEC);
  Serial.println("");

}

// Function for reading the Potentiometer
int speedUp() {

  int customDelay = analogRead(A0); // Reads the potentiometer
  int newCustom = map(customDelay, 0, 1023, 300,4000); // Convrests the read values of the potentiometer from 0 to 1023 into desireded delay values (300 to 4000)

  return newCustom;

}
