
auto constexpr redPin = D7;
auto constexpr greenPin = D5;
auto constexpr bluePin = D0;


int delayMillis = 5000;

void setColor(int red, int green, int blue)
{

  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void setup() {
  // set the digital pin as output:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {

  setColor(255, 0, 0);  // red
  delay(delayMillis);
  setColor(0, 255, 0);  // green
  delay(delayMillis);
  setColor(0, 0, 255);  // blue
  delay(delayMillis);
  setColor(255, 255, 0);  // yellow
  delay(delayMillis);
  setColor(0, 0, 0);  // black
  delay(delayMillis);
  setColor(80, 0, 80);  // purple
  delay(delayMillis);
  setColor(0, 255, 255);  // aqua
  delay(delayMillis);

}
