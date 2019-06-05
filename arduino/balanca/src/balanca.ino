/**
 * @file: balanca.ino
 * @author: Lucas Martins Mendes
 * @brief: a fixed point implementation for a simple weight scale using a rolling average
 *
 */

/*--------- Includes ---------*/
#include "HX711.h" // Biblioteca  HX711
#include <Arduino.h>

/*---------- Macros ----------*/

#define DEBUG 0

#define VAR_DUMP(x) \
Serial.print(" " #x ": ");\
Serial.print(x)

#define VAR_DUMPN(x) \
Serial.print(" " #x ": ");\
Serial.println(x)

/*----------- Constants --------------*/

#define DOUT  A0                      // HX711 DATA OUT = pino A0 do Arduino
#define CLK  A1                       // HX711 SCK IN = pino A1 do Arduino
#define DATA_PT_SIZE 25
#define CONV_FACTOR 1000.0
#define DEC_SHIFT 100.0

#define READING_MAX 100000

/*--------- Variables ----------*/

HX711 scale;
float calibration_factor = 451977.3217;//2*228110.00;   // fator de calibração para teste inicial

String command, arg;

int32_t data_pts[DATA_PT_SIZE];
int32_t data_sum = 0;
int32_t pos = 0;
int32_t avgi=0, reading=0;

/*--------- Functions --------*/
inline void tare()
{
  scale.tare(10);   // zera a Balança
  //Serial.println("Balança tarada");
  for(int i=0;i<DATA_PT_SIZE;i++)
    {
      data_pts[i] = 0;
    }
    data_sum=0;
}

/*---------- Setup -----------*/
void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  scale.begin(DOUT, CLK,128);

  //Serial.println("\n\nParvus Metiri \n remova o qualquer massa da balanca\n envie t para tarar e +/- <valor> para ajustar constante");

  scale.set_scale(calibration_factor);   // configura a escala da Balança
  for(int i=0;i<DATA_PT_SIZE;i++)
    {
      data_pts[i] = 0;
    }
  delay(1000);
  tare();

}
/*---------- Loop ----------*/
void loop()
{
  if(Serial.available())
  {
    //delay(1000);
    float adj = 0;
    command = Serial.readStringUntil('\n');
    arg = command.substring(2);
    switch(command.substring(0).c_str()[0])
    {
        case '+':
          calibration_factor += arg.toFloat();
          break;
        case '-':
          calibration_factor -= arg.toFloat();
          break;
        case 't':
          tare();   // zera a Balança
          break;
        /*case 'r':
          Serial.println(avgi/DEC_SHIFT);
          //Serial.print(" g");
          break;*/
        default:
          Serial.println("Comando inválido");
      }
      scale.set_scale(calibration_factor);
    }
    /*this is a fixed point moving average implementation*/
    reading = round(-scale.get_units()*CONV_FACTOR*DEC_SHIFT); /*CONV_FACTOR converts the input to grams and dec shift "shifts" the number to the left 2 places*/

    if(reading < (READING_MAX*DEC_SHIFT)) /*remove aberration values */
    {
      data_sum -= data_pts[pos];
      data_sum += reading;
      data_pts[pos]= reading;
      pos = pos+1 >= DATA_PT_SIZE ? 0 : pos + 1;
      avgi = data_sum/DATA_PT_SIZE;
      //Serial.print("      Fator de Calibração: ");   // imprime no monitor serial
      //S
      #if DEBUG == 1
      VAR_DUMP(avgi/DEC_SHIFT);
      VAR_DUMP(calibration_factor);
      VAR_DUMP(data_sum);
      VAR_DUMP(reading);
      VAR_DUMPN(avgi);
      #endif
      digitalWrite(13,0);
    }
    else
    {
      #if DEBUG == 1
      Serial.print("sensor error: ");
      Serial.println(reading);
      #endif
      digitalWrite(13, 1);
    }
    Serial.println(avgi/DEC_SHIFT,1);
  }
    //float avg = data_sum/(DEC_SHIFT);
    //avg = data_sum/DATA_PT_SIZE;


  /*
  float reading= -scale.get_units(20)*1000;

  Serial.println(reading, 3);
  //float reading=-scale.get_units()*1000
  */
/*
  balanca.set_scale(calibration_factor);   // ajusta fator de calibração
  Serial.print("Peso: ");   // imprime no monitor serial
  Serial.print(balanca.get_units(), 3);   // imprime peso da balança com 3 casas decimais
  Serial.print(" kg");
  Serial.print("      Fator de Calibração: ");   // imprime no monitor serial
  Serial.println(calibration_factor);   // imprime fator de calibração
}
*/
