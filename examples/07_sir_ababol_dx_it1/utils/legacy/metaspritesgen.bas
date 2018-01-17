'
'

Sub usage 
	Print "metaspritesgen 0.1"
	Print
	Print "Usage:"
	Print
	Print "$ metaspritesgen w h ini xoffs yoffs n pal prefix"
	Print
	Print "Where..."
	Print "* w and h are the size of your metapsrites in tiles."
	Print "* ini is the first tile # in the first metasprite."
	Print "* xoffs, yoffs are the offset of the topmost sprite."
	Print "* n is the number of metasprites to generate."
	Print "* prefix is a C prefix for the labels and identifiers."
	Print
	Print "Will output to stdout. You can always redirect or pipe."
	Print
End Sub

If Command (8) = "" Then 
	usage
	End
End If

Dim As Integer i, n
Dim As Integer x, y
Dim As Integer ti
Dim As Integer w, h, wi, hi
Dim As Integer x0, y0
Dim As Integer pal
Dim As String prefix

' Read command line

w = Val (Command (1))
h = Val (Command (2))
ti = Val (Command (3))
x0 = Val (Command (4))
y0 = Val (Command (5))
n = Val (Command (6))
pal = Val (Command (7))
prefix = Command (8)

' Do

For i = 1 To n
	y = y0
	Print "const char " & prefix & "_" & Hex (i - 1, 2) & " [] = {"
	For hi = 1 To h
		Print "	";
		x = x0
		For wi = 1 To w
			' x, y, ti, p
			Print "0x" & Hex (x, 2) & ", 0x" & Hex (y, 2) & ", 0x" & Hex (ti, 2) & ", 0x" & Hex (pal, 2) & ", "; 
			x = x + 8
			ti = ti + 1
		Next wi
		y = y + 8
		Print
	Next hi
	Print "	";
	Print "0x" & Hex (128, 2)
	Print "};"
	Print
Next i

? "// Next : " & ti & " (0x" & Hex (ti, 2) & ")"