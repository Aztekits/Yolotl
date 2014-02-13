/*  
    Firmware para kit electrónico #YOLOTL, para ser instalado en ATtiny85
    Utilizar core: http://highlowtech.org/?p=1695 ó https://code.google.com/p/arduino-tiny/
    Aztekits - Electrónica Inteligente
    aztekits.cc
*/

//Carga de interrupción para modo dormido
#include <avr/sleep.h>
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//Declaración de pines
int led1 = 0;
int led2 = 1;
int led3 = 2;
int sensor = A2;

//Variables de intensidad de brillo y tiempo
int brillo = 0;
int cantBrillo = 2;
int limBrillo = 255;
int ritmo;

//Variables de modo dormido
long tiempo=0;
int intervalo=5000;

void setup()  
{ 
  //Definición de pines de LED como salida
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  sbi(GIMSK,PCIE); // Habilitar interrupción en modo CHANGE
  sbi(PCMSK,PCINT4); // Pin afectado por interrupción
} 

void loop()  
{
  int amor = analogRead(sensor); //Sensado de amor :D
  
  if(amor<1000)
  {
    PCMSK &= ~(1<<PCINT4);   // Desabilitar interrupción
    ritmo=map(amor,0,1000,100,600); //convertir a valores de rimo
    tiempo=millis();
        
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
    digitalWrite(led3,HIGH);delayMicroseconds(ritmo/5);digitalWrite(led3,LOW);
    delay(ritmo/3);
    digitalWrite(led3,HIGH);delayMicroseconds(ritmo/6);digitalWrite(led3,LOW);
    delay(ritmo/3); 
  }
  else
  {
    sbi(PCMSK,PCINT4); // Turn on Pin Change interrupt
    if(millis()-tiempo>intervalo) system_sleep(); 
    return; //Reiniciar programa
  }
}

//Rutinas de modo "dormido"
void system_sleep() 
{
  cbi(ADCSRA,ADEN); // Apagar convertidor analógico/digital
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Seleccionar modo Sleep
  sleep_mode(); // Activar modo dormir
  sbi(ADCSRA,ADEN);  // Activar ADC cuando despierte
}
ISR(PCINT3_vect) {} // Desabilitar interrupciones 

