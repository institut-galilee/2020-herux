//Code réalisé par Arthur LE GOFF et Smail LEKLOU

//Librairies utilisées 
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

//Creation des PINs
#define BUZZER 2 //pin = 2 => Buzzer
#define SERVO_PIN 3 //pin = 3 => Servo
#define LROUGE 4 //pin = 4 => Led Rouge
#define LVERT 5 //pin = 5 => Led Verte
#define RST_PIN  //pin = 9 => RST (RFID)
#define SS_PIN 10 //pin = 10 => SS (RFID)

//Creation des instances
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Creation de l'instance MFRC522 (RFID)
Servo servo; //Creation de l'instance servo
String listecarte[4] = {"37 0A 79 B4","92 8A 63 C6","73 1A 57 A3"}; //Creation de la liste de base des cartes autorisées

void setup()
{
  Serial.begin(9600);   

  //Initialisation de la reconnaissance RFID
  SPI.begin();      // Initialisation du  SPI bus
  mfrc522.PCD_Init();   // Initialisation du MFRC522

  //Initialisation du ServoMoteur
  servo.attach(SERVO_PIN); //servo pin
  servo.write(0); //servo start position

  //Initialisation des leds
  pinMode(LVERT, OUTPUT); //Initialisation LED VERTE
  pinMode(LROUGE, OUTPUT); //Initialisation LED ROUGE

  //Initialisation du buzzer et configuration
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  
  msgln("Inserer la carte");
  msgln();

}
void loop()
{
  // Check de la carte
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Selection de la carte
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  
  //Renvoie le numero associé à la carte
  msg("Numero de la carte : ");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  msgln();
  Serial.print("Message : ");
  content.toUpperCase();
  numerochecking(content);

}


//Check si la carte est autorisée ou non et fait une action en consequence
void numerochecking(String numero){
  if (contains(listecarte,numero.substring(1))) 
  {
    msgln("Acces autorisé");
    msgln();
    delay(500);
    digitalWrite(LVERT, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    servo.write(90);
    delay(5000);
    servo.write(0);
    digitalWrite(LVERT, LOW);
  }

  else   {
    msgln(" Access non autorisé");
    digitalWrite(LROUGE, HIGH);
    tone(BUZZER, 300);
    delay(1000);
    digitalWrite(LROUGE, LOW);
    noTone(BUZZER);
  }
}

//Ajout de carte dans la liste des cartes autorisées
void creationautorisation(String numero){
  
}

//Fonction pour savoir si un element appartient à une liste de String
//i.e : Savoir si le numero de la carte est inclus dans la liste des cartes autorisées
bool contains(String* liste, String elem){
  for(byte i=0;i<sizeof(liste);i++){
    if(liste[i] == elem)return true;
  }
  return false;
}

//Ecriture d'un message sur le moniteur en serie

void msgln(String message){
  Serial.println(message);
}

void msgln(){
  Serial.println();
}

void msg(String message){
  Serial.print(message);
}

void msg(){
  Serial.print();
}

//Code réalisé par Arthur LE GOFF et Smail LEKLOU
