#include "file.bi"
#include "fbpng.bi"
#include "fbgfx.bi"
#include once "crt.bi"

Dim As Any Ptr img
Dim As Integer x, y, f
Dim As Any Ptr imgOut

screenres 640, 480, 32, , -1
imgOut = ImageCreate (128, 32, rgb (0, 0, 0))
img = png_load ("inpal.png")
f = FreeFile

Open "nespal.bas" For Output as #f
Print #f, "Dim As Integer nesPal (63) = {";

For y = 0 To 3
	For x = 0 To 15
		If x = 0 Or x = 8 Then Print #f, " _": Print #f, "	";
		Line imgOut, (x * 8, y * 8) - (7 + x * 8, 7 + y * 8), Point (x * 32, y * 32, img), BF
		Print #f, "&H" & Hex (Point (x * 32, y * 32, img), 8);
		If x < 15 Or y < 3 Then Print #f, ", ";
	Next x
Next y
Print #f, "_ "
Print #f, "}"
Close #f

png_save ("fullnespal.png", imgOut)
