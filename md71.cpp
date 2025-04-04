/*
Copyright (C) 2025 by HellGauss
CRC routines from DelayCut by jsoto

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <math.h>
#define BUILD 250404


int fsizetable[256]={ 64 , 64 , 80 , 80 , 96 , 96 , 112, 112, 128, 128, 160, 160, 192, 192, 224, 224, 256, 256, 320, 320, 384, 384, 448, 448, 512, 512, 640, 640, 768, 768, 896, 896,1024,1024,1152,1152,1280,1280,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //48khz
                      69 , 70 , 87 , 88 , 104, 105, 121, 122, 139, 140, 174, 175, 208, 209, 243, 244, 278, 279, 348, 349, 417, 418, 487, 488, 557, 558, 696, 697, 835, 836, 975, 976,1114,1115,1253,1254,1393,1394,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //44.1khz
                      96 , 96 , 120, 120, 144, 144, 168, 168, 192, 192, 240, 240, 288, 288, 336, 336, 384, 384, 480, 480, 576, 576, 672, 672, 768, 768, 960, 960,1152,1152,1344,1344,1536,1536,1728,1728,1920,1920,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //32khz
                      0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //??

int table58[256]  = { 40 , 40 , 50 , 50 , 60 , 60 , 70 , 70 , 80 , 80 , 100, 100, 120, 120, 140, 140, 160, 160, 200, 200, 240, 240, 280, 280, 320, 320, 400, 400, 480, 480, 560, 560, 640, 640, 720, 720, 800, 800,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					  42 , 43 , 53 , 55 , 65 , 65 , 75 , 76 , 86 , 87 , 108, 108, 130, 130, 151, 152, 173, 173, 217, 217, 260, 261, 303, 305, 347, 348, 435, 435, 521, 522, 608, 610, 696, 696, 782, 783, 870, 871,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					  60 , 60 , 75 , 75 , 90 , 90 , 105, 105, 120, 120, 150, 150, 180, 180, 210, 210, 240, 240, 300, 300 ,360, 360, 420, 420, 480, 480, 600, 600, 720, 720, 840, 840, 960, 960,1080,1080,1200,1200,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					  0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		
////////////////////////////////////////////////////////////////////////
//      crc functions
////////////////////////////////////////////////////////////////////////
#define CRC16_POLY ((1 << 0) | (1 << 2) | (1 << 15) | (1 << 16))
unsigned int crc_table[256];
static void ac3_crc_init(void)
{
    unsigned int c, n, k;
    for(n=0;n<256;n++)
        {
        c = n << 8;
        for (k = 0; k < 8; k++) {
            if (c & (1 << 15))
                c = ((c << 1) & 0xffff) ^ (((1 << 0) | (1 << 2) | (1 << 15) | (1 << 16)) & 0xffff);
            else
                c = c << 1;
        }
        crc_table[n] = c;
    }
}
uint16_t ac3_crc(uint8_t* data, int n, uint16_t crc)  {
    int i;            // loop in bytes
    for(i=0;i<n;i++)  {
        crc = (crc_table[(data[i] & 0xff)^ (crc >> 8)] ^ (crc << 8)) & 0xffff;
    }
    return crc;
}
static unsigned int mul_poly(unsigned int a, unsigned int b, unsigned int poly)
{
    unsigned int c;
    c = 0;
    while (a) {
        if (a & 1)
            c ^= b;
        a = a >> 1;
        b = b << 1;
        if (b & (1 << 16))
            b ^= poly;
    }
    return c;
}
static unsigned int pow_poly(unsigned int a, unsigned int n, unsigned int poly)
{
    unsigned int r;
    r = 1;
    while (n) {
        if (n & 1)
            r = mul_poly(r, a, poly);
        a = mul_poly(a, a, poly);
        n >>= 1;
    }
    return r;
}
////////////////////////////////////////////////////////////////////////
//     end crc functions
////////////////////////////////////////////////////////////////////////

FILE* i1=NULL;
FILE* i2=NULL;
FILE* o1=NULL;
FILE* o2=NULL;

uint8_t* data=NULL;
int fcounter=-1;

struct change_data
{
	uint8_t dialn_value;
	uint8_t dialn2_value;
	uint8_t compr_value;
	uint8_t compr2_value;
	uint8_t chmap_value0;
	uint8_t chmap_value1;
	
	bool dialn_change;
	bool dialn2_change;
	bool compr_change;
	bool compr2_change;
	bool chmap_change;
};

void error(const char* str_err)
{
	int i=strlen(str_err);
	
	if (i1!=NULL) fclose(i1);
	if (i2!=NULL) fclose(i2);
	if (o1!=NULL) fclose(o1);
	if (o2!=NULL) fclose(o2);
	
	if (data!=NULL) delete[] data;
	
	printf("\nFrame [pairs] read: ");
	if (fcounter>=0)
		printf("%d",fcounter);
	else
		putchar('-');
	printf("\nERROR: %s\nExit program\n",str_err);
	exit(i*65536+ac3_crc((uint8_t*) str_err, i, 0)+257);
};

int hex(char c)
{
	if ( ( (c < '0') || (c > '9') ) && ( (c < 'A') || (c > 'F') ) && ( (c < 'a') || (c > 'f') ) )
		error("Wrong hex digit in options");
	
	if ( (c>='0') && (c<='9') ) return c-'0';
	else if ( (c>='a') && (c<='f') ) return c-'a'+10;
	
	return c-'A'+10;
};

int dec(char c)
{
	int d=c-'0';
	if ( (d<0) || (d>9) )
		error("Wrong decimal digit in options");
	
	return d;
};

void setbyte(uint8_t* data, uint8_t byte, int bitoffset)
{	
	int offs=bitoffset%8;
	if( offs ==0 )
	{
		data[bitoffset/8]=byte;
		return;
	}
	data[bitoffset/8]&=0xFF<<8-offs;
	data[bitoffset/8]+=byte>>offs;
	data[1+bitoffset/8]&=0xFF>>offs;
	data[1+bitoffset/8]+=byte<<(8-offs);
	return;
}

uint8_t getbyte(uint8_t* data, int bitoffset)
{	
	int offs=bitoffset%8;
	if( offs == 0 )
		return data[bitoffset/8];
	uint8_t byte;
	byte=data[bitoffset/8]<<offs;
	return byte+(data[bitoffset/8+1]>>(8-offs));
}

void setbit(uint8_t* data, uint8_t bit, int bitoffset)
{
	if(bit==0)
		data[bitoffset/8]&=0xff - (0x80>>(bitoffset%8));
	else
		data[bitoffset/8]|=0x80>>(bitoffset%8);
	return;
}

uint8_t getbit(uint8_t* data, int bitoffset)
{
	if ( (data[bitoffset/8]) & (0x80>>(bitoffset%8)) ) return 1;
	return 0;
}

void printbits(uint8_t* data, const char* msg, int offset, int numbits)
{
	int x;
	if (msg!=NULL)
	{
		int l=strlen(msg);
		printf(msg);
		for(x=0;x<15-l;x++)
		{
			putchar(' ');
		}
		printf(": ");
	}
	for(x=0;x<numbits;x++) fputchar(getbit(data,offset+x)+'0');
	return;
}

void parse_and_change(uint8_t* data,struct change_data* cdata)
{
	int offset=0;
	int frmsiz;
	int frmsiz58;
	uint16_t crc;
	uint8_t acmod;
	uint8_t strmtyp;
	
	if (data[5] >>3 <10) //ac3
	{
		if (fcounter==0) printf("AC3");
		
		if (fcounter==0) printbits(data,"\nsyncword",offset,16);
		offset+=16;
		
		if (fcounter==0) printbits(data,"\ncrc1",offset,16);
		offset+=16;
		
		if (fcounter==0) printbits(data,"\nfscod",offset,2);
		offset+=2;
		
		if (fcounter==0) printbits(data,"\nfrmsizecod",offset,6);
		frmsiz=fsizetable[data[4]]*2;
		frmsiz58=table58[data[4]]*2;
		offset+=6;
		
		
		if (fcounter==0) printbits(data,"\nbsid",offset,5) ;
		offset+=5;
		
		if (fcounter==0) printbits(data,"\nbsmod",offset,3);
		offset+=3;
		
		if (fcounter==0) printbits(data,"\nacmod",offset,3);
		acmod=getbyte(data,offset)>>5;
		offset+=3;
		
		if ((acmod & 0x1) && (acmod != 0x1))
		{
			if (fcounter==0) printbits(data,"\ncmixlevel",offset,2);
			offset+=2;
		}
		
		if (acmod & 0x4)
		{
			if (fcounter==0) printbits(data,"\nsurmixlev",offset,2);
			offset+=2;	
		}
		
		if (acmod == 0x2)
		{
			if (fcounter==0) printbits(data,"\ndsurmod",offset,2);
			offset+=2;	
		}
		
		if (fcounter==0) printbits(data,"\nlfeon",offset,1);
		offset+=1;
		
		if (fcounter==0) printbits(data,"\ndialnorm",offset,5);
		if (cdata->dialn_change)
		{
			setbit(data,cdata->dialn_value&16,offset);
			setbit(data,cdata->dialn_value&8,offset+1);
			setbit(data,cdata->dialn_value&4,offset+2);
			setbit(data,cdata->dialn_value&2,offset+3);
			setbit(data,cdata->dialn_value&1,offset+4);
		}
		offset+=5;
		
		if (fcounter==0) printbits(data,"\ncompre",offset,1);
		offset+=1;
		
		if (getbit(data,offset-1))
		{
			if (fcounter==0) printbits(data,"\ncompr",offset,8);
			if (cdata->compr_change) setbyte(data,cdata->compr_value,offset);
			offset+=8;
		}
		
		if (fcounter==0) printbits(data,"\naudprodie",offset,1);
		offset+=1;
		
		if (getbit(data,offset-1))
		{
			if (fcounter==0) printbits(data,"\nmixlevel",offset,5);
			offset+=5;
			
			if (fcounter==0) printbits(data,"\nroomtyp",offset,2);
			offset+=2;
		}
		
		if (acmod==0)
		{
			if (fcounter==0) printbits(data,"\ndialnorm2",offset,5);
			if (cdata->dialn2_change)
			{
				setbit(data,cdata->dialn2_value&16,offset);
				setbit(data,cdata->dialn2_value&8,offset+1);
				setbit(data,cdata->dialn2_value&4,offset+2);
				setbit(data,cdata->dialn2_value&2,offset+3);
				setbit(data,cdata->dialn2_value&1,offset+4);
			}
			offset+=5;
			
			if (fcounter==0) printbits(data,"\ncompr2e",offset,1);
			offset+=1;
			
			if (getbit(data,offset-1))
			{
				if (fcounter==0) printbits(data,"\ncompr2",offset,8);
				if (cdata->compr2_change) setbyte(data,cdata->compr2_value,offset);
				offset+=8;
			}
		}		
		////crc
		crc = ac3_crc(&(data[4]), (2 * frmsiz58) - 4, 0);
        crc = mul_poly(pow_poly((CRC16_POLY >> 1), (16 * frmsiz58) - 16, CRC16_POLY), crc, CRC16_POLY);
        data[2]=crc>>8;
        data[3]=crc;
	}
	else //eac3
	{
		if (fcounter==0) printf("EAC3");
		
		if (fcounter==0) printbits(data,"\nsyncword",offset,16);
		offset+=16;
		
		if (fcounter==0) printbits(data,"\nstrmtype",offset,2);
		strmtyp=getbyte(data,offset)>>6;
		offset+=2;
		
		if (fcounter==0) printbits(data,"\nsubstreamid",offset,3);
		offset+=3;
		
		if (fcounter==0) printbits(data,"\nfrmsiz",offset,11);
		offset+=11;
		
		if (fcounter==0) printbits(data,"\nfscod",offset,2);
		frmsiz= (256*(data[2]&7)+data[3])*2+2;
		offset+=2;
		
		if  ( (getbit(data,offset-2)==1) && (getbit(data,offset-2)==1) )
		{
			if (fcounter==0) printbits(data,"\nfscod2",offset,2);
			offset+=2;
		}
		else
		{
			if (fcounter==0) printbits(data,"\nnumblkscod",offset,2);
			offset+=2;
		}
		
		if (fcounter==0) printbits(data,"\nacmod",offset,3);
		acmod=getbyte(data,offset)>>5;
		offset+=3;
		
		if (fcounter==0) printbits(data,"\nlfeon",offset,1);
		offset+=1;
		
		if (fcounter==0) printbits(data,"\nbsid",offset,5);
		offset+=5;
		
		if (fcounter==0) printbits(data,"\ndialnorm",offset,5);
		if (cdata->dialn_change)
		{
			setbit(data,cdata->dialn_value&16,offset);
			setbit(data,cdata->dialn_value&8,offset+1);
			setbit(data,cdata->dialn_value&4,offset+2);
			setbit(data,cdata->dialn_value&2,offset+3);
			setbit(data,cdata->dialn_value&1,offset+4);
		}		
		offset+=5;
		
		if (fcounter==0) printbits(data,"\ncompre",offset,1);
		offset+=1;
		
		if (getbit(data,offset-1))
		{
			if (fcounter==0) printbits(data,"\ncompr",offset,8);      
			if (cdata->compr_change) setbyte(data,cdata->compr_value,offset);
			offset+=8;
		}
		
		if (acmod==0)
		{
			if (fcounter==0) printbits(data,"\ndialnorm2",offset,5);
			if (cdata->dialn2_change)
			{
				setbit(data,cdata->dialn2_value&16,offset);
				setbit(data,cdata->dialn2_value&8,offset+1);
				setbit(data,cdata->dialn2_value&4,offset+2);
				setbit(data,cdata->dialn2_value&2,offset+3);
				setbit(data,cdata->dialn2_value&1,offset+4);
			}
			offset+=5;
			
			if (fcounter==0) printbits(data,"\ncompr2e",offset,1);
			offset+=1;
			
			if (getbit(data,offset-1))
			{
				if (fcounter==0) printbits(data,"\ncompr2",offset,8);
				if (cdata->compr2_change) setbyte(data,cdata->compr2_value,offset);
				offset+=8;
			}
		}
		
		if (strmtyp==1)
		{
			if (fcounter==0) printbits(data,"\nchanmape",offset,1);
			offset+=1;
			
			if (getbit(data,offset-1))
			{
				if (fcounter==0) printbits(data,"\nchanmap",offset,16);
				if(cdata->chmap_change)
				{
					setbyte(data,cdata->chmap_value0,offset);
					setbyte(data,cdata->chmap_value1,offset+8);
				}
			}
		}		
		////crc
		crc = ac3_crc(&(data[2]), frmsiz-4, 0);
    	data[frmsiz-2]=crc>>8;
    	data[frmsiz-1]=crc;
	}
	return;	
}


////////////////////////////////////////////////////////////////////////
//
//     M A I N ()
//
//

int main (int argc, char** argv)
{
	ac3_crc_init();
	int x;
	
	struct change_data cdata;
	
	cdata.dialn_change=false;
	cdata.compr_change=false;
	cdata.chmap_change=false;
	cdata.dialn2_change=false;
	cdata.compr2_change=false;
	
	printf("md71 by HellGauss (HG) - Build %d\n\n",BUILD);
	
	////////////// CLI PARSE AND FILE OPENING
	if ( ( argc==1 ) || (argc>10) )
	{
		printf("USAGE:\n\nmd71 inputFileA inputFileB outputFileA outputFileB dialn_value compr_flags chanmap_flag dialn2_value compr2_flags\n\nSee readme.txt for more info\n");
		return 0;
	}
	
	for(x=0;x<argc;x++)
		if (argv[x][0]==0)
			error("Arguments cannot be [void]");
	
	// FLAG PARSE	
	if (argc>5) if ( strcmp(argv[5],":") )
	{
		int l=strlen(argv[5]);
		if ( (l!=2) && (l!=1) )
			error("Wrong dialn_value length in arguments");
		
		cdata.dialn_change=true;	
		cdata.dialn_value=dec(argv[5][0]);
		if (l==2)
		{
			cdata.dialn_value*=10;
			cdata.dialn_value+=dec(argv[5][1]);
		}
		if (cdata.dialn_value>31) error("dialn must be less or equal 31");
	}
	
	if (argc>6) if ( strcmp(argv[6],":") )
	{
		cdata.compr_change=true;
		if (strlen(argv[6])!=2)
			error("Wrong compr_flags length in arguments");
		cdata.compr_value=hex(argv[6][0])*16+hex(argv[6][1]);
	}
	
	if (argc>7) if ( strcmp(argv[7],":") )
	{
		cdata.chmap_change=true;
		if (strlen(argv[7])!=4)
			error("Wrong chanmap_flags length in arguments");
		cdata.chmap_value0=hex(argv[7][0])*16+hex(argv[7][1]);
		cdata.chmap_value1=hex(argv[7][2])*16+hex(argv[7][3]);
	}
	
	if (argc>8) if ( strcmp(argv[8],":") )
	{
		int l=strlen(argv[8]);
		if ( (l!=2) && (l!=1) )
			error("Wrong dialn2_value length in arguments");
		
		cdata.dialn2_change=true;	
		cdata.dialn2_value=dec(argv[8][0]);
		if (l==2)
		{
			cdata.dialn2_value*=10;
			cdata.dialn2_value+=dec(argv[8][1]);
		}
		if (cdata.dialn2_value>31) error("dialn2 must be less or equal 31");
	}
	
	if (argc>9) if ( strcmp(argv[9],":") )
	{
		cdata.compr2_change=true;
		if (strlen(argv[10])!=2)
			error("Wrong compr2_flags length in arguments");
		cdata.compr2_value=hex(argv[10][0])*16+hex(argv[10][1]);
	}
	
	// FILE PARSING
	errno_t err_file;
	
	if (strcmp(argv[1],":"))
	{
		err_file=fopen_s(&i1, argv[1], "rb");
		if ((i1 == NULL) || (err_file!=0))
			error("Fail opening inputA");
	}
	if (i1==NULL)  error("inputA must be specified");
	
	i2=NULL;
	if (argc>2) if (strcmp(argv[2],":"))
	{
		if ( strcmp(argv[2],argv[1]) )
		{
			err_file=fopen_s(&i2, argv[2], "rb");
			if ((i2 == NULL) || (err_file!=0))
				error("Fail opening inputB");
		}		
		else i2=i1;
	}
	
	o1=NULL;
	if (argc>3) if (strcmp(argv[3],":"))
	{
		if ( !strcmp(argv[3],argv[1])  || !strcmp(argv[3],argv[2])  )
			error("outputA cannot be equal to input");
		
		err_file=fopen_s(&o1, argv[3], "wb");
		if ((o1 == NULL) || (err_file!=0))
			error("Fail opening outputA");
	}
	
	o2=NULL;
	if (argc>4) if (strcmp(argv[4],":"))
	{
		if (i2==NULL) error("outputB needs inputB");
		
		if ( !strcmp(argv[4],argv[1])  || !strcmp(argv[4],argv[2])  )
			error("outputB cannot be equal to input");
			
		if ( strcmp(argv[4],argv[3]) )
		{
			err_file=fopen_s(&o2, argv[4], "wb");
			if ((o2 == NULL) || (err_file!=0))
				error("Fail opening outputB");
		}
		else o2=o1;
	}
	
	////////////// END CLI PARSE
	////////////////////////////
	int bsid;	
	int frmsiz,c,frmsiz58;
	int eof_flag=0;
	
	double suma=0;
	double sumb=0;
	double sumqa=0;
	double sumqb=0;
	int ac3a=0;
	int eac3a=0;
	int ac3b=0;
	int eac3b=0;
	
	fcounter=0;
	
	data=new uint8_t[4500];
	
	while(1) ///// M A I N   L O O P
	{		
		//FRAME A
		frmsiz=6;
		for(x=0;x<frmsiz;x++)
		{
			c=fgetc(i1);
			if (c==EOF)
			{
				if (x==0)
				{
					eof_flag=1;
					break;
				}
				else
					error("Unexpected EOF found in inputA");
			}
			
			data[x]=c;
			if (x==0) if (c!=0x0b) error("Wrong syncframe word in inputA (byte 1)");
			if (x==1) if (c!=0x77) error("Wrong syncframe word in inputA (byte 2)");
			if (x==5)
			{
				bsid = c >>3;
				if (bsid<10) //ac3
				{
					frmsiz=fsizetable[data[4]]*2;
					frmsiz58=table58[data[4]]*2;
					if(frmsiz==0) error("Wrong frmsizecod in inputA");
					ac3a++;
				}						
				else
				{
					frmsiz= (256*(data[2]&7)+data[3])*2+2;
					if(frmsiz<10) error("Wrong frmsiz in inputA");
					eac3a++;
				}
				if(fcounter==0) printf("Parsing first frame in inputA: SIZE %d - ",frmsiz);
			}
		}
		if (eof_flag==1) break;
		
		suma+=frmsiz;
		sumqa+=((double) (frmsiz) )*((double) (frmsiz) );
		
		parse_and_change(data,&cdata);
      	
      	if(o1!=NULL) for (x=0;x<frmsiz;x++)
      		fputc(data[x],o1);
		
		//FRAME B
		if (i2!=NULL)
		{
			frmsiz=6;
			for(x=0;x<frmsiz;x++)
			{
				c=fgetc(i2);
				if (c==EOF)
					error("Unexpected EOF found in inputB");				
				data[x]=c;
				if (x==0) if (c!=0x0b) error("Wrong syncframe word in inputB (byte 1)");
				if (x==1) if (c!=0x77) error("Wrong syncframe word in inputB (byte 2)");
				if (x==5)
				{
					bsid = c >>3;					
					if (bsid<10) //ac3
					{
						frmsiz=fsizetable[data[4]]*2;
						if(frmsiz==0) error("Wrong frmsizecod in inputB");
						ac3b++;
					}						
					else
					{
						frmsiz= (256*(data[2]&7)+data[3])*2+2;
						if(frmsiz<10) error("Wrong frmsiz in inputB");
						eac3b++;
					}
					if(fcounter==0) printf("\n\nParsing first frame in inputB: SIZE %d - ",frmsiz);			
				}
			}
			sumb+=frmsiz;
			sumqb+=((double) (frmsiz) )*((double) (frmsiz) );
	      	
	      	parse_and_change(data,&cdata);
	      	
	      	if(o2!=NULL) for (x=0;x<frmsiz;x++)
	      		fputc(data[x],o2);
		}
		fcounter++;
	}
	
	delete[] data;
	fclose(i1);
	if (i2!=NULL) fclose(i2);
	if (o1!=NULL) fclose(o1);
	if (o2!=NULL) fclose(o2);
		
	printf("\n\nSCRIPT COMPLETE - %d [pairs of] frame processed\n", fcounter);
	printf("StreamA -- AC3 %d : EAC3 %d : avgSize %f : stdvSize %f\n",ac3a,eac3a,suma/fcounter,sqrt(fcounter*sumqa-suma*suma)/fcounter);
	if (i2!=NULL)
		printf("StreamB -- AC3 %d : EAC3 %d : avgSize %f : stdvSize %f\n",ac3b,eac3b,sumb/fcounter,sqrt(fcounter*sumqb-sumb*sumb)/fcounter);

	return 0;
}