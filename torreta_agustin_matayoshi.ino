#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

  const byte ROWS = 4; //Cuatro filas
  const byte COLS = 3; //Tres columnas
  char keys[ROWS][COLS] = {
    {'1','2','3'}, //array bidimensional para las teclas
    {'4','5','6'}, //keypad 4x3
    {'7','8','9'},
    {'*','0','#'}
  };
  byte rowPins[ROWS] = {28, 27, 26, 25}; //pines que se utilizan para las filas del keypad
  byte colPins[COLS] = {24, 23, 22}; //pines que se utilizan para las columnas del keypad
  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
  LiquidCrystal lcd(38,37,36,35,34,33);
  Servo AlaIzq;
  Servo AlaDer;
  byte control=0, pad[4]={0,0,0,0}, contra[4]={'1','5','9','3'};//se utiliza una variable para que se guarden los datos del pad y otra para comparar
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  bool torretaACT=true, banderapir=LOW, alarma=LOW, parpadeo=HIGH, carga=HIGH;
  byte pir=10, buzz=30, ledR=6, ledV=7, ledR2=3, ledV2=4;
  byte cursorlcd=0, pos=0;
  unsigned long tiempo=0, pausa=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  lcd.begin(16,2);
  Serial.begin(9600);
  pinMode(pir,INPUT);
  pinMode(buzz,OUTPUT);
  pinMode(ledR,OUTPUT);
  pinMode(ledV,OUTPUT);
  pinMode(ledR2,OUTPUT);
  pinMode(ledV2,OUTPUT);
  AlaDer.attach(11);
  AlaIzq.attach(8);
 }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  bool estpir;
  char key = keypad.getKey();
//////////////////////////////////////////////////////
  if ((torretaACT==true) && (alarma==LOW)){
    estpir=digitalRead(pir);
    lcd.setCursor(0,0);     
    if (carga==LOW){
      
      for(byte x=0;x<10;x++){
      digitalWrite(buzz,HIGH);
      delay(500);
      digitalWrite(buzz,LOW);
      delay(500);
      }
      carga=HIGH;
    }
    lcd.setCursor(0,0);
    lcd.print("Alarma On     ");
     if (millis()>pausa+500){
       digitalWrite(ledR,parpadeo);
       digitalWrite(ledR2,parpadeo);
       parpadeo=!parpadeo;
       pausa=millis();
     }
    digitalWrite(ledV,LOW);
    digitalWrite(ledV2,LOW); 
  }

  if (torretaACT==false){
    estpir=0;
    lcd.setCursor(0,0);
    lcd.print("Alarma Off      ");
    digitalWrite(ledR,LOW);
    digitalWrite(ledR2,LOW);
    digitalWrite(ledV,HIGH);
    digitalWrite(ledV2,HIGH); 
    AlaIzq.write(180);
    AlaDer.write(180);
    delay(10);
  }
  Serial.println(estpir);
//////////////////////////////////////////////////////
  if (estpir==LOW){
    banderapir=estpir;
  }

  if ((estpir==HIGH)&&(estpir!=banderapir)){
     lcd.setCursor(0,0);
     lcd.print("Ingrese la clave");//no lo dejo en el setup porque habria que resetear la torreta para que volviera a imprimir el mensaje
     digitalWrite(buzz,HIGH);
     delay(50);
     digitalWrite(buzz,LOW);
     delay(50);
     digitalWrite(buzz,HIGH);
     delay(50);
     digitalWrite(buzz,LOW);
     delay(50);
     digitalWrite(ledR,HIGH);
     digitalWrite(ledR2,HIGH); 
     digitalWrite(ledV,LOW);
     digitalWrite(ledV2,LOW);
     tiempo=millis();
     banderapir=estpir;
     alarma=HIGH; 
     carga=LOW;
     AlaIzq.write(90);
     AlaDer.write(90);
     delay(10);
  }

  if ((millis()>tiempo+7000)&&(alarma==HIGH)){
    alarmaactivada();
    
  
    
  }
//////////////////////////////////////////////////////////////////////////////////////
  if (pos<4){
    if (key != NO_KEY){//entrada de datos para el keypad

      if (key=='#'){
        tone(buzz,4275);
        delay(50);
        noTone(buzz);
        delay(50);
        tone(buzz,4275);
        delay(50);
        noTone(buzz);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ingrese la clave");
        lcd.setCursor(0,1);
        pos=0;
        cursorlcd=0;
      }
      else{
            tone(buzz,4275);
            delay(50);
            noTone(buzz);
            lcd.setCursor(cursorlcd,1);
            //lcd.print(key);
            lcd.print("*");
            pad[pos]=key;
            cursorlcd++;
            pos++;
          }
      }
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if(pos==4){
    for(byte x=0;x<4;x++){
       if(pad[x]==contra[x]){//si control es cuatro entonces la contraseña es correcta
         control++;
       }
    }
    if(control<4){//en caso de ser incorrecta suena la alarma
      lcd.setCursor(0,0);
      lcd.print("Incorrecta       ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      delay(500);
      pos=0;
      cursorlcd=0;
      alarmaactivada();
      tiempo=0;
      digitalWrite(ledR,HIGH);
      digitalWrite(ledR2,HIGH); 
      digitalWrite(ledV,LOW);
      digitalWrite(ledV2,LOW); 
    }        
    if(control==4){//en caso de ser correcta se desactiva la alarma
      lcd.setCursor(0,0);
      lcd.print("Correcta        ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      delay(500);
      control=control+1;
      pos++;
      alarma=LOW;
      noTone(buzz);
      digitalWrite(buzz,HIGH);
      delay(50);
      digitalWrite(buzz,LOW);
      delay(50);
      digitalWrite(buzz,HIGH);
      delay(50);
      digitalWrite(buzz,LOW);
      delay(50);
      //  banderaprincipal=LOW;
      //tiempo=0;
      tiempo=millis();
      torretaACT=!torretaACT;
      pos=0;
      cursorlcd=0;
    }
        
    control=0;
  }
}    
////////////////////////////////////////////////////////////////////////////////////////////////
void alarmaactivada(){//si pasan mas de 20 segundos suena la alarma indefinidamente
  tone(buzz,4345);
  delay(50);
  noTone(buzz);
  delay(50);
}
