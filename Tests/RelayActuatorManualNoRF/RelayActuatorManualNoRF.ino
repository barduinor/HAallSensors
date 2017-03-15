
void setup()  
{ 
  pinMode(3, OUTPUT); 
  pinMode(4, OUTPUT); 
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
}

void loop() 
{
    for( int i=3;i<=6;i++){
      digitalWrite(i,HIGH);
      delay(500);
    
    }
    for( int i=3;i<=6;i++){
      digitalWrite(i,LOW);
      delay(500);
    }
}


