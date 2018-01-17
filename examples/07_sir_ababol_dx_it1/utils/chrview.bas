'' Simple chr viewer 
'' Just throw a chr file on the exe, sit and watch.

#include "gui.bi"
#include "fbpng.bi"

Sub saveImage 
	Dim As Any Ptr img
	Dim As Integer y

	img = ImageCreate (256 + 8 + 256, 256, 0)
	Get (0, 0) - (256 + 8 + 255, 255), img
	y = png_save (Command (1) & ".png", img)
End Sub

Dim As Integer fIn, x, y, pattern, row, xx, yy, plane, x0, bank, fine, mbtn
Dim As uByte d 
Dim As uByte planes (1, 7)
Dim As Integer colours (3) = { &HFF000000, &HFF555555, &HFFAAAAAA, &HFFFFFFFF }
Dim As String p2d
Dim As Integer pn, opn, bn, obn

If Command (1) = "" Then ? "$ chrview tileset.chr [save]": System

If Command (2) = "save" Then
	screenres 256 + 8 + 256, 256, 32, , -1
Else
	OpenWindow 256 + 8 + 256, 256+16, "chrview v0.1"
End If

fIn = FreeFile
Open Command (1) For Binary As #fIn
If Lof (fIn) = 0 Then
	Close #fIn
	Kill Command (1)
	System
End If

x0 = 0
For bank = 0 To 1
	x = x0: y = 0

	For pattern = 0 To 255
		For plane = 0 To 1
			For row = 0 To 7
				Get #fIn, , planes (plane, row)
			Next row
		Next plane

		For yy = 0 To 7
			For xx = 0 To 7
				d = 0
				If planes (0, yy) And (1 Shl (7 - xx)) Then d = d + 1
				If planes (1, yy) And (1 Shl (7 - xx)) Then d = d + 2

				Line (x + xx + xx, y + yy + yy) - (1 + x + xx + xx, 1 + y + yy + yy), colours (d), B
			Next xx
		Next yy

		x = x + 16: If x = x0 + 256 Then x = x0: y = y + 16
	Next pattern

	x0 = x0 + 256 + 8
Next

Close #fIn

obn = 99: bn = 3
opn = 99: pn = 0

If Command (2) = "save" Then 
	saveImage
Else
	fine = 0
	While Not fine
		Getmouse x, y, , mbtn
		p2d = Inkey
		If p2d = "s" Then
			saveImage
		ElseIf p2d <> "" Then
			fine = -1
		End If

		If x < 256 Then
			bn = 0
			pn = (x \ 16) + 16 * (y \ 16)
		ElseIf x >= 256+8 Then
			bn = 1
			pn = ((x-256-8)\16) + 16 * (y \ 16)
		Else
			bn = 3
		End If

		If bn = 3 Then
			Line (0, 256) - (127, 256+16), 0, bf
		Else
			If bn <> obn Or pn <> opn Then
				Line (0, 256) - (127, 256+16), 0, bf
				Draw String (0, 256), "BANK " & Hex (bn,1) & ", PATT " & Hex (pn, 2), RGB (255, 255, 255)
			End if
		End If
		obn = bn: opn = pn
	Wend
End If
