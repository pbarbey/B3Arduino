//biblioteque
#include <LiquidCrystal.h>

//initialisation et variables
  //lcd (6 pin)
  LiquidCrystal lcd(12,11,5,4,3,2); // on crée l'objet écran
  //interrupteur 
 int pinInterrupteur = 7;//
  //mouvement (1 pin) (gris = 5v, noir = GND), fil blanc = la mesure
 int pinMouvement = 10;
  //ouverture/fermeture fenetre (1 pin)
 int pinPorte = 9;
  //buzzer (1 pin)
 int pinBuzzer = 8;
  //température (A0)
  //pas besoin de pin pour celui là

  //ventilateur (1 pin)
 int pinVentilateur = 13;
  //des leds liés aux différents appareil (4 ?)
 //int pinLedRouge = 7;
 int pinLedVert = 6;
  //pinLedBleu = ;
  //etat booleen
  int etatCapteurMouvement;
  float etatInterrupteur;
  boolean etatPorte;
  
  //chaine a envoyé pour le raspbery (vide au début et on concaténe l'état de chque capteur après )
  String message ="";

//temperature
int valeur_brute;
float temperature_celcius;
  
//fonction supplémentaire
 void buzzer()
{
  digitalWrite(pinBuzzer, HIGH);
  tone(pinBuzzer, 1000); 
  delay(100);  
  noTone(pinBuzzer);          
  digitalWrite(pinBuzzer, LOW);       
}

/////////////////////////////////////
void setup() {
  //mode des pins
pinMode(pinInterrupteur, INPUT);
pinMode(pinMouvement, INPUT);
pinMode(pinPorte, INPUT);
pinMode(pinBuzzer,OUTPUT);
pinMode(pinVentilateur, OUTPUT);
//pinMode(pinLedRouge, OUTPUT);
pinMode(pinLedVert, OUTPUT);

//initialisation de la communication Serial
Serial.begin(9600);

//initialisation des pins en mode sortie sur pas de courant
digitalWrite(pinBuzzer, LOW);
digitalWrite(pinVentilateur, LOW);
//digitalWrite(pinLedRouge, LOW);
digitalWrite(pinLedVert, LOW);
}
/////////////////////////////////////
int flag =0;
String message2 ="";String message3 ="O";String message4 ="F";
void loop() {
 lcd.clear();
//on fait le tour des capteurs 
message ="";
//interrupteur
etatInterrupteur = digitalRead(pinInterrupteur);

if(etatInterrupteur == HIGH) //si il est sur HIGH on surveille tout les capteurs
  {
 
    //led 
      digitalWrite(pinLedVert, LOW);
      //digitalWrite(pinLedRouge, LOW);
    
    //cpt mouvement (gris = 5v, noir = GND)  + pas de buzzer
      etatCapteurMouvement = digitalRead(pinMouvement); 
      if (etatCapteurMouvement == HIGH)//                             BLOCAGE 4sec sur la détection de mouvement
        {
          message2 = message;
          message = message2+message3; //ouvert =  allumé
          //Serial.println("OUVERT MOUV");
       
        }
      else
       {// Serial.println("FERME MOUV");
       message2 = message;
        message = message2+message4;} // fermé = pas de mouvement
    
    //la porte si ouvert -> led
      etatPorte = digitalRead(pinPorte); 
      if (etatPorte == HIGH)
      {message2 = message;
         message = message2+message3; //ouvert =  allumé

      }
      else
      {message2 = message;
      message = message2+message4;} // fermé = pas de mouvement
      
    //cpt température
      // Mesure la tension sur la broche A0
      valeur_brute = analogRead(A0);
      // Transforme la mesure (nombre entier) en température via un produit en croix
      temperature_celcius = valeur_brute * (5.0 / 1023.0 * 100.0)-3.0;
      // affichage sur le LCD
      lcd.setCursor(0,0);
      lcd.print("temperature:");
      lcd.print(temperature_celcius);
   //Serial.println(temperature_celcius);
    
    //ventilateur
      if (temperature_celcius > 22.0)
      {
        if(flag == 0)
        {
          digitalWrite(pinVentilateur, HIGH);
          delay(1000);
         digitalWrite(pinVentilateur, LOW);
         }
         flag = 1;
      }
      else
      {
        flag=0;
        digitalWrite(pinVentilateur, LOW);
       }
      //envoie du message
      Serial.println(message);
  }

else //si l'interruteur est sur LOW on surveille les capteurs extérieurs
  {
    //led 
      digitalWrite(pinLedVert, HIGH);
      //digitalWrite(pinLedRouge, HIGH);
    
    //cpt mouvement (gris = 5v, noir = GND)  + buzzer
      etatCapteurMouvement = digitalRead(pinMouvement); 
      if (etatCapteurMouvement == HIGH)//                             BLOCAGE 4sec sur la détection de mouvement
        {
          message2 = message;
          message = message2+message3; //ouvert =  allumé
          //Serial.println("OUVERT MOUV");
       
        }
      else
       {// Serial.println("FERME MOUV");
       message2 = message;
        message = message2+message4;} // fermé = pas de mouvement
        
   //on gère pas la porte dans ce mode là
   
  //cpt température
      // Mesure la tension sur la broche A0
      valeur_brute = analogRead(A0);
      // Transforme la mesure (nombre entier) en température via un produit en croix
      temperature_celcius = valeur_brute * (5.0 / 1023.0 * 100.0);
      // affichage sur le LCD
      lcd.setCursor(0,0);
      lcd.print("temperature:");
      lcd.print(temperature_celcius);
    //Serial.println(temperature_celcius);
    //ventilateur
       if (temperature_celcius > 40.0)
      {
        if(flag == 0)
        {
          digitalWrite(pinVentilateur, HIGH);
          delay(1000);
         digitalWrite(pinVentilateur, LOW);
         }
         flag = 1;
      }
      else
      {
        flag=0;
        digitalWrite(pinVentilateur, LOW);
       }
      Serial.println(message);
  }
  delay(1000);
}
