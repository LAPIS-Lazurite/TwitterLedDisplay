
/* FILE NAME: Read_SubGHz.c
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015  Lapis Semiconductor Co.,Ltd.
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/

#define SUBGHZ_CH	36
#define SUBGHZ_PANID	0xABCD
uint8_t rx_data[256];
SUBGHZ_STATUS rx;

#define DEBUG_WR
bool ProgramDataToFlash(uint16_t read_len)
{
	bool refresh = false;
	uint8_t block;
	static uint8_t last_block=7;
	int i;
	uint16_t len=9;
	uint16_t tmp;
	
	//Flashに書き込むデータか確認する。
	if(strncmp("FlashWrite",&rx_data[len],sizeof(10))==0)
	{
		Serial.println("FlashWriteOK");
		len+=10;
	}
	else goto invalid_data;

	//ブロック番号の取得、データが再送されてきた場合は無視する。
	block = rx_data[len] - '0';
	len+=2;				// in the data of ruby 0x20 is added.
	if(block>=8) goto invalid_data;
	if(last_block == block) goto invalid_data;
	else last_block = block;

	Serial.println_long(block,DEC);
	
	// ブロック番号が0と4の時はデータFlashを初期化する。
	switch(block)
	{
	case 0:
		Flash.erase(0);
//		Flash.erase(1);
		break;
	case 8:
		Flash.erase(1);
	}
	// Flashメモリにデータを書き込み
	for(i=block*64;i<(block+1)*64;i++)
	{
		uint8_t bank = 0;
		tmp = (uint16_t)rx_data[len], len++;
		tmp = tmp + ((uint16_t)rx_data[len]<<8), len++;
		Flash.write(bank,i,tmp);
#ifdef	DEBUG_WR
		Serial.print_long(bank,DEC);
		Serial.print("\t");
		Serial.print_long(i,DEC);
		Serial.print("\t");
		Serial.print_long((long)(tmp&0xFF),DEC);
		Serial.print(" ");
		Serial.println_long((long)(tmp>>8),DEC);
#endif		// DEBUG_WR
	}
#ifdef	DEBUG_WR
	Serial.print("rcv=");
	Serial.print_long(read_len,DEC);
	Serial.print(", used=");
	Serial.print_long(len,DEC);
#endif		// DEBUG_WR
	
	if(block == 7) refresh = true;
	
invalid_data:
	return refresh;
}

void setup(void)
{
	SUBGHZ_MSG msg;
	long myAddress;

	Serial.begin(115200);
	
	msg = SubGHz.init();
	if(msg != SUBGHZ_OK)
	{
		SubGHz.msgOut(msg);
		while(1){ }
	}
	
	myAddress = SubGHz.getMyAddress();
	Serial.print("myAddress1 = ");
	Serial.println_long(myAddress,HEX);	
	msg = SubGHz.begin(SUBGHZ_CH, SUBGHZ_PANID,  SUBGHZ_100KBPS, SUBGHZ_PWR_1MW);
	if(msg != SUBGHZ_OK)
	{
		SubGHz.msgOut(msg);
		while(1){ }
	}
	msg = SubGHz.rxEnable(NULL);
	if(msg != SUBGHZ_OK)
	{
		SubGHz.msgOut(msg);
		while(1){ }
	}
	return;
}

void loop(void)
{
	short len;
	bool refresh = false;
	
	len = SubGHz.readData(rx_data,sizeof(rx_data));
	if(len > 0)
	{
		refresh = ProgramDataToFlash(len);
	}
	
#ifdef	DEBUG_WR
	if(refresh == true)
	{
		int i,j;
		uint8_t data;
		for(i=0;i<2;i++)
		{
			for(j=0;j<1024;j+=2)
			{
				Serial.print_long((long)i,DEC);
				Serial.print("\t");
				Serial.print_long((long)j,DEC);
				Serial.print("\t");
				data = Flash.read_byte(i,j);
				Serial.print_long((long)data,DEC);
				Serial.print(" ");
				data = Flash.read_byte(i,j+1);
				Serial.print_long((long)data,DEC);
				Serial.println("");
			}
			delay(100);
			Serial.println("");
		}
	}
#endif		// DEBUG_WR
	
	return;
}

