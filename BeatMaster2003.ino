//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
long t_r, t_d, now;
int t_beat;
const int base = 30000;
byte bpm;
byte mem, oldBPM;
boolean rclPressed;
boolean pressed;
byte sigPin = 12;
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnPause  0
#define btnUP     1
#define btnDOWN   2
#define btnRCL   3
#define btnSTORE 4
#define btnNONE   5

void raise(long t_now){
  digitalWrite(sigPin, HIGH);
  t_r = t_now + t_beat;
  t_d = t_now + 15;
  pressed = !pressed;
}

void drop(){
  digitalWrite(sigPin, LOW);
}
// read the buttons
int read_LCD_buttons()
{
  adc_key_in = analogRead(0);      // read the value from the sensor 
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adc_key_in < 50)   return btnPause;  
  if (adc_key_in < 250)  return btnUP; 
  if (adc_key_in < 450)  return btnDOWN; 
  if (adc_key_in < 650)  return btnRCL; 
  if (adc_key_in < 850)  return btnSTORE;  
  return btnNONE;  // when all others fail, return this...
}

void setBPM(byte _bpm)
{
  if(!pressed)
  {
    bpm = _bpm;
    lcd.setCursor(5,1);
    if(bpm < 100)
    {
      lcd.print(" ");
    }
    lcd.print(bpm);
    t_beat = base / bpm;
    pressed = true;
  }
}

void storeBPM(byte _bpm)
{
  mem = _bpm;
  lcd.setCursor(11,1);
  if(_bpm < 100){
    lcd.print(" ");
  }
  lcd.print(_bpm);
}

void setup()
{
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0,0);
  lcd.print("BeatMaster 2003"); // print a simple message
  lcd.setCursor(0,1);
  lcd.print("BPM:");
  lcd.setCursor(5,1);
  pinMode(sigPin, OUTPUT);
  bpm = 120;
  mem = bpm;
  t_beat = base / bpm;
  lcd.print(bpm);
}

void loop()
{
  now = millis();
  if (now >= t_r){
    raise(now);
  }
  if (now >= t_d){
    drop();
  }
  // move to the begining of the second line
  lcd_key = read_LCD_buttons();  // read the buttons

    switch (lcd_key)               // depending on which button was pushed, we perform an action
  {
  case btnPause:
    {
      pressed = true;
      t_r = millis() + 5;
      break;
    }
  case btnRCL:
    {    
      if(!rclPressed){
        oldBPM = bpm;
        setBPM(mem);
        storeBPM(oldBPM);
      }
      rclPressed = true;
      break;
    }
  case btnUP:
    {
      setBPM(bpm + 1);
      break;
    }
  case btnDOWN:
    {
      setBPM(bpm - 1);
      break;
    }
  case btnSTORE:
    {
      storeBPM(bpm);
      break;
    }
  case btnNONE:
    {
      if(pressed)
      {
        raise(millis());
      }
      rclPressed = false;
      pressed = false;
      break;
    }
  }

}





