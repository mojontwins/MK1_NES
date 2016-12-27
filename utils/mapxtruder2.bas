''
'' This program will extrude a ZX Spectrum, Churrera/MK2, 15x10 tiles per screen map
'' To fit a wider, 16x12 playing area, ideal for the NES (and maybe the SMS?)

'' This is what it does:

'' First pass, from this (15x10)
'' ·······A·······
'' ·······A·······
'' ·······A·······
'' ·······A·······
'' ·······A·······
'' ·······A·······
'' ·······A·······
'' ·······A·······
'' ·······A·······
'' ·······A·······

'' To this (16x10)
'' ·······AA·······
'' ·······AA·······
'' ·······AA·······
'' ·······AA·······
'' ·······AA·······
'' ·······AA·······
'' ·······AA·······
'' ·······AA·······
'' ·······AA·······
'' ·······AA·······

'' Second pass, from this (16x10)
'' ················
'' ················
'' ················
'' ················
'' AAAAAAAAAAAAAAAA
'' BBBBBBBBBBBBBBBB
'' ················
'' ················
'' ················
'' ················

'' To this (16x12)
'' ················
'' ················
'' ················
'' ················
'' AAAAAAAAAAAAAAAA
'' AAAAAAAAAAAAAAAA
'' BBBBBBBBBBBBBBBB
'' BBBBBBBBBBBBBBBB
'' ················
'' ················
'' ················
'' ················

'' This should give good results which need little or no editing as per screen basis.

' Vars
Dim As Integer f, w, h, i, x, y, xx, yy, tidx, nPant, maxPants
Redim As uByte myMap (0, 0, 0)
Dim As uByte d

' Read Command
If Len (Command (4)) = 0 Then
	Print "usage: "
	Print "$ mapxtruder2 input.map output.map w h"
	Print "w, h in screens."
	End
End If

w = Val (Command (3))
h = Val (Command (4))
maxPants = w * h
Redim myMap (maxPants - 1, 15, 11)

' Go.

' Read input map
f = Freefile
Open Command (1) For Binary as #f

' Read maxPants screens worth of tiles (150 per screen)
For tidx = 0 To maxPants * 150 - 1
	' Screen location
	x = (tidx \ 15) Mod w
	y = tidx \ (w * 150)
	nPant = y * w + x
	
	' Screen coordinates
	xx = tidx Mod 15
	yy = (tidx \ (15 * w)) Mod 10
	
	' Read byte
	Get #f, , d
	
	' Write to mem
	myMap (nPant, xx, yy) = d
Next tidx

Close f

' Extrude
For i = 0 To maxPants - 1
	
	' First pass: duplicate central column

	For y = 0 To 9
		For x = 14 To 8 Step -1
			myMap (i, x + 1, y) = myMap (i, x, y)
		Next x
	Next y
	
	' Second pass: repeat the two central rows
	For x = 0 To 15
		For y = 9 To 5 Step - 1
			myMap (i, x, y + 2) = myMap (i, x, y)
		Next y
		myMap (i, x, 5) = myMap (i, x, 4)
		myMap (i, x, 6) = myMap (i, x, 7)
	Next x
	
Next i

' Write results
f = Freefile
Open Command (2) For Binary as #f
	
' Now it's 192 bytes per screen
For tidx = 0 To maxPants * 192 - 1
	' Screen location
	x = (tidx \ 16) Mod w
	y = tidx \ (w * 192)
	nPant = y * w + x
	
	' Screen coordinates
	xx = tidx Mod 16
	yy = (tidx \ (16 * w)) Mod 12
		
	Put #f, , myMap (nPant, xx, yy)
Next tidx

Close

Print "mapxtruder2, " & Command (1) & " extruded into " & Command (2) & ". " & maxPants & " screens processed."
