object FormNameOffset: TFormNameOffset
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Nametable offset'
  ClientHeight = 146
  ClientWidth = 259
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 120
  TextHeight = 16
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 259
    Height = 146
    Align = alClient
    TabOrder = 0
    object Label1: TLabel
      Left = 12
      Top = 27
      Width = 117
      Height = 16
      Caption = 'Tile numbers range:'
    end
    object Label2: TLabel
      Left = 90
      Top = 69
      Width = 39
      Height = 16
      Caption = 'Offset:'
    end
    object EditOff: TEdit
      Left = 135
      Top = 66
      Width = 33
      Height = 24
      TabOrder = 0
      Text = '0'
      OnChange = EditOffChange
    end
    object UpDownOff: TUpDown
      Left = 168
      Top = 66
      Width = 19
      Height = 24
      Associate = EditOff
      Min = -255
      Max = 255
      TabOrder = 1
    end
    object ButtonOk: TButton
      Left = 93
      Top = 110
      Width = 75
      Height = 25
      Caption = 'Ok'
      TabOrder = 2
      OnClick = ButtonOkClick
    end
    object ButtonCancel: TButton
      Left = 174
      Top = 110
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 3
      OnClick = ButtonCancelClick
    end
    object EditFrom: TEdit
      Left = 135
      Top = 24
      Width = 33
      Height = 24
      TabOrder = 4
      Text = '0'
      OnChange = EditFromChange
    end
    object EditTo: TEdit
      Left = 196
      Top = 24
      Width = 33
      Height = 24
      TabOrder = 5
      Text = '255'
      OnChange = EditToChange
    end
    object UpDownFrom: TUpDown
      Left = 168
      Top = 24
      Width = 19
      Height = 24
      Associate = EditFrom
      Max = 255
      TabOrder = 6
    end
    object UpDownTo: TUpDown
      Left = 229
      Top = 24
      Width = 19
      Height = 24
      Associate = EditTo
      Max = 255
      Position = 255
      TabOrder = 7
    end
  end
end
