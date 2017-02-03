/*
 * semester_day_counter.c
 *
 * Created: 04-Jun-16 7:25:26 PM
 *  Author: SJ
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

#define delay _delay_ms
 
int word[5]={0x88,0x83,0xC6,0xA1,0x86};
int digit[10]={192,249,164,176,153,146,130,248,128,144};

int rbC[5] = {0x50,0x7C,6,63,6};
int rbA[5] = {0xAF,0x83,249,192,249};

//int word[6]={0x77,0x7C,0x39,0x5E,0x79,0x71}; //common cathode
//int digit[10]={63,6,91,79,102,109,125,7,127,111};
	
int week, day;

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE)) ;
	
	/* Set up address and data registers */
	EEAR = uiAddress;
	EEDR = ucData;

	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE))
	;
	
	/* Set up address register */
	EEAR = uiAddress;
	
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from data register */
	return EEDR;
}

void init_led()
{
	PORTB = 0xFF;
	PORTD = 0;
}

void janu()
{
	init_led();
	
			PORTB = digit[week/10];
			PORTD = 0b00000001;
			//PORTD = 0b11111110;
			delay(7);
			
    init_led();
			PORTB = digit[week%10];
			PORTD = 0b00000010;
			//PORTD = 0b11111101;
			delay(7);
			
	init_led();
			PORTB = word[day];
			PORTD = 0b00000100;
	//		PORTD = 0b11111011;
			delay(7);
			
}

void show_off()
{
	init_led();
	//null
	PORTD = 0;
	PORTB = 0;
	_delay_ms(500);
	
	
	//r
	init_led();
	
	PORTD = 0b0000100 ;
	PORTB = rbA[0];
	_delay_ms(500);
	
	//rb
	int i = 50;
	while(i--)
	{
		init_led();
		PORTD = 0b00000100 ;
		PORTB = rbA[1];
		_delay_ms(5);
		
		init_led();
		PORTD = 0b00000010 ;
		PORTB = rbA[0];
		_delay_ms(5);
	}
	
	//rb1
	i = 33 ;
	while(i--)
	{
		init_led();
		PORTD = 0b00000100 ;
		PORTB = rbA[2];
		_delay_ms(5);
		
		init_led();
		PORTD = 0b00000010 ;
		PORTB = rbA[1];
		_delay_ms(5);
		
		init_led();
		PORTD = 0b00000001 ;
		PORTB = rbA[0];
		_delay_ms(5);
	}
	
	//b10
	i = 33 ;
	while(i--)
	{
		init_led();
		//PORTD = 0b11111011 ;
		PORTD = 0b00000100 ;
		PORTB = rbA[3];
		_delay_ms(5);
		
		init_led();
		//PORTD = 0b11111101 ;
		PORTD = 0b00000010 ;
		PORTB = rbA[2];
		_delay_ms(5);
		
		init_led();
		//PORTD = 0b11111110 ;
		PORTD = 0b00000001 ;
		PORTB = rbA[1];
		_delay_ms(5);
	}
	
	//101
	i = 33 ;
	while(i--)
	{
		init_led();
		//PORTD = 0b11111011 ;
		PORTD = 0b00000100 ;
		PORTB = rbA[4];
		_delay_ms(5);
		
		init_led();
		//PORTD = 0b11111101 ;
		PORTD = 0b00000010 ;
		PORTB = rbA[3];
		_delay_ms(5);
		
		init_led();
		//PORTD = 0b11111110 ;
		PORTD = 0b00000001 ;
		PORTB = rbA[2];
		_delay_ms(5);
	}
	
	//01
	i = 50 ;
	while(i--)
	{
		init_led();
		//PORTD = 0b11111101 ;
		PORTD = 0b00000010 ;
		PORTB = rbA[4];
		_delay_ms(5);
		
		init_led();
		//PORTD = 0b11111110 ;
		PORTD = 0b00000001 ;
		PORTB = rbA[3];
		_delay_ms(5);
	}
	
	//1
	i = 100 ;
	while(i--)
	{
		init_led();
		//PORTD = 0b11111110 ;
		PORTD = 0b00000001 ;
		PORTB = rbA[4];
		_delay_ms(5);
	}
	
	init_led();
	//null
	PORTD = 0;
	PORTB = 0;
	_delay_ms(500);
}


int main(void)
{
	MCUCSR |= (1 << JTD); //initialization
	MCUCSR |= (1 << JTD);
	
	DDRB = 255;
	DDRD = 255;
	DDRA = 0;
	
	int i = 0;
	
	week = EEPROM_read(100);
	day = EEPROM_read(101);
	
	if(week > 99) week = 1;
	if(day > 4) day = 0;
	
	unsigned long long meow = 0;
	
	show_off();
	
	
    while(1)
    {
		for(i = 0 ; i < 12 ; i++) 
			janu();
	  
      if(PINA & (1 << PA0) && (PINA & (1 << PA1)))
	  {
		  show_off();
		  week = 1;
		  day = 0;
		  EEPROM_write(100,week);
		  EEPROM_write(101,day);
		  
	  }
	  
	  else if(PINA & (1 << PA0) && !(PINA & (1 << PA1)))
	  {
		  day++;
		  //janu();
		  
		  if(day > 4)
		  {
			week++;
			day = 0;
		  }  
		  
		  		  EEPROM_write(100,week);
		  		  EEPROM_write(101,day);
	  }
	  
	  else if(PINA & (1 << PA1) &&  !(PINA & (1 << PA0)) )
	  {
		  day--;
		  
		  if(day < 0)
		  {  
			  if(week <= 1)
			  {
				  week = 1;
				  day = 0;
			  }
			  else
			  {
				  week--;
				  day = 4;
			  }
		  }
		  
			  		  EEPROM_write(100,week);
			  		  EEPROM_write(101,day);
	  }
	  
	  meow++;
	  if(meow >= 14400)
	  {
		  meow = 0;
		  show_off();
	  }
	  
	}
}