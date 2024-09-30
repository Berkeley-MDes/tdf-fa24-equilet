const pin_t led = D7;
char str[12] = "Hello World";

int counter = 0;

// setup() runs once, when the device is first turned on.
void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW); 
  delay(1000);
  Serial.printf("the character: %c\n", str[counter]);
  counter++;
  if (counter > 11) {
    counter = 0;
  }
}
