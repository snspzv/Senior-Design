#ifndef light_h
#define light_h

//Important this pin is OC1A or radioTimer function to blink light will break
const int RELAY = 9;

void lightInit()
{
  pinMode(RELAY, OUTPUT); 
  digitalWrite(RELAY, LOW); 
}

void lightOnSolid() 
{
  digitalWrite(RELAY, HIGH); 
}

void lightOff() 
{
  digitalWrite(RELAY, LOW); 
}

#endif /* light_h */
