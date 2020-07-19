/*---------------------------------------------------------------------------------------*
                                8051 At2404/02  library
Filename: eeprom.c
Controller: P89V51RD2/89c51(8051 family)
Oscillator: 11.0592 MHz
Author: ExploreEmbedded
website: www.ExploreEmbedded.com

 ----------------------------------------------------------------------------------------*/

#include<reg51.h>
#include"i2c.h"
#include"delay.h"


/* EEPROM_ID is fixed and should not be changed,
   Refer At2404/02 DataSheet for more info*/
#define EEPROM_ID 0xA0





/*---------------------------------------------------------------------------------------
  void EEPROM_WriteByte(unsigned char eeprom_Address, unsigned char eeprom_Data)
 ----------------------------------------------------------------------------------------
 * I/P Arguments: char,char-->eeprom_address at which eeprom_data is to be written.
 * Return value	: none

 * description:This function is used to write the data at specified EEPROM_address..
               At2404 ic is enabled by sending its ID on the i2c bus.
               After selecting At2404 ic,select the address where the data is to written
               Write the Data at selected EppromAddress
               Stop the I2c communication.
----------------------------------------------------------------------------------------*/
void EEPROM_WriteByte(unsigned char eeprom_Address, unsigned char eeprom_Data, unsigned char Page_Number)
{

    I2C_Start();               // Start i2c communication
   	I2C_Write(EEPROM_ID|(Page_Number<<1));	   // connect to AT2404 by sending its ID on I2c Bus
	I2C_Ack();
   	I2C_Write(eeprom_Address); // Select the Specified EEPROM address of AT2404
	I2C_Ack();
   	I2C_Write(eeprom_Data);    // Write the data at specified address
	I2C_Ack();
    I2C_Stop();           	   // Stop i2c communication after Writing the data
	delay_ms(5);               // Write operation takes max 5ms, refer At2404 datasheet 
}






/*-----------------------------------------------------------------------------------------
               unsigned char EEPROM_ReadByte(unsigned char eeprom_Address)
 ------------------------------------------------------------------------------------------
 * I/P Arguments: char-->eeprom_address from where eeprom_data is to be read.
 * Return value	: char-->data read from Eeprom.

 * description:
           This function is used to read the data from specified EEPROM_address.
           At2404 ic is enabled by sending its ID on the i2c bus.
           After selecting At2404 ic,select the address from where the data is to read
           Read the Data from selected EppromAddress
           Stop the I2c communication.
           Return the Data read from Eeprom
-----------------------------------------------------------------------------------------*/
unsigned char EEPROM_ReadByte(unsigned char eeprom_Address,unsigned char Page_Number)
{
  unsigned char eeprom_Data;

    I2C_Start();               // Start i2c communication
   	I2C_Write(EEPROM_ID|(Page_Number<<1));	   // connect to AT2404(write) by sending its ID on I2c Bus
	I2C_Ack();
   	I2C_Write(eeprom_Address); // Select the Specified EEPROM address of AT2404
    I2C_Ack();

    I2C_Start();		       // Start i2c communication
    I2C_Write(0xA1|(Page_Number<<1));           // connect to AT2404(read) by sending its ID on I2c Bus
    I2C_Ack();
	eeprom_Data = I2C_Read();  // Read the data from specified address
	I2C_NoAck();
    I2C_Stop();		           // Stop i2c communication after Reading the data
	delay_us(10);
  return eeprom_Data;          // Return the Read data

}






/*---------------------------------------------------------------------------------------
void EEPROM_WriteNBytes(unsigned char EepromAddr, unsigned char *RamAddr, char NoOfBytes)
 ----------------------------------------------------------------------------------------
 * I/P Arguments: char,-->eeprom_address from where the N-bytes are to be written.
                  char*-->Pointer to the N-bytes of data to be written.
                  char --> Number of bytes to be written

 * Return value	: none

 * description:
            This function is used to write N-bytes of data at specified EEPROM_address.
            EEPROM_WriteByte() function is called to write a byte at atime.
            Source(RAM) and destination(EEPROM) address are incremented after each write
            NoOfBytes is Decemented each time a byte is written.
            Above Operation is carried out till all the bytes are written(NoOfBytes!=0)
---------------------------------------------------------------------------------------*/
void EEPROM_WriteNBytes(unsigned char EepromAddr, unsigned char *RamAddr, char NoOfBytes, unsigned char Page_Number)
 {
  while(NoOfBytes !=  0)
   {
	EEPROM_WriteByte(EepromAddr,*RamAddr,Page_Number); //Write a byte from RAM to EEPROM
	    EepromAddr++;					   //Incerement the Eeprom Address
		RamAddr++;						   //Increment the RAM Address
		NoOfBytes--;					   //Decrement NoOfBytes after writing each Byte
	   }
 }



/*---------------------------------------------------------------------------------------
void EEPROM_ReadNBytes(unsigned char EepromAddr, unsigned char *RamAddr, char NoOfBytes)
 ----------------------------------------------------------------------------------------
 * I/P Arguments: char,-->eeprom_address from where the N-bytes is to be read.
                  char*-->Pointer into which the N-bytes of data is to be read.
                  char --> Number of bytes to be Read

 * Return value	: none

 * description:
            This function is used to Read N-bytes of data from specified EEPROM_address.
            EEPROM_ReadByte() func is called to read a byte at a time.
            Source(RAM) and destination(EEPROM) address are incremented each time.
            NoOfBytes is Decemented after a byte is read.
            Above Operation is carried out till all the bytes are read(NoOfBytes!=0)
---------------------------------------------------------------------------------------*/
void EEPROM_ReadNBytes(unsigned char EepromAddr, unsigned char *RamAddr, char NoOfBytes,unsigned char Page_Number)
 {
   while(NoOfBytes !=  0)
	{
	 *RamAddr = EEPROM_ReadByte(EepromAddr,Page_Number);//Read a byte from EEPROM to RAM
	    EepromAddr++;						//Incerement the Eeprom Address
	 	RamAddr++;							//Increment the RAM Address
	 	NoOfBytes--;						//Decrement NoOfBytes after Reading each Byte

	 }
 }







/*---------------------------------------------------------------------------------------
 void EEPROM_WriteString(unsigned char eeprom_address, unsigned char * source_address)
 ----------------------------------------------------------------------------------------
 * I/P Arguments: char,-->eeprom_address where the String is to be written.
                  char*-->Pointer to String which has to be written.

 * Return value	: none

 * description:This function is used to Write a String at specified EEPROM_address.
               EEPROM_WriteByte() function is called to write a byte at a time.
               Source(RAM) and destination(EEPOM) address are incremented each time.
               Above Operation is carried out till Null char is identified.

   NOTE: Null char is also written into the eeprom.
---------------------------------------------------------------------------------------*/
void EEPROM_WriteString(unsigned char eeprom_address, unsigned char * source_address,unsigned char Page_Number)
  {

   do
 	{
 	  EEPROM_WriteByte(eeprom_address,*source_address,Page_Number); //Write a byte from RAM to EEPROM
 	    source_address++;								//Incerement the RAM Address
 		eeprom_address++;								//Increment the Eeprom Address
 	  }while(*(source_address-1) !=0);
  }





/*---------------------------------------------------------------------------------------
void EEPROM_ReadString(unsigned char eeprom_address, unsigned char * destination_address)
 ----------------------------------------------------------------------------------------
 * I/P Arguments: char,-->eeprom_address from where the String is to be read.
                  char*-->Pointer into which the String is to be read.

 * Return value	: none

 * description:This function is used to Read a String from specified EEPROM_address.
               EEPROM_ReadByte() function is called to read a byte at a time.
               Source(EEPROM) and destination(RAM) address are incremented each time.
               Above Operation is carried out till Null char is identified.
---------------------------------------------------------------------------------------*/
void EEPROM_ReadString(unsigned char eeprom_address, unsigned char * destination_address,unsigned char Page_Number)
  {
   char eeprom_data;

     do
 	 {
	  eeprom_data = EEPROM_ReadByte(eeprom_address,Page_Number); //Read a byte from EEPROM to RAM
 	    *destination_address = eeprom_data;			 //Copy the data into String Buffer
 	     destination_address++;						 //Incerement the RAM Address
 	 	 eeprom_address++;							 //Increment the Eeprom Address
 		}while(eeprom_data!=0);
  }





/*-----------------------------------------------------------------------------------------
                                   void EEPROM_Erase()
 ------------------------------------------------------------------------------------------
 * I/P Arguments: none

 * Return value	: none

 * description:This function is used to erase the entire Eeprom memory.
               Eeprom is filled with 0xFF to accomplish the Eeprom Erase.
               EEPROM_WriteByte() function is called to write a byte at a time.
               Whole memory(0-255) is traversed and filled with 0xFF
-----------------------------------------------------------------------------------------*/
  void EEPROM_Erase()
   {
	 unsigned char eeprom_address;

	 for(eeprom_address=0;eeprom_address<255;eeprom_address++)
	    {
		   EEPROM_WriteByte(eeprom_address,0xff); // Write Each memory location with OxFF
	      }
   }








