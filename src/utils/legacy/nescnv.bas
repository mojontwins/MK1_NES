' nescnv
' by na_th_an
' fbc nescnv.bas pnginterface.bas'

#include "fbgfx.bi"
#include once "crt.bi"

#include "pnginterface.bi"

#define RGBA_R( c ) ( CUInt( c ) Shr 16 And 255 )
#define RGBA_G( c ) ( CUInt( c ) Shr  8 And 255 )
#define RGBA_B( c ) ( CUInt( c )        And 255 )
#define RGBA_A( c ) ( CUInt( c ) Shr 24         )

Sub Usage () 
	Print "** USO **"
	Print "   nescnv in.png"
	Print "Genera tileset.chr"
	Print
End Sub

dim as any ptr img 			' will contain the image loaded from the PNG
dim as FB.Image ptr img2
dim as String filename1
dim as String filename2
dim as integer i, j, x, y, xx, yy, xxx, yyy,f
dim as byte myb

If Len (Command (1)) = 0  Then 
	Usage
	End
End If

' Primero cargo el archivo de imagen
screenres 640, 480, 32, , -1

filename1 = Command (1)
img = imageread_png ( filename1,32 )

if img then
'
else
	puts "Failed to load"
end if

f=freefile
open "tileset.chr" for binary as #f

x = 0
y = 0
for i = 0 to 511

	' write 8x8 lsbits (1 if colour = RED or BLUE)
	For yy = 0 to 7
		myb = 0
		For xx = 0 to 7
			If RGBA_R(point (x + xx, y + yy, img)) <> 0 Or RGBA_B(point (x + xx, y + yy, img)) <> 0 Then
				myb = myb Or (1 Shl (7 - xx))
			End If
		Next xx
		put #f,,myb
	next yy
	
	' write 8x8 msbits (1 if colour = GREEN or BLUE)
	For yy = 0 to 7
		myb = 0
		For xx = 0 to 7
			If RGBA_G(point (x + xx, y + yy, img)) <> 0 Or RGBA_B(point (x + xx, y + yy, img)) <> 0 Then
				myb = myb Or (1 Shl (7 - xx))
			End If
		Next xx
		put #f,,myb
	next yy
	
	x = x + 8
	If x = 256 Then 
		x = 0
		y = y + 8
	End If
next i

close (f)

puts "�Todo correcto!"
