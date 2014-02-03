//Carga de interrupción para modo dormido
#include <avr/sleep.h>
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//Declaración de pines
int led1 = 0; //LED izquierdo en modulo SMD 5050
int led2 = 1; //LED dererecho
int led3 = 2; //LED central
int sensor = A3; //Pin de sensado de resistencia

//Variables de intensidad de brillo y tiempo
int brillo = 0;
int cantBrillo = 2;
int limBrillo = 255;
int ritmo = 500;

//Variables de modo dormido
long tiempo=0;
int intervalo=5000;

void setup()  
{ 
  //Definición de pines de LED como salida
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  sbi(GIMSK,PCIE); // Turn on Pin Change interrupt
  sbi(PCMSK,PCINT3); // Which pins are affected by the interrupt
} 

void loop()  
{
  int amor = analogRead(sensor); //Sensado de amor :D
  
  if(amor<950) //Si el amor supera este nivel
  {
    ritmo=map(amor,0,900,100,500); //convertir a valores de rimo
    tiempo=millis();
  } 
  else
  {
    if(millis()-tiempo>intervalo) system_sleep(); 
    return; //Reiniciar programa
  }
  
  for( brillo = 0; brillo < limBrillo; brillo = brillo + cantBrillo )//Incremento de brillo
  {
    analogWrite(led1, brillo);    
    analogWrite(led2, brillo);
    delay(ritmo/100);
  }
  for( brillo = limBrillo; brillo > 0; brillo = brillo - cantBrillo )//Decremento de brillo
  {
    analogWrite(led1, brillo);    
    analogWrite(led2, brillo);
    delay(ritmo/100);
  }
  
  delay(ritmo/3);
  digitalWrite(led3,HIGH);delayMicroseconds(80);digitalWrite(led3,LOW);
  delay(ritmo/3);
  digitalWrite(led3,HIGH);delayMicroseconds(50);digitalWrite(led3,LOW);
  delay(ritmo/3); 
}

//Rutinas de modo "dormido"
void system_sleep() 
{
  cbi(ADCSRA,ADEN); // Apagar convertidor analógico/digital
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Seleccionar modo Sleep
  sleep_mode(); // Activar modo dormir
  sbi(ADCSRA,ADEN);  // Activar ADC cuando despierte
}
ISR(PCINT3_vect) {} 

