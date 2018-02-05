//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitSwapColors.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSwapColors *FormSwapColors;
//---------------------------------------------------------------------------
__fastcall TFormSwapColors::TFormSwapColors(TComponent* Owner)
: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapColors::ButtonSwapClick(TObject *Sender)
{
	Swap=true;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapColors::ButtonCancelClick(TObject *Sender)
{
	Swap=false;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapColors::FormCreate(TObject *Sender)
{
	Map[0]=0;
	Map[1]=1;
	Map[2]=2;
	Map[3]=3;
	Swap=false;
	CheckBoxPal->Checked=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::RadioButton4KClick(TObject *Sender)
{
	if(RadioButton4K->Checked)
	{
		Selection=false;
		WholeCHR=false;
	}
	if(RadioButton8K->Checked)
	{
		Selection=false;
		WholeCHR=true;
	}
	if(RadioButtonSelection->Checked)
	{
		Selection=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::CheckBoxPalClick(TObject *Sender)
{
	RemapPalette=CheckBoxPal->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::Button1Click(TObject *Sender)
{
	int pp;

	pp=((TButton*)Sender)->Tag;
	Map[pp]=(Map[pp]+1)&3;

	Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::FormPaint(TObject *Sender)
{
	Button1->Caption=Map[0];
	Button2->Caption=Map[1];
	Button3->Caption=Map[2];
	Button4->Caption=Map[3];
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::ButtonResetClick(TObject *Sender)
{
	Map[0]=0;
	Map[1]=1;
	Map[2]=2;
	Map[3]=3;

	Repaint();
}
//---------------------------------------------------------------------------

