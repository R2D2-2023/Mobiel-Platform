int Sensor1 = 0;
int Sensor2 = 0;
int Sensor3 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(Sensor1, INPUT);
  pinMode(Sensor2, INPUT);
  pinMode(Sensor3, INPUT);
}

void loop(){
  //Read the Sensor if HIGH (BLACK Line) or LOW (WHITE Line)
  Sensor1 = digitalRead(8);
  Sensor2 = digitalRead(9);
  Sensor3 = digitalRead(10);


  //Set conditions for FORWARD, LEFT and RIGHT

  if(Sensor3 == HIGH && Sensor2 == LOW && Sensor1 == LOW){
    Serial.println("right");
    setMotorSpeed(1000, 500 );
  }

  else if (Sensor3 == LOW && Sensor2 == LOW && Sensor1 == HIGH){
    Serial.println("left");
    setMotorSpeed(500, 1000 );
  }

  else{
    Serial.println("good");
    setMotorSpeed(500, 500 );
  }
}
