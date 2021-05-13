#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>
 

 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#define SS_PIN 10 //RFID
#define RST_PIN 9 //RFID
#define trigPin 6 //Distance sensor
#define echoPin 7 //Distance Sensor
const uint8_t I2C_ADDRESS =0x27;
const uint8_t LCD_CHAR= 16;
const uint8_t LCD_ROW= 2;
float duration, distance;
float tmp;

LiquidCrystal_I2C lcd(I2C_ADDRESS, LCD_CHAR,LCD_ROW);
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  
  if (!mlx.begin()) //Begin returns 0 on a good init
  {
      lcd.print("MLX90614 Failed");
      lcd.setCursor(0,1);
      lcd.print("check wiring!");      
    while (1)
      ;
  }  
  Wire.begin();
  lcd.begin();
  lcd.backlight();  
      lcd.print("Temp Screaning");
      lcd.setCursor(0,1);
      lcd.print("Scan Card.");          
      delay(1000);       
      clearCharacters(LCD_ROW-1,0, LCD_CHAR-1);
   Serial.println("CLEARDATA");
   Serial.println("LABEL,Time,Name,Temperature");
  
}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  //Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
//Show UID on serial monitor
  //Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  //Serial.println();
  //Serial.print("Message : ");
  content.toUpperCase();
 

//Conditions
 if (content.substring(1) == "69 1A FA A2")  
  { delay(5000);
    temp(); 
     Serial.print("DATA,TIME,Shubham,");
     Serial.println(tmp);
  }else if (content.substring(1) == "14 59 C8 40")
  {delay(5000);
     temp();
     Serial.print("DATA,TIME,Abhijeet,");
     Serial.println(tmp);
  } else{
    clearCharacters(LCD_ROW-1,0, LCD_CHAR-1);
    lcd.setCursor(0,1);
    lcd.print("Card Not Verfid"); 
    delay(5000);
    clearCharacters(LCD_ROW-1,0, LCD_CHAR-1);
    lcd.setCursor(0,1);
    lcd.print("Rescan");
    delay(5000);
    clearCharacters(LCD_ROW-1,0, LCD_CHAR-1);
     
   }
}
void clearCharacters(uint8_t row,uint8_t start, uint8_t stop )
{
    for (int i=start; i<=stop; i++)
    {
    lcd.setCursor (i,row); //  
    lcd.write(254);
    } 

}
void temp()
{
//condition for Distance sensor
    // Write a pulse to the HC-SR04 Trigger Pin
  
   digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    // Measure the response from the HC-SR04 Echo Pin
 
    duration = pulseIn(echoPin, HIGH);
  
    // Determine distance from duration
    // Use 343 metres per second as speed of sound
  
    distance = (duration / 2) * 0.0343;
  
    // Send results to Serial Monitor
 
     
    if (distance <= 15) {
            clearCharacters(LCD_ROW-1,0, LCD_CHAR-1);
            tmp=mlx.readObjectTempC();
            lcd.setCursor(0,1);
            lcd.print("TEMP");  
            lcd.print(" ");
            lcd.print(tmp);        
            lcd.print(" ");
            lcd.print("C"); 
            delay(5000);
            clearCharacters(LCD_ROW-1,0, LCD_CHAR-1);

        
    }
    else {
      clearCharacters(LCD_ROW-1,0, LCD_CHAR-1);
          lcd.setCursor(0,1);
          lcd.print("Away ReScan Cad"); 
          delay(5000);
          clearCharacters(LCD_ROW-1,0, LCD_CHAR-1);

    }
    delay(500);
}
