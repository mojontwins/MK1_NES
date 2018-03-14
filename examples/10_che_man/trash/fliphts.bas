
#include "file.bi"
#include "fbpng.bi"
#include "fbgfx.bi"
#include once "crt.bi"

#define RGBA_R( c ) ( CUInt( c ) Shr 16 And 255 )
#define RGBA_G( c ) ( CUInt( c ) Shr  8 And 255 )
#define RGBA_B( c ) ( CUInt( c )        And 255 )
#define RGBA_A( c ) ( CUInt( c ) Shr 24         )

Sub usage
	Puts "usage: "
	Puts ""
	Puts "$ fliphts.exe in.png out.png"
End Sub

Dim As Any Ptr img, imgOut
Dim As Integer w, h, x, y, xx, yy

Print "fliphts.exe v0.1 20180314"
Print "Horizontally flips every 16x16 tile in a metatileset"
Print "Copyleft 2018 the Mojon Twins"
Print 

If Command (2) = "" Then usage: End

screenres 640, 480, 32, , -1

img = png_load (Command (1))
If ImageInfo (img, w, h, , , , ) Then
	Puts "ERROR reading " & Command (1)
	System
End If

imgOut = ImageCreate (w, h)

Puts "Read " & w & " x " & h

For y = 0 To h - 1 Step 16
	For x = 0 To w - 1 Step 16
		For yy = 0 To 15
			For xx = 0 To 15
				Pset imgOut, (x + 15 - xx, y + yy), Point (x + xx, y + yy, img)

Next xx, yy, x, y

Puts "Writing"

png_save Command (2), imgOut

Puts "DONE!"
