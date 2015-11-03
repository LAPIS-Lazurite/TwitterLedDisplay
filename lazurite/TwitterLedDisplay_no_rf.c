
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
 * furnished to do so, subject to the folloing conditions:
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
	uint8_t bank;
	
	//Flashに書き込むデータか確認する。
	if(strncmp("FlashWrite",&rx_data[len],sizeof(10))==0)
	{
		Serial.print("FlashWrite");
		len+=10;
	}
	else goto invalid_data;

	//ブロック番号の取得、データが再送されてきた場合は無視する。
	block = rx_data[len] - '0';
	if((block < 0) | ( block > 9))
	{
		block = rx_data[len] - 'a'+10;
	}
	Serial.println_long(block,HEX);
	
	len+=2;				// in the data of ruby 0x20 is added.
	if(last_block == block) goto invalid_data;
	
	// 0 - 15のブロックを、セクター(bank)と0-7のブロックに変更する
	if(block<8)
	{
		bank = 0;
	}
	else
	{
		bank = 1;
		bank -= 8;
	}
	if((bank == 0) & (block == 0))
	{
		Flash.erase(0);
		Flash.erase(1);
	}
	// Flashメモリにデータを書き込み
	for(i=block*64;i<(block+1)*64;i++)
	{
		uint8_t bank = (block >= 8) ? 1: 0;
		tmp = (uint16_t)rx_data[len], len++;
		tmp = tmp + ((uint16_t)rx_data[len]<<8), len++;
		Flash.write(bank,i,tmp);
#if	0
		Serial.print_long(bank,DEC);
		Serial.print("\t");
		Serial.print_long(i,DEC);
		Serial.print("\t");
		Serial.print_long((long)(tmp&0xFF),DEC);
		Serial.print(" ");
		Serial.println_long((long)(tmp>>8),DEC);
#endif		// DEBUG_WR
	}
#if	0
	Serial.print("rcv=");
	Serial.print_long(read_len,DEC);
	Serial.print(", used=");
	Serial.print_long(len,DEC);
#endif		// DEBUG_WR
	
	if((bank == 0) & (block == 0x07)) refresh = true;
	
invalid_data:
	return refresh;
}

void setup(void)
{
	SUBGHZ_MSG msg;
	long myAddress;
	uint16_t len;
	bool refresh;
	

	Serial.begin(115200);
	
//	msg = SubGHz.init();
//	if(msg != SUBGHZ_OK)
//	{
//		SubGHz.msgOut(msg);
//		while(1){ }
//	}
//	
//	myAddress = SubGHz.getMyAddress();
	Serial.print("myAddress1 = ");
	Serial.println_long(myAddress,HEX);	
//	msg = SubGHz.begin(SUBGHZ_CH, SUBGHZ_PANID,  SUBGHZ_100KBPS, SUBGHZ_PWR_20MW);
//	if(msg != SUBGHZ_OK)
//	{
//		SubGHz.msgOut(msg);
//		while(1){ }
//	}
	LedDotMatrix.init();
	
	/*
	while(refresh==false)
	{
		len = SubGHz.readData(rx_data,sizeof(rx_data));
		if(len > 0)
		{
			refresh = ProgramDataToFlash(len);
		}
	}
	*/

	return;
}

void loop(void)
{
	short len;
	int i;
	bool refresh = false;
	uint32_t st_time;
	
	
//	SubGHz.rxEnable(NULL);
	
//	SubGHz.send(0xABCD,0xAC54,"Twitter Beacon",sizeof("TwitterBeacon"),NULL);
/*	
	st_time = millis();
	while((millis()-st_time) < 1000)
	{
		len = SubGHz.readData(rx_data,sizeof(rx_data));
		if(len > 0)
		{
			st_time = millis();
			refresh = ProgramDataToFlash(len);
			if (refresh) break;
		}
	}
	SubGHz.rxDisable();
*/
	Serial.println_long((long)len,HEX);
	len = Flash.read(0,0);
	Serial.print("lo_size=");
	Serial.println_long((long)(len-8),HEX);
	LedDotMatrix.setFlash(0,2,8,0,2,len);
	LedDotMatrix.shift(50,false,true);

	return;
}

