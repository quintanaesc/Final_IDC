//definimos pinnes
int anlg = A0;
int digt = 5;
int let = 14;
int mot = 12;

void setup() {
  //definimos pines
  Serial.begin(9600);
  pinMode(anlg, INPUT);
  pinMode(digt, INPUT);
  pinMode(let, OUTPUT);
  pinMode(mot, OUTPUT);
}

void loop() {
  int lecAnlg = analogRead(anlg);
  int lecDigt= digitalRead(digt);

// ensender directamente
//  digitalWrite(let,HIGH);
//  digitalWrite(mot,HIGH);

//comprobar
  if(lecDigt == 1){
    digitalWrite(let,HIGH);
  }else{
    digitalWrite(let,LOW);
  }
   if(lecAnlg > 90){
    digitalWrite(mot,HIGH);
  }else{
    digitalWrite(mot,LOW);
  }
  Serial.println("valor anlg = "); 
  Serial.println(lecAnlg);
  Serial.println("; valor digt;  ");
  Serial.println(lecDigt);
  delay(500);
}
