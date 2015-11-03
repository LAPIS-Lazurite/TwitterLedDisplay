
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


void setup(void)
{
	int i,j;
	uint8_t data;
	Serial.begin(115200);
	
	for(i=0;i<2;i++)
	{
		for(j=0;j<1024;j+=2)
		{
			Serial.print_long((long)i,HEX);
			Serial.print("\t");
			Serial.print_long((long)j,HEX);
			Serial.print("\t");
			data = Flash.read_byte(i,j);
			Serial.print_long((long)data,HEX);
			Serial.print(" ");
			data = Flash.read_byte(i,j+1);
			Serial.print_long((long)data,HEX);
			Serial.println("");
		}
		delay(100);
		Serial.println("");
	}
	return;
}

void loop(void)
{
}

