//---------------------------------------------------------------------------

#ifndef UnitCHREditorH
#define UnitCHREditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormCHREditor : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TPaintBox *PaintBoxChr;
	void __fastcall PaintBoxChrPaint(TObject *Sender);
	void __fastcall PaintBoxChrMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall PaintBoxChrMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
public:		// User declarations
	__fastcall TFormCHREditor(TComponent* Owner);
	void __fastcall DrawCHR(int,int,int);
	void __fastcall Draw(void);
	void __fastcall ScrollLeft(void);
	void __fastcall ScrollRight(void);
	void __fastcall ScrollUp(void);
	void __fastcall ScrollDown(void);
	void __fastcall MirrorHorizontal(void);
	void __fastcall MirrorVertical(void);
	void __fastcall Fill(int,int);
	void __fastcall TileChange(int,int);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCHREditor *FormCHREditor;
//---------------------------------------------------------------------------
#endif
