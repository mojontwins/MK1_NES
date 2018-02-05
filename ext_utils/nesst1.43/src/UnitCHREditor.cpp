//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMain.h"
#include "UnitCHREditor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCHREditor *FormCHREditor;

extern int outPalette[];
extern unsigned char bgPal[4][4];
extern int bgPalCur;
extern unsigned char chr[];
extern int palActive;
extern int tileActive;
extern int bankActive;



void __fastcall TFormCHREditor::DrawCHR(int xs,int ys,int tile)
{
	int i,j,x,y,pp,col;
	TRect r;

	y=ys;
	pp=tile*16+bankActive;

	if(tile>=0&&tile<256)
	{
		for(i=0;i<8;i++)
		{
			x=xs;
			for(j=0;j<8;j++)
			{
				col=(((chr[pp+i]<<j)&128)>>7)|(((chr[pp+i+8]<<j)&128)>>6);
				PaintBoxChr->Canvas->Brush->Color=TColor(outPalette[bgPal[palActive][col]]);
				r.left=x;
				r.top=y;
				r.right=x+15;
				r.Bottom=y+15;
				PaintBoxChr->Canvas->FillRect(r);
				x+=16;
			}
			y+=16;
		}
	}
	else
	{
		for(i=0;i<8;i++)
		{
			x=xs;
			for(j=0;j<8;j++)
			{
				PaintBoxChr->Canvas->Brush->Color=FormCHREditor->Color;
				r.left=x;
				r.top=y;
				r.right=x+15;
				r.Bottom=y+15;
				PaintBoxChr->Canvas->FillRect(r);
				x+=16;
			}
			y+=16;
		}
	}
}



void __fastcall TFormCHREditor::Draw(void)
{
	int tx,ty;

	tx=tileActive&15;
	ty=tileActive/16;

	DrawCHR(-64        ,-64        ,ty>0&&tx>0  ?tileActive-17:-1);
	DrawCHR( 64+8      ,-64        ,ty>0        ?tileActive-16:-1);
	DrawCHR( 64+8+128+8,-64        ,ty>0&&tx<15 ?tileActive-15:-1);
	DrawCHR(-64        , 64+8      ,tx>0        ?tileActive-1 :-1);
	DrawCHR( 64+8      , 64+8      ,tileActive);
	DrawCHR( 64+8+128+8, 64+8      ,tx<15       ?tileActive+1 :-1);
	DrawCHR(-64        , 64+8+128+8,ty<15&&tx>0 ?tileActive+15:-1);
	DrawCHR( 64+8      , 64+8+128+8,ty<15       ?tileActive+16:-1);
	DrawCHR( 64+8+128+8, 64+8+128+8,ty<15&&tx<15?tileActive+17:-1);
}



void __fastcall TFormCHREditor::ScrollLeft(void)
{
	int i,pp;

	pp=tileActive*16+bankActive;

	for(i=0;i<16;i++)
	{
		chr[pp]=(chr[pp]<<1)|((chr[pp]>>7)&1);
		pp++;
	}

	Draw();
	FormMain->UpdateNameTable(-1,-1);
	FormMain->UpdateTiles(false);
}



void __fastcall TFormCHREditor::ScrollRight(void)
{
	int i,pp;

	pp=tileActive*16+bankActive;

	for(i=0;i<16;i++)
	{
		chr[pp]=(chr[pp]>>1)|((chr[pp]<<7)&128);
		pp++;
	}

	Draw();
	FormMain->UpdateNameTable(-1,-1);
	FormMain->UpdateTiles(false);
}



void __fastcall TFormCHREditor::ScrollUp(void)
{
	int i,pp,t1,t2;

	pp=tileActive*16+bankActive;
	t1=chr[pp];
	t2=chr[pp+8];

	for(i=0;i<7;i++)
	{
		chr[pp]=chr[pp+1];
		chr[pp+8]=chr[pp+9];
		pp++;
	}

	chr[pp]=t1;
	chr[pp+8]=t2;

	Draw();
	FormMain->UpdateNameTable(-1,-1);
	FormMain->UpdateTiles(false);
}



void __fastcall TFormCHREditor::ScrollDown(void)
{
	int i,pp,t1,t2;

	pp=tileActive*16+7+bankActive;
	t1=chr[pp];
	t2=chr[pp+8];

	for(i=0;i<7;i++)
	{
		chr[pp]=chr[pp-1];
		chr[pp+8]=chr[pp+7];
		pp--;
	}

	chr[pp]=t1;
	chr[pp+8]=t2;

	Draw();
	FormMain->UpdateNameTable(-1,-1);
	FormMain->UpdateTiles(false);
}



void __fastcall TFormCHREditor::MirrorHorizontal(void)
{
	int i,j,pp,tmp;

	pp=tileActive*16+bankActive;

	for(i=0;i<16;i++)
	{
		tmp=0;

		for(j=0;j<8;j++)
		{
			tmp|=(chr[pp]&(128>>j))?1<<j:0;
		}

		chr[pp++]=tmp;
	}

	Draw();
	FormMain->UpdateNameTable(-1,-1);
	FormMain->UpdateTiles(false);
}



void __fastcall TFormCHREditor::MirrorVertical(void)
{
	int i,pp;
	unsigned char tmp[16];

	pp=tileActive*16+bankActive;

	for(i=0;i<8;i++)
	{
		tmp[i]=chr[pp];
		tmp[i+8]=chr[pp+8];
		pp++;
	}
	for(i=0;i<8;i++)
	{
		pp--;
		chr[pp]=tmp[i];
		chr[pp+8]=tmp[i+8];
	}

	Draw();
	FormMain->UpdateNameTable(-1,-1);
	FormMain->UpdateTiles(false);
}



void __fastcall TFormCHREditor::Fill(int x,int y)
{
	unsigned char buf[10][10];
	int i,j,pp,col,cnt;

	pp=tileActive*16+bankActive;

	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			if(i==0||i==9||j==0||j==9)
			{
				buf[i][j]=255;
			}
			else
			{
				buf[i][j]=(chr[pp]&(128>>(j-1))?1:0)|(chr[pp+8]&(128>>(j-1))?2:0);
			}
		}
		if(i>0&&i<9) pp++;
	}

	col=buf[y+1][x+1];

	if(col==bgPalCur) return;

	buf[y+1][x+1]=254;

	while(true)
	{
		cnt=0;
		for(i=1;i<9;i++)
		{
			for(j=1;j<9;j++)
			{
				if(buf[i][j]==254)
				{
					if(buf[i-1][j]==col) buf[i-1][j]=254;
					if(buf[i+1][j]==col) buf[i+1][j]=254;
					if(buf[i][j-1]==col) buf[i][j-1]=254;
					if(buf[i][j+1]==col) buf[i][j+1]=254;
					buf[i][j]=253;
					cnt++;
				}
			}
		}
		if(!cnt) break;
	}

	for(i=1;i<9;i++)
	{
		for(j=1;j<9;j++)
		{
			if(buf[i][j]==253) buf[i][j]=bgPalCur;
		}
	}

	FormMain->SetUndo();

	pp=tileActive*16+bankActive;

	for(i=1;i<9;i++)
	{
		chr[pp]=0;
		chr[pp+8]=0;
		for(j=1;j<9;j++)
		{
			chr[pp]|=(buf[i][j]&1)<<(8-j);
			chr[pp+8]|=((buf[i][j]&2)>>1)<<(8-j);
		}
		pp++;
	}
}



void __fastcall TFormCHREditor::TileChange(int xoff,int yoff)
{
	int tx,ty;

	tx=(tileActive&15)+xoff;
	ty=(tileActive/16)+yoff;

	if(tx<0||tx>15||ty<0||ty>15) return;

	FormMain->SetTile(tileActive+=(yoff*16+xoff));
	Draw();
}



//---------------------------------------------------------------------------
__fastcall TFormCHREditor::TFormCHREditor(TComponent* Owner)
: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormCHREditor::PaintBoxChrPaint(TObject *Sender)
{
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::PaintBoxChrMouseDown(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(!Shift.Contains(ssCtrl)&&!Shift.Contains(ssRight)) FormMain->SetUndo();

	if(Y<64)
	{
		if(X<64) TileChange(-1,-1);
		if(X>=64+8&&X<64+8+128) TileChange(0,-1);
		if(X>=64+8+128+8) TileChange(1,-1);
	}

	if(Y>=64+8&&Y<64+8+128)
	{
		if(X<64) TileChange(-1,0);
		if(X>=64+8+128+8) TileChange(1,0);
	}

	if(Y>=64+8+128+8)
	{
		if(X<64) TileChange(-1,1);
		if(X>=64+8&&X<64+8+128) TileChange(0,1);
		if(X>=64+8+128+8) TileChange(1,1);
	}

	PaintBoxChrMouseMove(Sender,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::PaintBoxChrMouseMove(TObject *Sender,
TShiftState Shift, int X, int Y)
{
	int px,py,pp,mask;

	if(X>=64+8&&X<64+8+128&&Y>=64+8&&Y<64+8+128)
	{
		px=(X-(64+8))/16;
		py=(Y-(64+8))/16;
		pp=tileActive*16+py+bankActive;
		mask=128>>px;

		if(Shift.Contains(ssLeft))
		{
			if(!Shift.Contains(ssCtrl))
			{
				chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
				chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
			}
			else
			{
				Fill(px,py);
			}
			Draw();
			FormMain->UpdateNameTable(-1,-1);
			FormMain->UpdateTiles(false);
		}
		if(Shift.Contains(ssRight))
		{
			bgPalCur=(((chr[pp]<<px)&128)>>7)|(((chr[pp+8]<<px)&128)>>6);
			FormMain->DrawPalettes();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::FormKeyDown(TObject *Sender, WORD &Key,
TShiftState Shift)
{
	if(Shift.Contains(ssCtrl))
	{
		if(Key=='Z') FormMain->Undo();
		if(Key=='X') FormMain->CopyCHR(true,true);
		if(Key=='C') FormMain->CopyCHR(true,false);
		if(Key=='V') FormMain->PasteCHR();
	}
	else
	{
		if(Key==VK_DELETE) FormMain->CopyCHR(false,true);
		if(Key==VK_LEFT)  ScrollLeft();
		if(Key==VK_RIGHT) ScrollRight();
		if(Key==VK_UP)    ScrollUp();
		if(Key==VK_DOWN)  ScrollDown();
		if(Key=='H') MirrorHorizontal();
		if(Key=='V') MirrorVertical();
	}
}
//---------------------------------------------------------------------------


