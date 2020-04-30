/*  Created by iamvobi
 *  12.06.2019 
 *  "VobiWatering"
 *  simseklifurkan0@gmail.com
 */
 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pump = 8;     //WaterPump

const int xPin = A0;    // Joystick pins
const int yPin = A1;
const int SW = 2;

int menu = 1;

int defHours = 6;                //Default interval
int defWateringDuration = 10;    //Default watering duration

// I'm going to use these variables for millis function
// and I'm going to use millis function for DefaultWatering

unsigned long lastTime = 0;

void setup() {

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(SW, INPUT_PULLUP);

  pinMode(pump,OUTPUT);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Sulaniyor..");               // Everytime program has started, this one will work
  lcd.setCursor(0,1);
  lcd.println("Lutfen bekleyin ");             // for 5 seconds
  
  digitalWrite(pump,LOW);
  delay(2000);                             
  lcd.clear();
  updateMenu();
  digitalWrite(pump,HIGH);
}

void loop() {

  DefaultWatering();
  Menu();
}

void Menu(){                                           // When we click the menu buttons, they do somethings. Like scroll down,up or select the page
  if (analogRead(yPin) > 900){    //Down
    Serial.println("Down");
    menu++;
    updateMenu();
    delay(100);
  }
  if (analogRead(yPin) < 20){  //Up
    Serial.println("Up");
    menu--;
    updateMenu();
    delay(100);
  }
  if (digitalRead(SW) == 0){     //Select
    Serial.println("Select");
    executeAction();
    updateMenu();
    delay(100);
  }
}

void updateMenu() {                                  // We'll use this one for displaying the Menu screen on LCD
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Ana Sayfa");
      lcd.setCursor(0, 1);
      lcd.print(" Ayarlar");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Ana Sayfa");
      lcd.setCursor(0, 1);
      lcd.print(">Ayarlar");
      break;
    case 3:
      lcd.clear();
      lcd.print(">Manuel Sulama");
      lcd.setCursor(0, 1);
      lcd.print(" Hakkinda");
      break;
    case 4:
      lcd.clear();
      lcd.print(" Manuel Sulama");
      lcd.setCursor(0, 1);
      lcd.print(">Hakkinda");
      break;
    case 5:
      menu = 4;
      break;
  }
}

void executeAction() {                               // We have used this one in the void Menu.. 
  switch (menu) {
    case 1:
      HomePage();
      break;
    case 2:
      SecondScreen();
      break;
    case 3:
      ThirdScreen();
      break;
    case 4:
      About();
  }
}

void HomePage() {                          
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("VobiWatering");

  delay(300);
  if(digitalRead(SW) == 0) //This one is for exit from the page
    updateMenu();
  else
    HomePage();
}

void SecondScreen() {
  // In this screen, we sets the settings.
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Aralik: ");
  lcd.print(defHours);
  lcd.print(" saat");

  lcd.setCursor(0,1);
  lcd.print("S. Suresi: ");
  lcd.print(defWateringDuration);
  lcd.print(" sn");
  
  Settings();
  
  if(digitalRead(SW) == 0) //This one is for exit from the page
    updateMenu();
  else
    SecondScreen();

  delay(300);
}

void ThirdScreen() {
  // This screen is for manual watering, user press the button and the pump works
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sulamak icin");
  lcd.setCursor(0,1);
  lcd.print("yukari");

  ManualWatering();

  delay(300);

  if(digitalRead(SW) == 0) //This one is for exit from the page
    updateMenu();
  else
    ThirdScreen();
}

void About(){
  //This screen displays about the project. Just owner's info :D
  lcd.clear();
  lcd.print("Furkan Simsekli");
  lcd.setCursor(5,1);
  lcd.print("@iamvobi");
  
  delay(300);
  
  if(digitalRead(SW) == 0) //This one is for exit from the page
    updateMenu();
  else
    About();
}

void Settings(){                                  //User will set the interval time and the watering duration with 1x4 KeyPad 
  // increasing or decrasing the levels
                                                                      
     if(analogRead(yPin) < 20){
       defHours++;
     }
     if(analogRead(yPin) > 900){
       defHours--;
        if(defHours == 0)
          defHours == 1;
     }
     if(analogRead(xPin) > 900){
      defWateringDuration++;
     }
     if(analogRead(xPin) < 200){
      defWateringDuration--;
      if(defWateringDuration == 0)
          defWateringDuration == 1;
     }          
     delay(100);
}

void ManualWatering(){                     // User can water the plants whenever he/she wants, this is for it 
 
     if(analogRead(yPin) < 20){
      digitalWrite(pump,LOW);
      Serial.print("Sulanıyor..");
     }
     else
      digitalWrite(pump, HIGH);
     delay(400);
}

void DefaultWatering(){                    // This is the point of the project. Other ones are improvement. 
  if(millis()-lastTime >= defHours*3600*1000){
    digitalWrite(pump,LOW);
    delay(defWateringDuration*1000);
    lastTime = millis(); 
  }else
    digitalWrite(pump,HIGH);
}
