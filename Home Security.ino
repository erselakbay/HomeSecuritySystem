#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd (0x3F,16,2);


const byte satir = 4;
const byte sutun = 4;
char tus_takimi[satir][sutun] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte satir_pinleri[satir] = {6,7,8,9};
byte sutun_pinleri[sutun] = {2,3,4,5};

Keypad giris = Keypad(makeKeymap(tus_takimi) , satir_pinleri , sutun_pinleri , satir , sutun);

String sifre {1234};                            //Set Password
String girilensifre {};
int sifrehane=0;
int sifregizli=0;
int manyetikdurum;

void setup()
{
  Serial.begin(9600);
  lcd.init();   
  pinMode(10,OUTPUT);
  pinMode(11,INPUT_PULLUP);
  pinMode(12,OUTPUT);
}

void loop()
{
  ilk:
  lcd.setCursor(6,0);
  lcd.print("AKBAY");
  lcd.setCursor(0,1);
  lcd.print("ALARM SISTEMLERI");
  
  char buton = giris.getKey();                        //waiting for input from keypad

  if(buton != NO_KEY && buton != 'A')                   //if any character entered except A, which is the setup button
     {
        digitalWrite(10,HIGH);                            //beep sound (this command is present every time a key is pressed)
        delay(100);
        digitalWrite(10,LOW);
        lcd.clear();
        lcd.backlight();
        lcd.setCursor(6,0);
        lcd.print("HATA");                                //Error message on the lcd
        delay(3000);
        lcd.clear();  
        lcd.setCursor(6,0);
        lcd.print("AKBAY");
        lcd.setCursor(0,1);
        lcd.print("ALARM SISTEMLERI");
        delay (2000);
        lcd.noBacklight();
     }
                              
 
  if(buton != NO_KEY && buton == 'A')                     //if the character 'A' is entered on the keypad
     {
        delay(300);
        digitalWrite(10,HIGH);
        delay(100);
        digitalWrite(10,LOW);
        lcd.clear();
        lcd.backlight();
        lcd.setCursor(2,0);
        lcd.print("SIFRE GIRINIZ");                        //waiting for password for alarm setup
      
      
        while(sifrehane < 5)                               //if a entering is made within the character limits
          {
            char buton = giris.getKey(); 
            if(buton != NO_KEY && buton != '#' && buton != 'D')          //for all entering except delete and confirm buttons
              {   
                digitalWrite(10,HIGH);
                delay(100);
                digitalWrite(10,LOW);
                sifregizli=sifrehane + 6;
                lcd.setCursor(sifregizli,1);
                lcd.print('*');                                             //'*' symbol is printed per character entered in the appropriate place on the LCD.
                girilensifre = girilensifre + buton;
                buton = NO_KEY;
                Serial.println("sifre");                                    //for test
                Serial.println(girilensifre);                               //for test
                sifrehane=sifrehane+1;
              }
            if(buton == '#')                                             //if the character '#', which is the confirmation button, is entered on the keypad
              {
                digitalWrite(10,HIGH);
                delay(100);
                digitalWrite(10,LOW);
                girilensifre = girilensifre + buton;
                Serial.println("sifre");
                Serial.println(girilensifre);
                sifrehane=sifrehane+1;
                if (girilensifre.substring(0,4)==sifre)                     //if the password entered is correct
                  {
                    lcd.clear();
                    lcd.setCursor(2,0);
                    lcd.print("ALARM KURULDU");                            //alarm set information is written to LCD
                    delay(20000);                                          //waiting 20 seconds for the alarm to activate
                    int z=0;
                    for(z=0; z<5; z++)                                     //alert that the alarm is set
                      {
                        digitalWrite(10,HIGH);
                        delay(100);
                        digitalWrite(10,LOW);
                        delay(100);
                      }
                    int r=0;
                    manyetikdurum=digitalRead(11);                          //the status of the door magnet is assigned to the variable (Attention: it sends a value of 1 if the door is open)
                    while (manyetikdurum==0)
                      { 
                        for(r;r<1;r++)
                          {
                            lcd.clear();
                            lcd.setCursor(2,0);
                            lcd.print("ALARM DEVREDE");                          //write "the alarm is on message" on lcd
                            delay(3000);
                            lcd.noBacklight();
                           }
                        manyetikdurum=digitalRead(11);                           //door magnetic query is made continuously
                        Serial.println("okunan");                                //for test
                        Serial.println(manyetikdurum);                           //for test
                      }
                                                                         //if the magnetic sensor state change (if the door is opened), it exit to while loop 
                    lcd.backlight();
                    int girisicinsure = 0;
                    girilensifre = {""};
                    sifrehane=0;
                    sifregizli=0;
                    buton = NO_KEY;
                    lcd.clear();
                    while(girisicinsure<1000 && sifrehane < 5)                //The password must be entered within 25 seconds. The process begins
                      {
                          tekrar:
                          char buton = giris.getKey();
                          delay(1); 
                          girisicinsure=girisicinsure+1;       
                          Serial.println("giris icin sure : ");                    //for test                             
                          Serial.println(girisicinsure);                           //for test
                          digitalWrite(10,HIGH);                                   //buzzer will sound until password is entered
                          lcd.setCursor(2,0);
                          lcd.print("SIFRE GIRINIZ");
                          if(buton != NO_KEY && buton != '#' && buton != 'D')      //for all entering except delete and confirm buttons
                            {   
                                sifregizli=sifrehane + 6;
                                lcd.setCursor(sifregizli,1);
                                lcd.print('*');
                                girilensifre = girilensifre + buton;
                                buton = NO_KEY;
                                Serial.println("sifre");                       //for test
                                Serial.println(girilensifre);                  //for test
                                sifrehane=sifrehane+1;
                             }
                          if(buton == '#')                                          //if the character '#', which is the confirmation button, is entered on the keypad
                             { 
                                girilensifre = girilensifre + buton;
                                Serial.println("sifre");
                                Serial.println(girilensifre);
                                sifrehane=sifrehane+1;
                                if (girilensifre.substring(0,4)==sifre)          //if the password entered is correct
                                    {     
                                      digitalWrite(10,LOW);                        //stop buzzer
                                      goto bitir;                                  //goes to the 'bitir' where the setup ends
                                    }
                                else                                             //if the password entered is not correct
                                    {  
                                      haneasti:
                                      lcd.clear();
                                      lcd.setCursor(2,0);
                                      lcd.print("YANLIS SIFRE");                   //write "wrong password" on lcd
                                      delay(1000);
                                      int girisicinsure = 0;                       //all values are reset sequentially
                                      girilensifre = {""};                         //all values are reset sequentially
                                      sifrehane=0;                                 //all values are reset sequentially
                                      sifregizli=0;                                //all values are reset sequentially
                                      buton = NO_KEY;                              //all values are reset sequentially
                                      lcd.clear();                                 //all values are reset sequentially
                                      goto tekrar;                                 //goes to the beginning of the password entry
                                    } 
                             }
                          if(buton == 'D')                                         //'delete button' that resets everything
                             {                                                     //all values are reset sequentially
                                int girisicinsure = 0;
                                girilensifre = {""};
                                sifrehane=0;
                                sifregizli=0;
                                buton = NO_KEY;
                                lcd.clear();
                                goto tekrar;                                       //goes to the beginning of the password entry
                             }
                      }
                   if(sifrehane>=5)                                                //if an entry exceeding character limits is made
                      {
                          goto haneasti;                                           //go to where all values will be deleted and password entry will begin
                      }
                                                                                   //if it comes to this line, it means that the password was not entered in the time allocated for the password.
                   int alarmsuresi = 0;                                            //all values are reset sequentially
                   buton=NO_KEY;
                   lcd.clear();
                   girilensifre = {""};
                   sifrehane=0;
                   sifregizli=0;
                   int s1=750 ;                                                 //The frequency values required for the siren sound are assigned to the variables
                   int s2= 800;
                   while (alarmsuresi<6000)                                     //siren will sound for more than 2 minutes
                      {
                          alarmsuresi=alarmsuresi+1;
                          if(s1<800)                                                //frequency increase of the siren sound
                              {
                                tone(12,s1);
                                s1++;
                              }
                          else if(s2>750)                                            //frequency decreasing of the siren sound
                              {
                                tone(12,s2);
                                s2--;;
                              }
                          else                                                       //reset of frequencies
                              {
                                s1=750;
                                s2= 800;
                              }
                          tekrarr:
                          char buton = giris.getKey();                               //waiting for a password to silence the alarm
                          lcd.setCursor(2,0);
                          lcd.print("SIFRE GIRINIZ");
                          if(buton != NO_KEY && buton != '#' && buton != 'D')
                              {   
                                sifregizli=sifrehane + 6;
                                lcd.setCursor(sifregizli,1);
                                lcd.print('*');
                                girilensifre = girilensifre + buton;
                                buton = NO_KEY;
                                Serial.println("sifre");
                                Serial.println(girilensifre);
                                sifrehane=sifrehane+1;
                              }
                          if(buton == '#')
                              { 
                                girilensifre = girilensifre + buton;
                                Serial.println("sifre");
                                Serial.println(girilensifre);
                                sifrehane=sifrehane+1;
                                if (girilensifre.substring(0,4)==sifre)               //if the password is entered correctly,
                                   {     
                                      noTone(12);                                    //the alarm is deactivated and
                                      goto bitir;                                    //goes to the 'bitir' where the setup ends
                                    }
                                else                                                 //if the password entered is not correct
                                    {
                                      haneastii:
                                      lcd.clear();
                                      lcd.setCursor(2,0);
                                      lcd.print("YANLIS SIFRE");
                                      delay(1000);
                                      int girisicinsure = 0;
                                      girilensifre = {""};
                                      sifrehane=0;
                                      sifregizli=0;
                                      buton = NO_KEY;
                                      lcd.clear();
                                      goto tekrarr;                                   //goes to the beginning of the password entry
                                    } 
                              }             
                          if(buton == 'D')
                              {
                                int girisicinsure = 0;
                                girilensifre = {""};
                                sifrehane=0;
                                sifregizli=0;
                                buton = NO_KEY;
                                lcd.clear();
                                goto tekrarr;                                         //goes to the beginning of the password entry
                              }
                          if(sifrehane>=5)
                              {
                                goto haneastii;                                       //go to where all values will be deleted and password entry will begin
                              }
                      }
                                                                                      //siren turns off when the siren time expires
                   noTone(12);          
                   digitalWrite(10,LOW);
                   goto bitir;
                  }
               else                                                                    //if the password entered (for set) is not correct
                  {
                    haneasimi:
                    lcd.clear();
                    lcd.setCursor(2,0);
                    lcd.print("YANLIS SIFRE");
                    delay(1500);
                    goto bitir;
                  }
              }
           if(buton == 'D')                                                            //'delete button' that resets everything
              {
                bitir:
                digitalWrite(10,HIGH);
                delay(100);
                digitalWrite(10,LOW);
                girilensifre = {""};
                sifrehane=0;
                sifregizli=0;
                buton = NO_KEY;
                lcd.clear();
                lcd.noBacklight();
                goto ilk;
              }
           } 
              goto haneasimi;
     }   
}



 
  
