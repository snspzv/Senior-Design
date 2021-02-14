const int Light = 4;

void setup() {
  pinMode(Light, OUTPUT);
}

void loop() {
  digitalWrite(Light, LOW);
  delay(1000);
  digitalWrite(Light, HIGH);
  delay(1000);
}
