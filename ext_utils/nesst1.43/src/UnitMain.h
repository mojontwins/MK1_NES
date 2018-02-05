//---------------------------------------------------------------------------

#ifndef UnitMainH
#define UnitMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBoxPal;
	TPaintBox *PaintBoxPal;
	TSpeedButton *SpeedButtonPal;
	TLabel *Label1;
	TGroupBox *GroupBoxTiles;
	TSpeedButton *SpeedButtonTiles;
	TLabel *Label2;
	TImage *ImageTiles;
	TSpeedButton *SpeedButtonGridAll;
	TLabel *Label3;
	TOpenDialog *OpenDialogChr;
	TSaveDialog *SaveDialogChr;
	TMainMenu *MainMenu1;
	TMenuItem *MPatterns;
	TMenuItem *MOpenCHR;
	TMenuItem *MSaveCHR;
	TGroupBox *GroupBox1;
	TImage *ImageName;
	TOpenDialog *OpenDialogName;
	TSaveDialog *SaveDialogName;
	TMenuItem *MNameTable;
	TMenuItem *MOpenNameTable;
	TMenuItem *MSaveNameTableOnly;
	TMenuItem *MSaveNameTableAttributes;
	TMenuItem *MPalettes;
	TMenuItem *MOpenPalettes;
	TMenuItem *MSavePalettes;
	TMenuItem *MPutCodeToClipboard;
	TOpenDialog *OpenDialogPal;
	TSaveDialog *SaveDialogPal;
	TMenuItem *MImport;
	TMenuItem *MCHREditor;
	TOpenDialog *OpenDialogImport;
	TMenuItem *PutDataToClipboardASM;
	TMenuItem *MOpen;
	TOpenDialog *OpenDialogAll;
	TGroupBox *GroupBoxStats;
	TLabel *LabelStats;
	TMenuItem *MPutSelectedBlockToClipboardASM;
	TMenuItem *MClearCHR;
	TMenuItem *N1;
	TSpeedButton *SpeedButtonChrBank1;
	TSpeedButton *SpeedButtonChrBank2;
	TMenuItem *MOptimizeCHR;
	TMenuItem *N2;
	TMenuItem *N3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TSpeedButton *SpeedButtonGridTile;
	TSpeedButton *SpeedButtonGridAtr;
	TSpeedButton *SpeedButtonGridBlock;
	TMenuItem *MSaveChrSelection;
	TSaveDialog *SaveDialogChrSelection;
	TMenuItem *MInterleaveCHR;
	TMenuItem *MDeinterleaveCHR;
	TMenuItem *MSwapColorsCHR;
	TMenuItem *MAddOffset;
	TSpeedButton *SpeedButtonMaskB;
	TSpeedButton *SpeedButtonMaskG;
	TSpeedButton *SpeedButtonMaskR;
	TSpeedButton *SpeedButtonMaskM;
	TMenuItem *MExport;
	TMenuItem *MExportScreenShot;
	TMenuItem *MExportTilesetImage;
	TSaveDialog *SaveDialogImage;
	TMenuItem *PutDataToClipboardC;
	TMenuItem *MPutSelectedBlockToClipboardC;
	TMenuItem *N4;
	TSpeedButton *SpeedButtonChecker;
	TLabel *Label11;
	TMenuItem *N5;
	TMenuItem *MPutMetaSpriteToClipboardC;
	TMenuItem *MPutMetaSpriteToClipboardHFlipC;
	TMenuItem *MSaveAttributes;
	TSaveDialog *SaveDialogAttr;
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall PaintBoxPalMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ImageTilesMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall SpeedButtonGridAllClick(TObject *Sender);
	void __fastcall MOpenCHRClick(TObject *Sender);
	void __fastcall PaintBoxNamePaint(TObject *Sender);
	void __fastcall ImageNameMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ImageNameMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall MOpenNameTableClick(TObject *Sender);
	void __fastcall MSaveNameTableOnlyClick(TObject *Sender);
	void __fastcall MSaveNameTableAttributesClick(TObject *Sender);
	void __fastcall MOpenPalettesClick(TObject *Sender);
	void __fastcall MSavePalettesClick(TObject *Sender);
	void __fastcall MPutCodeToClipboardClick(TObject *Sender);
	void __fastcall PaintBoxPalPaint(TObject *Sender);
	void __fastcall MCHREditorClick(TObject *Sender);
	void __fastcall MImportClick(TObject *Sender);
	void __fastcall MSaveCHRClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall PutDataToClipboardASMClick(TObject *Sender);
	void __fastcall ImageTilesDblClick(TObject *Sender);
	void __fastcall MOpenClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall ImageNameMouseLeave(TObject *Sender);
	void __fastcall ImageTilesMouseLeave(TObject *Sender);
	void __fastcall ImageTilesMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall MPutSelectedBlockToClipboardASMClick(TObject *Sender);
	void __fastcall MClearCHRClick(TObject *Sender);
	void __fastcall SpeedButtonChrBank1Click(TObject *Sender);
	void __fastcall MOptimizeCHRClick(TObject *Sender);
	void __fastcall PaintBoxPalMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall PaintBoxPalMouseLeave(TObject *Sender);
	void __fastcall MSaveChrSelectionClick(TObject *Sender);
	void __fastcall MInterleaveCHRClick(TObject *Sender);
	void __fastcall MDeinterleaveCHRClick(TObject *Sender);
	void __fastcall MSwapColorsCHRClick(TObject *Sender);
	void __fastcall MAddOffsetClick(TObject *Sender);
	void __fastcall SpeedButtonMaskBClick(TObject *Sender);
	void __fastcall SpeedButtonMaskGClick(TObject *Sender);
	void __fastcall SpeedButtonMaskRClick(TObject *Sender);
	void __fastcall SpeedButtonMaskMClick(TObject *Sender);
	void __fastcall MExportScreenShotClick(TObject *Sender);
	void __fastcall MExportTilesetImageClick(TObject *Sender);
	void __fastcall PutDataToClipboardCClick(TObject *Sender);
	void __fastcall MPutSelectedBlockToClipboardCClick(TObject *Sender);
	void __fastcall SpeedButtonCheckerClick(TObject *Sender);
	void __fastcall MPutMetaSpriteToClipboardCClick(TObject *Sender);
	void __fastcall MPutMetaSpriteToClipboardHFlipCClick(TObject *Sender);
	void __fastcall MSaveAttributesClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormMain(TComponent* Owner);
	void __fastcall DrawPalettes(void);
	void __fastcall DrawCol(int,int,int,bool);
	void __fastcall DrawBGPal(int,int,int);
	void __fastcall DrawSelection(TImage*,TRect);
	void __fastcall DrawTile(TPicture*,int,int,int,int,int,int,bool);
	void __fastcall DrawTileChecker(TPicture*,int,int,int,int,int,int,bool);
	void __fastcall DrawSmallTile(TPicture*,int,int,int,int,int,int);
	void __fastcall UpdateTiles(bool);
	void __fastcall UpdateNameTable(int,int);
	void __fastcall CopyCHR(bool,bool);
	void __fastcall PasteCHR(void);
	void __fastcall NameTableScrollLeft(bool);
	void __fastcall NameTableScrollRight(bool);
	void __fastcall NameTableScrollUp(bool);
	void __fastcall NameTableScrollDown(bool);
	bool __fastcall OpenCHR(AnsiString);
	void __fastcall InterleaveCHR(bool);
	bool __fastcall OpenNameTable(AnsiString);
	bool __fastcall OpenPalette(AnsiString);
	void __fastcall CopyMap(bool);
	void __fastcall CopyMapCodeASM(void);
	void __fastcall CopyMapCodeC(void);
	void __fastcall PasteMap(void);
	void __fastcall FillMap(void);
	void __fastcall GetSelection(TRect,int&,int&,int&,int&);
	void __fastcall OpenAll(AnsiString);
	void __fastcall UpdateStats(void);
	void __fastcall SetUndo(void);
	void __fastcall Undo(void);
	void __fastcall SetTile(int);
	void __fastcall CopyMetaSpriteCodeC(bool);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
