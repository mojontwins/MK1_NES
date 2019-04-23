' namgen v0.5 20190226 - Converts a .png into a nametable with attributes
' Using a fixed palette and CHR files.
' Copyleft 2016, 2017, 2019 The Mojon Twins
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
Dim Shared As Integer binMode

Sub usage 
	Puts "usage:"
	Puts ""
	Puts "$ namgen.exe in=screen.png out=file.nam pals=pals.png chr=tileset.chr [bank=B] [rle[=label]|bin]"
	Puts ""
	Puts "in             a 256x240 input file. Not checked."
	Puts "out            The output result you are aiming for!"
	Puts "pals           a 16x1 png file with the palette."
	Puts "chr            The charset."
	Puts "bank           0 or 1. 0 is default"
	Puts "rle            output is code in .h with rle'd screen using label"
	Puts "               if no label is specified, sanitized 'in' will be used."
	Puts "               if bin is specified, binary output will be produced."
End Sub

Function mySubsts (stIn As String, sFind As String, sReplace As String) As String
	Dim As Integer i, idx
	Dim As String sOut, m

	sOut = ""
	For i = 1 To Len (stIn) 
		m = Mid (stIn, i, 1)
		idx = Instr (sFind, m)
		If idx Then m = Mid (sReplace, idx, 1)
		sOut = sOut & m
	Next i

	mySubsts = sOut
End Function

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

Sub writeRledOutput (fn As String)
	Dim As Integer fOut, i, rleI
	Dim As Integer runMode, namIdx, runLength, sizeT
	Dim As uByte rleByte, runByte
	Dim As uByte rle (2047) ' Should be enough
	Dim As Integer marker
	Dim As uByte marked (255)

	' First of all, find marker
	For i = 0 To 255
		marked (i) = 0
	Next i
	For i = 0 To 1023
		marked (nametable (i)) = 1
	Next i

	marker = -1
	For i = 0 To 255
		If marked (i) = 0 Then marker = i: Exit For
	Next i

	If marker = -1 Then Print "Image must be < 255 patterns to be compressable. Exiting doing nothing.": Exit Sub

	fOut = FreeFile
	rleI = 0
	sizeT = 1024
	rle (rleI) = marker: rleI = rleI + 1

	namIdx = 0: rleByte = marker: runMode = 0
	While namIdx < sizeT
		If runMode Then
			' Read byte
			runByte = nametable (namIdx): namIdx = namIdx + 1
			If runByte = rleByte And runLength < 254 Then
				runLength = runLength + 1
			Else
				' Optimization: Runs of 1 are inefficient.
				' If one happens, substitute for eq. string.
				If runLength = 1 Then
					' Overwrite run marker with actual value:
					rle (rleI - 1) = rleByte
				Else
					' Write runLength
					rle (rleI) = runLength: rleI = rleI + 1
				End If
				runMode = 0
				' Output byte
				rle (rleI) = runByte: rleI = rleI + 1
				rleByte = runByte
			End If
		Else
			' Read byte
			runByte = nametable (namIdx): namIdx = namIdx + 1
			If runByte <> rleByte Then
				' Output byte
				rle (rleI) = runByte: rleI = rleI + 1
				rleByte = runByte
			Else
				' start run
				rle (rleI) = marker: rleI = rleI + 1
				runLength = 1
				runMode = -1
			End If
		End If
	Wend

	If runMode Then rle (rleI) = runLength: rleI = rleI + 1

	' Write end marker
	rle (rleI) = marker: rleI = rleI + 1
	rle (rleI) = 0: rleI = rleI + 1

	If binMode Then
		Open fn For Binary As #fOut
		For i = 0 To rleI - 1
			Put #fOut, , rle (i)
		Next i
		Close #fOut
	Else
		Open fn For Output As #fOut
		Print #fOut, "// RLE'd nametable by namgen v0.5 20190226"
		Print #fOut, ""
		If sclpGetValue ("rle") = "%%TRUE%%" Then
			Print #fOut, "const unsigned char " & Lcase (mySubsts (sclpGetValue ("in"), "\/. ", "____")) & "_rle [] = {";
		Else
			Print #fOut, "const unsigned char " & sclpGetValue ("rle") & " [] = {";
		End If

		For i = 0 To rleI - 1
			If i Mod 16 = 0 Then Print #fOut, "": Print #fOut, "    ";
			Print #fOut, "0x" & lCase (Hex (rle (i), 2));
			If i < rleI - 1 Then Print #fOut, ", ";
		Next i
		Print #fOut, "};"
		Print #fOut, "#define " & Ucase (mySubsts (sclpGetValue ("in"), "\/. ", "____")) & "_RLE_SIZE " & rleI
		Print #fOut, ""
		Close #fOut
	End If
End Sub

Dim As String mandatory (3) = { "in", "out", "pals", "chr" }

Puts "namgen v0.5 20190226"
Puts "Converts a .png into a nametable with attributes."
Puts "Copyleft 2016, 2018 The Mojon Twins"
Puts ""

sclpParseAttrs
If Not sclpCheck (mandatory ()) Then usage: End
If sclpGetValue ("bank") <> "" Then bank = Val (sclpGetValue ("bank")) Else bank = 0
binMode = (sclpGetValue ("bin") <> "")

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

If sclpGetValue ("rle") <> "" Then
	writeRledOutput sclpGetValue ("out")
Else
	writeOutput sclpGetValue ("out")
End If

Puts "DONE!"
