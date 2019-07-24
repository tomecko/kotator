int CLEAN = 2;
int DIRTY = 3;
int MOVE = 5;
int DIST_TRIGGER = 6;
int DIST_ECHO = 7;

const int INTERVAL = 100;
const int DIST_TIMEOUT = 8000;

String state = "dirty";
unsigned long cleanTime;
unsigned long dirtyTime;
bool recentlyDirty = false;

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
  bool nowDirty = isNowDirty();
  if (nowDirty && !recentlyDirty) {
    Serial.println("Marking as DIRTY");
    recentlyDirty = true;
    dirtyTime = millis();
  }
  if (nowDirty && recentlyDirty && dirtyTime + DIST_TIMEOUT < millis()) {
    Serial.println("Long DIRTY, updating dirtyTime");
    dirtyTime = millis() - DIST_TIMEOUT;
  }
  if (!nowDirty) {
    recentlyDirty = false;
    Serial.println("No movement in last 8s");
  }
  if (isClean()) {
    Serial.println("Marked as CLEAN");
    cleanTime = millis();
  }
  if (nowDirty && cleanTime > dirtyTime) {
    Serial.println("Dirty now but marked as CLEAN");
  }
  
  Serial.print("cleanTime: ");
  Serial.print(cleanTime);
  Serial.print(" , dirtyTime: ");
  Serial.println(dirtyTime);
  
  state = cleanTime > dirtyTime ? "clean" : "dirty";
  
  Serial.print("state: ");
  Serial.println(state);

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
  Serial.print("Distance: ");
  Serial.println(CM);
  return CM < 10;
}

void indicate()
{
  digitalWrite(CLEAN, state == "clean" ? HIGH : LOW);
  digitalWrite(DIRTY, (state == "dirty" || recentlyDirty) ? HIGH : LOW);
}

