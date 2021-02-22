#ifndef light_h
#define light_h

void lightOnSolid()
{
  digitalWrite(4, HIGH);
}

void lightOnBlinking()
{
    digitalWrite(4, HIGH);
    delay(1000);
    digitalWrite(4, LOW);
}

void lightOff()
{
  digitalWrite(4, LOW);
}

void lightInit()
{
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
}

#endif /* light_h */
