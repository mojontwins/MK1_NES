//---------------------------------------------------------------------------

#ifndef UnitSwapColorsH
#define UnitSwapColorsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormSwapColors : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TRadioButton *RadioButton4K;
	TRadioButton *RadioButton8K;
	TRadioButton *RadioButtonSelection;
	TCheckBox *CheckBoxPal;
	TButton *ButtonSwap;
	TButton *ButtonCancel;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TLabel *Label1;
	TButton *ButtonReset;
	void __fastcall ButtonSwapClick(TObject *Sender);
	void __fastcall ButtonCancelClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall RadioButton4KClick(TObject *Sender);
	void __fastcall CheckBoxPalClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall ButtonResetClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormSwapColors(TComponent* Owner);
	int Map[4];
	bool Selection;
	bool WholeCHR;
	bool RemapPalette;
	bool Swap;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSwapColors *FormSwapColors;
//---------------------------------------------------------------------------
#endif
