#include <esp8266_peri.h>


int step;

void setup() {
  // put your setup code here, to run once:
  pinMode(D8, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D5, OUTPUT);

  pinMode(A0, INPUT);

  Serial.begin(115200);  
  step = 0;
}
#define N_STEPS (8)
const uint32_t steps[N_STEPS] = {
 (1 << 5 | 0 << 6 | 0 << 7 | 0 << 8),
 (1 << 5 | 1 << 6 | 0 << 7 | 0 << 8),
 (0 << 5 | 1 << 6 | 0 << 7 | 0 << 8),
 (0 << 5 | 1 << 6 | 1 << 7 | 0 << 8),
 (0 << 5 | 0 << 6 | 1 << 7 | 0 << 8),
 (0 << 5 | 0 << 6 | 1 << 7 | 1 << 8),
 (0 << 5 | 0 << 6 | 0 << 7 | 1 << 8),
 (1 << 5 | 0 << 6 | 0 << 7 | 1 << 8)};

#define STEP_MASK (uint32_t)(~(1 << 5 | 1 << 6 | 1 << 7 | 1 << 8))


void loop() {

  uint32_t gpo_reg = GPO;   
  gpo_reg &= STEP_MASK; 
  gpo_reg |= steps[step];
  Serial.print("Modified ");  
  Serial.print(gpo_reg, HEX);
  Serial.print("\n");
  
  
  GPO = gpo_reg;
  gpo_reg = GPO;
  Serial.print("Written ");
  Serial.print(gpo_reg, HEX);
  Serial.print("\n\n");
  
  step++;
  if (step >= N_STEPS)
    step = 0;
    
  delay(1000);

  int raw = analogRead(A0);
  int volt=raw/1023.0;
 // Serial.print("AO reads ");
 // Serial.print(raw, DEC);

}
