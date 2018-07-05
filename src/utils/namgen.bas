' namgen 0.2 - Converts a .png into a nametable with attributes
' Using a fixed palette and CHR files.
' Copyleft 2016, 2017 The Mojon Twins
' fbc namgen.bas cmdlineparser.bas mtparser.bas
#include "file.bi"
#include "fbpng.bi"
#include "fbgfx.bi"
#include once "crt.bi"

#include "cmdlineparser.bi"
#include "mtparser.bi"

#define RGBA_R( c ) ( CUInt( c ) Shr 16 And 255 )
#define RGBA_G( c ) ( CUInt( c ) Shr  8 And 255 )
#define RGBA_B( c ) ( CUInt( c )        And 255 )
#define RGBA_A( c ) ( CUInt( c ) Shr 24         )

Dim Shared As String patternTable (255)
Dim Shared As Integer pal (15)
Dim Shared As uByte nametable (1023)
Dim Shared As Integer bank

Sub usage 
	Puts "usage:"
	Puts ""
	Puts "$ namgen.exe in=screen.png out=file.nam pals=pals.png chr=tileset.chr [bank=B]"
	Puts ""
	Puts "in             a 256x240 input file. Not checked."
	Puts "out            The output result you are aiming for!"
	Puts "pals           a 16x1 png file with the palette."
	Puts "chr            The charset."
	Puts "bank           0 or 1. 0 is default"
End Sub

Sub readChr (fn As String) 
	Dim As Integer i, j, fIn
	Dim As uByte d

	fIn = FreeFile
	Open fn For Binary As #fIn
	If bank = 1 Then Seek #fIn, 4097
	For i = 0 To 255
		patternTable (i) = ""
		For j = 0 To 15
			Get #fIn, , d
			patternTable (i) = patternTable (i) & Hex (d, 2)
		Next j
	Next i
	Close #fIn
End Sub

Sub readPals (fn As String) 
	Dim As Any Ptr img
	Dim As Integer x

	img = png_load (fn)
	For x = 0 To 15
		pal (x) = Point (x, 0, img)
	Next x
End Sub

Function findColour (c As Integer, wp As Integer) As Integer
	Dim As Integer result
	Dim As Integer i
	For i = 0 To 3
		If c = pal (wp * 4 + i) Then result = i
	Next i
	findColour = result
End Function

Function extractPatternFrom (img As Any Ptr, x0 As Integer, y0 As Integer, wp As Integer) As String
	Dim As String pattern
	Dim As Integer x, y, c, bp1, bp2, bitmask

	pattern = "00000000000000000000000000000000"

	For y = 0 To 7
		bp1 = 0: bp2 = 0
		For x = 0 To 7
			bitmask = (1 Shl (7 - x))
			c = findColour (Point (x0 + x, y0 + y, img), wp)
			If (c And 1) Then bp1 = bp1 Or bitmask
			If (c And 2) Then bp2 = bp2 Or bitmask
		Next x

		Mid (pattern, 1 + y * 2, 2) = Hex (bp1, 2)
		Mid (pattern, 17 + y * 2, 2) = Hex (bp2, 2)
	Next y

	extractPatternFrom = pattern
End Function

Sub mySort (array () As Integer)
	Dim As Integer i, changed
	Do
		changed = 0
		For i = 1 To uBound (array)
			If array (i - 1) > array (i) Then
				Swap array (i - 1), array (i)
				changed = -1
			End If
		Next i
	Loop While changed

	For i = 0 To uBound (array): Puts Hex (array (i), 8): Next i: Puts ""
End Sub

Function sameNonZeroElements (array1withZeroes () As Integer, array2 () As Integer) As Integer
	Dim As Integer i, j, wasThisOneFound
	' For every element in array 1 which is non zero, fetch in array2 

	For i = 0 To uBound (array1withZeroes)
		If array1withZeroes (i) = 0 Then Continue For
		wasThisOneFound = 0
		For j = 0 To uBound (array2)
			If array1withZeroes (i) = array2 (j) Then 
				wasThisOneFound = -1
				Exit For
			End If
		Next j
		If Not wasThisOneFound Then Return 0
	Next i

	Return -1
End Function

Function findPalFrom (img As Any Ptr, x0 As Integer, y0 As Integer) As Integer
	Dim As Integer x, y, c, result, i, j, wp
	Dim As Integer colours (3), cidx, found
	Dim As Integer palTemp (3)

	result = 0
	For j = 0 To 3: colours (j) = &H00000000: Next j

	cidx = 0
	For y = 0 To 15
		For x = 0 To 15
			c = Point (x0 + x, y0 + y, img)
			found = 0
			For j = 0 To cidx - 1
				If c = colours (j) Then found = -1: Exit For
			Next j
			If Not found Then
				If cidx < 4 Then
					colours (cidx) = c: cidx = cidx + 1
				Else
					Puts "rect (" & x0 & ", " & y0 & ")-(" & (x0+15) & ", " & (y0 + 15) & ") has more than 4 colours."
				End If
			End If
		Next x
	Next y

	result = -1

	For wp = 0 To 3
		For i = 0 To 3
			palTemp (i) = pal (wp * 4 + i)
		Next i
		
		If sameNonZeroElements (colours (), palTemp ()) Then result = wp: Exit For
	Next wp

	If result = -1 Then 
		Puts "rect (" & x0 & ", " & y0 & ")-(" & (x0 + 15) & ", " & (y0 + 15) & ") has an unknown colour combination.": result = 0
	Else
		'puts "IT'S THIS -> " & wp:
	End If

	'Puts ""

	findPalFrom = result
End Function

Function findPattern (pattern As String) As Integer
	Dim As Integer result, i

	For i = 0 To 255
		If pattern = patternTable (i) Then result = i: Exit For
	Next i

	findPattern = result
End Function

Sub processImage (fn As String)
	Dim As Any Ptr img
	Dim As Integer x, y
	Dim As String pattern
	Dim As Integer palsMap (15, 15)
	Dim As uByte attribute

	img = png_load (fn)

	' Attempt to detect pals
	For y = 0 To 239 Step 16
		For x = 0 To 255 Step 16
			palsMap (x \ 16, y \ 16) = findPalFrom (img, x, y)
		Next x
	Next y

	' Process patterns
	For x = 0 To 255 Step 8
		For y = 0 To 239 Step 8
			pattern = extractPatternFrom (img, x, y, palsMap (x \ 16, y \ 16))
			nametable ((y \ 8) * 32 + (x \ 8)) = findPattern (pattern)
		Next y 
	Next x

	' Write attrs
	For y = 0 To 15 Step 2
		For x = 0 To 15 Step 2
			attribute = palsMap (x, y) Or (palsMap (x + 1, y) Shl 2) Or (palsMap (x, y + 1) Shl 4) Or (palsMap (x + 1, y + 1) Shl 6)
			nametable (960 + x \ 2 + (y \ 2) * 8) = attribute
		Next x
	Next y
End Sub

Sub writeOutput (fn As String)
	Dim As Integer fOut, i

	fOut = FreeFile

	Open fn For Binary As #fOut

	For i = 0 To 1023
		Put #fOut, , nametable (i)
	Next i

	Close #fOut
End Sub

Dim As String mandatory (3) = { "in", "out", "pals", "chr" }

Puts "namgen v0.2 20160327"
Puts "Converts a .png into a nametable with attributes."
Puts "Copyleft 2016 The Mojon Twins"
Puts ""

sclpParseAttrs
If Not sclpCheck (mandatory ()) Then usage: End
If sclpGetValue ("bank") <> "" Then bank = Val (sclpGetValue ("bank")) Else bank = 0

'' First of all I'm going to read the chr file and create a nice string array
'' So pattern comparison can be done by means of string comparing making my life 
'' easier.

'' Not having to optimize every bit in your program is a joy. And a bore.

readChr sclpGetValue ("chr")

' Setup screen

screenres 640, 480, 32, , -1

'' Load the palete. Don't care about restrictions, the only mandatory thing is
'' that colours in the picture match those in the palette.

readPals sclpGetValue ("pals")

'' Now load and process the input image

processImage sclpGetValue ("in")

'' Write output

writeOutput sclpGetValue ("out")

Puts "DONE!"
