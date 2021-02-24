#ifndef light_h
#define light_h

// Change control pin by changing value of RELAY
const int RELAY = 9;

// Setup funtion for relay control
void lightInit()
{
  pinMode(RELAY, OUTPUT); // Set pin mode
  digitalWrite(RELAY, HIGH); // making sure that it is off
}

void lightOnSolid() // Turns relay on
{
  digitalWrite(RELAY, HIGH); // Push 5 V to Pin 4
}

void lightOnBlinking() // Blink relay
{
  // Light blinks for one second
  digitalWrite(RELAY, HIGH);
  delay(1000);
  digitalWrite(RELAY, LOW);
  delay(1000);
}

void lightOff() // Turns relay off
{
  digitalWrite(RELAY, LOW); // Push 0 V to Pin 4
}

#endif /* light_h */
