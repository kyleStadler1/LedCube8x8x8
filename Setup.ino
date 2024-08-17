void setup() {
  
  Serial.begin(9600);
  digitalWrite(oe, HIGH);
  delay(1000);
  digitalWrite(oe, LOW);
  pinMode(srclk, OUTPUT);
  pinMode(rclk, OUTPUT);
  pinMode(ser, OUTPUT);
  pinMode(clr, OUTPUT);
  digitalWrite(rclk, LOW);
  digitalWrite(srclk, LOW);
  digitalWrite(clr, LOW);
  digitalWrite(clr, HIGH);
  digitalWrite(oe, LOW);
}
