/*
 * Thermistor.cpp
 *
 * Created: 2013-12-30 00:54:44
 *  Author: rexina
 */ 
#include "MicroSat.h"


#include "LED.h"


 void Thermistor_::init()
 {
	 this->Avg = 0;
	 this->SamplesCounter = 0;
	 this->NewData = false;
	 
	 
	 Timer1.begin(Presc1); //~168Hz
 }
 
  void Thermistor_::main_init()
  {
	  DDR_THERM &= ~_BV(PIN_THERM);
	  DDR_THERM_ON |= (1 << PIN_THERM_ON);
	  
	  this->Avg = 0;
	  this->SamplesCounter = 0;
	  this->NewData = false;
	  
	  ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0);
	  //ADCSRA &= ~(1 << ADEN);
	  ADCSRA |= (1 << ADIE);
	  DIDR0 |= (1 << ADC7D);	  
  }
  
  
 void Thermistor_::new_measurement(uint16_t val)
 {
	 this->Avg += val;
	 ++this->SamplesCounter;
	 
	 if( this->SamplesCounter > this->AVG_SAMPLES )
	 {
		 ADCSRA &= ~(1 << ADEN);
		 Timer1.end();
		 Interrupt = true;
		 this->NewData = true;
	 }
 }
 
 void Thermistor_::end()
 {
	 //[todo] - write data to SD
	 this->disable();
	 this->NewData = false;
	 //SD.write_data('A',(uint16_t)this->Avg);
	 
	 sprintf(buffer,"A%d\n",(uint16_t)this->Avg);
	 SD.write_buffer();
 }
 
 
 Thermistor_ Thermistor;
 