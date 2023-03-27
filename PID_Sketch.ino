#define relay1 P1_2
#define relay2 P1_4
#define Isen P6_0
#define fet P2_5
#define LED P1_0

double Current;
bool state;
unsigned long previousMillis;

void setup() {
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(Isen, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void open() {
  digitalWrite(LED, HIGH);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, LOW);
  state = true;
}

void close() {
  digitalWrite(LED, LOW);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, HIGH);
  state = false;
}

void blink(int interval) {
  unsigned long currentMillis = millis();
  unsigned long dt = currentMillis - previousMillis;

  if (dt > interval) {
    if (state == true) {
      open();
      previousMillis = currentMillis;
    }
    else {
      close();
      previousMillis = currentMillis;
    }
  }
}

int calculatePID(int Kp, int Ki, int Kd, int input, int SetPoint) {
  int error = SetPoint - input;
  static int integral = 0;
  static int previousError = 0;
  int derivative = error - previousError;
  integral += error;
  previousError = error;
  return constrain((Kp * error) + (Ki * integral) + (Kd * derivative), 0, 255);
}

void loop() {
  double rawInput = analogRead(Isen);
  double rawVoltage = (rawInput*3.3)/4096;
  double Current = 1000*(rawVoltage-0.36)/0.4;
  
  int output = calculatePID(1, 0, 0, Current, 300);
  analogWrite(fet, output);
  Serial.println(Current);
  blink(1000);
}
