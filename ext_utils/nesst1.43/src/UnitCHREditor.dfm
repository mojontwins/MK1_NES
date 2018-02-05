object FormCHREditor: TFormCHREditor
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'CHR Editor'
  ClientHeight = 288
  ClientWidth = 281
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  OnKeyDown = FormKeyDown
  PixelsPerInch = 120
  TextHeight = 16
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 281
    Height = 288
    Align = alClient
    TabOrder = 0
    object PaintBoxChr: TPaintBox
      Left = 5
      Top = 12
      Width = 272
      Height = 272
      OnMouseDown = PaintBoxChrMouseDown
      OnMouseMove = PaintBoxChrMouseMove
      OnPaint = PaintBoxChrPaint
    end
  end
end
