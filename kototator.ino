int CLEAN = 2;
int DIRTY = 3;
int MOVE = 5;
int DIST_TRIGGER = 6;
int DIST_ECHO = 7;

const int INTERVAL = 100;

String state = "clean";
bool dirty = false;

void setup()
{
  Serial.begin(9600);

  pinMode(CLEAN, OUTPUT);
  pinMode(DIRTY, OUTPUT);
  
  pinMode(DIST_TRIGGER, OUTPUT);
  pinMode(DIST_ECHO, INPUT);

  pinMode(MOVE, INPUT);
  digitalWrite(MOVE, LOW);

  Serial.println("Kotator");
}

void loop()
{
  Serial.print("========= ");
  Serial.println(millis());
  
  bool prevDirty = dirty;
  dirty = isNowDirty();
  bool clean = isClean();

  if (!prevDirty && dirty) {
    state = "dirty";
  }
  if (clean) {
    state = "clean";
  }

  indicate();
  delay(INTERVAL);
}

bool isNowDirty()
{
  return digitalRead(MOVE) == HIGH;
}

float isClean()
{
  float CM;
  digitalWrite(DIST_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(DIST_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(DIST_TRIGGER, LOW);
  digitalWrite(DIST_ECHO, HIGH);
  CM = pulseIn(DIST_ECHO, HIGH) / 58;
  // Serial.print("Distance: ");
  // Serial.println(CM);
  return CM < 10;
}

void indicate()
{
  Serial.println(state);
  digitalWrite(CLEAN, state == "clean" ? HIGH : LOW);
  digitalWrite(DIRTY, dirty || state == "dirty" ? HIGH : LOW);
}

