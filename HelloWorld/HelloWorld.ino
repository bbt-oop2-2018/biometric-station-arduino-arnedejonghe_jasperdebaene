void setup() {
Serial.begin(115200);
Serial.println("Hello World!");

}

void loop() {
double level = random(21);
int pubg = random(256);
  
Serial.print("[");
Serial.print(level);
Serial.print("|");
Serial.print(pubg);
Serial.println("]");

delay(500);

}
