#include <string.h>
#include <iarduino_RTC.h>
#include <EEPROM.h> //библиотека для работы с энергонезависимой памятью
iarduino_RTC time(RTC_DS1307);
 
char unitID_in[10];
char command_in[20];
char data_in[100];
//Переменные для записи даты/время в микросхему DS1307
char s[10];
char m[10];
char h[10];
char d[10];
char M[10];
char y[100];
char w[100];
//Переменные для записи даты/время в микросхему DS1307

//Переменные для установки включения и выключения по времени
int set_on_hh;
int set_on_mm;
int set_off_hh;
int set_off_mm;

const int keyPin = 3;
const int ledPin = 13;
int flag = 2;
//Переменные для установки включения и выключения по времени
 
void setup() {
  // открываем порт 
    delay(300);
    Serial.begin(9600);
    time.begin();
    //time.settime(0,21,16,31,01,18,3);  // 0  сек, 51 мин, 21 час, 27, октября, 2015 года, вторник
    //time*03,30,14,21,03,18,3*COMMM

    pinMode(keyPin,INPUT);
    pinMode(ledPin,OUTPUT);
}
 
void loop() {    
/*Начало Включение выключение с кнопки*/
        if(digitalRead(keyPin) ==HIGH && flag == 2)
        {
        digitalWrite(2,HIGH);//подаем питание в 13 контакт
        if(flag == 2){flag = 1;}
        delay(500);
        }
        else if(digitalRead(keyPin) ==HIGH && flag == 1)
        {
        digitalWrite(2,LOW);
        if(flag == 1){flag = 2;}
        delay(500);
        }
/*Конец Включение выключение с кнопки*/

/*Начало Включение и выключение по времени*/

//*************
if(millis()%1000==0){
  Serial.println(time.gettime("d-m-Y, H:i:s, D"));

set_on_hh = EEPROM.read(0); //Считываем из памяти данные
set_on_mm = EEPROM.read(1);
set_off_hh = EEPROM.read(2);
set_off_mm = EEPROM.read(3);
/*Проверка чтения из энергосберегающей памяти
Serial.println(set_on_hh);
Serial.println(set_on_mm);
Serial.println(set_off_hh);
Serial.println(set_off_mm);
*/
      if (time.Hours==set_on_hh && time.minutes==set_on_mm && time.seconds==0){
              digitalWrite(2, HIGH);
              
              }
      if (time.Hours==set_off_hh && time.minutes==set_off_mm && time.seconds==0){
              digitalWrite(2, LOW);
              
              }
        delay(2);
                     }

/*Конец Включение и выключение по времени*/

  int i=0;
  char buffer[100];
/* 
//Резервная копия кода
//если есть данные - считаем их
  if(Serial.available()){
     delay(100);
//сохраним прочитанное в буфер
     while( Serial.available() && i< 99) { buffer[i++] = Serial.read(); } //закрываем массив buffer[i++]='\0'; } //если вдруг буфер наполнен if(i>0){  
//разобьем его на части, отделенные двоеточием
    sscanf(buffer, "%[^'*']*%[^'*']*%s", &unitID_in, &command_in, &data_in);
  }
 //Резервная копия кода
 */

//если есть данные - считаем их
  if(Serial.available()){
     delay(100);
//сохраним прочитанное в буфер
     while( Serial.available() && i< 99) { buffer[i++] = Serial.read(); } //закрываем массив buffer[i++]='\0'; } //если вдруг буфер наполнен if(i>0){  
//разобьем его на части, отделенные двоеточием
    sscanf(buffer, "%[^'*']*%[^'*']*%s", &unitID_in, &command_in, &data_in);
    //sscanf(buffer, "%[^'*']*%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^'*']*%s", &unitID_in, &s, &m, &h, &d, &M, &y, &w, &command_in);
    if ((String)unitID_in == "time"){
    sscanf(command_in, "%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%s", &s, &m, &h, &d, &M, &y, &w); //Разбираем переменную char для создания даты/время
    }

    /*Начало Получение данных с COM порта для Включение и выключение по времени*/
    if ((String)unitID_in == "timer"){
      sscanf(command_in, "%[^','],%[^','],%[^','],%s", &s, &m, &h, &d); //Разбираем переменную char для создания включения выключения 
    }
    /*Конец Получение данных с COM порта для Включение и выключение по времени*/
  }

//Исполнительная часть программы
//Проверяем, к какому устройству пришли данные
 
//Тестовое устройство 001
    if ((String)unitID_in == "001"){  //test serial read
      Serial.print("001,arduino recive: unit ");
      Serial.print(unitID_in);
      Serial.print("\n");
      Serial.print("command: ");
      Serial.print(command_in);
      Serial.print("\n");
      Serial.print("data: ");
      Serial.print(data_in);      
      Serial.print("\n");
      unitID_in [0] = '\0'; 
      command_in [0] = '\0';   
      data_in [0] = '\0';         
    }
 
//пример выполнения команды устройством 013
     if ((String)unitID_in == "led"){
 
      if ((String)command_in == "on"){
          digitalWrite(2, HIGH);}
      if ((String)command_in == "off"){
          digitalWrite(2, LOW);}   
 /*
      Serial.print("013,arduino recive: unit ");
      Serial.print(unitID_in);
      Serial.print(" command ");
      Serial.print(command_in);
      Serial.print("\n");*/
      unitID_in [0] = '\0';
      command_in [0] = '\0';     
    }
//пример выполнения команды устройством time
     if ((String)unitID_in == "time"){
      //преобразовываем char переменные в int
      int ss = atoi(s);
      int mm = atoi(m);
      int hh = atoi(h);
      int dd = atoi(d);
      int MM = atoi(M);
      int yy = atoi(y);
      int ww = atoi(w);
      
      time.settime(ss,mm,hh,dd,MM,yy,ww); // устанавливаем дату
      unitID_in [0] = '\0';
      command_in [0] = '\0';
   }

    /*Начало Выполнение команды устройством timer*/
           if ((String)unitID_in == "timer"){
      //преобразовываем char переменные в int
      EEPROM.update(0, atoi(s));
      EEPROM.update(1, atoi(m));
      EEPROM.update(2, atoi(h));
      EEPROM.update(3, atoi(d));
     
      unitID_in [0] = '\0';
      command_in [0] = '\0';
   }
    /*Конец Выполнение команды устройством timer  timer*12,30,12,40**/
   
   /* 
    if(millis()%1000==0){ // если прошла 1 секунда
      Serial.println(time.gettime("d-m-Y, H:i:s, D")); // выводим время
      delay(1); // приостанавливаем на 1 мс, чтоб не выводить время несколько раз за 1мс
   }*/
   
   if((String)unitID_in == "volume"){
    Serial.println(time.gettime("d-m-Y, H:i:s, D")); // выводим время
    delay(1); // приостанавливаем на 1 мс, чтоб не выводить время несколько раз за 1мс
      unitID_in [0] = '\0';
      command_in [0] = '\0'; 
    }




}
