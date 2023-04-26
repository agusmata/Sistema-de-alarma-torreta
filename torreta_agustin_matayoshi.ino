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
byte pir=10, buzz=8;
bool banderaprincipal=LOW, banderaalarma=LOW;
unsigned long tiempo=0;
byte rowPins[ROWS] = {9, A5, A4, A3}; //pines que se utilizan para las filas del keypad
byte colPins[COLS] = {A2, A1, A0}; //pines que se utilizan para las columnas del keypad
byte cursorlcd=0, pos=0;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal lcd(12,11,5,4,3,2);
byte control=0, pad[4]={0,0,0,0}, contra[4]={'1','5','9','3'};//se utiliza una variable para que se guarden los daton del pad y otra para comparar
void setup() {
  lcd.begin(16,2);
  pinMode(buzz,OUTPUT);
  pinMode(pir,INPUT);
}

void loop() {
  char key = keypad.getKey();
  bool estpir=digitalRead(pir);
  if (estpir==HIGH){
      banderaprincipal=HIGH;//se crea una bandera ya que si el estado del pir vuelve a mandar un pulso alto resetearia la contraseña que se este ingresando
      banderaalarma=HIGH;
  }
  if (banderaprincipal==HIGH){
    inicio();//no lo dejo en el setup porque habria que resetear la torreta para que volviera a imprimir el mensaje
    if (millis()==tiempo+20000){
      alarmaactivada();
    }
    if (pos<4){
        if (key != NO_KEY){//entrada de datos para el keypad
          lcd.setCursor(cursorlcd,1);
          lcd.print(key);
          pad[pos]=key;
          cursorlcd++;
          pos++;
        }
      }
      if(pos==4){
        for(byte x=0;x<4;x++){
          if(pad[x]==contra[x]){//si control es cuatro entonces la contraseña es correcta
            control++;
        }
        }
        
          if(control<4){//en caso de ser incorrecta suena la alarma
            lcd.setCursor(0,0);
            lcd.print("incorrecta       ");
            alarmaactivada();
            tiempo=0;
          }
          if(control==4){//en caso de ser correcta se desactiva la alarma
            lcd.setCursor(0,0);
            lcd.print("correcta       ");
            control=control+1;
            pos++;
            banderaprincipal=LOW;
            tiempo=0;
            digitalWrite(buzz,LOW);
          }
      control=0;
    }
  }
}
void alarmaactivada(){//si pasan mas de 20 segundos suena la alarma indefinidamente
  digitalWrite(buzz,HIGH);
  delay(100);
  digitalWrite(buzz,LOW);
  delay(50);
}
void inicio(){
if(banderaalarma==HIGH){
      lcd.setCursor(0,0);
      lcd.print("ponga la clave");//no lo dejo en el setup porque habria que resetear la torreta para que volviera a imprimir el mensaje
      digitalWrite(buzz,HIGH);
      delay(50);
      digitalWrite(buzz,LOW);
      delay(50);
      digitalWrite(buzz,HIGH);
      delay(50);
      digitalWrite(buzz,LOW);
      delay(50);
      banderaalarma=LOW;
      tiempo=millis();
    }
}

