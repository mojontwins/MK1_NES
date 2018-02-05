//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitNametableOffset.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormNameOffset *FormNameOffset;
//---------------------------------------------------------------------------
__fastcall TFormNameOffset::TFormNameOffset(TComponent* Owner)
: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormNameOffset::FormCreate(TObject *Sender)
{
	MakeOffset=false;
}
//---------------------------------------------------------------------------
void __fastcall TFormNameOffset::ButtonOkClick(TObject *Sender)
{
	MakeOffset=true;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormNameOffset::ButtonCancelClick(TObject *Sender)
{
	MakeOffset=false;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormNameOffset::EditOffChange(TObject *Sender)
{
	Offset=StrToIntDef(EditOff->Text,0);
	UpDownOff->Position=Offset;
}
//---------------------------------------------------------------------------


void __fastcall TFormNameOffset::EditFromChange(TObject *Sender)
{
	From=StrToIntDef(EditFrom->Text,0);
	UpDownFrom->Position=From;
}
//---------------------------------------------------------------------------

void __fastcall TFormNameOffset::EditToChange(TObject *Sender)
{
	To=StrToIntDef(EditTo->Text,0);
	UpDownTo->Position=To;
}
//---------------------------------------------------------------------------

