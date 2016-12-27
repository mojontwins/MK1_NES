' mkts v 0.1
' Converts several kinds of tilesets
' Copyleft 2015 by The Mojon Twins

' fbc mkts.bas cmdlineparser.bas

#include "file.bi"
#include "fbpng.bi"
#include "fbgfx.bi"
#include once "crt.bi"

#include "cmdlineparser.bi"

#define RGBA_R( c ) ( CUInt( c ) Shr 16 And 255 )
#define RGBA_G( c ) ( CUInt( c ) Shr  8 And 255 )
#define RGBA_B( c ) ( CUInt( c )        And 255 )
#define RGBA_A( c ) ( CUInt( c ) Shr 24         )

Dim Shared As Integer verbose, silent, mainIdx, cPoolIndex, tMapsIndex, flipped
Dim Shared As uByte mainBin (65535)
Dim Shared As String cPool (255)
Dim Shared As uByte tMaps (255, 63)
Dim Shared As Integer nesPal (63) = { _
	&HFF7F7F7F, &HFF0000FF, &HFF0000BF, &HFF472BBF, &HFF970087, &HFFAB0023, &HFFAB1300, &HFF8B1700, _
	&HFF533000, &HFF007800, &HFF006B00, &HFF005B00, &HFF004358, &HFF000000, &HFF000000, &HFF000000, _
	&HFFBFBFBF, &HFF0078F8, &HFF0058F8, &HFF6B47FF, &HFFDB00CD, &HFFE7005B, &HFFF83800, &HFFE75F13, _
	&HFFAF7F00, &HFF00B800, &HFF00AB00, &HFF00AB47, &HFF008B8B, &HFF2C2C2C, &HFF000000, &HFF000000, _
	&HFFF8F8F8, &HFF3FBFFF, &HFF6B88FF, &HFF9878F8, &HFFF878F8, &HFFF85898, &HFFF87858, &HFFFFA347, _
	&HFFF8B800, &HFFB8F818, &HFF5BDB57, &HFF58F898, &HFF00EBDB, &HFF787878, &HFF000000, &HFF000000, _
	&HFFFFFFFF, &HFFA7E7FF, &HFFB8B8F8, &HFFD8B8F8, &HFFF8B8F8, &HFFFBA7C3, &HFFF0D0B0, &HFFFFE3AB, _
	&HFFFBDB7B, &HFFD8F878, &HFFB8F8B8, &HFFB8F8D8, &HFF00FFFF, &HFFF8D8F8, &HFF000000, &HFF000000 _ 
}

Sub filteredPuts (s As String)
	If Not silent Then Puts s
End Sub

Sub usage
	Print "Usage:"
	Print
	Print "$ mkts.exe in=file.png pals=pals.png out=output.bin mode=mode tsmap=tsmap.h"
	Print "           offset=x,y size=w,h metasize=w,h label=label tmapoffs=offset max=n"
	Print "           genflipped silent"
	Print
	Print "in             Is the input file."
	Print "pals           Must be a 16x1 png image with the 4 pals, ordered."
	Print "out            Output binary (partial CHR ROM you must handle)"
	Print "mode           chars does straight 8x8 conversion -> binary"
	Print "               tiles does straight 16x16 conversion -> binary, v-h ordered"
	Print "               mapped does metatile conversion, with ts mapping"
	Print "               sprites is for spritesets, generates metasprites w/f and tsm"
	Print "               pals calculates the palette values, outputs code to out"
	Print "tsmap          Will output palette # in tiles mode, full tsmap in mapped mode"
	Print "offset         (x, y) to start from (in chars)"
	Print "size           (w, h) size of rectangle to process (in chars, tiles or mtiles)"
	Print "metasize       (w, h) size of metatiles (in chars), needed for tiles and mapped"
	Print "sprorg         (x, y) in pixels for topmost sprite in metasprites."
	Print "label          Used in the generated code."
	Print "tmapoffset     Offset chars in tmaps by this amount. Can be omitted."
	Print "max            Process only #max items in the area. Can be omitted."
	Print "genflipped     For sprites mode, generates flipped metas for each meta."
	Print
	Print "Full tsmap in mapped mode is t11, t12, t21, t22 plus a separate palette array."
	Print "in tiles mode only the palette array will be written to tsmap.h but not needed."
	Print "in chars mode tsmap will be ignored."
	Print
	Print "If offset is omitted, cut will start at 0, 0. "
	Print "If size is omitted, will process the whole image (nearest multiple of 8)"
	Print "If metasize is omitted, will assume 2x2."
	Print
	Print "Palette output uses label for palette name in generated code."
End Sub

Sub mbWrite (v As uByte)
	mainBin (mainIdx) = v
	mainIdx = mainIdx + 1
End Sub

Function nesColour (c As Integer) As uByte
	Dim As uByte i
	
	If c = 0 Then
		nesColour = &H0F
	End If
	
	For i = 0 To 63
		If nesPal (i) = c Then 
			If i = &H0D Then i = &H0F	' Safe
			nesColour = i
			Exit Function
		End If
	Next i
	nesColour = &HFF
End Function

Sub doPals (pal () As Integer, outputFn As String, inputFn As String, label As String)
	Dim As Integer i, f
	
	f = FreeFile
	Open outputFn For Output as #f
	Print #f, "// Palette data from " & inputFn
	Print #f, "const unsigned char " & label & " [] = {"
	
	For i = 0 To 15
		If (i And 3) = 0 Then Print #f, "	";
		Print #f, "0x" & Hex (nesColour (pal (i)), 2);
		If i < 15 Then Print #f, ", ";
		If (i And 3) = 3 Then Print #f, ""
	Next i
	
	Print #f, "};"
	Print #f, ""
	
	Close #f
	
	If silent Then Puts ("mkts, pals mode. Palettes from " & inputFn & " extracted to " & outputFn & ", label = " & label)
End Sub

Sub findPalette (img As Any Ptr, pal () As Integer) 
	Dim As Integer i, cur, found
	Dim As Integer x, y, w, h
	Dim As Integer c
	
	If ImageInfo (img, w, h, , , , ) Then
		' Error!
	End If
	
	cur = 0
	
	For y = 0 To h - 1
		For x = 0 To w - 1
			'c = smsColour (Point (x, y, img))
			c = Point (x, y, img)
			found = 0
			For i = 0 To cur - 1
				If c = pal (i) Then found = -1: Exit For
			Next i
			If Not found Then
				If cur > 15 Then
					filteredPuts "Warning: Palette limit reached. More than 16 colours found in palette file."
				Else
					filteredPuts "    Found Colour " & RGBA_R (c) & ", " & RGBA_G (c) & ", " & RGBA_B (c)
					pal (cur) = c
					cur = cur + 1
				End If
			End If
		Next x
	Next y
End Sub

Sub findStraightPalette (img As Any Ptr, pal () As Integer) 
	Dim As Integer x
	
	For x = 0 To 15
		pal (x) = Point (x, 0, img)
	Next x
End Sub

Function findColour (c As Integer, pal () As Integer) As Integer
	Dim As Integer res
	Dim As Integer i
	res = 0
	For i = 0 To 15
		If pal (i) = c Then res = i: Exit For
	Next i
	findColour = res
End Function

Sub extractPatternFrom (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, pattern () As uByte)
	Dim As Integer x, y, c, idx
	Dim As uByte bitmask
	
	' There are two bitplanes.
	' Bitplane A => 1 if pixel is 1 or 3 (lsbits)
	' Bitplane B => 1 if pixel is 2 (msbits)

	' Bitplanes are not interlaced. First 8 rows of bitplane A, then 8 rows of bitplane B.
	
	idx = 0
	For y = 0 To 7
		pattern (idx) = 0
		pattern (idx + 8) = 0
		For x = 0 To 7
			bitmask = (1 Shl (7 - x))
			c = findColour (Point (x0 + x, y0 + y, img), pal ()) And 3
			If (c And 1) Then
				pattern (idx) = pattern (idx) Or bitmask
			End If
			If (c And 2) Then
				pattern (idx + 8) = pattern (idx + 8) Or bitmask
			End If
		Next x
		idx = idx + 1
	Next y
End Sub

Function findWhichPal (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, tn As Integer, hSize As Integer, vSize As Integer) As Integer
	Dim As Integer x, y, c, i, j, pi, found, different
	Dim As Integer whichCols (3)
	Dim As Integer idx, said
	
	For i = 0 To 3: whichCols (i) = &HFF: Next i
	
	idx = 0
	said = 0
	For y = 0 To vSize - 1
		For x = 0 To hSize - 1
			c = Point (x0 + x, y0 + y, img)
			If c = &HFFFF00FF Then 
				findWhichPal = &H20
				Exit Function
			End If
			found = 0
			For i = 0 To idx - 1
				If whichCols (i) = c Then 
					found = -1
					Exit For
				End If
			Next i
			If Not found Then
				If idx < 4 Then
					whichCols (idx) = c
					idx = idx + 1
				Else
					If Not (said) Then
						said = -1
						filteredPuts ("** WARNING ** " & hSize & "x" & vSize & " tile #" & tn & " has more than 4 colours")
					End If
				End If
			End If
		Next x
	Next y
	
	'puts ("TILE " & tn)
	' All four palettes
	For pi = 0 To 3
		' Is whichCols === current palete #pi?
		different = 0
		For i = 0 To 3
			If whichCols (i) <> &HFF Then
				found = 0
				For j = 0 To 3
					
					If whichCols (i) = pal (pi * 4 + j) Then
						found = -1
						Exit For
					End If
				Next j
			Else
				found = -1
			End If
			If Not found Then
				different = -1
				Exit For
			End If
		Next i
		
		' If found = 0 at this point, THIS is the palette we are looking for
		If Not different Then
			findWhichPal = pi
			'Puts ("   -> Palette is " & pi)
			Exit Function
		End If
	Next pi
	
	filteredPuts ("** WARNING ** 16x16 tile #" & tn & " with a WRONG palette")
	findWhichPal = 0
End Function

Sub coordinatesStringToArray (coordsString as String, coords () As Integer)
	Dim As Integer i, idx
	Dim As String m, coordString
	
	coordsString = coordsString & ","
	idx = 0
	
	For i = 1 To Len (coordsString)
		m = Mid (coordsString, i, 1)
		If m = "," Then
			coords (idx) = Val (coordString)
			idx = idx + 1
			coordString = ""			
		Else
			coordString = coordString & m
		End If
	Next i
End Sub

Sub showNesPal (pal () As Integer)
	Dim As Integer i
	Dim As String palStr
	For i = 0 To 15
		If (i And 3) = 0 Then palStr = "	"
		palStr = palStr & "0x" & Lcase (Hex (nesColour (pal (i)), 2))
		If i < 15 Then palStr = palStr & ", "
		If (i And 3) = 3 Then filteredPuts (palStr)
	Next i
End Sub

Sub copyArrayToMainBin (array () As uByte)
	Dim As Integer i
	
	For i = Lbound (array) To Ubound (array)
		mbWrite array (i)
	Next i
End Sub

' doChars img, pal (), xOrg, yOrg, wProc, hProc
Sub doChars (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, max As Integer)
	Dim As Integer x0, y0
	Dim As Integer x1, y1
	Dim As Integer x, y
	Dim As Integer ct
	Dim As uByte pattern (15)
	
	' I know I can calculate ct from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * 8 - 1
	y1 = y0 + hProc * 8 - 1
	ct = 0
	
	filteredPuts ("Processing image in 'chars' mode")
	
	For y = y0 To y1 Step 8
		For x = x0 To x1 Step 8
			extractPatternFrom img, x, y, pal (), pattern ()
			copyArrayToMainBin pattern ()
			ct = ct + 1
			If max <> -1 And ct = max Then Exit For
		Next x
		If max <> -1 And ct = max Then Exit For
	Next y
	
	filteredPuts ("+ " & ct & " patterns extracted.")
	If silent Then Puts ("mkts, chars mode. " & ct & " patterns extracted")
End Sub

Sub doTiles (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, max As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, fOut, xx, yy, xa, ya, wp
	Dim As Integer hSize, vSize
	Dim As uByte pattern (15)
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	' Open tsmap file
	If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then
		fOut = FreeFile
		Open tsMapFn For Output As #fOut	

		' Write some headers to output tsmap file
		Print #fOut, "// Tileset pals generated by mkts"
		Print #fOut, "// Copyleft 2015 The Mojon Twins"
		Print #fOut, " "
		Print #fOut, "const unsigned char " & label & "_pals [] = {"
	End If
		
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'tiles' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	If wMeta = 2 And hMeta = 2 Then filteredPuts ("+ Writing pal info to " & tsMapFn)
	
	For y = y0 To y1 Step vSize
		Print #fOut, "	";
		For x = x0 To x1 Step hSize
			' Find which pal
			If wMeta = 2 And hMeta = 2 Then 
				wp = findWhichPal (img, x, y, pal (), ctt, hSize, vSize)
				If tsMapFn <> "" Then 
					Print #fOut, wp
					If y < y1 Or x < x1 Then Print #fOut, ", ";
				End If
			End If
			
			ya = y
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					extractPatternFrom img, xa, ya, pal (), pattern ()
					copyArrayToMainBin pattern ()
					ct = ct + 1
					xa = xa + 8
				Next xx
				ya = ya + 8
			Next yy
			
			ctt = ctt + 1
			If max <> -1 And ctt = max Then Exit For
		Next x
		If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then Print #fOut, ""
		If max <> -1 And ctt = max Then Exit For
	Next y
	
	' Hang up...
	If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then
		Print #fOut, "};"
		Close #fOut
	End If
	
	filteredPuts ("+ " & ct & " patterns extracted from " & ctt & " tiles.")
	If silent Then Puts ("mkts, tiles mode. " & ct & " patterns extracted from " & ctt & " " & wMeta & "x" & hMeta & " tiles.")
End Sub

'patternS = patternToString (pattern ())

Function patternToString (pattern () As uByte) As String
	Dim As String res
	Dim As Integer i
	
	For i = 0 To 15
		res = res & Hex (pattern (i), 2)
	Next i
	
	patternToString = res
End Function

'ct = findPatternInPoolAndAdd ()

Function findPatternInPoolAndAdd (patternS As String, ByRef wasNew As Integer) As Integer
	Dim As Integer res, i
	
	For i = 0 To cPoolIndex - 1
		If cPool (i) = patternS Then
			findPatternInPoolAndAdd = i
			wasNew = 0
			Exit Function
		End If
	Next i
	
	' Add
	cPool (cPoolIndex) = patternS
	
	' Next
	cPoolIndex = cPoolIndex + 1
	
	' Return
	wasNew = -1
	findPatternInPoolAndAdd = cPoolIndex - 1
End Function

' cPoolIndex, tMapsIndex

Sub doTmaps (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, fOut, xx, yy, xa, ya, wp, found, i, j
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pattern (15), tMap (63)
	Dim As String patternS
	Dim As Integer wasNew
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	fOut = FreeFile
	Open tsMapFn For Output As #fOut	

	' Write some headers to output tsmap file
	Print #fOut, "// Tileset tilemaps & pals generated by mkts"
	Print #fOut, "// Copyleft 2015 The Mojon Twins"
	Print #fOut, " "
	Print #fOut, "const unsigned char " & label & "_pals [] = {"
		
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'meta' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	If wMeta = 2 And hMeta = 2 Then filteredPuts ("+ Writing pal info to " & tsMapFn)
	
	cPoolIndex = 0: tMapsIndex = 0
	
	For y = y0 To y1 Step vSize
		Print #fOut, "	";
		For x = x0 To x1 Step hSize
			' Find which pal
			If wMeta = 2 And hMeta = 2 Then 
				wp = findWhichPal (img, x, y, pal (), tMapsIndex, hSize, vSize)
				If tsMapFn <> "" Then 
					Print #fOut, Ltrim (Str (wp));
					If y < y1+1-vSize Or x < x1+1-hSize Then Print #fOut, ", ";
				End If
			End If
			
			ya = y
			tMapI = 0
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					extractPatternFrom img, xa, ya, pal (), pattern ()
					patternS = patternToString (pattern ())
					ct = findPatternInPoolAndAdd (patternS, wasnew)
					If wasnew Then copyArrayToMainBin pattern ()
					tMap (tMapI) = ct + offset
					tMapI = tMapI + 1
					xa = xa + 8
				Next xx
				ya = ya + 8
			Next yy
		
			' Store tmap
			For i = 0 To (hMeta * wMeta) - 1
				tMaps (tMapsIndex, i) = tMap (i)
			Next i
			tMapsIndex = tMapsIndex + 1
			If max <> -1 And tMapsIndex = max Then Exit For
		Next x
		If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then Print #fOut, ""
		If max <> -1 And tMapsIndex = max Then Exit For
	Next y
	
	' Hang up...
	If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then
		Print #fOut, "};"
		Print #fOut, ""
	End If
	
	' Write tmaps
	Print #fOut, "const unsigned char " & label & "_tmaps [] = {"
	For i = 0 To tMapsIndex - 1
		Print #fOut, "	";
		For j = 0 To (hMeta * wMeta) - 1
			Print #fOut, tMaps (i, j);
			If j < (hMeta * wMeta) - 1 Or i < tMapsIndex - 1 Then Print #fOut, ", ";
		Next j
		Print #fOut, ""
	Next i
	Print #fOut, "};"
	
	filteredPuts ("+ " & tMapsIndex & " tmaps extracted using " & cPoolIndex & " tiles.")
	Close #fOut
	
	If silent Then Puts ("mkts, mapped mode. " & cPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " tiles. Next = " & cPoolIndex + offset)
End Sub

Sub doSprites (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, sprOrgX As Integer, sprOrgY As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, fOut, xx, yy, xa, ya, wp, found, i, j
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pattern (15), tMap (63)
	Dim As String patternS
	Dim As Integer wasNew
	Dim As uByte metapals (63)
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	fOut = FreeFile
	Open tsMapFn For Output As #fOut	

	' Write some headers to output tsmap file
	Print #fOut, "// Metaspriteset generated by mkts"
	Print #fOut, "// Copyleft 2015 The Mojon Twins"
	Print #fOut, " "
		
	If flipped Then Print #fOut, "// Each cell: A = normal, B = flipped": Print #fOut, " "
	
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'sprites' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	filteredPuts ("+ Writing metasprites info to " & tsMapFn)
	
	cPoolIndex = 0: tMapsIndex = 0
	
	For y = y0 To y1 Step vSize
		For x = x0 To x1 Step hSize
			ya = y
			tMapI = 0
			
			Print #fout, "// Cell #" & tMapsIndex & " cut from (" & x & ", " & y & ") to (" & (x + hSize - 1) & ", " & (y + vSize - 1) & ")"
			Print #fout, ""
			
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					wp = findWhichPal (img, x, y, pal (), ctt, 8, 8)
					extractPatternFrom img, xa, ya, pal (), pattern ()
					patternS = patternToString (pattern ())
					ct = findPatternInPoolAndAdd (patternS, wasnew)
					If wasnew Then copyArrayToMainBin pattern ()
					tMap (tMapI) = ct + offset
					metapals (tMapI) = wp
					tMapI = tMapI + 1
					xa = xa + 8
				Next xx
				ya = ya + 8
			Next yy
			
			' Write metaspriteset
			If flipped Then 
				Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_a [] = {"
			Else
				Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & " [] = {"
			End If
			tMapI = 0
			ya = sprOrgY
			For yy = 1 To hMeta
				xa = sprOrgX
				Print #fOut, "	";
				For xx = 1 To wMeta
					' X, Y, T, A
					Print #fOut, "0x" & Lcase (Hex (xa, 2)) & ", 0x" & Lcase (Hex (ya, 2)) & ", 0x" & Lcase (Hex (tMap (tMapI), 2)) & ", 0x" & Lcase (Hex (metapals (tMapI), 2));
					Print #fOut, ", ";
					tMapI = tMapI + 1
					xa = xa + 8
				Next xx
				ya = ya + 8
				Print #fOut, ""
			Next yy
			Print #fOut, "	";
			Print #fOut, "0x" & Hex (128, 2)
			Print #fOut, "};"
			
			' Write flipped metaspriteset
			If flipped Then
				Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_b [] = {"
				ya = sprOrgY
				For yy = 1 To hMeta
					xa = sprOrgX
					Print #fOut, "	";
					For xx = 1 To wMeta
						' Flip horz order, don't forget!
						tMapI = (wMeta - xx) + (yy - 1) * wMeta
						' X, Y, T, A
						Print #fOut, "0x" & Lcase (Hex (xa, 2)) & ", 0x" & Lcase (Hex (ya, 2)) & ", 0x" & Lcase (Hex (tMap (tMapI), 2)) & ", 0x" & Lcase (Hex (64 Or metapals (tMapI), 2));
						Print #fOut, ", ";
						tMapI = tMapI + 1
						xa = xa + 8
					Next xx
					ya = ya + 8
					Print #fOut, ""
				Next yy
				Print #fOut, "	";
				Print #fOut, "0x" & Hex (128, 2)
				Print #fOut, "};"
			End If
			Print #fOut, ""
		
			tMapsIndex = tMapsIndex + 1
			If max <> -1 And tMapsIndex = max Then Exit For
		Next x
		If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then Print #fOut, ""
		If max <> -1 And tMapsIndex = max Then Exit For
	Next y
	
	Print #fOut, "// " & cPoolIndex & " patterns used from #" & offset & ", Next = " & cPoolIndex + offset & "."
	Print #fOut, ""
	
	filteredPuts ("+ " & tMapsIndex & " cells extracted using " & cPoolIndex & " patterns.")
	Close #fOut
	
	If silent Then Puts ("mkts, sprites mode. " & cPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " metasprites. Next = " & cPoolIndex + offset)
End Sub

Dim As Integer xOrg, yOrg, w, h, wMeta, hMeta, wProc, hProc, coords (8), sprxorg, spryorg
Dim As Integer i, fOut, offset
Dim As String mandatory (3) = { "in", "out", "pals", "mode" }
Dim As String mandatoryPals (3) = { "out", "pals", "mode", "label" }
Dim As Integer pal (15)
Dim As Any Ptr img
Dim As Integer max

' DO!

sclpParseAttrs

If sclpGetValue ("silent") <> "" Then
	silent = -1
Else
	silent = 0
End If

filteredPuts "mkts v0.1"
filteredPuts "Cuts & imports tilesets from a png."
filteredPuts ""

' Parse & read

If sclpGetValue ("mode") = "pals" Then
	If Not sclpCheck (mandatoryPals ()) Then
		usage
		End
	End If
Else
	If Not sclpCheck (mandatory ()) Then
		usage
		End
	End If
End If 

' Setup screen

screenres 640, 480, 32, , -1

' Set pal

filteredPuts ("Reading palette")
filteredPuts ("+ Pal file is " & sclpGetValue ("pals"))
img = png_load (sclpGetValue ("pals"))
findStraightPalette img, pal ()
showNesPal pal ()

' load input

filteredPuts ("Input file is " & sclpGetValue ("in"))
img = png_load (sclpGetValue ("in"))
If ImageInfo (img, w, h, , , , ) Then
	' Error!
End If

' Set xorg, yorg

If sclpGetValue ("offset") <> "" Then
	coordinatesStringToArray sclpGetValue ("offset"), coords ()
	xOrg = coords (0)
	yOrg = coords (1)
	filteredPuts ("+ offset specified. Will process from (" & xOrg & ", " & yOrg & 	")")
Else
	xOrg = 0
	yOrg = 0
	filteredPuts ("+ offset not specified. Will process from top-left")
End If

' Set sprxorg, spryorg -< sprorg

If sclpGetValue ("sprorg") <> "" Then
	coordinatesStringToArray sclpGetValue ("sprorg"), coords ()
	sprxorg = coords (0)
	spryorg = coords (1)
	filteredPuts ("+ sprorg specified. Will process from (" & sprxorg & ", " & spryorg & 	")")
Else
	sprxorg = 0
	spryorg = 0
	filteredPuts ("+ sprorg assumed = 0, 0")
End If
	
' Set size

wProc = 0: hProc = 0
If sclpGetValue ("size") <> "" Then
	coordinatesStringToArray sclpGetValue ("size"), coords ()
	wProc = coords (0): hProc = coords (1)
End If

If wProc = 0 Or hProc = 0 Then
	wProc = w \ 8
	hProc = h \ 8
End If
filteredPuts ("+ rectangle size = " & wProc & "x" & hProc & ". ")

' Set meta size
 
If sclpGetValue ("mode") = "tiles" Or sclpGetValue ("mode") = "mapped" Or sclpGetValue ("mode") = "sprites" Then
	coordinatesStringToArray sclpGetValue ("metasize"), coords ()
	wMeta = coords (0): hMeta = coords (1)
	If wMeta = 0 Or hMeta = 0 Then
		wMeta = 2: hMeta = 2
	End If
	filteredPuts ("+ metatiles size = " & wMeta & "x" & hMeta & ". ")
End If

If sclpGetValue ("mode") = "mapped" Or sclpGetValue ("mode") = "sprites" Then
	If sclpGetValue ("tsmap") = "" Then
		filteredPuts ("**ERROR** mode=tiles and mode=mapped need tsmap")
		filteredPuts ("")
		filteredPuts ("Run mkts.exe with no params for help.")
		filteredPuts ("")
		End
	End If
End If

If sclpGetValue ("tmapoffset") <> "" Then
	offset = Val (sclpGetValue ("tmapoffset"))
Else 
	offset = 0
End If

If sclpGetValue ("max") <> "" Then
	max = Val (sclpGetValue ("max"))
Else
	max = -1
End If

flipped = (sclpGetValue ("genflipped") <> "")

' Do

mainIdx = 0

Select Case sclpGetValue ("mode")
	Case "chars":
		doChars img, pal (), xOrg, yOrg, wProc, hProc, max
	Case "tiles":
		doTiles img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), max
	Case "mapped":
		doTmaps img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max
	Case "sprites":
		doSprites img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, sprxorg, spryorg
	Case "pals":
		doPals pal (), sclpGetValue ("out"), sclpGetValue ("pals"), sclpGetValue ("label")
End Select

If sclpGetValue ("mode") <> "pals" Then
	filteredPuts ("Writing partial CHR ROM binary to " & sclpGetValue ("out"))
	Kill sclpGetValue ("out")
	fOut = FreeFile
	Open sclpGetValue ("out") For Binary as #fOut
	For i = 0 To mainIdx - 1
		Put #fOut, , mainBin (i)
	Next i
	Close #fOut
	
	filteredPuts "+ " & mainIdx & " bytes written"
	filteredPuts ("DONE!")
End If
