#include <reg51.h> 
#include "lcd.h"  	 
#include "ds1307.h" 
#include "eeprom.h"
#include "i2c.h"
#include "delay.h"

unsigned char cnt;
 
/* main program */
void main() 
{
   unsigned char eeprom_address=0x00, read_string[15], med_name[15];
   unsigned char sec,min,hour,day,month,year;
 
  /* Initilize the lcd */
    lcd_Init();
 
  /* Initilize the RTC(ds1307)*/
    ds1307_Init();
 
 
	EEPROM_WriteString(eeprom_address,"10",0);
	EEPROM_WriteString(eeprom_address,"Paracetamol",1);
	

 	 /*set the time and Date only once */
 	 ds1307_SetTime(0x09,0x59,0x55);  //  
 	 ds1307_SetDate(0x15,0x07,0x20);  //  


  	 

 	 /* Display "time" on first line*/
 	 lcd_DisplayString("Time: ");
 
   /* Display the Time and Date continously */ 
   while(1)
    {
	   /* Read the Time from RTC(ds1307) */ 
        ds1307_GetTime(&hour,&min,&sec);        
 
		/* Display the time on firstline 7th position*/
 
         lcd_GoToXY(0,6); 		 
         lcd_DisplayRtcTime(hour,min,sec);	 


		for(cnt = 0; cnt<8; cnt+=2)
		{		 		
			EEPROM_ReadString(eeprom_address,read_string,cnt);	// Read the String from memory Location 0x00
			if((((hour>>4) & 0x0f) + 0x30 == read_string[0]) && ((hour & 0x0f) + 0x30 == read_string[1]))
			 {
				  lcd_GoToXY(1,2);
				  EEPROM_ReadString(eeprom_address,med_name,cnt);
				  lcd_DisplayString(med_name);
			 }
			delay_sec(2);
		}









		 
		 

     }

}