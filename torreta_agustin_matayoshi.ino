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
  byte rowPins[ROWS] = {9, A5, A4, A3}; //pines que se utilizan para las filas del keypad
  byte colPins[COLS] = {A2, A1, A0}; //pines que se utilizan para las columnas del keypad
  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
  LiquidCrystal lcd(12,11,5,4,3,2);
  byte control=0, pad[4]={0,0,0,0}, contra[4]={'1','5','9','3'};//se utiliza una variable para que se guarden los datos del pad y otra para comparar
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  bool torretaACT=true, banderapir=LOW, alarma=LOW, parpadeo=HIGH;
  byte pir=10, buzz=8, ledR=6, ledV=7;
  byte cursorlcd=0, pos=0;
  unsigned long tiempo=0, pausa=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  lcd.begin(16,2);
  Serial.begin(9600);
  pinMode(pir,INPUT);
  for(byte x=6;x<9;x++){
    pinMode(x,OUTPUT);
  }
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
    lcd.print("Alarma On     ");
     if (millis()>pausa+500){
       digitalWrite(ledR,parpadeo);
       parpadeo=!parpadeo;
       pausa=millis();
     }
    digitalWrite(ledV,LOW); 
  }

  if (torretaACT==false){
    estpir=0;
    lcd.setCursor(0,0);
    lcd.print("Alarma Off      ");
    digitalWrite(ledR,LOW);
    digitalWrite(ledV,HIGH); 
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
     digitalWrite(ledV,LOW);
     tiempo=millis();
     banderapir=estpir;
     alarma=HIGH; 
     
  }

  if ((millis()>tiempo+7000)&&(alarma==HIGH)){
    alarmaactivada();
  }
//////////////////////////////////////////////////////////////////////////////////////
  if (pos<4){
    if (key != NO_KEY){//entrada de datos para el keypad
      if (key=='#'){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ingrese la clave");
        lcd.setCursor(0,1);
        pos=0;
        cursorlcd=0;
      }
      else{
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
      digitalWrite(ledV,LOW); 
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
