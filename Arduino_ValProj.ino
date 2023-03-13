const int OnRel = 0;
const int OffRel = 1;

//Inputs
const int ttl1 = 2;     //LED2 from Imetronic
const int ttl2 = 3;     //LED3 from Imetronic
const int ttl3 = 4;     //LED4 from Imetronic
const int ttl4 = 5;     //LED5 from Imetronic

const int lickL = 6;    //Lick photogate Left
const int lickR = 7;    //Lick photogate Right
//Controls
const int usX5 = 48;    //x5 Switch
const int usWeak = 49;  //US Low
const int ledR = 50;    //Led Left
const int ledL = 51;    //Led Right
const int rewR = 52;    //Reward Left
const int rewL = 53;    //Reward Right

//initialize current input states
int ttl1Val = 0;
int ttl2Val = 0;
int ttl3Val = 0;
int ttl4Val = 0;

int lickLVal = 0;
int lickRVal = 0;
int shockHVal = 0;
//others
int auxRnd;
int hasStarted = 0;
int ongoingCSRew = 0;
int ongoingCSShk = 0;
int ongoingShk = 0;
int flagLick;
unsigned long iniTm=0; 
unsigned long iniTrialTm;

void setup() {

  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(ttl1, INPUT);
  pinMode(ttl2, INPUT);
  pinMode(ttl3, INPUT);
  pinMode(ttl4, INPUT);
  pinMode(lickL, INPUT);
  pinMode(lickR, INPUT);
  pinMode(rewL, OUTPUT);
  pinMode(rewR, OUTPUT);

  digitalWrite(usX5, OffRel);
  digitalWrite(usWeak, OffRel);
  digitalWrite(rewL, OffRel);
  digitalWrite(rewR, OffRel);

}

void loop() {

  ttl1Val=digitalRead(ttl1);ttl2Val=digitalRead(ttl2);ttl3Val=digitalRead(ttl3);ttl4Val=digitalRead(ttl4); //Read IME Word
  //Serial.print(ttl1Val);Serial.print(ttl2Val);Serial.print(ttl3Val);Serial.println(ttl4Val);

  //*** Imetronic Started the exercise
  if (ttl1Val==1 & ttl2Val==1 & ttl3Val==1 & ttl4Val==1 & hasStarted==0) {
    iniTm=millis();
    hasStarted=1;
  }
  
  //*** Imetronic Stop
  if (ttl1Val==1 & ttl2Val==1 & ttl3Val==1 & ttl4Val==1 & hasStarted==1 ) {    
    if (millis()-iniTm>10000) {
      Serial.println("-1,1,1,1,1");
      delay(600000);
      return;
    }    
  }  


  if (ttl1Val==0 & ttl2Val==0 & ttl3Val==0 & ttl4Val==0) {   
      ongoingCSRew=0;
      ongoingCSShk=0;
  }

  //*** An CS Rew starts
  if (ttl1Val==1 & ttl2Val==0 & ttl3Val==0 & ttl4Val==1 & ongoingCSRew==0 & ongoingCSShk==0) {   
    Serial.print(millis()-iniTm);Serial.println(",1,0,0,1");
    ongoingCSRew=1;
  }   

  //*** Set Left Water port Routine
  if (ttl1Val==0 & ttl2Val==1 & ttl3Val==0 & ttl4Val==0 & ongoingCSRew==1) {  
    ongoingCSRew=0; 
    //ttl1Val=0;ttl2Val=0;ttl3Val=0;ttl4Val=0;
    flagLick = 0;
    digitalWrite(ledL,OnRel); //turn on LED
    Serial.print(millis()-iniTm);Serial.println(",0,1,0,0");
    iniTrialTm=millis();
    while(millis()-iniTrialTm<ledMaxTime & !flagLick){ //Give reward if lick in less than ledMaxTime
      flagLick=digitalRead(lickL);  
    }
    if (flagLick){
      digitalWrite(rewL,OnRel); 
      delay(valveOpenTime);
      digitalWrite(rewL,OffRel);
      Serial.print(millis()-iniTm);Serial.println(",0,1,1,0");
    }
    digitalWrite(ledL,OffRel); //turn off LED
  }

  //*** Set Right Water port Routine
  if (ttl1Val==0 & ttl2Val==0 & ttl3Val==0 & ttl4Val==1 & ongoingCSRew==1) {  
    ongoingCSRew=0; 
    //ttl1Val=0;ttl2Val=0;ttl3Val=0;ttl4Val=0; 
    flagLick = 0;
    digitalWrite(ledR,OnRel); //turn on LED
    Serial.print(millis()-iniTm);Serial.println(",0,0,0,1");
    iniTrialTm=millis();
    while(millis()-iniTrialTm<ledMaxTime & !flagLick){ //Give reward if lick in less than ledMaxTime
      flagLick=digitalRead(lickR);  
    }
    if (flagLick){
      digitalWrite(rewR,OnRel); 
      Serial.print(millis()-iniTm);Serial.println(",0,0,1,1");
    }
    digitalWrite(ledR,OffRel); //turn off LED
  }
      
  //*** An CSM starts
  if (ttl1Val==1 & ttl2Val==1 & ttl3Val==0 & ttl4Val==0 & ongoingCSRew==0 & ongoingCSShk==0) {   
    Serial.print(millis()-iniTm);Serial.println(",1,1,0,0");
    ongoingCSShk=1;
  } 
  //*** An CSPweak starts
  if (ttl1Val==1 & ttl2Val==0 & ttl3Val==1 & ttl4Val==0 & ongoingCSRew==0 & ongoingCSShk==0) {   
    Serial.print(millis()-iniTm);Serial.println(",1,0,1,0");
    ongoingCSShk=1;
    digitalWrite(usWeak,OnRel); //Set US intensity to Low
  }     
  //*** An CSPstrong starts
  if (ttl1Val==1 & ttl2Val==0 & ttl3Val==1 & ttl4Val==1 & ongoingCSRew==0 & ongoingCSShk==0) {   
    Serial.print(millis()-iniTm);Serial.println(",1,0,1,1");
    ongoingCSShk=1;
  }   
  //*** Log shuttle response
  if (ttl1Val==0 & ttl2Val==0 & ttl3Val==0 & ttl4Val==1 & ongoingCSShk==1) {  
    Serial.print(millis()-iniTm);Serial.println(",0,0,0,1");
    ongoingCSShk=0;
    digitalWrite(usWeak,OffRel);
  } 
  //*** Log US activation
  if (ttl1Val==0 & ttl2Val==0 & ttl3Val==1 & ttl4Val==0 & ongoingShk==0) {  
    Serial.print(millis()-iniTm);Serial.println(",0,0,1,0");
    ongoingShk=1;
  } 
  //*** Log escape response
  if (ttl1Val==0 & ttl2Val==1 & ttl3Val==0 & ttl4Val==0 & ongoingShk==1) {  
    Serial.print(millis()-iniTm);Serial.println(",0,1,0,0");
    ongoingShk=0;
    digitalWrite(usWeak,OffRel);
  }
  //*** Log error response
  if (ttl1Val==0 & ttl2Val==1 & ttl3Val==1 & ttl4Val==0 & ongoingShk==1) {  
    Serial.print(millis()-iniTm);Serial.println(",0,1,1,0");
    ongoingShk=0;
    ongoingCSShk=0;
    delay(100);
    digitalWrite(usWeak,OffRel);
  }


}
