#include <Keypad.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define red 13
#define green 11
#define blue 12

#define servo 10

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

Servo servo1;

const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte row[rows] = {9,8,7,6};
byte col[cols] = {5,4,3,2};

Keypad keypad = Keypad(makeKeymap(keys), row, col, rows, cols);

String pass = "";
bool isASetPass = false;
int trangthai = 0;

void setup() {
  Serial.begin(9600);  
  lcd.begin(16, 2);
  
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  servo1.attach(servo);
   analogWrite(red, 0);
  analogWrite(green, 0);
  analogWrite(blue, 0);
  lcd.print("No Password");
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    if (!isASetPass && key == 'A') {
      lcd.clear();
      lcd.print("Dat mat khau:");
      setpass();
    }
    else if (isASetPass && key == 'B') {
      lcd.clear();
      lcd.print("Nhap mat khau:");
      checkpass();
    }
    else if (trangthai == 3 && key == 'C') {
      lock();
    }
  }
}

void setpass() {
  Serial.println("Nhap mat khau lan 1:");
  while (!Serial.available()) {}
  String p1 = Serial.readStringUntil('\n');
  lcd.clear();
  lcd.print(p1);
  
  Serial.println("Nhap lai mat khau:");
  while (!Serial.available()) {}
  String p2 = Serial.readStringUntil('\n');
  lcd.clear();
  lcd.print(p2); 
  
  if (p1 == p2) {
    pass = p1;
    isASetPass = true;
    lock();
  } else {
    lcd.clear();
    lcd.print("Khong khop!");
    delay(2000);
    lcd.clear();
    lcd.print("Khong co mat khau");
  }
}

void checkpass() {
  int i = 3;
  
  while (i--) {
    Serial.println("Nhap mat khau:");
    while (!Serial.available()) {}
    String input = Serial.readStringUntil('\n');
    lcd.clear();
    lcd.print(input);
    delay(1000);
    
    if (input == pass) {
      unlock();
      return;
    } else {
      lcd.clear();
      lcd.print("Sai! Con ");
      lcd.print(i);
      lcd.print(" lan");
      delay(500);
    }
  }
  
  canhbao();
}

void lock() {
  trangthai = 1;
  servo1.write(90);
   analogWrite(red, 255);
  analogWrite(green, 255);
  analogWrite(blue, 255);
  lcd.clear();
  lcd.print("Da khoa");
}

void unlock() {
  trangthai = 2;
  servo1.write(180);
   analogWrite(red, 0);
  analogWrite(green, 255);
  analogWrite(blue, 0);
  lcd.clear();
  lcd.print("Da mo khoa");
  delay(10000);
  lock();
}

void canhbao() {
  trangthai = 3;
  analogWrite(red, 255);
  analogWrite(green, 0);
  analogWrite(blue, 0);
  lcd.clear();
  lcd.print("CANH BAO!!!");
}


