//---------------------------------------------------------------------------

#ifndef UnitNametableOffsetH
#define UnitNametableOffsetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormNameOffset : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TEdit *EditOff;
	TUpDown *UpDownOff;
	TButton *ButtonOk;
	TButton *ButtonCancel;
	TEdit *EditFrom;
	TEdit *EditTo;
	TUpDown *UpDownFrom;
	TUpDown *UpDownTo;
	TLabel *Label1;
	TLabel *Label2;
	void __fastcall ButtonOkClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonCancelClick(TObject *Sender);
	void __fastcall EditOffChange(TObject *Sender);
	void __fastcall EditFromChange(TObject *Sender);
	void __fastcall EditToChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormNameOffset(TComponent* Owner);
	int From;
	int To;
	int Offset;
	bool MakeOffset;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormNameOffset *FormNameOffset;
//---------------------------------------------------------------------------
#endif
