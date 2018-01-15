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

Const PLATFORM_NES = 0
Const PLATFORM_SG1000 = 1

Dim Shared As Integer verbose, silent, mainIdx, cPoolIndex, tMapsIndex, flipped, clrIdx
Dim Shared As uByte mainBin (65535), clrBin (65535)
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
Dim Shared As Integer sgPal (15) = { _
	&HFF000000, &HFF202020, &HFF3FB84A, &HFF75CF7E, &HFF5955DE, &HFF8077F0, &HFFB85E51, &HFF66DBEF, _
	&HFFDA665A, &HFFFE8A7E, &HFFCCC360, &HFFDED088, &HFF3BA141, &HFFB667B5, &HFFCCCCCC, &HFFFFFFFF _
}

Sub filteredPuts (s As String)
	If Not silent Then Puts s
End Sub

Sub usage
	Print "Usage:"
	Print
	Print "$ mkts.exe in=file.png pals=pals.png out=output.bin mode=mode tsmap=tsmap.h"
	Print "           offset=x,y size=w,h metasize=w,h label=label tmapoffs=offset max=n"
	Print "           platform=nes|sg1000 genflipped silent"
	Print
	Print "in             Is the input file."
	Print "pals           Must be a 16x1 png image with the 4 pals, ordered."
	Print "out            Output binary (nes) or c array (sg1000)"
	Print "mode           chars does straight 8x8 conversion -> binary"
	Print "               tiles does straight 16x16 conversion -> binary, v-h ordered"
	Print "               mapped does metatile conversion, with ts mapping"
	Print "               sprites is for spritesets, generates metasprites w/f and tsm"
	Print "               sprites16 same, but using 8x16 sprites"
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
	Print "platform       can be nes (default if omitted) or sg1000"
	Print
	Print "For some platforms (this is getting messy), if out=bin, then two binary files"
	Print "will be output: [label]_pgt.bin and [label]_cgt.bin"
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

Sub cbWrite (v As uByte)
	clrBin (clrIdx) = v
	clrIdx = clrIdx + 1
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

Function sg1000colour (c As Integer) As uByte
	Dim As Integer i, dr, dg, db, r0, g0, b0, delta
	Dim As Integer difference = &HFF
	Dim As Integer result = 0
	
	r0 = RGBA_R (c): g0 = RGBA_G (c): b0 = RGBA_B (c)
	For i = 0 To 15
		dr = abs (r0 - RGBA_R (sgPal (i)))
		dg = abs (g0 - RGBA_G (sgPal (i)))
		db = abs (b0 - RGBA_B (sgPal (i)))
		delta = (dr + dg + db) \ 3
		If delta < difference Then difference = delta: result = i
	Next i
	
	sg1000colour = result
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
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], pals mode. Palettes from " & inputFn & " extracted to " & outputFn & ", label = " & label)
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
	If c = &HFFFF00FF Then
		findColour = 0
		Exit Function
	End If
	If c = &HFFDEDEDE Then 
		findColour = 1
		Exit Function
	End If
	For i = 0 To 15
		If pal (i) = c Then res = i: Exit For
	Next i
	findColour = res
End Function

Sub sg1000ExtractPatternFrom (img As Any Ptr, x0 As Integer, y0 As Integer, pgt () As uByte, cgt () As uByte)
	Dim As Integer x, y, c, idx, c1, c2
	Dim As uByte bitmask
	
	For y = 0 To 7
		' Find two colours
		c1 = sg1000colour (Point (x0, y0 + y, img)): c2 = 0
		
		For x = 1 To 7
			c = sg1000colour (Point (x0 + x, y0 + y, img))
			If c <> c1 Then c2 = c: Exit For
		Next x
		' Make c1 < c2 always - this decreases entropy and might result in better compression (at least in the CGT)
		If c1 > c2 Then Swap c1, c2
		cgt (y) = (c1 Shl 4) Or c2
		
		pgt (y) = 0
		For x = 0 To 7
			bitmask = (1 Shl (7 - x))
			c = sg1000colour (Point (x0 + x, y0 + y, img))
			If c = c1 Then pgt (y) = pgt (y) Or bitmask
		Next x
	Next y
End Sub

Sub nesExtractPatternFrom (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, pattern () As uByte)
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

Sub extract2PatternsFrom (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, pattern () As uByte)
	Dim As Integer x, y, c, idx
	Dim As uByte bitmask
	
	' There are two bitplanes.
	' Bitplane A => 1 if pixel is 1 or 3 (lsbits)
	' Bitplane B => 1 if pixel is 2 (msbits)

	' Bitplanes are not interlaced. First 8 rows of bitplane A, then 8 rows of bitplane B. x2
	
	' Pattern 1
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
	
	' Pattern 2
	idx = 16
	For y = 0 To 7
		pattern (idx) = 0
		pattern (idx + 8) = 0
		For x = 0 To 7
			bitmask = (1 Shl (7 - x))
			c = findColour (Point (x0 + x, y0 + y + 8, img), pal ()) And 3
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
			If c = &HFFFF00FF Or c = &HFFDEDEDE Then 
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
	
	filteredPuts ("** WARNING ** " & hSize & "x" & vSize & " tile #" & tn & " with a WRONG palette")
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

Sub copyArrayToClrBin (array () As uByte)
	Dim As Integer i
	
	For i = Lbound (array) To Ubound (array)
		cbWrite array (i)
	Next i
End Sub

Sub sg1000DoChars (img As Any Ptr, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, max As Integer)
	Dim As Integer x0, y0
	Dim As Integer x1, y1
	Dim As Integer x, y
	Dim As Integer ct
	Dim As uByte pgt (7), cgt (7)
	
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
			sg1000ExtractPatternFrom img, x, y, pgt (), cgt ()
			copyArrayToMainBin pgt ()
			copyArrayToClrBin cgt ()
			ct = ct + 1
			If max <> -1 And ct = max Then Exit For
		Next x
		If max <> -1 And ct = max Then Exit For
	Next y
	
	filteredPuts ("+ " & ct & " patterns extracted.")
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], chars mode. " & ct & " patterns extracted")
End Sub

' nesDoChars img, pal (), xOrg, yOrg, wProc, hProc
Sub nesDoChars (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, max As Integer)
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
			nesExtractPatternFrom img, x, y, pal (), pattern ()
			copyArrayToMainBin pattern ()
			ct = ct + 1
			If max <> -1 And ct = max Then Exit For
		Next x
		If max <> -1 And ct = max Then Exit For
	Next y
	
	filteredPuts ("+ " & ct & " patterns extracted.")
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], chars mode. " & ct & " patterns extracted")
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
		Print #fOut, "// Tileset pals generated by mkts [nes mode]"
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
					nesExtractPatternFrom img, xa, ya, pal (), pattern ()
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
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], tiles mode. " & ct & " patterns extracted from " & ctt & " " & wMeta & "x" & hMeta & " tiles.")
End Sub

'patternS = patternToString (pattern ())

Function patternToString (pattern () As uByte) As String
	Dim As String res
	Dim As Integer i
	
	For i = 0 To uBound (pattern)
		res = res & Hex (pattern (i), 2)
	Next i
	
	patternToString = res
End Function

Function sg1000patternToString (pgt () As uByte, cgt () As uByte) As String
	Dim As String res
	Dim As Integer i
	
	For i = 0 To uBound (pgt)
		res = res & Hex (pgt (i), 2)
	Next i
	For i = 0 To uBound (cgt)
		res = res & Hex (cgt (i), 2)
	Next i
	
	sg1000patternToString = res
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

Function find2PatternsInPoolAndAdd (patternS As String, ByRef wasNew As Integer) As Integer
	Dim As Integer res, i
	Dim As String pattern1S, pattern2S
	
	pattern1S = Left (patternS, 32)
	pattern2S = Right (patternS, 32)
	
	For i = 0 To cPoolIndex - 1 Step 2
		If cPool (i) = pattern1S And cPool (i + 1) = pattern2S Then
			find2PatternsInPoolAndAdd = i
			wasNew = 0
			Exit Function
		End If
	Next i
	
	' Add
	cPool (cPoolIndex) = pattern1S
	cPoolIndex = cPoolIndex + 1
	cPool (cPoolIndex) = pattern2S
	cPoolIndex = cPoolIndex + 1
	
	' Return
	wasNew = -1
	find2PatternsInPoolAndAdd = cPoolIndex - 2
End Function

' cPoolIndex, tMapsIndex

Sub sg1000DoTmaps (img As Any Ptr, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, fOut, xx, yy, xa, ya, wp, found, i, j
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pgt (7), cgt (7), tMap (63)
	Dim As String patternS
	Dim As Integer wasNew
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	fOut = FreeFile
	Open tsMapFn For Output As #fOut	

	' Write some headers to output tsmap file
	Print #fOut, "// Tileset tilemaps generated by mkts [sg1000 mode]"
	Print #fOut, "// Copyleft 2015 The Mojon Twins"
	Print #fOut, " "
		
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'meta' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
		
	cPoolIndex = 0: tMapsIndex = 0
	
	For y = y0 To y1 Step vSize
		For x = x0 To x1 Step hSize
						
			ya = y
			tMapI = 0
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					sg1000ExtractPatternFrom img, xa, ya, pgt (), cgt ()
					patternS = sg1000patternToString (pgt (), cgt ())
					ct = findPatternInPoolAndAdd (patternS, wasnew)
					If wasnew Then 
						copyArrayToMainBin pgt ()
						copyArrayToClrBin cgt ()
					End If
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
		If max <> -1 And tMapsIndex = max Then Exit For
	Next y
	
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
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], tilemap mode. " & cPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " tiles. Next = " & cPoolIndex + offset)
End Sub

Sub nesDoTmaps (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer)
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
	Print #fOut, "// Tileset tilemaps & pals generated by mkts [nes mode]"
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
					nesExtractPatternFrom img, xa, ya, pal (), pattern ()
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
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], tilemap mode. " & cPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " tiles. Next = " & cPoolIndex + offset)
End Sub

'sg1000DoTilemap img, xOrg, yOrg, wProc, hProc, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset
Sub sg1000DoTilemap (img As Any Ptr, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, tsMapFn As String, label As String, offset As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, fOut, xx, yy, xa, ya, wp, found, i, j
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pgt (7), cgt (7), tMap (767)
	Dim As String patternS
	Dim As Integer wasNew
	
	x0 = xOrg * 8
	y0 = yOrg * 8
	hSize = wProc * 8
	vSize = hProc * 8
	x1 = x0 + hSize - 1
	y1 = y0 + vSize - 1
	
	tMapI = 0
	For y = y0 To y1 Step 8
		For x = x0 To x1 Step 8
			sg1000ExtractPatternFrom img, x, y, pgt (), cgt ()
			patternS = sg1000patternToString (pgt (), cgt ())
			ct = findPatternInPoolAndAdd (patternS, wasnew)
			If wasnew Then 
				copyArrayToMainBin pgt ()
				copyArrayToClrBin cgt ()
			End If
			tMap (tMapI) = ct + offset
			tMapI = tMapI + 1
		Next x
	Next y
	
	' Write tilemap
	fOut = FreeFile
	Open tsMapFn For Output As #fOut	

	' Write some headers to output tsmap file
	Print #fOut, "// Tileset tilemaps & pals generated by mkts [nes mode]"
	Print #fOut, "// Copyleft 2015 The Mojon Twins"
	Print #fOut, " "
	Print #fOut, "const unsigned char " & label & "_tmap [] = {"
	tMapI = 0
	For y = 1 To hProc
		Print #fOut, "	";
		For x = 1 To wProc
			Print #fOut, "0x" & Lcase (Hex (tMap (tMapI), 2));
			tMapI = tMapI + 1
			If y < hProc Or x < wProc Then Print #fOut, ", ";
		Next x
		Print #fOut, ""
	Next y
	Print #fOut, "};"
	Print #fOut, ""
	
	Close #fOut
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], mapped mode. " & cPoolIndex & " patterns extracted. Next = " & cPoolIndex + offset)
End Sub

Sub sg1000DoSprites (img As Any Ptr, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, sprOrgX As Integer, sprOrgY As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, fOut, xx, yy, xa, ya, wp, found, i, j, xp, yp, c, xm, ym, xoff, yoff, xaa, yaa
	Dim As Integer hSize, vSize, tMapI, swapped
	Dim As uByte sgt (7), metaSprite (63)
	Dim As String patternS
	Dim As Integer wasNew
	
	Dim As Integer colours (15), nColours

	'' How to do this:
	' Each 16x16 area is scanned for N colours and generates N sprites.
	' All 4 patterns are written to a sgt in order, no repeats:
	' AB
	' CD => ACBD
	
	' There's no use in generating metasprites. They are better done by hand, as 256 patterns is a quite limited
	' number for multi-coloured sprites and the way to go is uploading new patterns to the SGT during VBlank.
		
	' Tmap would be...
	' S1P1 S1P2 ...
	' S2P1 S2P2 ...
	
	' Where SXPY is xOffs, yOffs, tOffs, Attr
	
	hSize = wMeta * 16
	vSize = hMeta * 16
	
	fOut = FreeFile
	Open tsMapFn For Output As #fOut	

	' Write some headers to output tsmap file
	Print #fOut, "// Metaspriteset generated by mkts [sg1000 mode]"
	Print #fOut, "// Copyleft 2015 The Mojon Twins"
	Print #fOut, " "
	
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'sprites' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	filteredPuts ("+ Writing metasprites info to " & tsMapFn)
	
	For y = y0 To y1 Step vSize
		For x = x0 To x1 Step hSize
			Print #fOut, "const unsigned char " & label & "_spr_" & Lcase (Hex (tMapsIndex, 2)) & " [] = {"
			ya = y
			yoff = sprOrgY
			For ym = 0 To hMeta - 1
				xa = x
				xoff = sprOrgX
				For xm = 0 To wMeta - 1
					Print #fOut, "	// Subspr (" & xm & ", " & ym & ")" & " cut from (" & xa & ", " & ya & ") => (" & (xa + 15) & "," & (ya + 15) & ")."
					' First we are going to count and store colours
					nColours = 0: For i = 0 To 15: colours (i) = 0: Next i
					For xx = 0 To 15
						For yy = 0 To 15
							c = sg1000colour (Point (xa + xx, ya + yy, img))
							wasNew = -1
							If c <> 0 Then
								For i = 0 To nColours - 1
									If c = colours (i) Then wasNew = 0: Exit For
								Next i
								If wasNew Then
									colours (nColours) = c: nColours = nColours + 1
								End If
							End If
						Next yy
					Next xx
					
					' Sort array... Not perfect, but usually works.
					Do
						swapped = 0
						For i = 1 To nColours - 1
							If colours (i - 1) > colours (i) Then
								c = colours (i)
								colours (i) = colours (i - 1)
								colours (i - 1) = c
								swapped = -1
							End If 
						Next i
					Loop Until Not swapped
					
					' Now we extract nColours sprites
					Print #fOut, "	";
					
					For i = 0 To nColours - 1
						' four patterns
						xaa = xa
						For xx = 0 To 1
							yaa = ya
							For yy = 0 To 1
								For yp = 0 To 7
									sgt (yp) = 0
									For xp = 0 To 7
										c = sg1000colour (Point (xaa + xp, yaa + yp, img))
										If c = colours (i) Then
											sgt (yp) = sgt (yp) Or (1 Shl (7 - xp))
										End If
									Next xp
								Next yp
								copyArrayToMainBin sgt ()
								cPoolIndex = cPoolIndex + 1
								yaa = yaa + 8
							Next yy
							xaa = xaa + 8
						Next xx
						Print #fOut, "0x" & Lcase (Hex (yoff, 2)) & ", 0x" & Lcase (Hex (xoff, 2)) & ", 0x" & Lcase (Hex (offset + cPoolIndex - 4,2)) & ", 0x" & Lcase (Hex (colours (i), 2));
						Print #fOut, ", ";
					Next i
					Print #fOut, ""
					xoff = xoff + 16
					xa = xa + 16
				Next xm
				yoff = yoff + 16		
				ya = ya + 16
			Next ym				
			' Write entry
			tMapsIndex = tMapsIndex + 1
			Print #fOut, "	";
			Print #fOut, "0x" & Lcase (Hex (128, 2))
			Print #fOut, "};"
		Next x
	Next y
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], sprites mode. " & cPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " metasprites. Next = " & cPoolIndex + offset)
End Sub

Sub nesDoSprites (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, sprOrgX As Integer, sprOrgY As Integer)
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
	Print #fOut, "// Metaspriteset generated by mkts [nes mode]"
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
					''ctt = (xa \ 8) + (ya \ 8) * (x1 + 1) \ 8
					ctt = (x \ hSize) + (y \ vSize) * wProc
					wp = findWhichPal (img, xa, ya, pal (), ctt, 8, 8)
					nesExtractPatternFrom img, xa, ya, pal (), pattern ()
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
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], sprites mode. " & cPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " metasprites. Next = " & cPoolIndex + offset)
End Sub

Sub nesDoSprites16 (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, sprOrgX As Integer, sprOrgY As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, fOut, xx, yy, xa, ya, wp, found, i, j
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pattern (31), tMap (63)
	Dim As String patternS
	Dim As Integer wasNew
	Dim As uByte metapals (63)
	
	hSize = wMeta * 8
	vSize = hMeta * 16
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	fOut = FreeFile
	Open tsMapFn For Output As #fOut	

	' Write some headers to output tsmap file
	Print #fOut, "// Metaspriteset generated by mkts [nes mode]"
	Print #fOut, "// Copyleft 2015 The Mojon Twins"
	Print #fOut, " "
		
	If flipped Then Print #fOut, "// Each cell: A = normal, B = flipped": Print #fOut, " "
	
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'sprites16' mode")
	filteredPuts ("Meta = " & wMeta & "x" & hMeta & ", Proc = " & wProc & "x" & hProc)
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
					''ctt = (xa \ 8) + (ya \ 8) * (x1 + 1) \ 8
					ctt = (x \ hSize) + (y \ vSize) * wProc
					wp = findWhichPal (img, xa, ya, pal (), ctt, 8, 16)
					extract2PatternsFrom img, xa, ya, pal (), pattern ()
					patternS = patternToString (pattern ())
					ct = find2PatternsInPoolAndAdd (patternS, wasnew)
					If wasnew Then copyArrayToMainBin pattern ()
					tMap (tMapI) = ct + offset + 1
					metapals (tMapI) = wp
					tMapI = tMapI + 1
					xa = xa + 8
				Next xx
				ya = ya + 16
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
				ya = ya + 16
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
					ya = ya + 16
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
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], sprites16 mode. " & cPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " metasprites. Next = " & cPoolIndex + offset)
End Sub

Dim As Integer xOrg, yOrg, w, h, wMeta, hMeta, wProc, hProc, coords (8), sprxorg, spryorg
Dim As Integer i, fOut, offset, ctr, ctrspr, j
Dim As String mandatoryNes (3) = { "in", "out", "pals", "mode" }
Dim As String mandatoryNesPals (3) = { "out", "pals", "mode", "label" }
Dim As String mandatorySg1000 (2) = { "in", "out", "mode" }
Dim As Integer pal (15)
Dim As Any Ptr img
Dim As Integer max
Dim As Integer platform = PLATFORM_NES

' DO!

sclpParseAttrs

If sclpGetValue ("platform") = "sg1000" Then platform = PLATFORM_SG1000

If sclpGetValue ("silent") <> "" Then
	silent = -1
Else
	silent = 0
End If

filteredPuts "mkts v0.2"
filteredPuts "Cuts & imports tilesets from a png."
filteredPuts ""

' Parse & read

If platform = PLATFORM_SG1000 Then
	If Not sclpCheck (mandatorySg1000 ()) Then
		usage
		End
	End If
Else
	If sclpGetValue ("mode") = "pals" Then
		If Not sclpCheck (mandatoryNesPals ()) Then
			usage
			End
		End If
	Else
		If Not sclpCheck (mandatoryNes ()) Then
			usage
			End
		End If
	End If 
End If

' Setup screen

screenres 640, 480, 32, , -1

' Set pal

If platform <> PLATFORM_SG1000 Then
	filteredPuts ("Reading palette")
	filteredPuts ("+ Pal file is " & sclpGetValue ("pals"))
	img = png_load (sclpGetValue ("pals"))
	findStraightPalette img, pal ()
	showNesPal pal ()
End If

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
	filteredPuts ("+ sprorg specified. Will start from (" & sprxorg & ", " & spryorg & 	")")
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
 
If sclpGetValue ("mode") = "tiles" Or sclpGetValue ("mode") = "mapped" Or sclpGetValue ("mode") = "sprites" Or sclpGetValue ("mode") = "sprites16" Then
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

If platform = PLATFORM_NES Then
	Select Case sclpGetValue ("mode")
		Case "chars":
			nesDoChars img, pal (), xOrg, yOrg, wProc, hProc, max
		Case "tiles":
			doTiles img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), max
		Case "mapped":
			nesDoTmaps img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max
		Case "sprites":
			nesDoSprites img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, sprxorg, spryorg
		Case "sprites16":
			nesDoSprites16 img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, sprxorg, spryorg
		Case "pals":
			doPals pal (), sclpGetValue ("out"), sclpGetValue ("pals"), sclpGetValue ("label")
		Case else:
			Puts ("mode specified not available for this platform")
			End
	End Select
ElseIf platform = PLATFORM_SG1000 Then
	Select Case sclpGetValue ("mode")
		Case "chars":
			sg1000DoChars img, xOrg, yOrg, wProc, hProc, max
		Case "mapped":
			sg1000DoTmaps img, xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max
		Case "sprites":
			sg1000DoSprites img, xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, sprxorg, spryorg
		Case "tilemap":
			sg1000DoTilemap img, xOrg, yOrg, wProc, hProc, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset
		Case else:
			Puts ("mode specified not available for this platform")
			End
	End Select
End If

If platform = PLATFORM_SG1000 Then
	' Write binary or code depending on filename extension
	If sclpGetValue ("out") <> "bin" Then 
		filteredPuts ("Writing PGT and CGT to " & sclpGetValue ("out"))
		fOut = FreeFile
		Open sclpGetValue ("out") For Output as #fOut
		Print #fOut, "// Charset generated by mkts [sg1000 mode]"
		Print #fOut, "// Copyleft 2015 The Mojon Twins"
		Print #fOut, " "
		Print #fOut, "#define " & Ucase (sclpGetValue ("label")) & "_NUM_PATTERNS " & (mainIdx \ 8)
		Print #fOut, " "
		If sclpGetValue ("mode") <> "sprites" Then
			Print #fOut, "const unsigned char " & sclpGetValue ("label") & "_pgt [] = {"
		Else
			Print #fOut, "const unsigned char " & sclpGetValue ("label") & "_sgt [] = {"
		End If
		ctr = 0
		j = 0
		ctrSpr = 0
		For i = 0 To mainIdx - 1
			If ctr = 0 Then 
				If sclpGetValue ("mode") = "sprites" And j = 0 Then Print #fOut, "	// Sprite face # " & ctrSpr
				Print #fOut, "	";
			End If
			Print #fOut, "0x" & Lcase (Hex (mainBin (i), 2));
			If i < mainIdx - 1 Then Print #fOut, ", ";
			ctr = ctr + 1: If ctr = 8 Then Print #fOut, "": ctr = 0: If sclpGetValue ("mode") = "sprites" Then j = j + 1: If j = 4 Then j = 0: Print #fOut, "": ctrSpr = ctrSpr + 1
		Next i
		Print #fOut, "};"
		If sclpGetValue ("mode") <> "sprites" Then
			Print #fOut, " "
			Print #fOut, "const unsigned char " & sclpGetValue ("label") & "_cgt [] = {"
			For i = 0 To clrIdx - 1
				If ctr = 0 Then Print #fOut, "	";
				Print #fOut, "0x" & Lcase (Hex (clrBin (i), 2));
				If i < mainIdx - 1 Then Print #fOut, ", ";
				ctr = ctr + 1: If ctr = 8 Then Print #fOut, "": ctr = 0
			Next i
			Print #fOut, "};"
		End If
		Close #fOut
	Else
		fOut = FreeFile
		If sclpGetValue ("mode") <> "sprites" Then
			Kill sclpGetValue ("label") & "_pgt.bin"
			Open sclpGetValue ("label") & "_pgt.bin" For Binary As #fOut
			filteredPuts ("Writing PGT to " & sclpGetValue ("label") & "_pgt.bin and CGT to " & sclpGetValue ("label") & "_cgt.bin")
		Else
			Kill sclpGetValue ("label") & "_sgt.bin"
			Open sclpGetValue ("label") & "_sgt.bin" For Binary As #fOut
			filteredPuts ("Writing PGT to " & sclpGetValue ("label") & "_sgt.bin")
		End If
		' Just write binary
		For i = 0 To mainIdx - 1
			Put #fOut, , mainBin (i)
		Next i
		Close #fOut
		If sclpGetValue ("mode") <> "sprites" Then
			Kill sclpGetValue ("label") & "_cgt.bin"
			fOut = FreeFile
			Open sclpGetValue ("label") & "_cgt.bin" For Binary As #fOut
			For i = 0 To clrIdx - 1
				Put #fOut, , clrBin (i)
			Next i
			Close #fOut
		End If
	End If		
	filteredPuts "+ " & (mainIdx * 2) & " bytes written"
	filteredPuts ("DONE!")
Else
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
End If
