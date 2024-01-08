#define TL 28
#include <Wire.h> 
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
RTC_DS1307 RTC;
int hora;
int minuto;
int estado =1;
unsigned char seg=0;
unsigned char r0=0; //va a ser un solo byte
unsigned char Incrementa=1;
int a[5] = {851,680, 510, 340, 170};
byte b[5] = {16, 8, 4, 2 ,1};
byte EntControl = 0;



void setup() {
     
  Wire.begin(); // Inicia el puerto I2C
  RTC.begin(); // Inicia la comunicaci¢n con el RTC
  //RTC.adjust(DateTime(2000,06,23,hora, minuto,0));
  //DateTime HoraFecha= RTC.now();
 //RTC.adjust(DateTime(__DATE__, __TIME__)); // Establece la fecha y hora (Comentar una vez establecida la hora)
  //el anterior se usa solo en la configuracion inicial
  Serial.begin(9600); // Establece la velocidad de datos del puerto serie
  lcd.init();
  lcd.noDisplay();// para que se muestren los datos debes cambiar esto a lcd.display();
  lcd.noBacklight();//Para que encienda la pantalla debes de poner lcd.backlight();
  lcd.clear();
  pinMode(2, OUTPUT);//latch
  pinMode(3, OUTPUT);//clock
  pinMode(4, OUTPUT);//dta
  digitalWrite(2, LOW); 

  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A1,INPUT);
 
}

void loop() {
  DateTime HoraFecha= RTC.now();//obtenemos la hora y fecha actual 
  
  if(estado ==1){//Este estado sirve para establecer la hora en tiempo real 
      minuto=HoraFecha.minute();//esto es para que cuando se quiere configurar la minutos empiece desde donde esta la minutos actual
      hora=HoraFecha.hour();//esto es para que cuando se quiere configurar la hora empiece desde donde esta la hora actual
       lcd.clear();
       //Empieza la impresion del reloj binario en tiempo real
       lcd.setCursor(0,0);
       lcd.print("RELOJ BINARIO");
       lcd.setCursor(1,1);
       lcd.print("Hora: ");
       if(hora<10){//If para imprimir un 0 antes de un numero menor a 0
       lcd.print('0');
       }
       lcd.print(hora , DEC);
       lcd.print(":");
       if(minuto<10){//If para imprimir un 0 antes de un numero menor a 0
       lcd.print('0');
        }
       lcd.print(minuto, DEC);
        delay(1000);
  }//Aqui cierra el if de estado 1
  
  if (estado ==2){//Este estado sirve para establecer la nueva hora configurada 
      //RTC.adjust(DateTime(hora, minuto,0));
       lcd.clear();
       //Empieza la impresion del reloj binario configurado por el usuario (contador)
       lcd.setCursor(0,0);
       lcd.print("RELOJ BINARIO");
       lcd.setCursor(1,1);
       lcd.print("Hora: ");
       if(hora<10){//If para imprimir un 0 antes de un numero menor a 0
       lcd.print('0');
      }
       lcd.print(hora);
       lcd.print(':');
       if(minuto<10){//If para imprimir un 0 antes de un numero menor a 0
       lcd.print('0');
      }
       lcd.print(minuto);
       //lcd.print(':');
       if(seg<10){//If para imprimir un 0 antes de un numero menor a 0
       //lcd.print('0');
      }
       //lcd.print(seg);
       delay(1000);
       seg++;//Sirve para que funcione el contador 
       if(seg>59){
       seg=0;
       minuto++;
      }

       if(minuto>59){
       minuto=0;
       hora++;
     }
      if(hora>=24){
      seg=0;
      minuto=0;
      hora=0;
     }
    }//Cierra el if de estado ==2
    
  //Aqui puse la configuracion de lo LEDS ///////  
  shiftOut(4,3,LSBFIRST, r0);
  digitalWrite(2, LOW);
  digitalWrite(2, HIGH);

  if(Incrementa==1){
    r0=minuto;
    r0=r0+1;
    delay(60000);
  }
  if(r0>=60){
    
    r0=0;

  }////Aqui termina lo de los leds /////
   
     minuto = int(minuto); 
     
  
  // put your main code here, to run repeatedly:
  int valor =0;
  valor = analogRead(A1);
  /////////////////////////////////
  int linf = 0;
  int lsup = 0;
  
  for(int i = 0; i < 5; i++){
    linf = a[i] - TL;
    lsup= a[i] + TL;
     if ((valor <= lsup) && (valor >= linf)){
    //Serial.print("Boton: ");
    //Serial.println(i +1);

      EntControl= b[i];


      
    if(EntControl == 16){//Para encender la pantalla e iniciar el estado ==1
      lcd.display();
      lcd.backlight();
      estado =1;
      }//Aqui cierra el if de EntCotrol== 16
      
      if(EntControl == 8 ){//Para apagar la pantalla
      lcd.noDisplay();
      lcd.noBacklight();
      estado =3;
      }//Aqui cierra el if de EntCotrol == 8
      
      if(EntControl == 4 ){//Para ajustar la hora
       hora = int(hora); 
       hora=hora+1;
       lcd.clear(); 
       lcd.setCursor(0,0);
       lcd.print("CONFIGURAR HRS");
       lcd.setCursor(4,1);
       if(hora<10){//If para imprimir un 0 antes de un numero menor a 0
       lcd.print('0');
       }
       if(hora>=24){
       hora=0;
       lcd.print('0');
       }
       lcd.print(hora);
       lcd.print(":");
       if(minuto<10){//If para imprimir un 0 antes de un numero menor a 0
       lcd.print('0');
       }
       if(minuto>59){
       minuto=0;
       lcd.print('0');
       }
       lcd.print(minuto);
       delay(1000);
      }//Aqui cierra el if de EntCotrol ==4
      
      if(EntControl == 2 ){//Ajustar los minutos
       estado =5;
       minuto = int(minuto); 
       minuto=minuto+1;
       lcd.clear(); 
       lcd.setCursor(0,0);
       lcd.print("CONFIGURAR MIN");
       lcd.setCursor(4,1);
       if(hora<10){//If para imprimir un 0 antes de un numero menor a 0
       lcd.print('0');
       }
       if(hora>=24){
       hora=0;
       lcd.print('0');
       }
       lcd.print(hora);
       lcd.print(":");
       if(minuto<10){//If para imprimir un 0 antes de un numero menor a 0
       lcd.print('0');
       }
       if(minuto>59){
       minuto=0;
       lcd.print('0');
       }
       lcd.print(minuto);
       delay(1000);
      }//Aqui cierra el if de EntCotrol= 2
      
      if(EntControl == 1 ){//Para aceptar los cambios y continuar con el estado== 2
       estado =2;
      }//Aqui cierra el if de EntCotrol=1
   
    } 

   }

 }
