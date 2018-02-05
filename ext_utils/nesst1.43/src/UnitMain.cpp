//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <Clipbrd.hpp>
#pragma hdrstop

#include "UnitMain.h"
#include "UnitCHREditor.h"
#include "UnitSwapColors.h"
#include "UnitNametableOffset.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;


#include "palette.h"


#define REG_SECTION "Software\\Shiru\\NES Screen Tool\\"

const char regWorkingDirectory[]="WorkDir";
const char colBlack=0x0f;

#define SAVE_NAMETABLE_NAM		1
#define SAVE_NAMETABLE_BIN		2
#define SAVE_NAMETABLE_RLE		3
#define SAVE_NAMETABLE_H		4




AnsiString reg_load_str(const char *name,AnsiString def)
{
	HKEY key;
	DWORD type,size;
	char *str;
	AnsiString ret;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,REG_SECTION,0,KEY_READ,&key)!=ERROR_SUCCESS) return def;

	type=REG_SZ;
	size=1024;
	str=(char*)malloc(size);
	if(RegQueryValueEx(key,name,NULL,&type,(unsigned char*)str,&size)!=ERROR_SUCCESS) ret=def; else ret=str;
	free(str);
	RegCloseKey(key);

	return ret;
}



void reg_save_str(const char *name,AnsiString str)
{
	HKEY key;
	DWORD disp;

	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE,REG_SECTION,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,&disp)!=ERROR_SUCCESS) return;

	RegSetValueEx(key,name,0,REG_SZ,str.c_str(),strlen(str.c_str())+1);
	RegCloseKey(key);
}



int basePalette[64];

float emphasis[8][3]={//from nintech.txt
	{100.0,100.0,100.0},
	{ 74.3, 91.5,123.9},
	{ 88.2,108.6, 79.4},
	{ 65.3, 98.0,101.9},
	{127.7,102.6, 90.5},
	{ 97.9, 90.8,102.3},
	{100.1, 98.7, 74.1},
	{ 75.0, 75.0, 75.0}
};


int outPalette[64];
int ppuMask;

unsigned char bgPal[4][4]={{0,15,16,48},{0,1,33,49},{0,6,22,38},{0,9,25,41}};

int bgPalCur;
int palActive;
int tileActive;
int bankActive;
int nameXC;
int nameYC;
int tileXC;
int tileYC;
int palHover;
int palColHover;
int colHover;

unsigned char chr[8192];
unsigned char chrCopy[4096];
unsigned char nameTable[960];
unsigned char attrTable[64];

TRect nameSelection;
int nameCopyWidth;
int nameCopyHeight;

unsigned char nameCopy[960];
unsigned char attrCopy[960];

unsigned char undoChr[8192];
unsigned char undoNameTable[960];
unsigned char undoAttrTable[64];

TRect chrSelection;
int chrCopyWidth;
int chrCopyHeight;
bool chrCopyRect;
bool chrSelected[256];
bool chrSelectRect;



int get_file_size(FILE *file)
{
	int size;

	fseek(file,0,SEEK_END);
	size=ftell(file);
	fseek(file,0,SEEK_SET);

	return size;
}



int read_dword(unsigned char *data)
{
	return data[0]+(data[1]<<8)+(data[2]<<16)+(data[3]<<24);
}



int attr_get(int x,int y)
{
	int pal;

	pal=attrTable[y/4*8+x/4];
	if(x&2) pal>>=2;
	if(y&2) pal>>=4;

	return pal&3;
}



void attr_set(int x,int y,int pal)
{
	int pp,mask;

	pp=y/4*8+x/4;
	mask=3;
	pal=pal&3;

	if(x&2)
	{
		pal<<=2;
		mask<<=2;
	}
	if(y&2)
	{
		pal<<=4;
		mask<<=4;
	}

	attrTable[pp]=(attrTable[pp]&(mask^255))|pal;
}



void save_data(const char *name,unsigned char *src,int size,int type)
{
	char arrname[256],temp[256];
	FILE *file;
	int i,min,tag,pp,len,sym;
	unsigned char *dst;
	int stat[256];
	bool rle;

	rle=(type==SAVE_NAMETABLE_RLE||type==SAVE_NAMETABLE_H)?true:false;

	if(rle)
	{
		dst=(unsigned char*)malloc(size*2);

		for(i=0;i<256;i++) stat[i]=0;
		for(i=0;i<size;i++) stat[src[i]]++;

		min=256;
		tag=255;

		for(i=0;i<256;i++)
		{
			if(stat[i]<min)
			{
				min=stat[i];
				tag=i;
			}
		}

		//tag=255;
		pp=0;
		dst[pp++]=tag;
		len=0;
		sym=-1;

		for(i=0;i<size;i++)
		{
			if(src[i]!=sym||len==255||i==size-1)
			{
				if(src[i]==sym&&i==size-1) len++;
				if(len) dst[pp++]=sym;
				if(len>1)
				{
					if(len==2)
					{
						dst[pp++]=sym;
					}
					else
					{
						dst[pp++]=tag;
						dst[pp++]=len-1;
					}
				}
				sym=src[i];
				len=1;
			}
			else
			{
				len++;
			}
		}

		dst[pp++]=tag;
		dst[pp++]=0;
		size=pp;
	}
	else
	{
		dst=src;
	}

	if(type!=SAVE_NAMETABLE_H)//binary
	{
		file=fopen(name,"wb");

		if(file)
		{
			fwrite(dst,size,1,file);
			fclose(file);
		}
	}
	else//text
	{
		file=fopen(name,"wt");

		if(file)
		{
			i=strlen(name);

			while(i)
			{
				if(name[i]=='\\'||name[i]=='/')
				{
					i++;
					break;
				}
				i--;
			}

			strcpy(arrname,&name[i]);

			for(i=strlen(arrname)-1;i>=0;i--)
			{
				if(arrname[i]=='.')
				{
					arrname[i]=0;
					break;
				}
			}

			if(arrname[0]!='_'&&!(arrname[0]>='a'&&arrname[0]<='z')&&!(arrname[0]>='A'&&arrname[0]<='Z'))
			{
				strcpy(temp,arrname);
				strcpy(arrname,"n");
				strcat(arrname,temp);
			}

			fprintf(file,"const unsigned char %s[%i]={\n",arrname,size);

			for(i=0;i<size;i++)
			{
				fprintf(file,"0x%2.2x",dst[i]);
				if(i<size-1) fprintf(file,",");
				if((i&15)==15||i==(size-1)) fprintf(file,"\n");
			}

			fprintf(file,"};\n");
			fclose(file);
		}
	}

	if(rle) free(dst);
}



void mem_exchange(unsigned char *src,unsigned char *dst,int len)
{
	int temp;

	while(len)
	{
		temp=*src;
		*src++=*dst;
		*dst++=temp;
		len--;
	}
}



void pal_validate(void)
{
	int i,j;

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			switch(bgPal[i][j])
			{
			case 0x0d:
			case 0x1d:
			case 0x0e:
			case 0x1e:
			case 0x2e:
			case 0x3e:
			case 0x1f:
			case 0x2f:
			case 0x3f:
				bgPal[i][j]=colBlack;
				break;
			}
		}
	}
}



void palette_calc(void)
{
	int i;
	float r,g,b;

	if(!(ppuMask&0x01))
	{
		for(i=0;i<64;i++)
		{
			r=((float)((basePalette[i]>>16)&0xff))/255.0;
			g=((float)((basePalette[i]>>8)&0xff))/255.0;
			b=((float)(basePalette[i]&0xff))/255.0;
			r=r*emphasis[ppuMask>>5][0]/100.0;
			g=g*emphasis[ppuMask>>5][1]/100.0;
			b=b*emphasis[ppuMask>>5][2]/100.0;
			if(r>1.0) r=1.0;
			if(g>1.0) g=1.0;
			if(b>1.0) b=1.0;
			outPalette[i]=(((int)(255.0*r))<<16)|(((int)(255.0*g))<<8)|((int)(255.0*b));
		}
	}
	else
	{
		for(i=0;i<64;i++)
		{
			outPalette[i]=basePalette[i&0xf0];
		}
	}
}



AnsiString RemoveExt(AnsiString name)
{
	return ChangeFileExt(name,"");
}



AnsiString GetExt(AnsiString name)
{
	name=ExtractFileName(name);

	return name.SubString(name.LastDelimiter(".")+1,name.Length()-name.LastDelimiter(".")).LowerCase();
}



void __fastcall TFormMain::DrawCol(int x,int y,int c,bool sel)
{
	TRect r;

	r.left=x;
	r.top=y;
	r.right=x+20;
	r.Bottom=y+20;
	PaintBoxPal->Canvas->Brush->Color=TColor(outPalette[c]);
	PaintBoxPal->Canvas->FillRect(r);
	if(sel)
	{
		PaintBoxPal->Canvas->Pen->Color=TColor(0xffffff);
		PaintBoxPal->Canvas->Rectangle(r);
		PaintBoxPal->Canvas->Pen->Color=TColor(0);
		r.left+=1;
		r.top+=1;
		r.right-=1;
		r.bottom-=1;
		PaintBoxPal->Canvas->Rectangle(r);
	}
}



void __fastcall TFormMain::DrawBGPal(int x,int y,int pal)
{
	DrawCol(x   ,y,bgPal[pal][0],pal==palActive&&bgPalCur==0?true:false);
	DrawCol(x+20,y,bgPal[pal][1],pal==palActive&&bgPalCur==1?true:false);
	DrawCol(x+40,y,bgPal[pal][2],pal==palActive&&bgPalCur==2?true:false);
	DrawCol(x+60,y,bgPal[pal][3],pal==palActive&&bgPalCur==3?true:false);
}



void __fastcall TFormMain::DrawPalettes(void)
{
	int i,j,x,y,pp,col;
	TRect r;

	if(!Visible) return;

	DrawBGPal(16,0,0);
	DrawBGPal(128,0,1);
	DrawBGPal(16,32,2);
	DrawBGPal(128,32,3);

	y=64;
	pp=0;
	col=bgPal[palActive][bgPalCur];

	for(i=0;i<4;i++)
	{
		x=0;
		for(j=0;j<14;j++)
		{
			PaintBoxPal->Canvas->Brush->Color=TColor(outPalette[pp]);
			r.left=x;
			r.top=y;
			r.right=x+16;
			r.bottom=y+16;
			PaintBoxPal->Canvas->FillRect(r);
			if(pp==col)
			{
				PaintBoxPal->Canvas->Pen->Color=TColor(0xffffff);
				PaintBoxPal->Canvas->Rectangle(r);
				r.left+=1;
				r.top+=1;
				r.right-=1;
				r.bottom-=1;
				PaintBoxPal->Canvas->Pen->Color=TColor(0);
				PaintBoxPal->Canvas->Rectangle(r);
			}
			pp++;
			x+=16;
		}
		pp+=2;
		y+=16;
	}
}



void __fastcall TFormMain::DrawTile(TPicture *pic,int x,int y,int tile,int pal,int tx,int ty,bool sel)
{
	int j,k,pp,col,r,g,b;
	unsigned char *d1,*d2;
	int hgrid,vgrid,hcol,vcol;

	pp=tile*16+bankActive;

	if(chrSelectRect) sel=false;

	for(j=0;j<8;j++)
	{
		d1=(unsigned char*)pic->Bitmap->ScanLine[y+j*2]+x*3;
		d2=(unsigned char*)pic->Bitmap->ScanLine[y+j*2+1]+x*3;
		for(k=0;k<8;k++)
		{
			col=(((chr[pp+j]<<k)&128)>>7)|(((chr[pp+j+8]<<k)&128)>>6);
			col=outPalette[bgPal[pal][col]];
			r=(col>>16)&0xff;
			g=(col>>8)&0xff;
			b=col&0xff;
			if(sel)
			{
				r-=64;
				if(r<0) r=0;
				g+=64;
				if(g>255) g=255;
				b-=64;
				if(b<0) b=0;
			}
			*d1++=r;
			*d1++=g;
			*d1++=b;
			*d1++=r;
			*d1++=g;
			*d1++=b;
			*d2++=r;
			*d2++=g;
			*d2++=b;
			*d2++=r;
			*d2++=g;
			*d2++=b;
		}
	}

	hgrid=0;
	vgrid=0;
	hcol=64;
	vcol=64;

	if(tx<0&&ty<0)
	{
		if(SpeedButtonGridTile->Down||SpeedButtonGridAtr->Down||SpeedButtonGridBlock->Down)
		{
			hgrid=2;
			vgrid=2;
		}
	}
	else
	{
		if(SpeedButtonGridTile->Down)
		{
			hgrid=2;
			vgrid=2;
		}
		if(SpeedButtonGridAtr->Down)
		{
			if(!(ty&1))
			{
				hcol=96;
				hgrid=1;
			}
			if(!(tx&1))
			{
				vcol=96;
				vgrid=1;
			}
		}
		if(SpeedButtonGridBlock->Down)
		{
			if(!(ty&3))
			{
				hcol=128;
				hgrid=1;
			}
			if(!(tx&3))
			{
				vcol=128;
				vgrid=1;
			}
		}
	}

	if(hgrid)
	{
		d1=(unsigned char*)pic->Bitmap->ScanLine[y]+x*3;
		for(j=0;j<16;j+=hgrid)
		{
			*d1+++=hcol;
			*d1+++=hcol;
			*d1+++=hcol;
			if(hgrid>1) d1+=(hgrid-1)*3;
		}
	}

	if(vgrid)
	{
		for(j=1;j<16;j+=vgrid)
		{
			d1=(unsigned char*)pic->Bitmap->ScanLine[y+j]+x*3;
			*d1+++=vcol;
			*d1+++=vcol;
			*d1+++=vcol;
		}
		*d1=*d1;//to prevent warning
	}
}



void __fastcall TFormMain::DrawTileChecker(TPicture *pic,int x,int y,int tile,int pal,int tx,int ty,bool sel)
{
	const unsigned char checker[16]={
		0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
		0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff
	};
	int j,k,col,r,g,b;
	unsigned char *d1,*d2;
	int hgrid,vgrid,hcol,vcol;

	if(chrSelectRect) sel=false;

	for(j=0;j<8;j++)
	{
		d1=(unsigned char*)pic->Bitmap->ScanLine[y+j*2]+x*3;
		d2=(unsigned char*)pic->Bitmap->ScanLine[y+j*2+1]+x*3;
		for(k=0;k<8;k++)
		{
			col=(((checker[j]<<k)&128)>>7)|(((checker[j+8]<<k)&128)>>6);
			col=outPalette[bgPal[pal][col]];
			r=(col>>16)&0xff;
			g=(col>>8)&0xff;
			b=col&0xff;
			if(sel)
			{
				r-=64;
				if(r<0) r=0;
				g+=64;
				if(g>255) g=255;
				b-=64;
				if(b<0) b=0;
			}
			*d1++=r;
			*d1++=g;
			*d1++=b;
			*d1++=r;
			*d1++=g;
			*d1++=b;
			*d2++=r;
			*d2++=g;
			*d2++=b;
			*d2++=r;
			*d2++=g;
			*d2++=b;
		}
	}

	hgrid=0;
	vgrid=0;
	hcol=64;
	vcol=64;

	if(tx<0&&ty<0)
	{
		if(SpeedButtonGridTile->Down||SpeedButtonGridAtr->Down||SpeedButtonGridBlock->Down)
		{
			hgrid=2;
			vgrid=2;
		}
	}
	else
	{
		if(SpeedButtonGridTile->Down)
		{
			hgrid=2;
			vgrid=2;
		}
		if(SpeedButtonGridAtr->Down)
		{
			if(!(ty&1))
			{
				hcol=96;
				hgrid=1;
			}
			if(!(tx&1))
			{
				vcol=96;
				vgrid=1;
			}
		}
		if(SpeedButtonGridBlock->Down)
		{
			if(!(ty&3))
			{
				hcol=128;
				hgrid=1;
			}
			if(!(tx&3))
			{
				vcol=128;
				vgrid=1;
			}
		}
	}

	if(hgrid)
	{
		d1=(unsigned char*)pic->Bitmap->ScanLine[y]+x*3;
		for(j=0;j<16;j+=hgrid)
		{
			*d1+++=hcol;
			*d1+++=hcol;
			*d1+++=hcol;
			if(hgrid>1) d1+=(hgrid-1)*3;
		}
	}

	if(vgrid)
	{
		for(j=1;j<16;j+=vgrid)
		{
			d1=(unsigned char*)pic->Bitmap->ScanLine[y+j]+x*3;
			*d1+++=vcol;
			*d1+++=vcol;
			*d1+++=vcol;
		}
		*d1=*d1;//to prevent warning
	}
}



void __fastcall TFormMain::DrawSmallTile(TPicture *pic,int x,int y,int tile,int pal,int tx,int ty)
{
	int j,k,pp,col,r,g,b;
	unsigned char *d1;
	int hgrid,vgrid,hcol,vcol;

	pp=tile*16+bankActive;

	for(j=0;j<8;j++)
	{
		d1=(unsigned char*)pic->Bitmap->ScanLine[y+j]+x*3;
		for(k=0;k<8;k++)
		{
			col=(((chr[pp+j]<<k)&128)>>7)|(((chr[pp+j+8]<<k)&128)>>6);
			col=outPalette[bgPal[pal][col]];
			r=(col>>16)&0xff;
			g=(col>>8)&0xff;
			b=col&0xff;
			*d1++=r;
			*d1++=g;
			*d1++=b;
		}
	}
}



void __fastcall TFormMain::DrawSelection(TImage *image,TRect rect)
{
	TRect r;

	if(rect.left>=0&&rect.top>=0)
	{
		r.left=rect.left*16;
		r.top=rect.top*16;
		r.right=rect.right*16;
		r.bottom=rect.bottom*16;

		image->Canvas->Brush->Style=bsClear;
		image->Canvas->Pen->Color=TColor(0xffffff);
		image->Canvas->Rectangle(r);
		r.left+=1;
		r.top+=1;
		r.right-=1;
		r.bottom-=1;
		image->Canvas->Pen->Color=TColor(0x000000);
		image->Canvas->Rectangle(r);
		r.left-=2;
		r.top-=2;
		r.right+=2;
		r.bottom+=2;
		image->Canvas->Pen->Color=TColor(0x000000);
		image->Canvas->Rectangle(r);
	}
}



void __fastcall TFormMain::UpdateTiles(bool updchr)
{
	int i,x,y;

	if(!Visible) return;

	x=0;
	y=0;

	for(i=0;i<256;i++)
	{
		DrawTile(ImageTiles->Picture,x,y,i,palActive,-1,-1,chrSelected[i]);
		x+=16;
		if(x>=256)
		{
			x=0;
			y+=16;
		}
	}

	if(chrSelectRect) DrawSelection(ImageTiles,chrSelection);

	ImageTiles->Repaint();
	if(updchr) FormCHREditor->PaintBoxChr->Repaint();
}



void __fastcall TFormMain::UpdateNameTable(int tx,int ty)
{
	int i,j,x,y;

	if(!Visible) return;

	if(tx<0||ty<0)
	{
		y=0;

		for(i=0;i<30;i++)
		{
			x=0;
			for(j=0;j<32;j++)
			{
				if(SpeedButtonChecker->Down)
				{
					DrawTileChecker(ImageName->Picture,x,y,nameTable[i*32+j],attr_get(j,i),j,i,false);
				}
				else
				{
					DrawTile(ImageName->Picture,x,y,nameTable[i*32+j],attr_get(j,i),j,i,false);
				}

				x+=16;
			}
			y+=16;
		}
	}
	else
	{
		tx&=~1;
		ty&=~1;
		y=ty*16;

		for(i=0;i<2;i++)
		{
			x=tx*16;
			for(j=0;j<2;j++)
			{
				if(SpeedButtonChecker->Down)
				{
					DrawTileChecker(ImageName->Picture,x,y,nameTable[(ty+i)*32+tx+j],attr_get(tx+j,ty+i),tx+j,ty+i,false);
				}
				else
				{
					DrawTile(ImageName->Picture,x,y,nameTable[(ty+i)*32+tx+j],attr_get(tx+j,ty+i),tx+j,ty+i,false);
				}

				x+=16;
			}
			y+=16;
		}
	}

	DrawSelection(ImageName,nameSelection);

	ImageName->Repaint();
}



void __fastcall TFormMain::CopyCHR(bool copy,bool cut)
{
	int i,j,k,pp,ps,x,y,w,h;

	if(chrSelectRect)
	{
		GetSelection(chrSelection,x,y,w,h);
		if(w&&h&&cut) SetUndo();

		pp=0;

		for(i=0;i<h;i++)
		{
			for(j=0;j<w;j++)
			{
				for(k=0;k<16;k++)
				{
					ps=bankActive+(x+j)*16+(y+i)*256+k;
					if(copy) chrCopy[pp++]=chr[ps];
					if(cut) chr[ps]=0;
				}
			}
		}

		if(copy)
		{
			chrCopyWidth=w;
			chrCopyHeight=h;
			chrCopyRect=true;
		}
	}
	else
	{
		if(cut) SetUndo();

		pp=0;
		w=0;
		h=-1;

		for(i=0;i<256;i++)
		{
			if(chrSelected[i])
			{
				for(j=0;j<16;j++)
				{
					ps=bankActive+i*16+j;
					if(copy) chrCopy[pp++]=chr[ps];
					if(cut) chr[ps]=0;
				}
				w++;
			}
		}

		if(copy)
		{
			chrCopyWidth=w;
			chrCopyHeight=h;
			chrCopyRect=false;
		}
	}

	if(cut)
	{
		UpdateTiles(true);
		UpdateNameTable(-1,-1);
	}
}



void __fastcall TFormMain::PasteCHR(void)
{
	int i,j,k,pp,pd,x,y,w,h;

	if(chrCopyRect)
	{
		if(chrCopyWidth<1||chrCopyHeight<1) return;

		GetSelection(chrSelection,x,y,w,h);
		SetUndo();

		pp=0;

		for(i=0;i<chrCopyHeight;i++)
		{
			for(j=0;j<chrCopyWidth;j++)
			{
				if(x+j<16&&y+i<16)
				{
					for(k=0;k<16;k++)
					{
						pd=bankActive+(x+j)*16+(y+i)*256+k;
						chr[pd]=chrCopy[pp+k];
					}
				}
				pp+=16;
			}
		}
	}
	else
	{
		if(chrCopyWidth<1) return;

		pp=0;
		pd=tileActive*16;

		for(i=0;i<chrCopyWidth;i++)
		{
			pd=bankActive+(pd&0x0fff);
			for(j=0;j<16;j++)
			{
				chr[pd++]=chrCopy[pp++];
			}
		}
	}

	UpdateTiles(true);
	UpdateNameTable(-1,-1);
}



void __fastcall TFormMain::NameTableScrollLeft(bool all)
{
	unsigned char temp[32];
	int i,j,k;

	for(k=0;k<(all?2:1);k++)
	{
		for(i=0;i<30;i++) temp[i]=nameTable[i*32];

		for(i=0;i<31;i++)
		{
			for(j=0;j<30;j++)
			{
				nameTable[j*32+i]=nameTable[j*32+i+1];
			}
		}

		for(i=0;i<30;i++) nameTable[i*32+31]=temp[i];
	}

	if(all)
	{
		for(i=0;i<16;i++) temp[i]=attr_get(0,i*2);

		for(i=0;i<15;i++)
		{
			for(j=0;j<16;j++)
			{
				attr_set(i*2,j*2,attr_get(i*2+2,j*2));
			}
		}

		for(i=0;i<16;i++) attr_set(30,i*2,temp[i]);
	}

	UpdateNameTable(-1,-1);
}



void __fastcall TFormMain::NameTableScrollRight(bool all)
{
	unsigned char temp[32];
	int i,j,k;

	for(k=0;k<(all?2:1);k++)
	{
		for(i=0;i<30;i++) temp[i]=nameTable[i*32+31];

		for(i=31;i>0;i--)
		{
			for(j=0;j<30;j++)
			{
				nameTable[j*32+i]=nameTable[j*32+i-1];
			}
		}

		for(i=0;i<30;i++) nameTable[i*32]=temp[i];
	}

	if(all)
	{
		for(i=0;i<16;i++) temp[i]=attr_get(30,i*2);

		for(i=15;i>0;i--)
		{
			for(j=0;j<16;j++)
			{
				attr_set(i*2,j*2,attr_get(i*2-2,j*2));
			}
		}

		for(i=0;i<16;i++) attr_set(0,i*2,temp[i]);
	}

	UpdateNameTable(-1,-1);
}



void __fastcall TFormMain::NameTableScrollUp(bool all)
{
	unsigned char temp[32];
	int i,j,k;

	for(k=0;k<(all?2:1);k++)
	{
		for(i=0;i<32;i++) temp[i]=nameTable[i];

		for(i=0;i<29;i++)
		{
			for(j=0;j<32;j++)
			{
				nameTable[i*32+j]=nameTable[(i+1)*32+j];
			}
		}

		for(i=0;i<32;i++) nameTable[960-32+i]=temp[i];
	}

	if(all)
	{
		for(i=0;i<16;i++) temp[i]=attr_get(i*2,0);

		for(i=0;i<14;i++)
		{
			for(j=0;j<16;j++)
			{
				attr_set(j*2,i*2,attr_get(j*2,i*2+2));
			}
		}

		for(i=0;i<16;i++) attr_set(i*2,28,temp[i]);
	}

	UpdateNameTable(-1,-1);
}



void __fastcall TFormMain::NameTableScrollDown(bool all)
{
	unsigned char temp[32];
	int i,j,k;

	for(k=0;k<(all?2:1);k++)
	{
		for(i=0;i<32;i++) temp[i]=nameTable[960-32+i];

		for(i=29;i>0;i--)
		{
			for(j=0;j<32;j++)
			{
				nameTable[i*32+j]=nameTable[(i-1)*32+j];
			}
		}

		for(i=0;i<32;i++) nameTable[i]=temp[i];
	}

	if(all)
	{
		for(i=0;i<16;i++) temp[i]=attr_get(i*2,28);

		for(i=14;i>0;i--)
		{
			for(j=0;j<16;j++)
			{
				attr_set(j*2,i*2,attr_get(j*2,i*2-2));
			}
		}

		for(i=0;i<16;i++) attr_set(i*2,0,temp[i]);
	}

	UpdateNameTable(-1,-1);
}



bool __fastcall TFormMain::OpenCHR(AnsiString name)
{
	unsigned char buf[4096];
	FILE *file;
	int i,pp,size,type;

	file=fopen(name.c_str(),"rb");

	type=-1;

	if(file)
	{
		size=get_file_size(file);

		switch(size)
		{
		case 8192:
			fread(chr,8192,1,file);
			type=4;
			break;

		case 4096:
			fread(chr+bankActive,4096,1,file);
			type=3;
			break;

		default:
			if(size<4096&&!(size&15))
			{
				fread(buf,size,1,file);
				pp=tileActive*16;
				for(i=0;i<size;i++)
				{
					chr[bankActive+pp++]=buf[i];
					if(pp>=4096) pp=0;
				}
				if(size==2048) type=2;
				if(size==1024) type=1;
			}
			else
			{
				Application->MessageBox("Wrong file size","Error",MB_OK);
				fclose(file);
				return false;
			}
		}
	}

	fclose(file);

	if(type<0) return false;

	SaveDialogChr->FilterIndex=type;
	UpdateTiles(true);
	UpdateNameTable(-1,-1);

	return true;
}



void __fastcall TFormMain::InterleaveCHR(bool dir)
{
	unsigned char buf[4096];
	int table[256];
	int i,j,pp,pd,num;

	SetUndo();

	for(i=0;i<16;i++)
	{
		num=(i/2)*32+(i&1);
		for(j=0;j<16;j++)
		{
			if(dir) table[i*16+j]=num; else table[num]=i*16+j;
			num+=2;
		}
	}

	pp=0;

	for(i=0;i<256;i++)
	{
		pd=table[i]*16;
		for(j=0;j<16;j++)
		{
			buf[pd++]=chr[pp++];
		}
	}

	memcpy(chr,buf,4096);

	UpdateTiles(true);
	UpdateNameTable(-1,-1);
}



bool __fastcall TFormMain::OpenNameTable(AnsiString name)
{
	FILE *file;
	int size;
	unsigned char src[2048],dst[1024];
	int i,tag,pp,pd;
	AnsiString ext;

	ext=GetExt(name);

	file=fopen(name.c_str(),"rb");

	if(file)
	{
		if(ext!="rle")
		{
			size=get_file_size(file);
			if(size==960||size==1024)
			{
				fread(nameTable,960,1,file);
				if(size==1024) fread(attrTable,64,1,file);
				fclose(file);
				UpdateNameTable(-1,-1);
				return true;
			}
			else
			{
				Application->MessageBox("nameTable should be 960 or 1024 bytes long","Error",MB_OK);
			}
		}
		else
		{
			size=get_file_size(file);

			if(size<2048)
			{
				fread(src,size,1,file);
				fclose(file);

				tag=src[0];
				pp=1;
				pd=0;

				while(pp<size)
				{
					if(src[pp]==tag)
					{
						pp++;
						if(src[pp]==0) break;
						for(i=0;i<src[pp];i++) dst[pd++]=dst[pd-1];
						pp++;
					}
					else
					{
						dst[pd++]=src[pp++];
					}
				}
			}

			if(pd==959||pd==1023) pd++;//for old files saved when RLE packer had bug
			if(pd==960||pd==1024) memcpy(nameTable,dst,960);
			if(pd==1024) memcpy(attrTable,dst+960,64);
			if(pd!=960&&pd!=1024) Application->MessageBox("Can't decode RLE","Error",MB_OK);

			UpdateNameTable(-1,-1);
			return true;
		}
	}

	return false;
}



bool __fastcall TFormMain::OpenPalette(AnsiString name)
{
	FILE *file;
	unsigned char pal[16];
	int i;

	file=fopen(name.c_str(),"rb");
	
	if(file)
	{
		if(get_file_size(file)==16)
		{
			fread(pal,16,1,file);
			fclose(file);
			for(i=0;i<4;i++)
			{
				bgPal[0][i]=pal[i];
				bgPal[1][i]=pal[i+4];
				bgPal[2][i]=pal[i+8];
				bgPal[3][i]=pal[i+12];
			}
			pal_validate();
			DrawPalettes();
			UpdateTiles(true);
			UpdateNameTable(-1,-1);
			return true;
		}
		else
		{
			Application->MessageBox("Palette file should be 16 bytes long","Error",MB_OK);
		}
	}

	return false;
}



void __fastcall TFormMain::GetSelection(TRect r,int &x,int &y,int &w,int &h)
{
	if(r.left<r.right)
	{
		x=r.left;
		w=r.right-x;
	}
	else
	{
		x=r.right;
		w=r.left-x;
	}
	if(r.top<r.bottom)
	{
		y=r.top;
		h=r.bottom-y;
	}
	else
	{
		y=r.bottom;
		h=r.top-y;
	}
}



void __fastcall TFormMain::CopyMap(bool cut)
{
	int i,j,x,y,w,h,pp;

	GetSelection(nameSelection,x,y,w,h);
	if(w&&h&&cut) SetUndo();

	pp=0;

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			nameCopy[pp]=nameTable[y*32+x+j];
			attrCopy[pp]=attr_get(x+j,y);
			if(cut&&SpeedButtonTiles->Down) nameTable[y*32+x+j]=0;
			pp++;
		}
		y++;
	}

	nameCopyWidth=w;
	nameCopyHeight=h;

	if(cut) UpdateNameTable(-1,-1);
}



void __fastcall TFormMain::CopyMapCodeASM(void)
{
	char str[65536],buf[1024];
	int i,j,x,y,w,h;

	if(nameSelection.left>=0&&nameSelection.top>=0)
	{
		GetSelection(nameSelection,x,y,w,h);

		strcpy(str,"");

		for(i=0;i<h;i++)
		{
			strcat(str,"\t.db ");
			for(j=0;j<w;j++)
			{
				sprintf(buf,"$%2.2x%c",nameTable[(y+i)*32+x+j],j<w-1?',':'\n');
				strcat(str,buf);
			}
		}

		Clipboard()->SetTextBuf(str);
	}
}



void __fastcall TFormMain::CopyMapCodeC(void)
{
	char str[65536],buf[1024];
	int i,j,x,y,w,h;

	if(nameSelection.left>=0&&nameSelection.top>=0)
	{
		GetSelection(nameSelection,x,y,w,h);

		sprintf(str,"const unsigned char nametable[%i*%i]={\n",w,h);

		for(i=0;i<h;i++)
		{
			strcat(str,"\t");
			for(j=0;j<w;j++)
			{
				sprintf(buf,"0x%2.2x",nameTable[(y+i)*32+x+j]);
				strcat(str,buf);
				if(i*w+j<w*h-1) strcat(str,",");
			}
			strcat(str,"\n");
		}

		strcat(str,"};\n");

		Clipboard()->SetTextBuf(str);
	}
}



void __fastcall TFormMain::CopyMetaSpriteCodeC(bool hflip)
{
	char str[65536],buf[1024];
	int i,j,x,y,w,h;

	if(nameSelection.left>=0&&nameSelection.top>=0)
	{
		GetSelection(nameSelection,x,y,w,h);

		sprintf(str,"const unsigned char metasprite[]={\n");

		if(!hflip)
		{
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{
					sprintf(buf,"%i,%i,0x%2.2x,%i,\n",j*8,i*8,nameTable[(y+i)*32+x+j],attr_get(x+j,y+i));
					strcat(str,buf);
				}
			}
		}
		else
		{
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{
					sprintf(buf,"%i,%i,0x%2.2x,%i|OAM_FLIP_H,\n",j*8,i*8,nameTable[(y+i)*32+x+(w-1-j)],attr_get(x+(w-1-j),y+i));
					strcat(str,buf);
				}
			}
		}

		strcat(str,"128\n};\n");

		Clipboard()->SetTextBuf(str);
	}
}



void __fastcall TFormMain::PasteMap(void)
{
	int i,j,x,y,w,h,pp;

	if(nameCopyHeight<1||nameCopyWidth<1) return;

	GetSelection(nameSelection,x,y,w,h);
	if(w&&h) SetUndo();

	pp=0;

	for(i=0;i<nameCopyHeight;i++)
	{
		for(j=0;j<nameCopyWidth;j++)
		{
			if(x+j<32&&y<30)
			{
				if(SpeedButtonTiles->Down) nameTable[y*32+x+j]=nameCopy[pp];
				if(SpeedButtonPal->Down) attr_set(x+j,y,attrCopy[pp]);
			}
			pp++;
		}
		y++;
	}

	UpdateNameTable(-1,-1);
}



void __fastcall TFormMain::FillMap(void)
{
	int i,j,x,y,w,h;

	GetSelection(nameSelection,x,y,w,h);
	if(w&&h) SetUndo();

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			if(SpeedButtonTiles->Down) nameTable[y*32+x+j]=tileActive;
		}
		y++;
	}

	UpdateNameTable(-1,-1);
}



void __fastcall TFormMain::OpenAll(AnsiString name)
{
	name=RemoveExt(name);

	if(OpenCHR(name+".chr"))
	{
		SaveDialogChr->FileName=name;
	}
	else
	{
		if(OpenCHR(name+".bin")) SaveDialogChr->FileName=name;
	}

	if(OpenNameTable(name+".nam"))
	{
		SaveDialogName->FileName=name;
		SaveDialogAttr->FileName=name;
	}
	else
	{
		if(OpenNameTable(name+".rle"))
		{
			SaveDialogName->FileName=name;
		}
	}

	if(OpenPalette(name+".pal"))
	{
		SaveDialogPal->FileName=name;
	}
}



void __fastcall TFormMain::UpdateStats(void)
{
	AnsiString str;
	int i,cnt,tile,off,col,sel;

	if(tileXC>=0&&tileYC>=0)
	{
		tile=tileYC*16+tileXC;
		cnt=0;

		for(i=0;i<960;i++) if(nameTable[i]==tile) cnt++;

		sel=0;

		for(i=0;i<256;i++) if(chrSelected[i]) sel++;

		str="Tile:$"+IntToHex(tile,2)+" ("+IntToStr(cnt)+" entries)";

		if(sel) str+=" "+IntToStr(sel)+" selected";
		
		LabelStats->Caption=str;

	}

	if(nameXC>=0&&nameYC>=0)
	{
		off=nameYC*32+nameXC;
		str="X:"+IntToStr(nameXC)+" Y:"+IntToStr(nameYC)+" Off:$"+IntToHex(off,3)+" Tile:$"+IntToHex(nameTable[off],2);
		str+=" AtrX:"+IntToStr(nameXC/2)+" AtrY:"+IntToStr(nameYC/2)+" AtrOff:$"+IntToHex(nameYC/4*8+nameXC/4+960,3)+"."+IntToStr((nameXC&2)+(nameYC&2)*2)+" Atr:$"+IntToHex(attrTable[nameYC/4*8+nameXC/4],2)+" Pal:"+IntToStr(attr_get(nameXC,nameYC));
		if(nameSelection.left>=0) str+=" W:"+IntToStr(nameSelection.right-nameSelection.left)+" H:"+IntToStr(nameSelection.bottom-nameSelection.top);
		LabelStats->Caption=str;
	}

	if(palHover>=0) LabelStats->Caption="Pal:"+IntToStr(palHover)+" Entry:"+IntToStr(palColHover)+" Adr:"+IntToHex(0x3f00+palHover*4+palColHover,4)+" Color:$"+IntToHex(bgPal[palHover][palColHover],2);

	if(colHover>=0)
	{
		col=colHover;
		if(col==0x0d||col==0x1d) col=0x0f;
		LabelStats->Caption="Color:$"+IntToHex(col,2);
	}

	if(tileXC<0&&tileYC<0&&nameXC<0&&nameYC<0&&palHover<0&&palColHover<0&&colHover<0) LabelStats->Caption="---";
}



void __fastcall TFormMain::SetUndo(void)
{
	memcpy(undoChr,chr,8192);
	memcpy(undoNameTable,nameTable,960);
	memcpy(undoAttrTable,attrTable,64);
}



void __fastcall TFormMain::Undo(void)
{
	mem_exchange(undoChr,chr,8192);
	mem_exchange(undoNameTable,nameTable,960);
	mem_exchange(undoAttrTable,attrTable,64);

	UpdateTiles(true);
	UpdateNameTable(-1,-1);
}



void __fastcall TFormMain::SetTile(int tile)
{
	int i;

	tileActive=tile;
	chrSelection.left=tile&15;
	chrSelection.top=tile/16;
	chrSelection.right=chrSelection.left+1;
	chrSelection.bottom=chrSelection.top+1;

	for(i=0;i<256;i++) chrSelected[i]=false;
	chrSelected[tile]=true;
	chrSelectRect=true;

	UpdateTiles(true);
}



//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormPaint(TObject *Sender)
{
	DrawPalettes();
	UpdateTiles(true);
	UpdateNameTable(-1,-1);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
	FILE *file;
	int i,pp;
	unsigned char buf[192];
	AnsiString dir;

	bgPalCur=0;
	palActive=0;
	tileActive=0;
	bankActive=0;

	for(i=0;i<960;i++)  nameTable[i]=0;
	for(i=0;i<64;i++)   attrTable[i]=0;
	for(i=0;i<8192;i++) chr[i]=0;
	for(i=0;i<4096;i++) chrCopy[i]=0;

	dir=ParamStr(0).SubString(0,ParamStr(0).LastDelimiter("\\/"));

	file=fopen((dir+"nes.pal").c_str(),"rb");

	if(file)
	{
		if(get_file_size(file)==192)
		{
			fread(buf,192,1,file);
			fclose(file);

			pp=0;

			for(i=0;i<64;i++)
			{
				basePalette[i]=(buf[pp+2]<<16)|(buf[pp+1]<<8)|buf[pp];
				pp+=3;
			}
		}
	}
	else
	{
			pp=0;

			for(i=0;i<64;i++)
			{
				basePalette[i]=(palette[pp+2]<<16)|(palette[pp+1]<<8)|palette[pp];
				pp+=3;
			}
	}

	ppuMask=0;
	palette_calc();

	nameSelection.left=-1;
	nameSelection.top=-1;
	nameCopyWidth=-1;
	nameCopyHeight=-1;

	chrSelection.left=0;
	chrSelection.right=0;
	chrSelection.right=1;
	chrSelection.bottom=1;
	chrCopyWidth=-1;
	chrCopyHeight=-1;
	chrCopyRect=true;
	for(i=0;i<256;i++) chrSelected[i]=false;
	chrSelected[tileActive]=true;
	chrSelectRect=true;

	tileXC=-1;
	tileYC=-1;
	nameXC=-1;
	nameYC=-1;
	palHover=-1;
	palColHover=-1;
	colHover=-1;

	dir=reg_load_str(regWorkingDirectory,"");

	if(ParamStr(1)!="")
	{
		OpenAll(ParamStr(1));
		dir=ParamStr(1);
	}

	SetCurrentDirectory(dir.c_str());

	SetUndo();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxPalMouseDown(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int i,x,y,col;

	y=0;
	x=16;

	for(i=0;i<4;i++)
	{
		if(Y>=y&&Y<y+20)
		{
			if(X>=x   &&X<x+20)
			{
				bgPalCur=0;
				palActive=i;
			}
			if(X>=x+20&&X<x+40)
			{
				bgPalCur=1;
				palActive=i;
			}
			if(X>=x+40&&X<x+60)
			{
				bgPalCur=2;
				palActive=i;
			}
			if(X>=x+60&&X<x+80)
			{
				bgPalCur=3;
				palActive=i;
			}
		}
		x+=128-16;
		if(x>128)
		{
			x=16;
			y=32;
		}
	}

	if(X>=0&&X<14*16&&Y>=64&&Y<64+4*16)
	{
		bgPal[palActive][bgPalCur]=X/16+(Y-64)/16*16;
		col=bgPal[palActive][0];
		bgPal[0][0]=col;
		bgPal[1][0]=col;
		bgPal[2][0]=col;
		bgPal[3][0]=col;
	}

	pal_validate();
	DrawPalettes();
	UpdateTiles(true);
	UpdateNameTable(-1,-1);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ImageTilesMouseDown(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int i;

	if(X>=0&&X<256&&Y>=0&&Y<256)
	{
		if(Shift.Contains(ssCtrl)&&(Shift.Contains(ssLeft)||Shift.Contains(ssRight)))
		{
			chrSelected[Y/16*16+X/16]=Shift.Contains(ssLeft);
			chrSelectRect=false;
		}
		else
		{
			tileActive=Y/16*16+X/16;
			chrSelection.left=X/16;
			chrSelection.top=Y/16;

			chrSelection.right=chrSelection.left+1;
			chrSelection.bottom=chrSelection.top+1;

			for(i=0;i<256;i++) chrSelected[i]=false;
			chrSelected[tileActive]=true;

			chrSelectRect=true;
		}

		nameSelection.left=-1;
		nameSelection.top=-1;

		UpdateTiles(true);
		UpdateNameTable(-1,-1);
		UpdateStats();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonGridAllClick(TObject *Sender)
{
	if(((TSpeedButton*)Sender)->GroupIndex==10)
	{
		SpeedButtonGridTile->Down=SpeedButtonGridAll->Down;
		SpeedButtonGridAtr->Down=SpeedButtonGridAll->Down;
		SpeedButtonGridBlock->Down=SpeedButtonGridAll->Down;
	}
	if(!SpeedButtonGridTile->Down||!SpeedButtonGridAtr->Down||!SpeedButtonGridBlock->Down) SpeedButtonGridAll->Down=false;
	if(SpeedButtonGridTile->Down&&SpeedButtonGridAtr->Down&&SpeedButtonGridBlock->Down) SpeedButtonGridAll->Down=true;

	UpdateTiles(true);
	UpdateNameTable(-1,-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MOpenCHRClick(TObject *Sender)
{
	if(OpenDialogChr->Execute())
	{
		if(OpenCHR(OpenDialogChr->FileName)) SaveDialogChr->FileName=OpenDialogChr->FileName;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxNamePaint(TObject *Sender)
{
	UpdateNameTable(-1,-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageNameMouseDown(TObject *Sender, TMouseButton Button,
TShiftState Shift, int X, int Y)
{
	int i;

	if(X>=0&&X<512&&Y>=0&&Y<480)
	{
		if((Shift.Contains(ssShift)||GetKeyState(VK_CAPITAL))&&Shift.Contains(ssLeft))
		{
			nameSelection.left=X/16;
			nameSelection.top=Y/16;
			nameSelection.right=nameSelection.left+1;
			nameSelection.bottom=nameSelection.top+1;
			UpdateNameTable(-1,-1);

			chrSelection.right=chrSelection.left+1;
			chrSelection.bottom=chrSelection.top+1;

			for(i=0;i<256;i++) chrSelected[i]=false;
			chrSelected[tileActive]=true;
			chrSelectRect=true;

			UpdateTiles(true);
		}
		else
		{
			if(Shift.Contains(ssLeft)) SetUndo();
			ImageNameMouseMove(Sender,Shift,X,Y);

			if(nameSelection.left>=0||nameSelection.top>=0)
			{
				nameSelection.left=-1;
				nameSelection.top=-1;
				UpdateNameTable(-1,-1);
			}
		}
	}

	UpdateStats();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ImageNameMouseMove(TObject *Sender, TShiftState Shift,
int X, int Y)
{
	int i,tx,ty;

	if(X>=0&&X<512&&Y>=0&&Y<480)
	{
		tx=X/16;
		ty=Y/16;
		nameXC=tx;
		nameYC=ty;

		if(Shift.Contains(ssShift)||GetKeyState(VK_CAPITAL))
		{
			if(Shift.Contains(ssLeft))
			{
				nameSelection.right=tx+(tx>=nameSelection.left?1:0);
				nameSelection.bottom=ty+(ty>=nameSelection.top?1:0);
			}
			if(Shift.Contains(ssRight))
			{
				nameSelection.left=-1;
				nameSelection.top=-1;
			}

			UpdateNameTable(-1,-1);
		}
		else
		{
			if(Shift.Contains(ssLeft))
			{
				if(SpeedButtonTiles->Down) nameTable[ty*32+tx]=tileActive;
				if(SpeedButtonPal->Down) attr_set(tx,ty,palActive);
				UpdateNameTable(tx,ty);
			}

			if(Shift.Contains(ssRight))
			{
				tileActive=nameTable[ty*32+tx];
				chrSelection.left=tileActive&15;
				chrSelection.top=tileActive/16;
				chrSelection.right=chrSelection.left+1;
				chrSelection.bottom=chrSelection.top+1;

				for(i=0;i<256;i++) chrSelected[i]=false;
				chrSelected[tileActive]=true;

				palActive=attr_get(tx,ty);
				UpdateTiles(true);
				DrawPalettes();
			}
		}
	}
	else
	{
		nameXC=-1;
		nameYC=-1;
	}

	UpdateStats();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MOpenNameTableClick(TObject *Sender)
{
	if(OpenDialogName->Execute())
	{
		if(OpenNameTable(OpenDialogName->FileName)) SaveDialogName->FileName=OpenDialogName->FileName;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveNameTableOnlyClick(TObject *Sender)
{
	AnsiString name;

	SaveDialogName->FileName=RemoveExt(SaveDialogName->FileName);
	SaveDialogName->Title="Save nametable only";
	
	if(SaveDialogName->Execute())
	{
		name=RemoveExt(SaveDialogName->FileName);

		switch(SaveDialogName->FilterIndex)
		{
		case 2:  name+=".bin"; break;
		case 3:  name+=".rle"; break;
		case 4:  name+=".h"; break;
		default: name+=".nam";
		}

		if(FileExists(name))
		{
			if(Application->MessageBox(("File "+name+" is already exist, overwrite?").c_str(),"Confirm",MB_YESNO)!=IDYES) return;
		}

		save_data(name.c_str(),nameTable,960,SaveDialogName->FilterIndex);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveNameTableAttributesClick(TObject *Sender)
{
	unsigned char buf[1024];
	AnsiString name;

	SaveDialogName->FileName=RemoveExt(SaveDialogName->FileName);
	SaveDialogName->Title="Save nametable and attributes";
	
	if(SaveDialogName->Execute())
	{
		name=RemoveExt(SaveDialogName->FileName);

		switch(SaveDialogName->FilterIndex)
		{
		case 2: name+=".bin"; break;
		case 3: name+=".rle"; break;
		case 4: name+=".h"; break;
		default: name+=".nam";
		}

		memcpy(buf,nameTable,960);
		memcpy(buf+960,attrTable,64);

		if(FileExists(name))
		{
			if(Application->MessageBox(("File "+name+" is already exist, overwrite?").c_str(),"Confirm",MB_YESNO)!=IDYES) return;
		}

		save_data(name.c_str(),nameTable,1024,SaveDialogName->FilterIndex);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MOpenPalettesClick(TObject *Sender)
{
	if(OpenDialogPal->Execute())
	{
		if(OpenPalette(OpenDialogPal->FileName)) SaveDialogPal->FileName=OpenDialogPal->FileName;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSavePalettesClick(TObject *Sender)
{
	FILE *file;
	unsigned char pal[16];
	int i;

	if(SaveDialogPal->Execute())
	{
		pal_validate();

		for(i=0;i<4;i++)
		{
			pal[i]=bgPal[0][i];
			pal[i+4]=bgPal[1][i];
			pal[i+8]=bgPal[2][i];
			pal[i+12]=bgPal[3][i];
		}

		file=fopen(SaveDialogPal->FileName.c_str(),"wb");
		if(file)
		{
			fwrite(pal,16,1,file);
			fclose(file);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPutCodeToClipboardClick(TObject *Sender)
{
	char str[1024],buf[1024];
	int i,j;

	pal_validate();

	sprintf(str,"\tlda #$3f\n\tsta $2006\n\tlda #$00\n\tsta $2006\n\tldx #$%2.2x\n\tstx $2007\n",bgPal[0][0]);

	for(i=0;i<4;i++)
	{
		if(i) strcat(str,"\tstx $2007\n");
		for(j=1;j<4;j++)
		{
			sprintf(buf,"\tlda #$%2.2x\n\tsta $2007\n",bgPal[i][j]);
			strcat(str,buf);
		}
	}

	Clipboard()->SetTextBuf(str);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxPalPaint(TObject *Sender)
{
	DrawPalettes();
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::MCHREditorClick(TObject *Sender)
{
	FormCHREditor->Visible^=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::MImportClick(TObject *Sender)
{
	FILE *file;
	unsigned char *data;
	int i,j,k,l,size,wdt,hgt,pp,ps,off,ptr,cnt,bpp;
	unsigned char tile[16];
	unsigned char chrt[4096];
	unsigned char namet[960];
	bool add;
	AnsiString ext;

	if(OpenDialogImport->Execute())
	{
		ext=GetExt(OpenDialogImport->FileName);

		if(ext=="bmp")//import BMP file
		{
			file=fopen(OpenDialogImport->FileName.c_str(),"rb");

			if(file)
			{
				size=get_file_size(file);
				data=(unsigned char*)malloc(size);
				fread(data,size,1,file);
				fclose(file);

				bpp=data[28];

				if(data[0]!='B'||data[1]!='M'||(bpp!=4&&bpp!=8)||data[29]!=0||read_dword(&data[30]))
				{
					Application->MessageBox("Wrong BMP format, only 4bpp and 8bpp files without compression are supported","Error",MB_OK);
					free(data);
					return;
				}

				off=read_dword(&data[10]);
				wdt=read_dword(&data[18]);
				hgt=read_dword(&data[22]);

				if(wdt!=256||(hgt!=240&&hgt!=-240))
				{
					Application->MessageBox("BMP should be 256x240 pixels","Error",MB_OK);
					free(data);
					return;
				}

				memset(chrt,0,4096);
				memset(namet,0,960);
				ptr=0;
				cnt=0;

				for(i=0;i<30;i++)
				{
					for(j=0;j<32;j++)
					{
						memset(tile,0,16);
						pp=0;

						if(bpp==4)
						{
							for(k=0;k<8;k++)
							{
								/*if(hgt>0) ps=off+(i*8+k)*128+j*4; else */ps=off+(239-i*8-k)*128+j*4;
								for(l=0;l<8;l++)
								{
									tile[pp]|=(data[ps]&(l&1?0x01:0x10))?1<<(7-l):0;
									tile[pp+8]|=(data[ps]&(l&1?0x02:0x20))?1<<(7-l):0;
									if(l&1) ps++;
								}
								pp++;
							}
						}
						else
						{
							for(k=0;k<8;k++)
							{
								/*if(hgt>0) ps=off+(i*8+k)*128+j*4; else */ps=off+(239-i*8-k)*256+j*8;
								for(l=0;l<8;l++)
								{
									tile[pp]|=(data[ps]&1)?1<<(7-l):0;
									tile[pp+8]|=(data[ps]&2)?1<<(7-l):0;
									ps++;
								}
								pp++;
							}
						}

						add=true;

						for(k=0;k<(cnt<256?cnt:256);k++)
						{
							if(memcmp(tile,&chrt[k*16],16)==0)
							{
								namet[i*32+j]=k;
								add=false;
								break;
							}
						}

						if(add)
						{
							if(cnt<256)
							{
								memcpy(&chrt[ptr],tile,16);
								ptr+=16;
								namet[i*32+j]=cnt;
							}
							cnt++;
						}
					}
				}

				if(cnt>=256) Application->MessageBox(("Too many unique characters ("+IntToStr(cnt)+"), "+IntToStr(cnt-256)+" skipped").c_str(),"Warning",MB_OK);

				memcpy(chr+bankActive,chrt,4096);
				memcpy(nameTable,namet,960);

				free(data);
			}
		}

		if(ext=="nes")//import ROM file
		{
			file=fopen(OpenDialogImport->FileName.c_str(),"rb");
			if(file)
			{
				fseek(file,0,SEEK_END);
				size=ftell(file);

				if(size==24592||size==40976)
				{
					fseek(file,size-8192,SEEK_SET);
					fread(chr,8192,1,file);
				}
				else
				{
					Application->MessageBoxA("Only 24K and 40K ROMs are supported","Error",MB_OK);
				}

				fclose(file);
			}
		}

		UpdateTiles(true);
		UpdateNameTable(-1,-1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveCHRClick(TObject *Sender)
{
	unsigned char buf[8192];
	FILE *file;
	int i,pp,size;

	if(SaveDialogChr->Execute())
	{
		switch(SaveDialogChr->FilterIndex)
		{
		case 1: size=1024; break;
		case 2: size=2048; break;
		case 3: size=4096; break;
		default: size=8192;
		}

		file=fopen(SaveDialogChr->FileName.c_str(),"rb");

		if(file)
		{
			fseek(file,0,SEEK_END);
			i=ftell(file);
			fclose(file);

			if(Application->MessageBox((size==i?"Overwrite?":"Previous file has different size! Overwrite?"),"Confirm",MB_YESNO)!=IDYES) return;
		}

		file=fopen(SaveDialogChr->FileName.c_str(),"wb");

		if(file)
		{
			if(size==4096||size==8192)
			{
				if(size==4096)
				{
					memcpy(buf,chr+bankActive,4096);
				}
				else
				{
					memcpy(buf,chr,8192);
				}
			}
			else
			{
				pp=tileActive*16+bankActive;
				for(i=0;i<size;i++)
				{
					buf[i]=chr[pp++];
					if(pp>=4096) pp=0;
				}
			}
			fwrite(buf,size,1,file);
			fclose(file);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormKeyDown(TObject *Sender, WORD &Key,
TShiftState Shift)
{
	if(Shift.Contains(ssShift))
	{
		if(Key=='C')
		{
			CopyMap(false);
			CopyMapCodeC();
			return;
		}
	}

	if(Shift.Contains(ssCtrl))
	{
		if(Key=='Z') Undo();

		if(nameSelection.left<0&&nameSelection.top<0)
		{
			if(Key=='X') CopyCHR(true,true);
			if(Key=='C') CopyCHR(true,false);
			if(Key=='V') PasteCHR();
		}
		else
		{
			if(Key=='X') CopyMap(true);
			if(Key=='C') CopyMap(false);
			if(Key=='V') PasteMap();
			if(Key=='F') FillMap();
		}
	}
	else
	{
		if(nameSelection.left<0&&nameSelection.top<0)
		{
			if(Key==VK_DELETE) CopyCHR(false,true);
			if(Key=='H') FormCHREditor->MirrorHorizontal();
			if(Key=='V') FormCHREditor->MirrorVertical();
		}
	}

	if(Key==VK_LEFT)  NameTableScrollLeft(Shift.Contains(ssCtrl));
	if(Key==VK_RIGHT) NameTableScrollRight(Shift.Contains(ssCtrl));
	if(Key==VK_UP)    NameTableScrollUp(Shift.Contains(ssCtrl));
	if(Key==VK_DOWN)  NameTableScrollDown(Shift.Contains(ssCtrl));

	if(Key=='A')
	{
		SpeedButtonChecker->Down^=true;
		SpeedButtonCheckerClick(Sender);
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::PutDataToClipboardASMClick(TObject *Sender)
{
	char str[1024],buf[1024];
	int i,j;

	strcpy(str,".db ");

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			sprintf(buf,"$%2.2x%c",bgPal[i][j],i*4+j<15?',':'\n');
			strcat(str,buf);
		}
	}

	Clipboard()->SetTextBuf(str);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageTilesDblClick(TObject *Sender)
{
	FormCHREditor->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MOpenClick(TObject *Sender)
{
	if(OpenDialogAll->Execute()) OpenAll(OpenDialogAll->FileName);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FormDestroy(TObject *Sender)
{
	char path[MAX_PATH];
	int len;

	GetCurrentDirectory(len,path);
	reg_save_str(regWorkingDirectory,path);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageNameMouseLeave(TObject *Sender)
{
	nameXC=-1;
	nameYC=-1;
	UpdateStats();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageTilesMouseLeave(TObject *Sender)
{
	tileXC=-1;
	tileYC=-1;
	UpdateStats();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageTilesMouseMove(TObject *Sender,
TShiftState Shift, int X, int Y)
{
	int i,j,tx,ty;
	int xs,ys,wdt,hgt;

	if(X>=0&&X<256&&Y>=0&&Y<256)
	{
		tx=X/16;
		ty=Y/16;
		tileXC=tx;
		tileYC=ty;

		if(Shift.Contains(ssShift)&&!Shift.Contains(ssCtrl))
		{
			if(Shift.Contains(ssLeft))
			{
				chrSelection.right=tx+(tx>=chrSelection.left?1:0);
				chrSelection.bottom=ty+(ty>=chrSelection.top?1:0);
				if(abs(chrSelection.left-chrSelection.right)>1||abs(chrSelection.top-chrSelection.bottom)>1)
				{
					nameSelection.left=-1;
					nameSelection.right=-1;
					UpdateNameTable(-1,-1);
				}

				for(i=0;i<256;i++) chrSelected[i]=false;

				xs=chrSelection.left<chrSelection.right?chrSelection.left:chrSelection.right;
				ys=chrSelection.top<chrSelection.bottom?chrSelection.top:chrSelection.bottom;
				wdt=abs(chrSelection.right-chrSelection.left);
				hgt=abs(chrSelection.bottom-chrSelection.top);

				for(i=0;i<hgt;i++)
				{
					for(j=0;j<wdt;j++)
					{
						chrSelected[(i+ys)*16+j+xs]=true;
					}
				}

				chrSelectRect=true;
			}
		}

		if(Shift.Contains(ssCtrl)&&(Shift.Contains(ssLeft)||Shift.Contains(ssRight)))
		{
			chrSelected[ty*16+tx]=Shift.Contains(ssLeft);
			chrSelectRect=false;
		}
	}
	else
	{
		tileXC=-1;
		tileYC=-1;
	}

	UpdateTiles(false);
	UpdateStats();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPutSelectedBlockToClipboardASMClick(TObject *Sender)
{
	CopyMapCodeASM();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MClearCHRClick(TObject *Sender)
{
	int i;

	if(Application->MessageBox("Do you really want to clear 8K CHR?","Confirm",MB_YESNO)==ID_YES)
	{
		SetUndo();
		for(i=0;i<8192;i++) chr[i]=0;
		UpdateTiles(true);
		UpdateNameTable(-1,-1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonChrBank1Click(TObject *Sender)
{
	bankActive=SpeedButtonChrBank1->Down?0:4096;
	UpdateTiles(true);
	UpdateNameTable(-1,-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MOptimizeCHRClick(TObject *Sender)
{
	int i,j,k,pp,x,y,w,h;
	bool clear[256],skip[256];

	SetUndo();

	GetSelection(chrSelection,x,y,w,h);

	for(i=0;i<256;i++)
	{
		clear[i]=false;
		skip[i]=false;
	}

	if(chrSelectRect)
	{
		if(w>1||h>1)
		{
			for(i=0;i<16;i++)
			{
				for(j=0;j<16;j++)
				{
					skip[i*16+j]=(i>=y&&i<y+h&&j>=x&&j<x+w)?true:false;
				}
			}
		}
	}
	else
	{
		for(i=0;i<256;i++) skip[i]=chrSelected[i];
	}

	for(i=0;i<256;i++)
	{
		if(!clear[i]&&!skip[i])
		{
			for(j=i+1;j<256;j++)
			{
				if(!memcmp(&chr[bankActive+i*16],&chr[bankActive+j*16],16)&&!skip[j])
				{
					clear[j]=true;
					for(k=0;k<960;k++) if(nameTable[k]==j) nameTable[k]=i;
				}
			}
		}
	}

	pp=bankActive;

	for(i=0;i<256;i++)
	{
		if(!clear[i]&&!skip[i])
		{
			while(skip[pp/16]) pp+=16;
			for(j=0;j<960;j++) if(nameTable[j]==i) nameTable[j]=pp/16;
			for(j=0;j<16;j++) chr[pp++]=chr[bankActive+i*16+j];
		}
	}

	for(;pp<4096;pp++) if(!skip[pp/16]) chr[bankActive+pp]=0;

	UpdateTiles(true);
	UpdateNameTable(-1,-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxPalMouseMove(TObject *Sender,
TShiftState Shift, int X, int Y)
{
	int i,x,y,col;

	y=0;
	x=16;

	palHover=-1;
	palColHover=-1;
	colHover=-1;

	for(i=0;i<4;i++)
	{
		if(Y>=y&&Y<y+20)
		{
			if(X>=x   &&X<x+20)
			{
				palHover=i;
				palColHover=0;
				colHover=-1;
			}
			if(X>=x+20&&X<x+40)
			{
				palHover=i;
				palColHover=1;
				colHover=-1;
			}
			if(X>=x+40&&X<x+60)
			{
				palHover=i;
				palColHover=2;
				colHover=-1;
			}
			if(X>=x+60&&X<x+80)
			{
				palHover=i;
				palColHover=3;
				colHover=-1;
			}
		}
		x+=128-16;
		if(x>128)
		{
			x=16;
			y=32;
		}
	}

	if(X>=0&&X<14*16&&Y>=64&&Y<64+4*16)
	{
		colHover=X/16+(Y-64)/16*16;
		palHover=-1;
		palColHover=-1;
	}

	UpdateStats();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxPalMouseLeave(TObject *Sender)
{
	palHover=-1;
	palColHover=-1;
	colHover=-1;
	UpdateStats();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveChrSelectionClick(TObject *Sender)
{
	FILE *file;
	int i,size;

	if(SaveDialogChrSelection->Execute())
	{
		size=0;

		for(i=0;i<256;i++) if(chrSelected[i]) size+=16;

		file=fopen(SaveDialogChrSelection->FileName.c_str(),"rb");

		if(file)
		{
			fseek(file,0,SEEK_END);
			i=ftell(file);
			fclose(file);

			if(Application->MessageBox((size==i?"Overwrite?":"Previous file has different size! Overwrite?"),"Confirm",MB_YESNO)!=IDYES) return;
		}

		file=fopen(SaveDialogChrSelection->FileName.c_str(),"wb");

		if(file)
		{
			for(i=0;i<256;i++)
			{
				if(chrSelected[i]) fwrite(chr+i*16,16,1,file);
			}

			fclose(file);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MInterleaveCHRClick(TObject *Sender)
{
	InterleaveCHR(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MDeinterleaveCHRClick(TObject *Sender)
{
	InterleaveCHR(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSwapColorsCHRClick(TObject *Sender)
{
	bool swap[256*2];
	int i,j,k,pp,col,bit;
	unsigned char paltemp[4];

	FormSwapColors->ShowModal();

	if(FormSwapColors->Swap)
	{
		SetUndo();

		for(i=0;i<256*2;i++) swap[i]=false;

		if(FormSwapColors->Selection)
		{
			for(i=0;i<256;i++) swap[(bankActive/4096*256)+i]=chrSelected[i];
		}
		else
		{
			if(FormSwapColors->WholeCHR)
			{
				for(i=0;i<256*2;i++) swap[i]=true;
			}
			else
			{
				for(i=0;i<256;i++) swap[(bankActive/4096*256)+i]=true;
			}
		}

		for(i=0;i<256*2;i++)
		{
			if(swap[i])
			{
				pp=i*16;

				for(j=0;j<8;j++)
				{
					for(k=0;k<8;k++)
					{
						bit=1<<k;
						col=((chr[pp]&bit)?1:0)|((chr[pp+8]&bit)?2:0);
						col=FormSwapColors->Map[col];
						chr[pp]=(chr[pp]&~bit)|((col&1)<<k);
						chr[pp+8]=(chr[pp+8]&~bit)|((col>>1)<<k);
					}
					pp++;
				}
			}
		}

		if(FormSwapColors->RemapPalette)
		{
			for(i=0;i<4;i++)
			{
				for(j=0;j<4;j++) paltemp[FormSwapColors->Map[j]]=bgPal[i][j];
				for(j=0;j<4;j++) bgPal[i][j]=paltemp[j];
			}
			bgPal[1][0]=bgPal[0][0];
			bgPal[2][0]=bgPal[0][0];
			bgPal[3][0]=bgPal[0][0];
		}

		UpdateTiles(true);
		UpdateNameTable(-1,-1);
		DrawPalettes();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MAddOffsetClick(TObject *Sender)
{
	int i;

	FormNameOffset->ShowModal();

	if(FormNameOffset->MakeOffset)
	{
		SetUndo();

		for(i=0;i<960;i++)
		{
			if(nameTable[i]>=FormNameOffset->From&&nameTable[i]<=FormNameOffset->To) nameTable[i]+=FormNameOffset->Offset;
		}

		UpdateNameTable(-1,-1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMaskBClick(TObject *Sender)
{
	ppuMask^=0x80;
	SpeedButtonMaskB->Caption="B"+IntToStr((ppuMask&0x80?1:0));
	palette_calc();
	UpdateTiles(true);
	UpdateNameTable(-1,-1);
	DrawPalettes();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMaskGClick(TObject *Sender)
{
	ppuMask^=0x40;
	SpeedButtonMaskG->Caption="G"+IntToStr((ppuMask&0x40?1:0));
	palette_calc();
	UpdateTiles(true);
	UpdateNameTable(-1,-1);
	DrawPalettes();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMaskRClick(TObject *Sender)
{
	ppuMask^=0x20;
	SpeedButtonMaskR->Caption="R"+IntToStr((ppuMask&0x20?1:0));
	palette_calc();
	UpdateTiles(true);
	UpdateNameTable(-1,-1);
	DrawPalettes();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMaskMClick(TObject *Sender)
{
	ppuMask^=0x01;
	SpeedButtonMaskM->Caption="M"+IntToStr((ppuMask&0x01?1:0));
	palette_calc();
	UpdateTiles(true);
	UpdateNameTable(-1,-1);
	DrawPalettes();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MExportScreenShotClick(TObject *Sender)
{
	TPicture *picture;
	Graphics::TBitmap *bmp;
	int i,j,x,y;

	SaveDialogImage->FileName=RemoveExt(SaveDialogName->FileName)+".bmp";

	if(SaveDialogImage->Execute())
	{
		picture=new TPicture();
		bmp=new Graphics::TBitmap();
		bmp->SetSize(256,240);
		bmp->PixelFormat=ImageName->Picture->Bitmap->PixelFormat;
		picture->Bitmap=bmp;

		y=0;

		for(i=0;i<30;i++)
		{
			x=0;
			for(j=0;j<32;j++)
			{
				DrawSmallTile(picture,x,y,nameTable[i*32+j],attr_get(j,i),j,i);
				x+=8;
			}
			y+=8;
		}

		picture->SaveToFile(SaveDialogImage->FileName);

		delete bmp;
		delete picture;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MExportTilesetImageClick(TObject *Sender)
{
	TPicture *picture;
	Graphics::TBitmap *bmp;
	int i,x,y;

	SaveDialogImage->FileName=RemoveExt(SaveDialogChr->FileName)+".bmp";
	
	if(SaveDialogImage->Execute())
	{
		picture=new TPicture();
		bmp=new Graphics::TBitmap();
		bmp->SetSize(128,128);
		bmp->PixelFormat=ImageTiles->Picture->Bitmap->PixelFormat;
		picture->Bitmap=bmp;

		x=0;
		y=0;

		for(i=0;i<256;i++)
		{
			DrawSmallTile(picture,x,y,i,palActive,-1,-1);
			x+=8;
			if(x>=128)
			{
				x=0;
				y+=8;
			}
		}

		picture->SaveToFile(SaveDialogImage->FileName);

		delete bmp;
		delete picture;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PutDataToClipboardCClick(TObject *Sender)
{
	char str[1024],buf[1024];
	int i,j;

	strcpy(str,"const unsigned char palette[16]={ ");

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			sprintf(buf,"0x%2.2x%c",bgPal[i][j],i*4+j<15?',':' ');
			strcat(str,buf);
		}
	}

	strcat(str,"};\n");

	Clipboard()->SetTextBuf(str);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPutSelectedBlockToClipboardCClick(TObject *Sender)
{
	CopyMapCodeC();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonCheckerClick(TObject *Sender)
{
	UpdateNameTable(-1,-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPutMetaSpriteToClipboardCClick(TObject *Sender)
{
	CopyMetaSpriteCodeC(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPutMetaSpriteToClipboardHFlipCClick(TObject *Sender)
{
	CopyMetaSpriteCodeC(true);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::MSaveAttributesClick(TObject *Sender)
{
	AnsiString name;

	SaveDialogAttr->FileName=RemoveExt(SaveDialogAttr->FileName);
	SaveDialogAttr->Title="Save attributes only";

	if(SaveDialogAttr->Execute())
	{
		name=RemoveExt(SaveDialogAttr->FileName);

		switch(SaveDialogAttr->FilterIndex)
		{
		case 2: name+=".bin"; break;
		case 3: name+=".rle"; break;
		case 4: name+=".h"; break;
		default: name+=".atr";
		}

		if(FileExists(name))
		{
			if(Application->MessageBox(("File "+name+" is already exist, overwrite?").c_str(),"Confirm",MB_YESNO)!=IDYES) return;
		}

		save_data(name.c_str(),attrTable,64,SaveDialogAttr->FilterIndex);
	}
}
//---------------------------------------------------------------------------

