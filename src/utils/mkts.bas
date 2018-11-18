' mkts v0.8.0 20180205 - Converts several kinds of tilesets
' Copyleft 2015 - 2018 The Mojon Twins

' fbc mkts.bas cmdlineparser.bas mtparser.bas pnginterface.bas

#include "file.bi"
#include "fbgfx.bi"
#include once "crt.bi"

#include "cmdlineparser.bi"
#include "mtparser.bi"
#include "pnginterface.bi"

#define RGBA_R( c ) ( CUInt( c ) Shr 16 And 255 )
#define RGBA_G( c ) ( CUInt( c ) Shr  8 And 255 )
#define RGBA_B( c ) ( CUInt( c )        And 255 )
#define RGBA_A( c ) ( CUInt( c ) Shr 24         )

Const PLATFORM_NES 		= 0
Const PLATFORM_SG1000 	= 1
Const PLATFORM_GB 		= 2
Const PLATFORM_SMS 		= 3


Dim Shared As Integer verbose, silent, mainIdx, cPoolIndex, tMapsIndex, flipped, upsideDown, clrIdx, outputPalList, patternsLimit, outputPatterns
Dim Shared As Integer blackiszero, columns, nextPattern, mirrored, simplemeta, supersimplemeta, aseprite, noskipempty, allbg, deinterlaced, debug
Dim Shared As Integer metalogicalwidth
Dim Shared As Integer succesfullyAddedPatternToPool, ignoreMapFile
Dim Shared As Integer metaSpriteCounter
Dim Shared As uByte mainBin (65535), clrBin (65535)
Dim Shared As String cPool (255)
Dim Shared As uByte tMaps (255, 255)
Dim Shared As uShort tMapsS (255, 255)
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
Dim Shared As Integer nesPalAseprite (63) = { _
	&HFF797979, &HFF2000B2, &HFF2800BA, &HFF6110A2, &HFF9A2079, &HFFB21030, &HFFA23000, &HFF794100,  _
	&HFF495900, &HFF386900, &HFF386D00, &HFF306141, &HFF305182, &HFF000000, &HFF000000, &HFF000000,  _
	&HFFB2B2B2, &HFF4161FB, &HFF4141FF, &HFF9241F3, &HFFDB41C3, &HFFDB4161, &HFFE35100, &HFFC37100,  _
	&HFF8A8A00, &HFF51A200, &HFF49AA10, &HFF49A269, &HFF4192C3, &HFF000000, &HFF000000, &HFF000000,  _
	&HFFEBEBEB, &HFF61A2FF, &HFF5182FF, &HFFA271FF, &HFFF361FF, &HFFFF61B2, &HFFFF7930, &HFFFFA200,  _
	&HFFEBD320, &HFF9AEB00, &HFF71F341, &HFF71E392, &HFF61D3E3, &HFF757575, &HFF000000, &HFF000000,  _
	&HFFFFFFFF, &HFF92D3FF, &HFFA2BAFF, &HFFC3B2FF, &HFFE3B2FF, &HFFFFBAEB, &HFFFFCBBA, &HFFFFDBA2,  _
	&HFFFFF392, &HFFCBF382, &HFFA2F3A2, &HFFA2FFCB, &HFFA2FFF3, &HFFA2A2A2, &HFF000000, &HFF000000_ 
}

Dim Shared As Integer sgPal (15) = { _
	&HFF000000, &HFF202020, &HFF3FB84A, &HFF75CF7E, &HFF5955DE, &HFF8077F0, &HFFB85E51, &HFF66DBEF, _
	&HFFDA665A, &HFFFE8A7E, &HFFCCC360, &HFFDED088, &HFF3BA141, &HFFB667B5, &HFFCCCCCC, &HFFFFFFFF _
}
Dim Shared As Integer gbPal (15) = { _
	&HFF9BBC0F, &HFF8BAC0F, &HFF306230, &HFF0F380F, _ 	' Aseprite
	&HFFDFDFDF, &HFFA0A0A0, &HFF676767, &HFF000000, _ 	' PCX2GB
	&HFFFFFFFF, &HFFC0C0C0, &HFF808080, &HFF000000, _ 	' Gameboy Tile Designer
	&HFFFFFFFF, &HFFCCCCCC, &HFF555555, &HFF000000 _ 	' Equalized
}
Dim Shared As Integer nextWillBe

Sub filteredPuts (s As String)
	If Not silent Then Puts s
End Sub

Sub usage
	Puts "Usage:"
	Puts ""
	Puts "$ mkts.exe in=file.png pals=pals.png out=output.bin mode=mode tsmap=tsmap.h"
	Puts "           offset=x,y size=w,h metasize=w,h label=label tmapoffs=offset max=n"
	Puts "           platform=nes|sg1000 [genflipped] [blackiszero] [upsidedown]"
	Puts "           [deinterlaced] [allbg] [aseprite] [simple] [silent] [ignoremapfile]"
	Puts ""
	Puts "platform       can be nes (default if omitted), gb, sms or sg1000"
	Puts "in             Is the input file."
	Puts "pals           NES: Must be a 16x1 png image with the 4 pals, ordered."
	Puts "               SMS: Must be a 16x2 png with the full palette."
	Puts "               GB: Must be 4 0-3 digits with the GB pal (0 lightest 3 darkest)"
	Puts "out            Output binary (nes, gb, sms) or c array (sg1000)"
	Puts "mode           chars does straight 8x8 conversion -> binary"
	Puts "               text is special chars with text output in .h. Ask na_th_an"
	Puts "               tiles does straight 16x16 conversion -> binary, v-h ordered"
	Puts "               coltiles is same as tiles, but h-v ordered"
	Puts "               mapped does metatile conversion, with ts mapping"
	Puts "               sprites is for spritesets, generates metasprites w/f and tsm"
	Puts "               sprites16 same, but using 8x16 sprites"
	Puts "               pals calculates the palette values, outputs code to out"
	Puts "               scripted ignores all parameters but 'in'/'out', executes in"
	Puts "tsmap          Will output palette # in tiles mode, full tsmap in mapped mode"
	Puts "offset         (x, y) to start from (in chars)"
	Puts "size           (w, h) size of rectangle to process (in chars, tiles or mtiles)"
	Puts "metasize       (w, h) size of metatiles (in chars), needed for tiles and mapped"
	Puts "sprorg         (x, y) in pixels for topmost sprite in metasprites."
	Puts "label          Used in the generated code."
	Puts "tmapoffset     Offset chars in tmaps by this amount. Can be omitted."
	Puts "max            Process only #max items in the area. Can be omitted."
	Puts "genflipped     For sprites mode, generates flipped metas for each meta."
	Puts "blackiszero    Every blank (all 0s) tile will be encoded as tile 0"
	Puts "columns        hack for mode=mapped, col ordered. NES only for now."
	Puts "simple         Simple metasprites for sms."
	Puts "aseprite       Use aseprite palette (NES mode)"
	Puts "allbg          All sprites with bg bit on (NES mode)"
	Puts "deinterlaced   Generate a deinterlaced tilemap"
	Puts ""
	Puts "For some platforms (this is getting messy), if out=bin, then two binary files"
	Puts "will be output: [label]_pgt.bin and [label]_cgt.bin"
	Puts ""
	Puts "If offset is omitted, cut will start at 0, 0. "
	Puts "If size is omitted, will process the whole image (nearest multiple of 8)"
	Puts "If metasize is omitted, will assume 2x2."
	Puts ""
	Puts "Palette output uses label for palette name in generated code."
	Puts "" 
	Puts "All GB conversions need a label"
End Sub

Sub mbWrite (v As uByte)
	mainBin (mainIdx) = v
	mainIdx = mainIdx + 1
End Sub

Sub cbWrite (v As uByte)
	clrBin (clrIdx) = v
	clrIdx = clrIdx + 1
End Sub

Function smsColour (c As Integer) As uByte
	smsColour = ( RGBA_R (CUInt( c )) \ 85) Or ((RGBA_G (CUInt( c )) \ 85) Shl 2) Or ((RGBA_B (CUInt( c )) \ 85) Shl 4 )
End Function 

Function nesColour (c As Integer) As uByte
	Dim As uByte i
	
	If c = 0 Then
		nesColour = &H0F
	End If
	
	For i = 0 To 63
		If (aseprite And nesPalAseprite (i) = c) Or (Not aseprite And nesPal (i) = c) Then 
			If i = &H0D Then i = &H0F	' Safe
			nesColour = i
			Exit Function
		End If
	Next i
	nesColour = &HFF
End Function

Function gbColour (c As Integer) As uByte
	Dim As uByte i

	For i = 0 To 15
		If gbPal (i) = c Then
			gbColour = i And 3
			Exit Function
		End If
	Next i

	gbColour = 0
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

Sub nesDoPals (pal () As Integer, outputFn As String, inputFn As String, label As String)
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

Sub smsDoPals (pal () As Integer, outputFn As String, inputFn As String, label As String)
	Dim As Integer i, f
	
	f = FreeFile
	Open outputFn For Output as #f
	Print #f, "// Palette data from " & inputFn
	Print #f, "const unsigned char " & label & " [] = {"
	
	For i = 0 To 31
		If (i And 7) = 0 Then Print #f, "	";
		Print #f, "0x" & Hex (smsColour (pal (i)), 2);
		If i < 31 Then Print #f, ", ";
		If (i And 7) = 7 Then Print #f, ""
	Next i
	
	Print #f, "};"
	Print #f, ""
	
	Close #f
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], pals mode. Palettes from " & inputFn & " extracted to " & outputFn & ", label = " & label)
End Sub

Sub nesFindPalette (img As Any Ptr, pal () As Integer) 
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

Sub nesFindStraightPalette (img As Any Ptr, pal () As Integer) 
	Dim As Integer x
	
	For x = 0 To 15
		pal (x) = Point (x, 0, img)
	Next x
End Sub

Sub smsFindStraightPalette (img As Any Ptr, pal () As Integer)
	Dim As Integer x, y

	For y = 0 To 1
		For x = 0 To 15
			pal (x + 16 * y) = Point (x, y, img)
		Next x
	Next y
End Sub

Function findColour (c As Integer, pal () As Integer) As Integer
	Dim As Integer res
	Dim As Integer i
	res = 0
	If c = &HFFFF00FF Then
		findColour = 0
		Exit Function
	End If
	If c = &HFF171717 Then 
		findColour = 1
		Exit Function
	End If
	For i = 0 To uBound (pal)
		If pal (i) = c Then res = i: Exit For
	Next i
	findColour = res
End Function

Function nesFindColourWithPal (c As Integer, pal () As Integer, wp As Integer) As Integer
	Dim As Integer res
	Dim As Integer i
	res = 0
	If c = &HFFFF00FF Then
		nesFindColourWithPal = 0
		Exit Function
	End If
	If c = &HFF171717 Then 
		nesFindColourWithPal = 1
		Exit Function
	End If
	For i = wp*4 To wp*4+3
		If pal (i) = c Then res = i: Exit For
	Next i
	nesFindColourWithPal = res-wp*4
End Function

Function smsFindColourWithPal (c As Integer, pal () As Integer, wp As Integer) As Integer
	Dim As Integer res
	Dim As Integer i
	res = 0
	If c = &HFF171717 Then 
		smsFindColourWithPal = 0
		Exit Function
	End If
	For i = wp*16 To wp*16+15
		If pal (i) = c Then res = i: Exit For
	Next i
	smsFindColourWithPal = res
End Function

Function smsColourExistsInPal (c As Integer, pal () As Integer, wp As Integer) As Integer
	Dim As Integer res
	Dim As Integer i
	res = 0
	For i = wp*16 To wp*16+15
		If pal (i) = c Then res = -1: Exit For
	Next i
	smsColourExistsInPal = res
End Function

Function gbFindColour (c As Integer, pal () As Integer) As Integer
	' c = ARGB pixel value
	' pal contains the order of GB colours 0-3 (0 = white)
	Dim As Integer res
	Dim As Integer i
	Dim As Integer gbC

	gbC = gbColour (c)

	For i = 0 To 3
		If pal (i) = gbC Then res = i: Exit For
	Next i

	gbFindColour = res
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

Function patternIsAllZeroes (pattern () as uByte) As Integer
	Dim As Integer res, i

	res = -1
	For i = 0 To uBound (pattern)
		If pattern (i) <> 0 Then res = 0: Exit For
	Next i

	patternIsAllZeroes = res
End Function

Sub smsExtractPatternFromWithPal (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, wp As Integer, pattern () As uByte)
	Dim As Integer x, y, c, idx, bitplane
	Dim As uByte bitmask
	
	' There are four bitplanes.
	' Bitplanes are interlaced, 0 to 3
	
	idx = 0
	For y = 0 To 7
		For bitplane = 0 To 3
			pattern (idx) = 0
			For x = 0 To 7
				bitmask = (1 Shl (7 - x))
				c = smsFindColourWithPal (Point (x0 + x, y0 + y, img), pal (), wp)
				If (c And (1 Shl bitplane)) Then
					pattern (idx) = pattern (idx) Or bitmask
				End If
			Next x
			idx = idx + 1
		Next bitplane
	Next y
End Sub

Sub smsExtractHFlippedPatternFromWithPal (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, wp As Integer, pattern () As uByte)
	Dim As Integer x, y, c, idx, bitplane
	Dim As uByte bitmask
	
	' There are four bitplanes.
	' Bitplanes are interlaced, 0 to 3
	
	idx = 0
	For y = 0 To 7
		For bitplane = 0 To 3
			pattern (idx) = 0
			For x = 0 To 7
				bitmask = (1 Shl (7 - x))
				c = smsFindColourWithPal (Point (x0 + 7 - x, y0 + y, img), pal (), wp)
				If (c And (1 Shl bitplane)) Then
					pattern (idx) = pattern (idx) Or bitmask
				End If
			Next x
			idx = idx + 1
		Next bitplane
	Next y
End Sub

Sub smsExtractVFlippedPatternFromWithPal (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, wp As Integer, pattern () As uByte)
	Dim As Integer x, y, c, idx, bitplane
	Dim As uByte bitmask
	
	' There are four bitplanes.
	' Bitplanes are interlaced, 0 to 3
	
	idx = 0
	For y = 7 To 0 Step -1
		For bitplane = 0 To 3
			pattern (idx) = 0
			For x = 7 To 0 Step -1
				bitmask = (1 Shl (7 - x))
				c = smsFindColourWithPal (Point (x0 + x, y0 + y, img), pal (), wp)
				If (c And (1 Shl bitplane)) Then
					pattern (idx) = pattern (idx) Or bitmask
				End If
			Next x
			idx = idx + 1
		Next bitplane
	Next y
End Sub

Sub gbExtractPatternFrom (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, pattern () As uByte)
	Dim As Integer x, y, c, idx
	Dim As uByte bitmask
	
	' There are two bitplanes.
	' Bitplane A => 1 if pixel is 1 or 3 (lsbits)
	' Bitplane B => 1 if pixel is 2 (msbits)

	' Bitplanes are interlaced. First LSB (bitplane A), then MSB (bitplane B)
	
	idx = 0
	For y = 0 To 7
		pattern (idx) = 0
		pattern (idx + 1) = 0
		For x = 0 To 7
			bitmask = (1 Shl (7 - x))
			c = gbFindColour (Point (x0 + x, y0 + y, img), pal ()) And 3
			If (c And 1) Then
				pattern (idx) = pattern (idx) Or bitmask
			End If
			If (c And 2) Then
				pattern (idx + 1) = pattern (idx + 1) Or bitmask
			End If
		Next x
		idx = idx + 2
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

Sub nesExtractPatternFromWithPal (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, wp As Integer, pattern () As uByte)
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
			c = nesFindColourWithPal (Point (x0 + x, y0 + y, img), pal (), wp) And 3
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

Sub smsExtract2PatternsFromWithPal (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, wp As Integer, pattern () As uByte)
	Dim As Integer x, y, c, idx, bitplane
	Dim As uByte bitmask
	
	' There are four bitplanes.
	' Bitplanes are interlaced, 0 to 3
	
	idx = 0
	For y = 0 To 15
		For bitplane = 0 To 3
			pattern (idx) = 0
			For x = 0 To 7
				bitmask = (1 Shl (7 - x))
				c = smsFindColourWithPal (Point (x0 + x, y0 + y, img), pal (), wp)
				If (c And (1 Shl bitplane)) Then
					pattern (idx) = pattern (idx) Or bitmask
				End If
			Next x
			idx = idx + 1
		Next bitplane
	Next y
End Sub

Sub gbExtract2PatternsFrom (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, pattern () As uByte)
	Dim As Integer x, y, c, idx
	Dim As uByte bitmask
	
	' There are two bitplanes.
	' Bitplane A => 1 if pixel is 1 or 3 (lsbits)
	' Bitplane B => 1 if pixel is 2 (msbits)

	' Bitplanes are interlaced. First LSB (bitplane A), then MSB (bitplane B)
	
	' Pattern 1
	idx = 0
	For y = 0 To 7
		pattern (idx) = 0
		pattern (idx + 1) = 0
		For x = 0 To 7
			bitmask = (1 Shl (7 - x))
			c = findColour (Point (x0 + x, y0 + y, img), pal ()) And 3
			If (c And 1) Then
				pattern (idx) = pattern (idx) Or bitmask
			End If
			If (c And 2) Then
				pattern (idx + 1) = pattern (idx + 1) Or bitmask
			End If
		Next x
		idx = idx + 2
	Next y
	
	' Pattern 2
	idx = 16
	For y = 0 To 7
		pattern (idx) = 0
		pattern (idx + 1) = 0
		For x = 0 To 7
			bitmask = (1 Shl (7 - x))
			c = findColour (Point (x0 + x, y0 + y + 8, img), pal ()) And 3
			If (c And 1) Then
				pattern (idx) = pattern (idx) Or bitmask
			End If
			If (c And 2) Then
				pattern (idx + 1) = pattern (idx + 1) Or bitmask
			End If
		Next x
		idx = idx + 2
	Next y
End Sub

Sub nesExtract2PatternsFrom (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, pattern () As uByte)
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

Function smsFindWhichPal (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, Byval tn As Integer, hSize As Integer, vSize As Integer) As Integer
	' Inspects area. Finds how many colours from pal0 and pal1 there are, then picks the one with the most colours
	' TODO: add support for foreground! (Using #FFFF00FF as background, for example)

	Dim As Integer c1PalCols, c2PalCols
	Dim As Integer x, y, c, i, j, cp
	Dim As Integer ticks (1, 15)

	For i = 0 To 1: For j = 0 To 15: ticks (i, j) = 0: Next j, i

	' How many different colours from pal 0 are there in the image?
	c1PalCols = 0: c2PalCols = 0
	For y = 0 To vSize - 1
		For x = 0 To hSize - 1
			c = Point (x0 + x, y0 + y, img): If c = &HFF171717 Then c = pal (0) 
			If smsColourExistsInPal (c, pal (), 0) Then
				cp = smsFindColourWithPal (c, pal (), 0) And 15
				If ticks (0, cp) = 0 Then c1PalCols = c1PalCols + 1
				ticks (0, cp) = -1
			End If
			If smsColourExistsInPal (c, pal (), 1) Then
				cp = smsFindColourWithPal (c, pal (), 1) And 15
				If ticks (1, cp) = 0 Then c2PalCols = c2PalCols + 1
				ticks (1, cp) = -1
			End If
		Next x
	Next y

	If c1PalCols >= c2PalCols Then smsFindWhichPal = 0 Else smsFindWhichPal = 1
End Function

'' TODO: Recode this using a different idea and make it work :-~
'' The above (SMS) approach seems more suited. Maybe.
Function nesFindWhichPal (img As Any Ptr, x0 As Integer, y0 As Integer, pal () As Integer, tn As Integer, hSize As Integer, vSize As Integer) As Integer
	Dim As Integer x, y, c, i, j, pi, found, different
	Dim As Integer whichCols (3)
	Dim As Integer idx, said
	Dim As Integer addTrans

	Dim As String coloursDetected
	Dim As String coloursInPal
	
	coloursDetected = "   "
	coloursInPal = ""

	For i = 0 To 3: whichCols (i) = &HFF: Next i
	
	idx = 0
	said = 0

	If allbg Then addTrans = &H20 Else addTrans = 0

	For y = 0 To vSize - 1
		For x = 0 To hSize - 1
			c = Point (x0 + x, y0 + y, img)
			If c = &HFFFF00FF Or c = &HFF171717 Then 
				addTrans = &H20
				Continue For
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


	For i = 0 To 3
		coloursDetected = coloursDetected & Hex (whichCols (i), 8) & " "
		coloursInPal = coloursInPal & "P" & i & " "
		For j = 0 To 3
			coloursInPal = coloursInPal & Hex (pal(i*4+j), 8) & " "
		Next j
	Next i

	
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
			nesFindWhichPal = addTrans + pi
			'Puts ("   -> Palette is " & addTrans + pi)
			Exit Function
		End If
	Next pi
	
	filteredPuts ("** WARNING ** " & hSize & "x" & vSize & " tile #" & tn & " with a WRONG palette")
	filteredPuts (coloursInPal)
	filteredPuts (coloursDetected)
	nesFindWhichPal = addTrans
End Function

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

Function isAllZeroes (patternS As String) As Integer
	Dim As Integer res, i
	res = -1
	For i = 1 To Len (patternS)
		If Mid (patternS, i, 1) <> "0" Then
			res = 0
			Exit For
		End If
	Next i
	isAllZeroes = res
End Function

Sub addPatternToPool (patternS As String)
	succesfullyAddedPatternToPool = 0
	If patternsLimit > 0 And outputPatterns >= patternsLimit Then  Exit Sub

	' Add
	cPool (cPoolIndex) = patternS
	succesfullyAddedPatternToPool = -1
	
	' Next
	cPoolIndex = cPoolIndex + 1

	outputPatterns = outputPatterns + 1
End Sub

Function findPatternInPoolAndAdd (patternS As String, ByRef wasNew As Integer) As Integer
	Dim As Integer res, i
	
	if blackiszero And isAllZeroes (patternS) Then
		findPatternInPoolAndAdd = 9999
		Exit Function
	End If
	
	For i = 0 To cPoolIndex - 1
		If cPool (i) = patternS Then
			findPatternInPoolAndAdd = i
			wasNew = 0
			Exit Function
		End If
	Next i
	
	' Add
	'cPool (cPoolIndex) = patternS
	' Next
	'cPoolIndex = cPoolIndex + 1
	addPatternToPool patternS
	
	' Return
	wasNew = -1
	findPatternInPoolAndAdd = cPoolIndex - 1
End Function

Sub copyArrayToMainBin (array () As uByte)
	If Not succesfullyAddedPatternToPool Then Exit Sub
	Dim As Integer i	
	For i = Lbound (array) To Ubound (array)
		mbWrite array (i)
	Next i
End Sub

Sub copyArrayToClrBin (array () As uByte)
	If Not succesfullyAddedPatternToPool Then Exit Sub
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
			addPatternToPool sg1000patternToString (pgt (), cgt ())
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
	Dim As String patternS
	
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
			patternS = patternToString (pattern ())
			addPatternToPool patternS
			copyArrayToMainBin pattern ()
			ct = ct + 1
			If max <> -1 And ct = max Then Exit For
		Next x
		If max <> -1 And ct = max Then Exit For
	Next y
	
	filteredPuts ("+ " & ct & " patterns extracted.")
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], chars mode. " & ct & " patterns extracted")
End Sub

Sub gbDoChars (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, max As Integer)
	Dim As Integer x0, y0
	Dim As Integer x1, y1
	Dim As Integer x, y
	Dim As Integer ct
	Dim As uByte pattern (15)
	Dim As String patternS
	
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
			gbExtractPatternFrom img, x, y, pal (), pattern ()
			patternS = patternToString (pattern ())
			addPatternToPool patternS
			copyArrayToMainBin pattern ()
			ct = ct + 1
			If max <> -1 And ct = max Then Exit For
		Next x
		If max <> -1 And ct = max Then Exit For
	Next y
	
	filteredPuts ("+ " & ct & " patterns extracted.")
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], chars mode. " & ct & " patterns extracted")
End Sub

Sub smsDoChars (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, max As Integer)
	Dim As Integer x0, y0
	Dim As Integer x1, y1
	Dim As Integer x, y
	Dim As Integer ct, wp
	Dim As uByte pattern (31)
	Dim As String patternS
	
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
			wp = smsFindWhichPal (img, x, y, pal (), (ct), 8, 8)
			smsExtractPatternFromWithPal img, x, y, pal (), wp, pattern ()
			patternS = patternToString (pattern ())
			addPatternToPool patternS
			copyArrayToMainBin pattern ()
			ct = ct + 1
			If max <> -1 And ct = max Then Exit For
		Next x
		If max <> -1 And ct = max Then Exit For
	Next y
	
	filteredPuts ("+ " & ct & " patterns extracted.")
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], chars mode. " & ct & " patterns extracted")
End Sub

' nesDoTexts img, pal (), xOrg, yOrg, wProc, hProc, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max
Sub nesDoTexts (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, fOut As Integer)
	Dim As Integer x0, y0
	Dim As Integer x1, y1
	Dim As Integer x, y
	Dim As Integer ct
	Dim As Integer strCtr, lineDone, wasnew, first, lctn, i, latest
	Dim As uByte pattern (15)
	Dim As String patternS
	Dim As Integer shouldClose

	shouldClose = 0
	
	' I know I can calculate ct from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * 8 - 1
	y1 = y0 + hProc * 8 - 1
	ct = 0
	
	filteredPuts ("Processing image in 'text' mode")
	
	If fOut = -1 And Not ignoreMapFile Then
		shouldClose = -1
		fOut = FreeFile
		Open tsMapFn For Output As #fOut
		Print #fOut, "// Strings of patterns generated by mkts [nes mode]"
		Print #fOut, "// Copyleft 2017 The Mojon Twins"
		Print #fOut, " "
	End If

	If Not ignoreMapFile Then
		Print #fOut, "#define STRINGS_OFFSET_" & uCase (label) & " 0x" & Lcase (Hex (offset, 2))
		Print #fOut, " "
		Print #fOut, "// Strings"
		Print #fOut, " "
	End If
	
	x = x0: y = y0: strCtr = 0: latest = 0
	While y <= y1
		If max <> -1 And ct = max Then Exit While
		' Process this line?
		If Point (x, y, img) = &HFF00FF00 Then
			If Not ignoreMapFile Then Print #fOut, "// Skipped line { ";
			x = x + 8
		Else
			If Not ignoreMapFile Then Print #fOut, "const unsigned char str_" & label & "_" & Lcase (Hex (strCtr, 2)) & " [] = { ";
			strCtr = strCtr + 1
		End If
		
		' Get a string.
		lineDone = 0
		' puts "X:" & x & ", Y: " & y
		While Not (x > x1 Or Point (x, y, img) = &HFFFF0000)
			If max <> -1 And ct = max Then Exit While
			nesExtractPatternFrom img, x, y, pal (), pattern ()
			patternS = patternToString (pattern ())
			ct = findPatternInPoolAndAdd (patternS, wasnew)
			if ct > latest Then latest = ct
			' puts " ++ " & ct & " -> " & patternS
			If wasnew Then copyArrayToMainBin pattern ()
			If Not ignoreMapFile Then Print #fOut, "0x" & Lcase (Hex (ct + offset, 2)) & ", ";
			x = x + 8
		Wend
		If Not ignoreMapFile Then Print #fOut, "0xff };"
		x = x0: y = y + 8
	Wend
	
	If Not ignoreMapFile Then
		Print #fOut, " "
		Print #fOut, "// Strings index"
		Print #fOut, " "
		
		Print #fOut, "const unsigned char *strs_" & label & " [] = {"

		lctn = 0
		Print #fOut, "	";
		For i = 0 To strCtr - 1
			Print #fOut, "str_" & label & "_" & Lcase (Hex (i, 2));
			If i < strCtr - 1 Then Print #fOut, ", ";: lctn = lctn + 1: If lctn = 8 Then lctn = 0: Print #fOut, "": Print #fOut, "	";
		Next i
		Print #fOut, ""
		Print #fOut, "};"
		Print #fOut, " "
		Print #fOut, "// " & latest & " patterns extracted. Next = " & (latest + offset)
		Print #fOut, " "
	End If		
	
	If shouldClose Then Close #fOut

	filteredPuts ("+ " & latest & " patterns extracted.")
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], text mode. " & latest & " patterns extracted. Next = " & (latest + offset))
	nextPattern = latest + offset
End Sub

Sub gbDoTexts (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, fOut As Integer)
	Dim As Integer x0, y0
	Dim As Integer x1, y1
	Dim As Integer x, y
	Dim As Integer ct
	Dim As Integer strCtr, lineDone, wasnew, first, lctn, i, latest
	Dim As uByte pattern (15)
	Dim As String patternS
	Dim As Integer shouldClose

	shouldClose = 0
	
	' I know I can calculate ct from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * 8 - 1
	y1 = y0 + hProc * 8 - 1
	ct = 0
	
	filteredPuts ("Processing image in 'text' mode")
	
	If fOut = -1 And Not ignoreMapFile Then
		shouldClose = -1
		fOut = FreeFile
		Open tsMapFn For Output As #fOut

		Print #fOut, "// Strings of patterns generated by mkts [gb mode]"
		Print #fOut, "// Copyleft 2017 The Mojon Twins"
		Print #fOut, " "
	End If
	
	If Not ignoreMapFile Then
		Print #fOut, "#define STRINGS_OFFSET_" & uCase (label) & " 0x" & Lcase (Hex (offset, 2))
		Print #fOut, " "
		Print #fOut, "// Strings"
		Print #fOut, " "
	End If
	
	x = x0: y = y0: strCtr = 0: latest = 0
	While y <= y1
		If max <> -1 And ct = max Then Exit While
		' Process this line?
		If Point (x, y, img) = &HFF00FF00 Then
			If Not ignoreMapFile Then Print #fOut, "// Skipped line { ";
			x = x + 8
		Else
			If Not ignoreMapFile Then Print #fOut, "const unsigned char str_" & label & "_" & Lcase (Hex (strCtr, 2)) & " [] = { ";
			strCtr = strCtr + 1
		End If
		
		' Get a string.
		lineDone = 0
		' puts "X:" & x & ", Y: " & y
		While Not (x > x1 Or Point (x, y, img) = &HFFFF0000)
			If max <> -1 And ct = max Then Exit While
			gbExtractPatternFrom img, x, y, pal (), pattern ()
			patternS = patternToString (pattern ())
			ct = findPatternInPoolAndAdd (patternS, wasnew)
			if ct > latest Then latest = ct
			' puts " ++ " & ct & " -> " & patternS
			If wasnew Then copyArrayToMainBin pattern ()
			Print #fOut, "0x" & Lcase (Hex (ct + offset, 2)) & ", ";
			x = x + 8
		Wend
		If Not ignoreMapFile Then Print #fOut, "0xff };"
		x = x0: y = y + 8
	Wend
	
	If Not ignoreMapFile Then
		Print #fOut, " "
		Print #fOut, "// Strings index"
		Print #fOut, " "
		
		Print #fOut, "const unsigned char *strs_" & label & " [] = {"
		lctn = 0
		Print #fOut, "	";
		For i = 0 To strCtr - 1
			Print #fOut, "str_" & label & "_" & Lcase (Hex (i, 2));
			If i < strCtr - 1 Then Print #fOut, ", ";: lctn = lctn + 1: If lctn = 8 Then lctn = 0: Print #fOut, "": Print #fOut, "	";
		Next i
		Print #fOut, ""
		Print #fOut, "};"
		Print #fOut, " "
		Print #fOut, "// " & latest & " patterns extracted. Next = " & (latest + offset)
		Print #fOut, " "
	End If

	If shouldClose Then Close #fOut
	
	filteredPuts ("+ " & latest & " patterns extracted.")
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], text mode. " & latest & " patterns extracted. Next = " & (latest + offset))
	nextPattern = latest + offset
End Sub

Sub smsDoTexts (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, fOut As Integer)
End Sub

Sub nesDoTiles (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, max As Integer, fOut As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, xx, yy, xa, ya, wp
	Dim As Integer hSize, vSize
	Dim As uByte pattern (15)
	Dim As Integer shouldClose
	Dim As Integer firstPoolIndex

	firstPoolIndex = cPoolIndex
	shouldClose = 0	
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	' Open tsmap file
	If wMeta = 2 And hMeta = 2 Then
		If fOut = -1 And Not ignoreMapFile Then
			shouldClose = -1
			fOut = FreeFile
			Open tsMapFn For Output As #fOut	
		End If

		' Write some headers to output tsmap file
		If Not ignoreMapFile Then
			Print #fOut, "// Tileset pals generated by mkts [nes mode]"
			Print #fOut, "// Copyleft 2017 The Mojon Twins"
			Print #fOut, " "

			Print #fOut, "const unsigned char " & label & "_pals [] = {"
		End If			
	End If
		
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'tiles' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	'If wMeta = 2 And hMeta = 2 Then filteredPuts ("+ Writing pal info to " & tsMapFn)
	
	For y = y0 To y1 Step vSize
		If Not ignoreMapFile Then Print #fOut, "	";
		For x = x0 To x1 Step hSize
			' Find which pal
			If wMeta = 2 And hMeta = 2 Then 
				wp = nesFindWhichPal (img, x, y, pal (), ctt, hSize, vSize)
				If Not ignoreMapFile Then Print #fOut, wp;
				If y < y1 Or x < x1 Then If Not ignoreMapFile Then Print #fOut, ", ";
			End If
			
			ya = y
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					nesExtractPatternFrom img, xa, ya, pal (), pattern ()
					addPatternToPool patternToString (pattern ())
					copyArrayToMainBin pattern ()
					ct = ct + 1
					xa = xa + 8
				Next xx
				ya = ya + 8
			Next yy
			
			ctt = ctt + 1
			If max <> -1 And ctt = max Then Exit For
		Next x
		If wMeta = 2 And hMeta = 2 Then If Not ignoreMapFile Then Print #fOut, ""
		If max <> -1 And ctt = max Then Exit For
	Next y
	
	' Hang up...
	If wMeta = 2 And hMeta = 2 Then
		If Not ignoreMapFile Then Print #fOut, "};"
		If shouldClose Then Close #fOut
	End If
	
	filteredPuts ("+ " & ct & " patterns extracted from " & ctt & " tiles.")
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], tiles mode. " & ct & " patterns extracted from " & ctt & " " & wMeta & "x" & hMeta & " tiles.")
	nextPattern = cPoolIndex
End Sub

Sub gbDoTiles (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, max As Integer, fOut As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, xx, yy, xa, ya, wp
	Dim As Integer hSize, vSize
	Dim As uByte pattern (15)
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'tiles' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	
	For y = y0 To y1 Step vSize
		If Not ignoreMapFile Then Print #fOut, "	";
		For x = x0 To x1 Step hSize
			ya = y
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					gbExtractPatternFrom img, xa, ya, pal (), pattern ()
					addPatternToPool patternToString (pattern ())
					copyArrayToMainBin pattern ()
					ct = ct + 1
					xa = xa + 8
				Next xx
				ya = ya + 8
			Next yy
			
			ctt = ctt + 1
			If max <> -1 And ctt = max Then Exit For
		Next x
		If max <> -1 And ctt = max Then Exit For
	Next y
	
	filteredPuts ("+ " & ct & " patterns extracted from " & ctt & " tiles.")
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], tiles mode. " & ct & " patterns extracted from " & ctt & " " & wMeta & "x" & hMeta & " tiles.")
End Sub

Sub smsDoTiles (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, max As Integer, fOut As Integer)
End Sub

Sub nesDoColTiles (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, max As Integer, fOut As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, xx, yy, xa, ya, wp
	Dim As Integer hSize, vSize
	Dim As uByte pattern (15)
	Dim As Integer shouldClose

	shouldClose = 0
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	' Open tsmap file
	If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then
		If fOut = -1 And Not ignoreMapFile Then
			shouldClose = -1
			fOut = FreeFile
			Open tsMapFn For Output As #fOut	

			' Write some headers to output tsmap file
			If Not ignoreMapFile Then
				Print #fOut, "// Tileset pals generated by mkts [nes mode]"
				Print #fOut, "// Copyleft 2017 The Mojon Twins"
				Print #fOut, " "
			End If
		End If

		If Not ignoreMapFile Then Print #fOut, "const unsigned char " & label & "_pals [] = {"
	End If
		
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'coltiles' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	'If wMeta = 2 And hMeta = 2 Then filteredPuts ("+ Writing pal info to " & tsMapFn)
	
	For y = y0 To y1 Step vSize
		If Not ignoreMapFile Then Print #fOut, "	";
		For x = x0 To x1 Step hSize
			' Find which pal
			If wMeta = 2 And hMeta = 2 Then 
				wp = nesFindWhichPal (img, x, y, pal (), ctt, hSize, vSize)
				If tsMapFn <> "" Then 
					If Not ignoreMapFile Then Print #fOut, wp;
					If y < y1 Or x < x1 Then If Not ignoreMapFile Then Print #fOut, ", ";
				End If
			End If
			
			xa = x
			For xx = 1 To hMeta
				ya = y
				For yy = 1 To wMeta
					nesExtractPatternFrom img, xa, ya, pal (), pattern ()
					addpatterntopool patternToString (pattern ())
					copyArrayToMainBin pattern ()
					ct = ct + 1
					ya = ya + 8
				Next yy
				xa = xa + 8
			Next xx
			
			ctt = ctt + 1
			If max <> -1 And ctt = max Then Exit For
		Next x
		If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then If Not ignoreMapFile Then Print #fOut, ""
		If max <> -1 And ctt = max Then Exit For
	Next y
	
	' Hang up...
	If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then
		If Not ignoreMapFile Then Print #fOut, "};"
		If shouldClose Then Close #fOut
	End If
	
	filteredPuts ("+ " & ct & " patterns extracted from " & ctt & " tiles.")
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], coltiles mode. " & ct & " patterns extracted from " & ctt & " " & wMeta & "x" & hMeta & " tiles.")
End Sub

Sub gbDoColTiles (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, max As Integer, fOut As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, xx, yy, xa, ya, wp
	Dim As Integer hSize, vSize
	Dim As uByte pattern (15)
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
		
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'coltiles' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	
	For y = y0 To y1 Step vSize
		If Not ignoreMapFile Then Print #fOut, "	";
		For x = x0 To x1 Step hSize
			xa = x
			For xx = 1 To hMeta
				ya = y
				For yy = 1 To wMeta
					gbExtractPatternFrom img, xa, ya, pal (), pattern ()
					addPatternToPool patternToString (pattern ())
					copyArrayToMainBin pattern ()
					ct = ct + 1
					ya = ya + 8
				Next yy
				xa = xa + 8
			Next xx
			
			ctt = ctt + 1
			If max <> -1 And ctt = max Then Exit For
		Next x
		If max <> -1 And ctt = max Then Exit For
	Next y
	
	filteredPuts ("+ " & ct & " patterns extracted from " & ctt & " tiles.")
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], coltiles mode. " & ct & " patterns extracted from " & ctt & " " & wMeta & "x" & hMeta & " tiles.")
End Sub

Sub smsDoColTiles (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, max As Integer, fOut As Integer)
End Sub

Function hFlip (pattern As String) As String
	Dim As String res
	Dim As uByte pat, patR
	Dim As Integer i, j
	
	' TODO: This works just for NES patterns. Add support for other systems when needed.
	
	For i = 1 To Len (pattern) Step 2
		pat = Val ("&H" & (Mid (pattern, i, 2)))
		patR = 0
		For j = 0 To 7
			If pat And (2^j) Then patR = patR Or (2^(7 - j))
		Next j
		res = res & Hex (patR, 2)
	Next i
	
	hFlip = res
End Function

Function vFlip (pattern As String) As String
	Dim As String res
	Dim As Integer i, j
	' TODO: This works just for NES patterns. Add support for other systems when needed.
	
	Select case sclpGetValue ("platform")
		case "gb":
			'0 2 4 6 8 1012141618202224262830
			'0 0 1 1 2 2 3 3 4 4 5 5 6 6 7 7
			'7 7 6 6 5 5 4 4 3 3 2 2 1 1 0 0
			res = Space (32)
			For i = 0 To 7
				j = i * 4
				Mid (res, 1 + j, 4) = Mid (Pattern, 1 + (28 - j), 4)
			Next i
		case "sms":
			'0 4 8 12162024283236404448525660
			'0 0 1 1 2 2 3 3 4 4 5 5 6 6 7 7
			'7 7 6 6 5 5 4 4 3 3 2 2 1 1 0 0
			res = Space (64)
			For i = 0 To 7
				j = i * 8
				Mid (res, 1 + j, 8) = Mid (Pattern, 1 + (56 - j), 8)
			Next i
		case Else:
			res = Space (32)
			For i = 0 To 7
				j = i * 2
				Mid (res, 1 + j, 2) = Mid (pattern, 1 + (14 - j), 2)
				Mid (res, 17 + j, 2) = Mid (pattern, 1 + (30 - j), 2)
			Next i
	End Select	
	vFlip = res
End Function

Function nesFindPatternInPoolAndAddEx (patternS As String, ByRef wasNew As Integer, ByRef modifier As Integer) As Integer
	Dim As Integer res, i
	Dim As String patternSH, patternSV, patternSHV
	
	If blackiszero And isAllZeroes (patternS) Then
		nesFindPatternInPoolAndAddEx = 9999
		Exit Function
	End If
	
	patternSH = hFlip (patternS)
	patternSV = vFlip (patternS)
	patternSHV = vFlip (hFlip (patternS))
	
	For i = 0 To cPoolIndex - 1
		' Straight
		If cPool (i) = patternS Then
			nesFindPatternInPoolAndAddEx = i
			wasNew = 0
			modifier = 0
			Exit Function
		End If
		
		' H flip
		If cPool (i) = patternSH Then
			nesFindPatternInPoolAndAddEx = i
			wasNew = 0
			modifier = &H40
			Exit Function
		End If
		
		' V flip
		If cPool (i) = patternSV Then
			nesFindPatternInPoolAndAddEx = i
			wasNew = 0
			modifier = &H80
			Exit Function
		End If
				
		' HV flip
		If cPool (i) = patternSHV Then
			nesFindPatternInPoolAndAddEx = i
			wasNew = 0
			modifier = &HC0
			Exit Function
		End If
	Next i

	modifier = 0
	
	' Add
	'cPool (cPoolIndex) = patternS
	
	' Next
	'cPoolIndex = cPoolIndex + 1
	addPatternToPool patternS
	
	' Return
	wasNew = -1
	nesFindPatternInPoolAndAddEx = cPoolIndex - 1
End Function

Function gbFindPatternInPoolAndAddEx (patternS As String, ByRef wasNew As Integer, ByRef modifier As Integer) As Integer
	Dim As Integer res, i
	Dim As String patternSH, patternSV, patternSHV
	
	If blackiszero And isAllZeroes (patternS) Then
		gbFindPatternInPoolAndAddEx = 9999
		Exit Function
	End If
	
	patternSH = hFlip (patternS)
	patternSV = vFlip (patternS)
	patternSHV = vFlip (hFlip (patternS))
	
	For i = 0 To cPoolIndex - 1
		' Straight
		If cPool (i) = patternS Then
			gbFindPatternInPoolAndAddEx = i
			wasNew = 0
			modifier = 0
			Exit Function
		End If
		
		' H flip
		If cPool (i) = patternSH Then
			gbFindPatternInPoolAndAddEx = i
			wasNew = 0
			modifier = &H20
			Exit Function
		End If
		
		' V flip
		If cPool (i) = patternSV Then
			gbFindPatternInPoolAndAddEx = i
			wasNew = 0
			modifier = &H40
			Exit Function
		End If
				
		' HV flip
		If cPool (i) = patternSHV Then
			gbFindPatternInPoolAndAddEx = i
			wasNew = 0
			modifier = &H60
			Exit Function
		End If
	Next i

	modifier = 0
	
	' Add
	'cPool (cPoolIndex) = patternS
	' Next
	'cPoolIndex = cPoolIndex + 1

	addPatternToPool patternS
	
	' Return
	wasNew = -1
	gbFindPatternInPoolAndAddEx = cPoolIndex - 1
End Function

Function smsFindPatternInPoolAndAddEx (patternS As String, ByRef wasNew As Integer, ByRef modifier As Integer) As Integer
	' modifier has to fill in v and h.
	' ---pcvhn
	' v  = 100 = 4,
	' h  = 010 = 2,
	' vh = 110 = 6.
	Dim As Integer res, i
	Dim As String patternSH, patternSV, patternSHV
	
	If blackiszero And isAllZeroes (patternS) Then
		smsFindPatternInPoolAndAddEx = 9999
		Exit Function
	End If
	
	patternSH = hFlip (patternS)
	patternSV = vFlip (patternS)
	patternSHV = vFlip (hFlip (patternS))
	
	For i = 0 To cPoolIndex - 1
		' Straight
		If cPool (i) = patternS Then
			smsFindPatternInPoolAndAddEx = i
			wasNew = 0
			modifier = 0
			Exit Function
		End If
		
		' H flip
		If cPool (i) = patternSH Then
			smsFindPatternInPoolAndAddEx = i
			wasNew = 0
			modifier = &H0200
			Exit Function
		End If
		
		' V flip
		If cPool (i) = patternSV Then
			smsFindPatternInPoolAndAddEx = i
			wasNew = 0
			modifier = &H0400
			Exit Function
		End If
				
		' HV flip
		If cPool (i) = patternSHV Then
			smsFindPatternInPoolAndAddEx = i
			wasNew = 0
			modifier = &H0600
			Exit Function
		End If
	Next i

	modifier = 0
	
	' Add
	'cPool (cPoolIndex) = patternS
	' Next
	'cPoolIndex = cPoolIndex + 1

	addPatternToPool patternS
	
	' Return
	wasNew = -1
	smsFindPatternInPoolAndAddEx = cPoolIndex - 1
End Function

Function smsDetectPriority (img As Any ptr, x0 As Integer, y0 As Integer) As Integer
	Dim As Integer c, x, y, res
	res = 0
	For x = x0 To x0 + 7
		For y = y0 To y0 + 7
			c = Point (x, y, img)
			If c = &HFF171717 Then 
				res = &H1000
				Exit For 
			End If
		Next y 
		If res <> 0 Then Exit For
	Next x

	smsDetectPriority = res
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
	'cPool (cPoolIndex) = pattern1S
	'cPoolIndex = cPoolIndex + 1
	'cPool (cPoolIndex) = pattern2S
	'cPoolIndex = cPoolIndex + 1
	addPatternToPool pattern1S
	addPatternToPool pattern2S
	
	' Return
	wasNew = -1
	find2PatternsInPoolAndAdd = cPoolIndex - 2
End Function

' cPoolIndex, tMapsIndex

Sub sg1000DoTmaps (img As Any Ptr, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, fOut, xx, yy, xa, ya, wp, found, i, j
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pgt (7), cgt (7), tMap (255)
	Dim As String patternS
	Dim As Integer wasNew
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	fOut = FreeFile
	Open tsMapFn For Output As #fOut	

	' Write some headers to output tsmap file
	If Not ignoreMapFile Then
		Print #fOut, "// Tileset tilemaps generated by mkts [sg1000 mode]"
		Print #fOut, "// Copyleft 2017 The Mojon Twins"
		Print #fOut, " "
	End If
		
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'meta' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
		
	tMapsIndex = 0
	
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
						addPatternToPool sg1000patternToString (pgt (), cgt ())
						copyArrayToMainBin pgt ()
						copyArrayToClrBin cgt ()
					End If
					if ct = 9999 Then tMap (tMapI) = 0 Else tMap (tMapI) = ct + offset
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
	If Not ignoreMapFile Then 
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
	End If		
	
	filteredPuts ("+ " & tMapsIndex & " tmaps extracted using " & cPoolIndex & " tiles.")
	Close #fOut
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], tilemap mode. " & cPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " tiles. Next = " & cPoolIndex + offset)
	nextPattern = cPoolIndex + offset
End Sub

Sub nesDoTmaps (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, fOut As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, xx, yy, xa, ya, wp, found, i, j, ctr
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pattern (15), tMap (255)
	Dim As String patternS
	Dim As Integer wasNew
	Dim As Integer wps (3)
	Dim As Integer shouldClose
	Dim As Integer firstPoolIndex
	Dim As Integer mtI
	Dim As String pallist

	Dim As Integer byteArrays (16, 255)

	firstPoolIndex = cPoolIndex

	shouldClose = 0
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	If fOut = -1 And Not ignoreMapFile Then 
		shouldClose = -1
		fOut = FreeFile
		Open tsMapFn For Output As #fOut	
	End If

	' Write some headers to output tsmap file
	If Not ignoreMapFile Then
		Print #fOut, "// Tileset tilemaps & pals generated by mkts [nes mode]"
		Print #fOut, "// Copyleft 2017 The Mojon Twins"
		Print #fOut, " "
		If Not deinterlaced Then Print #fOut, "const unsigned char " & label & "_pals [] = {"
	End If

	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'meta' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	'If wMeta = 2 And hMeta = 2 Then filteredPuts ("+ Writing pal info to " & tsMapFn)
	If wMeta = 4 And hMeta = 4 Then filteredPuts ("+ Writing precalculated pal bytes to " & tsMapFn)
	
	tMapsIndex = 0
	pallist = ""

	For y = y0 To y1 Step vSize
		If Not deinterlaced Then If Not ignoreMapFile Then Print #fOut, "	";
		For x = x0 To x1 Step hSize	
			' Find which pal
			If wMeta = 2 And hMeta = 2 Then 
				' 2x2 mode
				wp = nesFindWhichPal (img, x, y, pal (), tMapsIndex, hSize, vSize)
				pallist = pallist & wp
				If deinterlaced Then
					byteArrays (16, tMapsIndex) = wp
				Else
					If tsMapFn <> "" Then 
						If Not ignoreMapFile Then Print #fOut, "0x" & Lcase (Hex (wp, 2)); 'Ltrim (Str (wp));
						If Not ignoreMapFile Then If y < y1+1-vSize Or x < x1+1-hSize Then Print #fOut, ", ";
					End If
				End If
			ElseIf wMeta = 4 And hMeta = 4 Then
				' 4x4 mode, write full precalculated attribute byte
				wps (0) = nesFindWhichPal (img, x, y, pal (), tMapsIndex, 16, 16)			': filteredPuts "(" & x & "," & y & ") -> " & wps (0)
				wps (1) = nesFindWhichPal (img, x+16, y, pal (), tMapsIndex, 16, 16)		': filteredPuts "(" & (x+16) & "," & y & ") -> " & wps (1)
				wps (2) = nesFindWhichPal (img, x, y+16, pal (), tMapsIndex, 16, 16)		': filteredPuts "(" & x & "," & (y+16) & ") -> " & wps (2)
				wps (3) = nesFindWhichPal (img, x+16, y+16, pal (), tMapsIndex, 16, 16)	': filteredPuts "(" & (x+16) & "," & (y+16) & ") -> " & wps (3)				
				' build byte and write.
				i = wps (0) Or (wps (1) Shl 2) Or (wps (2) Shl 4) Or (wps (3) Shl 6)
				If deinterlaced Then 
					byteArrays (16, tMapsIndex) = i
				Else
					If tsMapFn <> "" Then 
						If Not ignoreMapFile Then Print #fOut, "0x" & Lcase (Hex (i, 2)); 'Ltrim (Str (i));
						If y < y1+1-vSize Or x < x1+1-hSize Then Print #fOut, ", ";
					End If
				End If
			End If
			
			ya = y
			tMapI = 0
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					'nesExtractPatternFrom img, xa, ya, pal (), pattern ()
					If wMeta = 4 And hMeta = 4 Then
						i = (((yy - 1)\2)*2) + (xx - 1)\2
						nesExtractPatternFromWithPal img, xa, ya, pal (), wps (i), pattern ()
					Else
						nesExtractPatternFromWithPal img, xa, ya, pal (), wp, pattern ()
					End If
					patternS = patternToString (pattern ())
					ct = findPatternInPoolAndAdd (patternS, wasnew)
					If wasnew Then copyArrayToMainBin pattern ()
					If ct = 9999 Then tMap (tMapI) = 0 Else tMap (tMapI) = ct + offset
					byteArrays (tMapI, tMapsIndex) = tMap (tMapI)
					tMapI = tMapI + 1: If tMapI > 255 Then Puts "MORE THAN 255 PATTERNS! ": System
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
		If Not deinterlaced Then
			If Not ignoreMapFile Then
				If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then Print #fOut, ""
				If wMeta = 4 And hMeta = 4 And tsMapFn <> "" Then Print #fOut, ""
			End If
		End If
		If max <> -1 And tMapsIndex = max Then Exit For
	Next y
	
	' Hang up...
	If Not deinterlaced Then 
		If Not ignoreMapFile Then
			If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then
				Print #fOut, "};"
				Print #fOut, ""
			End If
			If wMeta = 4 And hMeta = 4 And tsMapFn <> "" Then
				Print #fOut, "};"
				Print #fOut, ""
			End If
		End If
	End If
	
	' Write tmaps
	If Not ignoreMapFile Then
		If Not deinterlaced Then 
			If columns Then 
				Print #fOut, "// Column order ahead! Warning!"
				Print #fOut, "const unsigned char " & label & "_tmaps [] = {"
				For i = 0 To tMapsIndex - 1
					Print #fOut, "	";
					For j = 0 To (hMeta * wMeta) - 1
						Print #fOut, "0x" & Lcase (Hex (tMaps (i, (j\hMeta) + ((j Mod hMeta) * hMeta)), 2));
						If j < (hMeta * wMeta) - 1 Or i < tMapsIndex - 1 Then Print #fOut, ", ";
					Next j
					Print #fOut, ""
				Next i
				Print #fOut, "};"	
			Else
				Print #fOut, "const unsigned char " & label & "_tmaps [] = {"
				For i = 0 To tMapsIndex - 1
					Print #fOut, "	";
					For j = 0 To (hMeta * wMeta) - 1
						Print #fOut, "0x" & Lcase (Hex (tMaps (i, j), 2));
						If j < (hMeta * wMeta) - 1 Or i < tMapsIndex - 1 Then Print #fOut, ", ";
					Next j
					Print #fOut, ""
				Next i
				Print #fOut, "};"
			End If
		End If
	End If

	' Deinterlaced output
	If Not ignoreMapFile Then
		If deinterlaced Then
			For i = 0 To hMeta*wMeta - 1
				Print #fOut, "const unsigned char " & label & "_tmaps_" & lcase (Hex (i, 1)) & " [] = {"
				ctr = 0
				For j = 0 to tMapsIndex - 1
					If ctr = 0 Then Print #fOut, "	";
					Print #fOut, "0x" & lcase (Hex (byteArrays (i, j), 2));
					If j < tMapsIndex - 1 Then Print #fOut, ", ";
					ctr = ctr + 1: If ctr = 16 Then ctr = 0: Print #fOut, ""
				Next j
				If ctr Then Print #fOut, ""
				Print #fOut, "};"
				Print #fOut, ""
			Next i
			Print #fOut, "const unsigned char " & label & "_pals [] = {"
			ctr = 0
			For j = 0 to tMapsIndex - 1
				If ctr = 0 Then Print #fOut, "	";
				Print #fOut, "0x" & lcase (Hex (byteArrays (16, j), 2));
				If j < tMapsIndex - 1 Then Print #fOut, ", ";
				ctr = ctr + 1: If ctr = 16 Then ctr = 0: Print #fOut, ""
			Next j
			If ctr Then Print #fOut, ""
			Print #fOut, "};"
			Print #fOut, ""
		End If
	End If

	If outputPalList Then Puts pallist

	filteredPuts ("+ " & tMapsIndex & " tmaps extracted using " & cPoolIndex & " tiles.")
	If shouldClose Then Close #fOut Else Print #fOut, ""
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], tilemap mode. " & cPoolIndex - firstPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " tiles. Next = " & cPoolIndex + offset)
	nextPattern = cPoolIndex + offset
End Sub

Sub gbDoTmaps (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, fOut As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, xx, yy, xa, ya, wp, found, i, j, ctr
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pattern (15), tMap (255)
	Dim As String patternS
	Dim As Integer wasNew
	Dim As Integer shouldClose
	Dim As Integer firstPoolIndex
	Dim As Integer mtI

	Dim As Integer byteArrays (16, 255)

	firstPoolIndex = cPoolIndex

	shouldClose = 0
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	If fOut = -1 And Not ignoreMapFile Then
		shouldClose = -1
		fOut = FreeFile
		Open tsMapFn For Output As #fOut	
	End If

	' Write some headers to output tsmap file
	If Not ignoreMapFile Then
		Print #fOut, "// Tileset tilemaps generated by mkts [gb mode]"
		Print #fOut, "// Copyleft 2017 The Mojon Twins"
		Print #fOut, " "
		If Not deinterlaced Then Print #fOut, "const unsigned char " & label & "_pals [] = {"
	End If		
		
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'meta' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	
	tMapsIndex = 0
	
	For y = y0 To y1 Step vSize
		For x = x0 To x1 Step hSize
						
			ya = y
			tMapI = 0
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					gbExtractPatternFrom img, xa, ya, pal (), pattern ()
					patternS = patternToString (pattern ())
					ct = findPatternInPoolAndAdd (patternS, wasnew)
					If wasnew Then copyArrayToMainBin pattern ()
					If ct = 9999 Then tMap (tMapI) = 0 Else tMap (tMapI) = ct + offset
					byteArrays (tMapI, tMapsIndex) = tMap (tMapI)
					tMapI = tMapI + 1: If tMapI > 255 Then Puts "MORE THAN 255 PATTERNS! ": System
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
	If Not ignoreMapFile Then
		If Not deinterlaced Then 
			If columns Then 
				Print #fOut, "// Column order ahead! Warning!"
				Print #fOut, "const unsigned char " & label & "_tmaps [] = {"
				For i = 0 To tMapsIndex - 1
					Print #fOut, "	";
					For j = 0 To (hMeta * wMeta) - 1
						Print #fOut, "0x" & Lcase (Hex (tMaps (i, (j\hMeta) + ((j Mod hMeta) * hMeta)), 2));
						If j < (hMeta * wMeta) - 1 Or i < tMapsIndex - 1 Then Print #fOut, ", ";
					Next j
					Print #fOut, ""
				Next i
				Print #fOut, "};"	
			Else
				Print #fOut, "const unsigned char " & label & "_tmaps [] = {"
				For i = 0 To tMapsIndex - 1
					Print #fOut, "	";
					For j = 0 To (hMeta * wMeta) - 1
						Print #fOut, "0x" & Lcase (Hex (tMaps (i, j), 2));
						If j < (hMeta * wMeta) - 1 Or i < tMapsIndex - 1 Then Print #fOut, ", ";
					Next j
					Print #fOut, ""
				Next i
				Print #fOut, "};"
			End If
		End If
	End If

	' Deinterlaced output
	If Not ignoreMapFile Then
		If deinterlaced Then
			For i = 0 To hMeta*wMeta - 1
				Print #fOut, "const unsigned char " & label & "_tmaps_" & lcase (Hex (i, 1)) & " [] = {"
				ctr = 0
				For j = 0 to tMapsIndex - 1
					If ctr = 0 Then Print #fOut, "	";
					Print #fOut, "0x" & lcase (Hex (byteArrays (i, j), 2));
					If j < tMapsIndex - 1 Then Print #fOut, ", ";
					ctr = ctr + 1: If ctr = 16 Then ctr = 0: Print #fOut, ""
				Next j
				If ctr Then Print #fOut, ""
				Print #fOut, "};"
				Print #fOut, ""
			Next i
		End If
	End If

	filteredPuts ("+ " & tMapsIndex & " tmaps extracted using " & cPoolIndex & " tiles.")
	If shouldClose Then Close #fOut Else Print #fOut, ""
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], tilemap mode. " & cPoolIndex - firstPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " tiles. Next = " & cPoolIndex + offset)
	nextPattern = cPoolIndex + offset
End Sub

Sub smsDoTmaps (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, fOut As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, xx, yy, xa, ya, wp, found, i, j, ctr
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pattern (31)
	Dim As uShort tMap (255)
	Dim As uShort value, pri, wpalette
	Dim As String patternS
	Dim As Integer wasNew, modifier
	Dim As Integer shouldClose
	Dim As Integer firstPoolIndex

	Dim As Integer byteArrays (16, 255)

	' Each item in the tilemap is a full 16 bits integer with this format:
	' ---pcvhnnnnnnnnn
	' LSB is written first, MSB is written then.
	' Have to find for h/v-flipped occurences, of course, and fill in the 'p' bit

	shouldClose = 0
	firstPoolIndex = cPoolIndex
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	If fOut = -1 And Not ignoreMapFile Then
		shouldClose = -1
		fOut = FreeFile
		Open tsMapFn For Output As #fOut	
	End If

	' Write some headers to output tsmap file
	If Not ignoreMapFile Then
		Print #fOut, "// Tileset tilemaps generated by mkts [sms mode]"
		Print #fOut, "// Copyleft 2017 The Mojon Twins"
		Print #fOut, " "
	End If		

	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'meta' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	
	tMapsIndex = 0

	For y = y0 To y1 Step vSize
		For x = x0 To x1 Step hSize
			ya = y
			tMapI = 0
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					' Detect priority
					pri = smsDetectPriority (img, xa, ya)

					' Detect palette
					wp = smsFindWhichPal (img, xa, ya, pal (), (ct), 8, 8)
					If wp = 0 Then wpalette = 0 Else wpalette = &H0800

					smsExtractPatternFromWithPal img, xa, ya, pal (), wp, pattern ()
					patternS = patternToString (pattern ())
					ct = smsFindPatternInPoolAndAddEx (patternS, wasnew, modifier)
					If wasnew Then copyArrayToMainBin pattern ()

					' Compose bytes
					If ct = 9999 Then value = 0 Else value = ct + offset

					tMap (tMapi) = value + pri + modifier + wpalette
					byteArrays (tMapI, tMapsIndex) = tMap (tMapI)
					tMapI = tMapI + 1
					xa = xa + 8
				Next xx
				ya = ya + 8
			Next yy
		
			' Store tmap
			For i = 0 To (hMeta * wMeta) - 1
				tMapsS (tMapsIndex, i) = tMap (i)
			Next i
			tMapsIndex = tMapsIndex + 1
			If max <> -1 And tMapsIndex = max Then Exit For
		Next x
		If max <> -1 And tMapsIndex = max Then Exit For
	Next y
	
	' Write tmaps
	If Not ignoreMapFile Then
		If Not deinterlaced Then 
			If columns Then
				Print #fOut, "// Column order ahead! Warning!"
				Print #fOut, "const unsigned int " & label & "_tmaps [] = {"
				For i = 0 To tMapsIndex - 1
					Print #fOut, "	";
					For j = 0 To (hMeta * wMeta) - 1
						Print #fOut, "0x" & Lcase (Hex (tMapsS (i, (j\hMeta) + ((j Mod hMeta) * hMeta)), 4));
						If j < (hMeta * wMeta) - 1 Or i < tMapsIndex - 1 Then Print #fOut, ", ";
					Next j
					Print #fOut, ""
				Next i
				Print #fOut, "};"
			Else
				Print #fOut, "const unsigned int " & label & "_tmaps [] = {"
				For i = 0 To tMapsIndex - 1
					Print #fOut, "	";
					For j = 0 To (hMeta * wMeta) - 1
						Print #fOut, "0x" & Hex (tMapsS (i, j), 4);
						If j < (hMeta * wMeta) - 1 Or i < tMapsIndex - 1 Then Print #fOut, ", ";
					Next j
					Print #fOut, ""
				Next i
				Print #fOut, "};"
			End If
		End If

		' Deinterlaced output
		If deinterlaced Then
			For i = 0 To hMeta*wMeta - 1
				Print #fOut, "const unsigned int " & label & "_tmaps_" & lcase (Hex (i, 1)) & " [] = {"
				ctr = 0
				For j = 0 to tMapsIndex - 1
					If ctr = 0 Then Print #fOut, "	";
					Print #fOut, "0x" & lcase (Hex (byteArrays (i, j), 4));
					If j < tMapsIndex - 1 Then Print #fOut, ", ";
					ctr = ctr + 1: If ctr = 16 Then ctr = 0: Print #fOut, ""
				Next j
				If ctr Then Print #fOut, ""
				Print #fOut, "};"
				Print #fOut, ""
			Next i
		End If
	End If

	filteredPuts ("+ " & tMapsIndex & " tmaps extracted using " & cPoolIndex & " tiles.")
	If shouldClose Then Close #fOut Else Print #fOut, ""
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], tilemap mode. " & cPoolIndex - firstPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " tiles. Next = " & cPoolIndex + offset)
	nextPattern = cPoolIndex + offset
End Sub

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
			If ct = 9999 Then tMap (tMapI) = 0 Else tMap (tMapI) = ct + offset
			tMapI = tMapI + 1
		Next x
	Next y
	
	' Write tilemap
	If Not ignoreMapFile Then
		fOut = FreeFile
		Open tsMapFn For Output As #fOut	

		' Write some headers to output tsmap file
		Print #fOut, "// Tileset tilemaps generated by mkts [sg1000 mode]"
		Print #fOut, "// Copyleft 2017 The Mojon Twins"
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
	End If
		
	Close #fOut
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], mapped mode. " & cPoolIndex & " patterns extracted. Next = " & cPoolIndex + offset)
	nextPattern = cPoolIndex + offset
End Sub

Sub sg1000DoSprites (img As Any Ptr, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, sprOrgX As Integer, sprOrgY As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, fOut, xx, yy, xa, ya, wp, found, i, j, xp, yp, c, xm, ym, xoff, yoff, xaa, yaa
	Dim As Integer hSize, vSize, tMapI, swapped
	Dim As uByte sgt (7), metaSprite (255)
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
	Print #fOut, "// Copyleft 2017 The Mojon Twins"
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
								addPatternToPool patternToString (sgt ())
								copyArrayToMainBin sgt ()
								'cPoolIndex = cPoolIndex + 1
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
	nextPattern = cPoolIndex + offset
End Sub

Sub nesDoSprites (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, sprOrgX As Integer, sprOrgY As Integer, fOut As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, xx, yy, xa, ya, wp, found, i, j
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pattern (15), tMap (255)
	Dim As String patternS
	Dim As Integer wasNew, modifier
	Dim As uByte metapals (255)
	Dim As Integer shouldClose
	Dim As Integer firstPoolIndex

	firstPoolIndex = cPoolIndex

	shouldClose = 0
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	If fOut = -1 Then
		shouldClose = -1
		fOut = FreeFile
		Open tsMapFn For Output As #fOut	
		tMapsIndex = 0
	Else
		tMapsIndex = metaSpriteCounter
	End If

	If fOut = -1 Or wProc <> 1 or hProc <> 1 Then
		' Write some headers to output tsmap file
		Print #fOut, "// Metaspriteset generated by mkts [nes mode]"
		Print #fOut, "// Copyleft 2017 The Mojon Twins"
		Print #fOut, " "
	End If
		
	If flipped Then Print #fOut, "// Each cell: A = normal, B = flipped": Print #fOut, " "
	If mirrored Then Print #fOut, "// Each cell: A = normal, B = mirrored": Print #fOut, " "
	
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

			ya = y
			tMapI = 0
			
			If shouldClose Or wProc <> 1 or hProc <> 1 Then 
				Print #fout, "// Cell #" & tMapsIndex & " cut from (" & x & ", " & y & ") to (" & (x + hSize - 1) & ", " & (y + vSize - 1) & ")"
			Else
				Print #fout, "// Cell cut from (" & x & ", " & y & ") to (" & (x + hSize - 1) & ", " & (y + vSize - 1) & ")"
			End If 
			Print #fout, ""
			
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					''ctt = (xa \ 8) + (ya \ 8) * (x1 + 1) \ 8
					''ctt = (x \ hSize) + (y \ vSize) * wProc
					''ctt = ((y-y0)\8+yy-1) * wProc + ((x-x0)\8+xx-1)
					ctt = ((ya-y0)\8)*wProc*wMeta + (xa-x0)\8
					wp = nesFindWhichPal (img, xa, ya, pal (), ctt, 8, 8)
					nesExtractPatternFrom img, xa, ya, pal (), pattern ()

					' Only do stuff when pattern is not all zeroes
					If patternIsAllZeroes (pattern ()) And Not noskipempty Then
						metapals (tMapI) = &HFF
					Else
						patternS = patternToString (pattern ())
						ct = nesFindPatternInPoolAndAddEx (patternS, wasnew, modifier)
						If wasnew Then copyArrayToMainBin pattern ()
						If ct = 9999 Then tMap (tMapI) = 0 Else tMap (tMapI) = ct + offset
						metapals (tMapI) = modifier Or wp
					End If
					tMapI = tMapI + 1
					xa = xa + 8
				Next xx
				ya = ya + 8
			Next yy
			
			' Write metaspriteset
			If shouldClose Or wProc <> 1 or hProc <> 1 Then
				If flipped Or mirrored Then 
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_a [] = {"
				Else
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & " [] = {"
				End If
			Else
				If flipped Or mirrored Then 
					Print #fOut, "const unsigned char " & label & "_a [] = {"
				Else
					Print #fOut, "const unsigned char " & label & " [] = {"
				End If
			End If
			tMapI = 0
			ya = sprOrgY
			For yy = 1 To hMeta
				xa = sprOrgX
				Print #fOut, "	";
				For xx = 1 To wMeta
					' X, Y, T, A
					If metapals (tMapI) = &Hff then
						Print #fOut, "                        ";
					Else
						Print #fOut, "0x" & Lcase (Hex (xa, 2)) & ", 0x" & Lcase (Hex (ya, 2)) & ", 0x" & Lcase (Hex (tMap (tMapI), 2)) & ", 0x" & Lcase (Hex (metapals (tMapI), 2));
						Print #fOut, ", ";
					End If
					tMapI = tMapI + 1
					xa = xa + 8
				Next xx
				ya = ya + 8
				Print #fOut, ""
			Next yy
			Print #fOut, "	";
			Print #fOut, "0x" & Hex (128, 2)
			Print #fOut, "};"
			
			' Write flipped  / mirrored metaspriteset
			If flipped Or mirrored Then
				If shouldClose Or wProc <> 1 or hProc <> 1 Then
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_b [] = {"
				Else
					Print #fOut, "const unsigned char " & label & "_b [] = {"
				End If
				ya = sprOrgY
				For yy = 1 To hMeta
					If flipped Then
						xa = sprOrgX
					Else
						xa = -hSize - sprOrgX + metalogicalwidth
					End If
					Print #fOut, "	";
					For xx = 1 To wMeta
						' Flip horz order, don't forget!
						tMapI = (wMeta - xx) + (yy - 1) * wMeta
						' X, Y, T, A
						If metapals (tMapI) = &Hff then
							Print #fOut, "                        ";
						Else
							Print #fOut, "0x" & Lcase (Hex (xa, 2)) & ", 0x" & Lcase (Hex (ya, 2)) & ", 0x" & Lcase (Hex (tMap (tMapI), 2)) & ", 0x" & Lcase (Hex (64 Xor metapals (tMapI), 2));
							Print #fOut, ", ";
						End If 	
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
			
			' Write upside-down metaspriteset
			If upsideDown Then
				If shouldClose Or wProc <> 1 or hProc <> 1 Then
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_ud [] = {"
				Else
					Print #fOut, "const unsigned char " & label & "_ud [] = {"
				End If
				ya = sprOrgY
				For yy = 1 To hMeta
					xa = sprOrgX
					Print #fOut, "	";
					For xx = 1 To wMeta
						' Flip horz order, don't forget!
						tMapI = (xx - 1) + (hMeta - yy) * wMeta
						' X, Y, T, A
						If metapals (tMapI) = &Hff then
							Print #fOut, "                        ";
						Else
							Print #fOut, "0x" & Lcase (Hex (xa, 2)) & ", 0x" & Lcase (Hex (ya, 2)) & ", 0x" & Lcase (Hex (tMap (tMapI), 2)) & ", 0x" & Lcase (Hex (128 Xor metapals (tMapI), 2));
							Print #fOut, ", ";
						End If
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
		
			tMapsIndex = tMapsIndex + 1
			If max <> -1 And tMapsIndex = max Then Exit For
		Next x
		If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then Print #fOut, ""
		If max <> -1 And tMapsIndex = max Then Exit For
	Next y
	
	If shouldClose Then
		Print #fOut, "// " & cPoolIndex & " patterns used from #" & offset & ", Next = " & cPoolIndex + offset & "."
		Print #fOut, ""
	End if
	
	filteredPuts ("+ " & tMapsIndex & " cells extracted using " & cPoolIndex & " patterns.")
	If shouldClose Then Close #fOut
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], sprites mode. " & (cPoolIndex-firstPoolIndex) & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " metasprites. Next = " & cPoolIndex + offset)
	nextPattern = cPoolIndex + offset

	metaSpriteCounter = tMapsIndex
End Sub

Sub gbDoSprites (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, sprOrgX As Integer, sprOrgY As Integer, fOut As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, xx, yy, xa, ya, wp, found, i, j
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pattern (15), tMap (255)
	Dim As String patternS
	Dim As Integer wasNew, modifier
	Dim As uByte metapals (255)
	Dim As Integer shouldClose
	Dim As Integer firstPoolIndex

	firstPoolIndex = cPoolIndex

	shouldClose = 0
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	If fOut = -1 Then
		shouldClose = -1
		fOut = FreeFile
		Open tsMapFn For Output As #fOut	

		' Write some headers to output tsmap file
		Print #fOut, "// Metaspriteset generated by mkts [gb mode]"
		Print #fOut, "// Copyleft 2017 The Mojon Twins"
		Print #fOut, " "
	End If		
		
	If flipped Then Print #fOut, "// Each cell: A = normal, B = flipped": Print #fOut, " "
	If mirrored Then Print #fOut, "// Each cell: A = normal, B = mirrored": Print #fOut, " "
	
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'sprites' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	filteredPuts ("+ Writing metasprites info to " & tsMapFn)
	
	tMapsIndex = 0
	
	For y = y0 To y1 Step vSize
		For x = x0 To x1 Step hSize
			ya = y
			tMapI = 0
			
			If shouldClose Or wProc <> 1 or hProc <> 1 Then 
				Print #fout, "// Cell #" & tMapsIndex & " cut from (" & x & ", " & y & ") to (" & (x + hSize - 1) & ", " & (y + vSize - 1) & ")"
			Else
				Print #fout, "// Cell cut from (" & x & ", " & y & ") to (" & (x + hSize - 1) & ", " & (y + vSize - 1) & ")"
			End If 
			Print #fout, ""
			
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					''ctt = (xa \ 8) + (ya \ 8) * (x1 + 1) \ 8
					''ctt = (x \ hSize) + (y \ vSize) * wProc
					''ctt = ((y-y0)\8+yy-1) * wProc + ((x-x0)\8+xx-1)
					ctt = ((ya-y0)\8)*wProc*wMeta + (xa-x0)\8
					gbExtractPatternFrom img, xa, ya, pal (), pattern ()
					' Only do stuff when pattern is not all zeroes
					If patternIsAllZeroes (pattern ()) And Not noskipempty Then
						metapals (tMapI) = &HFF
					Else
						patternS = patternToString (pattern ())
						ct = gbFindPatternInPoolAndAddEx (patternS, wasnew, modifier)
						If wasnew Then copyArrayToMainBin pattern ()
						If ct = 9999 Then tMap (tMapI) = 0 Else tMap (tMapI) = ct + offset
						metapals (tMapI) = modifier
					End If
					tMapI = tMapI + 1
					xa = xa + 8
				Next xx
				ya = ya + 8
			Next yy
			
			' Write metaspriteset
			If shouldClose Or wProc <> 1 or hProc <> 1 Then
				If flipped Or mirrored  Then 
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_a [] = {"
				Else
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & " [] = {"
				End If
			Else
				If flipped Or mirrored Then 
					Print #fOut, "const unsigned char " & label & "_a [] = {"
				Else
					Print #fOut, "const unsigned char " & label & " [] = {"
				End If
			End If
			tMapI = 0
			ya = sprOrgY
			For yy = 1 To hMeta
				xa = sprOrgX
				Print #fOut, "	";
				For xx = 1 To wMeta
					' X, Y, T, A
					If metapals (tMapI) = &Hff then
						Print #fOut, "                        ";
					Else
						Print #fOut, "0x" & Lcase (Hex (xa, 2)) & ", 0x" & Lcase (Hex (ya, 2)) & ", 0x" & Lcase (Hex (tMap (tMapI), 2)) & ", 0x" & Lcase (Hex (metapals (tMapI), 2));
						Print #fOut, ", ";
					End If
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
			If flipped Or mirrored Then
				If shouldClose Or wProc <> 1 or hProc <> 1 Then
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_b [] = {"
				Else
					Print #fOut, "const unsigned char " & label & "_b [] = {"
				End If
				ya = sprOrgY
				For yy = 1 To hMeta
					If flipped Then
						xa = sprOrgX
					Else
						xa = -hSize - sprOrgX + metalogicalwidth
					End If
					Print #fOut, "	";
					For xx = 1 To wMeta
						' Flip horz order, don't forget!
						tMapI = (wMeta - xx) + (yy - 1) * wMeta
						' X, Y, T, A
						If metapals (tMapI) = &Hff then
							Print #fOut, "                        ";
						Else
							Print #fOut, "0x" & Lcase (Hex (xa, 2)) & ", 0x" & Lcase (Hex (ya, 2)) & ", 0x" & Lcase (Hex (tMap (tMapI), 2)) & ", 0x" & Lcase (Hex (64 Xor metapals (tMapI), 2));
							Print #fOut, ", ";
						End If 							
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
			
			' Write upside-down metaspriteset
			If upsideDown Then
				If shouldClose Or wProc <> 1 or hProc <> 1 Then
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_ud [] = {"
				Else
					Print #fOut, "const unsigned char " & label & "_ud [] = {"
				End If
				ya = sprOrgY
				For yy = 1 To hMeta
					xa = sprOrgX
					Print #fOut, "	";
					For xx = 1 To wMeta
						' Flip horz order, don't forget!
						tMapI = (xx - 1) + (hMeta - yy) * wMeta
						' X, Y, T, A
						If metapals (tMapI) = &Hff then
							Print #fOut, "                        ";
						Else
							Print #fOut, "0x" & Lcase (Hex (xa, 2)) & ", 0x" & Lcase (Hex (ya, 2)) & ", 0x" & Lcase (Hex (tMap (tMapI), 2)) & ", 0x" & Lcase (Hex (128 Xor metapals (tMapI), 2));
							Print #fOut, ", ";
						End If
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
		
			tMapsIndex = tMapsIndex + 1
			If max <> -1 And tMapsIndex = max Then Exit For
		Next x
		If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then Print #fOut, ""
		If max <> -1 And tMapsIndex = max Then Exit For
	Next y
	
	If shouldClose Then
		Print #fOut, "// " & cPoolIndex & " patterns used from #" & offset & ", Next = " & cPoolIndex + offset & "."
		Print #fOut, ""
	End if
	
	filteredPuts ("+ " & tMapsIndex & " cells extracted using " & (cPoolIndex-firstPoolIndex) & " patterns.")
	If shouldClose Then Close #fOut
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], sprites mode. " & cPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " metasprites. Next = " & cPoolIndex + offset)
	nextPattern = cPoolIndex + offset

	metaSpriteCounter = tMapsIndex
End Sub

Sub smsDoSprites (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, sprOrgX As Integer, sprOrgY As Integer, fOut As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, xx, yy, xa, ya, wp, found, i, j
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pattern (31), tMap (255)
	Dim As String patternS
	Dim As Integer wasNew, modifier
	Dim As Integer cellActive (255)
	Dim As Integer shouldClose

	shouldClose = 0
	
	hSize = wMeta * 8
	vSize = hMeta * 8
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	If fOut = -1 Then
		shouldClose = -1
		fOut = FreeFile
		Open tsMapFn For Output As #fOut	

		' Write some headers to output tsmap file
		Print #fOut, "// Metaspriteset generated by mkts [sms mode]"
		Print #fOut, "// Copyleft 2017 The Mojon Twins"
		Print #fOut, " "
	End If		

	If flipped Then Print #fOut, "// Each cell: A = normal, B = flipped": Print #fOut, " "
	If mirrored Then Print #fOut, "// Each cell: A = normal, B = mirrored": Print #fOut, " "

	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0

	filteredPuts ("Processing image in 'sprites' mode")
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	filteredPuts ("+ Writing metasprites info to " & tsMapFn)
	
	tMapsIndex = 0
	
	For y = y0 To y1 Step vSize
		For x = x0 To x1 Step hSize
			ya = y
			tMapI = 0
			
			If shouldClose  Or wProc <> 1 or hProc <> 1 Then 
				Print #fout, "// Cell #" & tMapsIndex & " cut from (" & x & ", " & y & ") to (" & (x + hSize - 1) & ", " & (y + vSize - 1) & ")"
			Else
				Print #fout, "// Cell cut from (" & x & ", " & y & ") to (" & (x + hSize - 1) & ", " & (y + vSize - 1) & ")"
			End If 
			Print #fout, ""
			
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					ctt = ((ya-y0)\8)*wProc*wMeta + (xa-x0)\8
					' Sprites use the sencond palette:
					smsExtractPatternFromWithPal img, xa, ya, pal(), 1, pattern ()
					' Only do stuff when pattern is not all zeroes
					If patternIsAllZeroes (pattern ()) And Not noskipempty Then
						cellActive (tMapI) = 0
					Else
						patternS = patternToString (pattern ())
						ct = FindPatternInPoolAndAdd (patternS, wasnew)
						If wasnew Then copyArrayToMainBin pattern ()
						cellActive (tMapI) = -1
						tMap (tMapI) = ct + offset
					End If
					tMapI = tMapI + 1
					xa = xa + 8
				Next xx
				ya = ya + 8
			Next yy
			
			' Write metaspriteset
			If shouldClose Or wProc <> 1 or hProc <> 1 Then
				If flipped Or mirrored Then 
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_a [] = {"
				Else
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & " [] = {"
				End If
			Else
				If flipped Or mirrored Then 
					Print #fOut, "const unsigned char " & label & "_a [] = {"
				Else
					Print #fOut, "const unsigned char " & label & " [] = {"
				End If
			End If
			If simplemeta Then Print #fOut, "	";: Print #fOut, "0x" & lcase (hex (sprOrgX, 2)) & ", 0x" & lcase (hex (sprOrgY, 2)) & ", 0x" & lcase (hex (wMeta, 2)) & ", 0x" & lcase (hex (hMeta, 2)) & ","
			tMapI = 0
			ya = sprOrgY
			For yy = 1 To hMeta
				xa = sprOrgX
				Print #fOut, "	";
				For xx = 1 To wMeta				
					If simplemeta Or supersimplemeta Then
						If cellActive (tMapI) Then
							Print #fOut, "0x" & Lcase (Hex (tMap (tMapI), 2));
						Else
							Print #fOut, "0xff";
						End If
						If yy < hMeta Or xx < wMeta Then Print #fOut, ", ";
					Else
						If cellActive (tMapI) then
							Print #fOut, "0x" & Lcase (Hex (xa, 2)) & ", 0x" & Lcase (Hex (ya, 2)) & ", 0x" & Lcase (Hex (tMap (tMapI), 2));
							Print #fOut, ", ";
						Else						
							Print #fOut, "                        ";
						End If
					End If
					tMapI = tMapI + 1
					xa = xa + 8
				Next xx
				ya = ya + 8
				Print #fOut, ""
			Next yy
			If Not (simplemeta Or supersimplemeta) Then
				Print #fOut, "	";
				Print #fOut, "0x" & Hex (128, 2)
			End If
			Print #fOut, "};"	

			' Generate flipped / mirrored

			If flipped Or mirrored Then
				ya = y
				tMapI = 0
				
				For yy = 1 To hMeta
					xa = x + hSize - 8 '''''' <- CHEEEESE
					For xx = 1 To wMeta
						ctt = ((ya-y0)\8)*wProc*wMeta + (xa-x0)\8
						' Sprites use the sencond palette:
						smsExtractHFlippedPatternFromWithPal img, xa, ya, pal(), 1, pattern ()
						' Only do stuff when pattern is not all zeroes
						If patternIsAllZeroes (pattern ()) And Not noskipempty Then
							cellActive (tMapI) = 0
						Else
							patternS = patternToString (pattern ())
							ct = FindPatternInPoolAndAdd (patternS, wasnew)
							If wasnew Then copyArrayToMainBin pattern ()
							cellActive (tMapI) = -1
							tMap (tMapI) = ct + offset
						End If
						tMapI = tMapI + 1
						xa = xa - 8 '''''' <- CHEEEESE
					Next xx
					ya = ya + 8
				Next yy

				Print #fOut, "// Software horizontally flipped"
				' Write metaspriteset
				If shouldClose Or wProc <> 1 or hProc <> 1 Then
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_b [] = {"
				Else
					Print #fOut, "const unsigned char " & label & "_b [] = {"
				End If
				If simplemeta Then 
					If mirrored Then
						Print #fOut, "	";: Print #fOut, "0x" & lcase (hex (-hSize - sprOrgX, 2)) & ", 0x" & lcase (hex (sprOrgY, 2)) & ", 0x" & lcase (hex (wMeta, 2)) & ", 0x" & lcase (hex (hMeta, 2)) & ","
					Else
						Print #fOut, "	";: Print #fOut, "0x" & lcase (hex (sprOrgX, 2)) & ", 0x" & lcase (hex (sprOrgY, 2)) & ", 0x" & lcase (hex (wMeta, 2)) & ", 0x" & lcase (hex (hMeta, 2)) & ","
					End If
				End if
				tMapI = 0
				ya = sprOrgY
				For yy = 1 To hMeta
					If flipped Then
						xa = sprOrgX
					Else
						xa = -hSize - sprOrgX + metalogicalwidth
					End If
					Print #fOut, "	";
					For xx = 1 To wMeta
						If simplemeta Or supersimplemeta Then
							If cellActive (tMapI) Then
								Print #fOut, "0x" & Lcase (Hex (tMap (tMapI), 2));
							Else
								Print #fOut, "0xff";
							End If
							If yy < hMeta Or xx < wMeta Then Print #fOut, ", ";
						Else
							If cellActive (tMapI) then
								Print #fOut, "0x" & Lcase (Hex (xa, 2)) & ", 0x" & Lcase (Hex (ya, 2)) & ", 0x" & Lcase (Hex (tMap (tMapI), 2));
								Print #fOut, ", ";
							Else						
								Print #fOut, "                        ";
							End If
						End If
						tMapI = tMapI + 1
						xa = xa + 8
					Next xx
					ya = ya + 8
					Print #fOut, ""
				Next yy
				If Not (simplemeta Or supersimplemeta) Then
					Print #fOut, "	";
					Print #fOut, "0x" & Hex (128, 2)
				End If
				Print #fOut, "};"	
			End If

			' Generate upside-down
			If upsideDown Then
				ya = y + vSize - 8
				tMapI = 0
				
				For yy = 1 To hMeta
					xa = x
					For xx = 1 To wMeta
						ctt = ((ya-y0)\8)*wProc*wMeta + (xa-x0)\8
						' Sprites use the sencond palette:
						smsExtractVFlippedPatternFromWithPal img, xa, ya, pal(), 1, pattern ()
						' Only do stuff when pattern is not all zeroes
						If patternIsAllZeroes (pattern ()) And Not noskipempty Then
							cellActive (tMapI) = 0
						Else
							patternS = patternToString (pattern ())
							ct = FindPatternInPoolAndAdd (patternS, wasnew)
							If wasnew Then copyArrayToMainBin pattern ()
							cellActive (tMapI) = -1
							tMap (tMapI) = ct + offset
						End If
						tMapI = tMapI + 1
						xa = xa + 8 
					Next xx
					ya = ya - 8 '''''' <- CHEEEESE
				Next yy

				Print #fOut, "// Software upside down"
				' Write metaspriteset
				If shouldClose  Or wProc <> 1 or hProc <> 1 Then
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_ud [] = {"
				Else
					Print #fOut, "const unsigned char " & label & "_ud [] = {"
				End If
				If simplemeta Then Print #fOut, "	";: Print #fOut, "0x" & lcase (hex (sprOrgX, 2)) & ", 0x" & lcase (hex (sprOrgY, 2)) & ", 0x" & lcase (hex (wMeta, 2)) & ", 0x" & lcase (hex (hMeta, 2)) & ","
				tMapI = 0
				ya = sprOrgY
				For yy = 1 To hMeta
					xa = sprOrgX
					Print #fOut, "	";
					For xx = 1 To wMeta
						If simplemeta Or supersimplemeta Then
							If cellActive (tMapI) Then
								Print #fOut, "0x" & Lcase (Hex (tMap (tMapI), 2));
							Else
								Print #fOut, "0xff";
							End If
							If yy < hMeta Or xx < wMeta Then Print #fOut, ", ";
						Else
							If cellActive (tMapI) then
								Print #fOut, "0x" & Lcase (Hex (xa, 2)) & ", 0x" & Lcase (Hex (ya, 2)) & ", 0x" & Lcase (Hex (tMap (tMapI), 2));
								Print #fOut, ", ";
							Else						
								Print #fOut, "                        ";
							End If
						End If
						tMapI = tMapI + 1
						xa = xa + 8
					Next xx
					ya = ya + 8
					Print #fOut, ""
				Next yy
				If Not (simplemeta Or supersimplemeta) Then
					Print #fOut, "	";
					Print #fOut, "0x" & Hex (128, 2)
				End If
				Print #fOut, "};"	
			End If

			tMapsIndex = tMapsIndex + 1
			If max <> -1 And tMapsIndex = max Then Exit For
		Next x
		If wMeta = 2 And hMeta = 2 And tsMapFn <> "" Then Print #fOut, ""
		If max <> -1 And tMapsIndex = max Then Exit For
	Next y
	
	If shouldClose Then
		Print #fOut, "// " & cPoolIndex & " patterns used from #" & offset & ", Next = " & cPoolIndex + offset & "."
		Print #fOut, ""
	End if
	
	filteredPuts ("+ " & tMapsIndex & " cells extracted using " & cPoolIndex & " patterns.")
	If shouldClose Then Close #fOut
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], sprites mode. " & cPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " metasprites. Next = " & cPoolIndex + offset)
	nextPattern = cPoolIndex + offset	

	metaSpriteCounter = tMapsIndex		
End Sub

Sub nesDoSprites16 (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, sprOrgX As Integer, sprOrgY As Integer, fOut As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, xx, yy, xa, ya, wp, found, i, j
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pattern (31), tMap (255)
	Dim As String patternS
	Dim As Integer wasNew
	Dim As uByte metapals (255)
	Dim As Integer shouldClose

	shouldClose = 0
	
	hSize = wMeta * 8
	vSize = hMeta * 16
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	If fOut = -1 Then
		shouldClose = -1
		fOut = FreeFile
		Open tsMapFn For Output As #fOut	

		' Write some headers to output tsmap file
		Print #fOut, "// Metaspriteset generated by mkts [nes mode]"
		Print #fOut, "// Copyleft 2017 The Mojon Twins"
		Print #fOut, " "
	End If
		
	If flipped Then Print #fOut, "// Each cell: A = normal, B = flipped": Print #fOut, " "
	If mirrored Then Print #fOut, "// Each cell: A = normal, B = mirrored": Print #fOut, " "
	
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'sprites16' mode")
	filteredPuts ("Meta = " & wMeta & "x" & hMeta & ", Proc = " & wProc & "x" & hProc)
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	filteredPuts ("+ Writing metasprites info to " & tsMapFn)
	
	tMapsIndex = 0
	
	For y = y0 To y1 Step vSize
		For x = x0 To x1 Step hSize
			ya = y
			tMapI = 0
			
			If shouldClose Then 
				Print #fout, "// Cell #" & tMapsIndex & " cut from (" & x & ", " & y & ") to (" & (x + hSize - 1) & ", " & (y + vSize - 1) & ")"
			Else
				Print #fout, "// Cell cut from (" & x & ", " & y & ") to (" & (x + hSize - 1) & ", " & (y + vSize - 1) & ")"
			End If 
			Print #fout, ""
			
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					''ctt = (xa \ 8) + (ya \ 8) * (x1 + 1) \ 8
					ctt = (x \ hSize) + (y \ vSize) * wProc
					wp = nesFindWhichPal (img, xa, ya, pal (), ctt, 8, 16)
					nesExtract2PatternsFrom img, xa, ya, pal (), pattern ()
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
			If shouldClose Then
				If flipped Or mirrored Then 
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_a [] = {"
				Else
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & " [] = {"
				End If
			Else
				If flipped Or mirrored Then 
					Print #fOut, "const unsigned char " & label & "_a [] = {"
				Else
					Print #fOut, "const unsigned char " & label & " [] = {"
				End If
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
			If flipped Or mirrored Then
				If shouldClose Then
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_b [] = {"
				Else
					Print #fOut, "const unsigned char " & label & "_b [] = {"
				End If
				ya = sprOrgY
				For yy = 1 To hMeta
					If flipped Then
						xa = sprOrgX
					Else
						xa = -hSize - sprOrgX + metalogicalwidth
					End If
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
	
	If shouldClose Then
		Print #fOut, "// " & cPoolIndex & " patterns used from #" & offset & ", Next = " & cPoolIndex + offset & "."
		Print #fOut, ""
	End if
	
	filteredPuts ("+ " & tMapsIndex & " cells extracted using " & cPoolIndex & " patterns.")
	If shouldClose Then Close #fOut
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], sprites16 mode. " & cPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " metasprites. Next = " & cPoolIndex + offset)
	nextPattern = cPoolIndex + offset

	metaSpriteCounter = tMapsIndex
End Sub

Sub gbDoSprites16 (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, sprOrgX As Integer, sprOrgY As Integer, fOut As Integer)
	Dim As Integer x0, y0, x1, y1, x, y, ct, ctt, xx, yy, xa, ya, wp, found, i, j
	Dim As Integer hSize, vSize, tMapI
	Dim As uByte pattern (31), tMap (255)
	Dim As String patternS
	Dim As Integer wasNew
	Dim As uByte metapals (255)
	Dim As Integer shouldClose

	shouldClose = 0
	
	hSize = wMeta * 8
	vSize = hMeta * 16
	
	' I know I can calculate ct, ctt from the info I have
	' But I find it better to calculate it this way as a way to find bugs.
	
	If fOut = -1 Then
		shouldClose = -1
		fOut = FreeFile
		Open tsMapFn For Output As #fOut	

		' Write some headers to output tsmap file
		Print #fOut, "// Metaspriteset generated by mkts [gb mode]"
		Print #fOut, "// Copyleft 2017 The Mojon Twins"
		Print #fOut, " "
	End If
		
	If flipped Then Print #fOut, "// Each cell: A = normal, B = flipped": Print #fOut, " "
	If mirrored Then Print #fOut, "// Each cell: A = normal, B = mirrored": Print #fOut, " "
	
	x0 = xOrg * 8
	y0 = yOrg * 8
	x1 = x0 + wProc * hSize - 1
	y1 = y0 + hProc * vSize - 1
	ct = 0: ctt = 0
	
	filteredPuts ("Processing image in 'sprites16' mode")
	filteredPuts ("Meta = " & wMeta & "x" & hMeta & ", Proc = " & wProc & "x" & hProc)
	filteredPuts ("Cut rect proc = " & x0 & ", " & y0 & " - " & x1 & ", " & y1 & ".")
	filteredPuts ("+ Writing metasprites info to " & tsMapFn)
	
	tMapsIndex = 0
	
	For y = y0 To y1 Step vSize
		For x = x0 To x1 Step hSize
			ya = y
			tMapI = 0
			
			If shouldClose Then 
				Print #fout, "// Cell #" & tMapsIndex & " cut from (" & x & ", " & y & ") to (" & (x + hSize - 1) & ", " & (y + vSize - 1) & ")"
			Else
				Print #fout, "// Cell cut from (" & x & ", " & y & ") to (" & (x + hSize - 1) & ", " & (y + vSize - 1) & ")"
			End If 
			Print #fout, ""
			
			For yy = 1 To hMeta
				xa = x
				For xx = 1 To wMeta
					''ctt = (xa \ 8) + (ya \ 8) * (x1 + 1) \ 8
					ctt = (x \ hSize) + (y \ vSize) * wProc
					nesExtract2PatternsFrom img, xa, ya, pal (), pattern ()
					patternS = patternToString (pattern ())
					ct = find2PatternsInPoolAndAdd (patternS, wasnew)
					If wasnew Then copyArrayToMainBin pattern ()
					tMap (tMapI) = ct + offset + 1
					metapals (tMapI) = 0
					tMapI = tMapI + 1
					xa = xa + 8
				Next xx
				ya = ya + 16
			Next yy
			
			' Write metaspriteset
			If shouldClose Then
				If flipped Or mirrored Then 
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_a [] = {"
				Else
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & " [] = {"
				End If
			Else
				If flipped Or mirrored Then 
					Print #fOut, "const unsigned char " & label & "_a [] = {"
				Else
					Print #fOut, "const unsigned char " & label & " [] = {"
				End If
			End If
			tMapI = 0
			ya = sprOrgY
			For yy = 1 To hMeta
				If flipped Then
						xa = sprOrgX
					Else
						xa = -hSize - sprOrgX
					End If
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
			If flipped Or mirrored Then
				If shouldClose Then
					Print #fOut, "const unsigned char " & label & "_" & Hex (tMapsIndex, 2) & "_b [] = {"
				Else
					Print #fOut, "const unsigned char " & label & "_b [] = {"
				End If
				ya = sprOrgY
				For yy = 1 To hMeta
					If flipped then
						xa = sprOrgX
					Else
						xa = -hSize - sprOrgX + metalogicalwidth
					End If
					Print #fOut, "	";
					For xx = 1 To wMeta
						' Flip horz order, don't forget!
						tMapI = (wMeta - xx) + (yy - 1) * wMeta
						' X, Y, T, A
						Print #fOut, "0x" & Lcase (Hex (xa, 2)) & ", 0x" & Lcase (Hex (ya, 2)) & ", 0x" & Lcase (Hex (tMap (tMapI), 2)) & ", 0x" & Lcase (Hex (32 Or metapals (tMapI), 2));
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
	
	If shouldClose Then
		Print #fOut, "// " & cPoolIndex & " patterns used from #" & offset & ", Next = " & cPoolIndex + offset & "."
		Print #fOut, ""
	End if
	
	filteredPuts ("+ " & tMapsIndex & " cells extracted using " & cPoolIndex & " patterns.")
	If shouldClose Then Close #fOut
	
	If silent Then Puts ("mkts [" & sclpGetValue ("platform") & "], sprites16 mode. " & cPoolIndex & " patterns extracted from " & tMapsIndex & " " & wMeta & "x" & hMeta & " metasprites. Next = " & cPoolIndex + offset)
	nextPattern = cPoolIndex + offset

	metaSpriteCounter = tMapsIndex
End Sub

Sub smsDoSprites16 (img As Any Ptr, pal () As Integer, xOrg As Integer, yOrg As Integer, wProc As Integer, hProc As Integer, wMeta As Integer, hMeta As Integer, tsMapFn As String, label As String, offset As Integer, max As Integer, sprOrgX As Integer, sprOrgY As Integer, fOut As Integer)
End Sub

Sub doScripted (inFileName As String, outFileName As String, platform As Integer) 
	Dim As Integer fIn, i, fMapFile, w, h
	Dim As String inLine
	Dim As String mapFile, label
	Dim As Integer charOffset
	Dim As String tokens (127), cmd
	Dim As Integer pal (15)
	Dim As Any Ptr img, palImg
	Dim As Integer max
	Dim As Integer xOrg, yOrg, wProc, hProc, wMeta, hMeta, sprxorg, spryorg, wA, hA
	Dim As Integer byteMark

	If platform = PLATFORM_SG1000 Then Puts "The SG-1000 platform is not supported in scripted mode, sorry": End

	' Some defaults, so things don't crash if users are stupid:
	charOffset = 0
	fMapFile = -1
	label = "stupid"

	metaSpriteCounter = 0
	noskipempty = 0
	aseprite = 0
	deinterlaced = 0
	outputpallist = 0
	patternsLimit = 0
	ignoreMapFile = 0
	
	Puts ("Processing script " & inFileName & ", platform = " & platform)
	fIn = FreeFile
	Open inFileName For Input As #fIn
	While Not Eof (fIn)
		Line Input #fIn, inLine
		parseTokenizeString inLine, tokens (), ",;" & Chr (9), "#"
		cmd = uCase (tokens (0))

		If debug Then 
			Puts "Line > " & inLine
			i = 0: Do 
				Puts tokens (i): i = i + 1
			Loop Until tokens (i) = ""
			Puts "cPoolIndex = " & cPoolIndex & ", NoSkipEmpty = " & noskipempty
		End If

		Select Case cmd
			Case "RESETPOOL"
				Puts ("Resetting patterns pool!")
				cPoolIndex = 0
			Case "PATTERNSLIMIT":
				Puts ("Patters limit = " & tokens (1))
				If tokens (1) = "OFF" Then patternsLimit = 0 Else patternsLimit = Val (tokens (1))
			Case "OUTPUTPALLIST":
				outputpallist = -1
			Case "NOPALLIST":
				outputpallist = 0
			Case "ASEPRITE":
				Puts "ASEPRITE MODE ON"
				aseprite = -1
			Case "NORMAL":
				Puts "NORMAL MODE ON"
				aseprite = 0
			Case "NOSKIPEMPTY"
				noskipempty = -1
			Case "SKIPEMPTY"
				noskipempty = 0
			Case "ALLBG"
				allbg = -1
			Case "NOALLBG"
				allbg = 0
			Case "MAPFILE":
				mapFile = tokens (1)
				fMapFile = FreeFile
				Open mapFile For Output As #fMapFile
				Print #fMapFile, "// Patterns, tiles, metatiles, and maybe dragons. Generated by mkts"
				Print #fMapFile, "// Copyleft 2017 The Mojon Twins"
				Print #fMapFile, ""
			Case "IGNOREMAPFILE":
				ignoreMapFile = -1
			Case "USEMAPFILE":
				ignoreMapFile = 0
			Case "LABEL":
				label = tokens (1)
				If tokens (2) <> "NORESET" THEN metaSpriteCounter = 0
			Case "CHAROFFSET":
				charOffset = Val (tokens (1))
			Case "OPEN":
				img = imageread_png (tokens (1),32 )
				If ImageInfo (img, w, h, , , , ) Then
					' Error!
				End If
			Case "PALS":
				Select Case platform
					Case PLATFORM_GB:
						If Len (tokens (1)) <> 4 Then 
							puts ("**ERROR** Bad palette string in " & inLine)
							End
						End If
						For i = 1 To 4
							pal (i - 1) = Val (Mid (tokens (1), i, 1))
						Next i
					Case PLATFORM_NES:
						palImg = imageread_png (tokens (1), 32)
						nesFindStraightPalette palImg, pal ()
					Case PLATFORM_SMS:
						palImg = imageread_png (tokens (1), 32)
						smsFindStraightPalette palImg, pal ()
				End Select
			Case "METASPRITE":
				outputPatterns = 0

				xOrg = Val (tokens (1))
				yOrg = Val (tokens (2))
				wMeta = Val (tokens (3))
				hMeta = Val (tokens (4))
				sprxorg = Val (tokens (5))
				spryorg = Val (tokens (6))

				' Modifier
				If parserFindTokenInTokens("FLIPPED", tokens (), "ucase") Then flipped = -1 Else flipped = 0
				If parserFindTokenInTokens("MIRRORED", tokens (), "ucase") Then 
					mirrored = -1 
					metalogicalwidth = Val (tokens (parserGetTokenIndex ("MIRRORED", tokens (), "ucase") + 1))
				Else 
					mirrored = 0
				End If
				If parserFindTokenInTokens("UPSIDEDOWN", tokens (), "ucase") Then upsideDown = -1 Else upsideDown = 0

				Select Case platform
					Case PLATFORM_NES:
						nesDoSprites img, pal (), xOrg, yOrg, 1, 1, wMeta, hMeta, "-", label & "_" &  (Hex (metaSpriteCounter, 2)), charOffset, 1, sprxorg, spryorg, fMapFile
					Case PLATFORM_GB:
						gbDoSprites img, pal (), xOrg, yOrg, 1, 1, wMeta, hMeta, "-", label & "_" &  (Hex (metaSpriteCounter, 2)), charOffset, 1, sprxorg, spryorg, fMapFile
					Case PLATFORM_SMS:
						smsDoSprites img, pal (), xOrg, yOrg, 1, 1, wMeta, hMeta, "-", label & "_" &  (Hex (metaSpriteCounter, 2)), charOffset, 1, sprxorg, spryorg, fMapFile						
				End Select
				
			Case "CHARSET":
				outputPatterns = 0

				xOrg = Val (tokens (1))
				yOrg = Val (tokens (2))
				wProc = Val (tokens (3))
				hProc = Val (tokens (4))
				max = Val (tokens (5)): If max = 0 Then max = -1

				Select Case platform
					Case PLATFORM_NES:
						nesDoChars img, pal (), xOrg, yOrg, wProc, hProc, max
					Case PLATFORM_GB:
						gbDoChars img, pal (), xOrg, yOrg, wProc, hProc, max
					Case PLATFORM_SMS:
						smsDoChars img, pal (), xOrg, yOrg, wProc, hProc, max
				End Select

				' 'If max <> -1 Then charOffset = charOffset + max Else charOffset = charOffset + wProc * hProc

			Case "TILESROW2X2"
				outputPatterns = 0

				xOrg = Val (tokens (1))
				yOrg = Val (tokens (2))
				wProc = Val (tokens (3))
				hProc = Val (tokens (4))
				max = Val (tokens (5)): If max = 0 Then max = -1

				Select Case platform
					Case PLATFORM_NES:
						nesDoTiles img, pal (), xOrg, yOrg, wProc, hProc, 2, 2, "-", label, max, fMapFile
					Case PLATFORM_GB:
						gbDoTiles img, pal (), xOrg, yOrg, wProc, hProc, 2, 2, "-", label, max, fMapFile
					Case PLATFORM_SMS:
						smsDoTiles img, pal (), xOrg, yOrg, wProc, hProc, 2, 2, "-", label, max, fMapFile
				End Select

			Case "MAPPED"
				outputPatterns = 0

				xOrg = Val (tokens (1))
				yOrg = Val (tokens (2))
				wMeta = Val (tokens (3))
				hMeta = Val (tokens (4))
				wProc = Val (tokens (5))
				hProc = Val (tokens (6))
				max = Val (tokens (7)): If max = 0 Then max = -1
				deinterlaced = parserFindTokenInTokens("DEINTERLACED", tokens (), "ucase") 

				Select Case platform
					Case PLATFORM_NES:
						nesDoTmaps img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, "-", label, charOffset, max, fMapFile
					Case PLATFORM_GB:
						gbDoTmaps img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, "-", label, charOffset, max, fMapFile
					Case PLATFORM_SMS:
						smsDoTmaps img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, "-", label, charOffset, max, fMapFile
				End Select

			Case "METASPRITESET"
				outputPatterns = 0
				'metaSpriteCounter = 0

				xOrg = Val (tokens (1))
				yOrg = Val (tokens (2))
				wMeta = Val (tokens (3))
				hMeta = Val (tokens (4))
				wProc = Val (tokens (5))
				hProc = Val (tokens (6))
				sprxorg = Val (tokens (7))
				spryorg = Val (tokens (8))
				max = Val (tokens (9)): If max = 0 Then max = -1

				' Modifier
				If parserFindTokenInTokens ("FLIPPED", tokens (), "ucase") Then flipped = -1 Else flipped = 0
				If parserFindTokenInTokens ("MIRRORED", tokens (), "ucase") Then 
					mirrored = -1 
					metalogicalwidth = Val (tokens (parserGetTokenIndex ("MIRRORED", tokens (), "ucase") + 1))
				Else 
					mirrored = 0
				End If
				If parserFindTokenInTokens ("UPSIDEDOWN", tokens (), "ucase") Then upsideDown = -1 Else upsideDown = 0

				Select Case platform
					Case PLATFORM_NES:
						nesDoSprites img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, "-", label, charOffset, max, sprxorg, spryorg, fMapFile
					Case PLATFORM_GB:
						gbDoSprites img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, "-", label, charOffset, max, sprxorg, spryorg, fMapFile
					Case PLATFORM_SMS:
						smsDoSprites img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, "-", label, charOffset, max, sprxorg, spryorg, fMapFile						
				End Select

			Case "TEXTS"
				outputPatterns = 0

				xOrg = Val (tokens (1))
				yOrg = Val (tokens (2))
				wProc = Val (tokens (3))
				hProc = Val (tokens (4))
				max = Val (tokens (5)): If max = 0 Then max = -1

				Select Case platform
					Case PLATFORM_NES:
						nesDoTexts img, pal (), xOrg, yOrg, wProc, hProc, "-", label, charOffset, max, fMapFile
					Case PLATFORM_GB:
						gbDoTexts img, pal (), xOrg, yOrg, wProc, hProc, "-", label, charOffset, max, fMapFile
					Case PLATFORM_SMS:
						smsDoTexts img, pal (), xOrg, yOrg, wProc, hProc, "-", label, charOffset, max, fMapFile
				End Select
			case "FILL"
				byteMark = Val (tokens (1))
				Puts ("Filling with zeroes until bc=" & byteMark)
				While byteMark > mainIdx
					mbWrite (0)
				Wend
		End Select
	Wend
	If fMapFile <> -1 Then Close fMapFile
	Close #fIn

	metaSpriteCounter = tMapsIndex
End Sub

Dim As Integer xOrg, yOrg, w, h, wMeta, hMeta, wProc, hProc, coords (8), sprxorg, spryorg
Dim As Integer i, fOut, offset, ctr, ctrspr, j
Dim As String mandatoryNes (3) = { "in", "out", "pals", "mode" }
Dim As String mandatorySMS (3) = { "in", "out", "pals", "mode" }
Dim As String mandatoryNesPals (3) = { "out", "pals", "mode", "label" }
Dim As String mandatorySMSPals (3) = { "out", "pals", "mode", "label" }
Dim As String mandatorySg1000 (2) = { "in", "out", "mode" }
Dim As String mandatoryGb (4) = { "in", "out", "mode", "pals", "label" }
Dim As Integer pal (31)
Dim As Any Ptr img
Dim As Integer max
Dim As Integer platform = PLATFORM_NES

' Initialize patterns pool
cPoolIndex = 0

' DO!

sclpParseAttrs

Select Case sclpGetValue ("platform")
	Case "sg1000": 
		platform = PLATFORM_SG1000
	Case "gb":
		platform = PLATFORM_GB
	Case "sms":
		platform = PLATFORM_SMS
End Select


If sclpGetValue ("silent") <> "" Then
	silent = -1
Else
	silent = 0
End If

filteredPuts "mkts v0.8.0 20180205"
filteredPuts "Cuts & imports tilesets from a png."
filteredPuts "Copyleft 2015 - 2018 The Mojon Twins"
filteredPuts ""

' Setup screen

screenres 640, 480, 32, , -1

' Parse & read

debug = (sclpGetValue ("debug") <> "")
If debug Then Puts "[DEBUG MODE ON]"

If sclpGetValue ("mode") = "scripted" Then 
	mainIdx = 0
	doScripted sclpGetValue ("in"), sclpGetValue ("out"), platform
Else 

	Select Case platform
		Case PLATFORM_SG1000:
			If Not sclpCheck (mandatorySg1000 ()) Then
				usage
				End
			End If
		Case PLATFORM_GB:
			If Not sclpCheck (mandatoryGb ()) Then
				usage
				End
			End If
		Case PLATFORM_NES:
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
		Case PLATFORM_SMS:
			If sclpGetValue ("mode") = "pals" Then
				If Not sclpCheck (mandatorySMSPals ()) Then
					usage
					End
				End If
			Else
				If Not sclpCheck (mandatorySMS ()) Then
					usage
					End
				End If
			End If 
	End Select

	' Set pal

	Select Case platform
		Case PLATFORM_GB:
			filteredPuts ("building GB palette")
			filteredPuts ("+ Pal string is " & sclpGetValue ("pals"))
			If Len (sclpGetValue ("pals")) <> 4 Then 
				puts ("**ERROR** Bad palette string.")
				puts ("")
				puts ("Run mkts.exe with no params for help.")
				puts ("")
				End
			End If
			For i = 1 To 4
				pal (i - 1) = Val (Mid (sclpGetValue ("pals"), i, 1))
			Next i
		Case PLATFORM_SG1000:
			' Ta daaaah!
		Case PLATFORM_SMS:
			filteredPuts ("Reading palette")
			filteredPuts ("+ Pal file is " & sclpGetValue ("pals"))
			img = imageread_png (sclpGetValue ("pals"), 32)
			smsFindStraightPalette img, pal ()
		Case PLATFORM_NES:
			filteredPuts ("Reading palette")
			filteredPuts ("+ Pal file is " & sclpGetValue ("pals"))
			img = imageread_png (sclpGetValue ("pals"), 32)
			nesFindStraightPalette img, pal ()
	End Select

	' load input

	filteredPuts ("Input file is " & sclpGetValue ("in"))
	img = imageread_png (sclpGetValue ("in"), 32)
	If ImageInfo (img, w, h, , , , ) Then
		' Error!
	End If

	' Set xorg, yorg

	If sclpGetValue ("offset") <> "" Then
		parseCoordinatesString sclpGetValue ("offset"), coords ()
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
		parseCoordinatesString sclpGetValue ("sprorg"), coords ()
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
		parseCoordinatesString sclpGetValue ("size"), coords ()
		wProc = coords (0): hProc = coords (1)
	End If

	If wProc = 0 Or hProc = 0 Then
		wProc = w \ 8
		hProc = h \ 8
	End If
	filteredPuts ("+ rectangle size = " & wProc & "x" & hProc & ". ")

	' Set meta size
	 
	If sclpGetValue ("mode") = "tiles" Or sclpGetValue ("mode") = "coltiles" Or sclpGetValue ("mode") = "mapped" Or sclpGetValue ("mode") = "sprites" Or sclpGetValue ("mode") = "sprites16" Then
		parseCoordinatesString sclpGetValue ("metasize"), coords ()
		wMeta = coords (0): hMeta = coords (1)
		If wMeta = 0 Or hMeta = 0 Then
			wMeta = 2: hMeta = 2
		End If
		filteredPuts ("+ metatiles size = " & wMeta & "x" & hMeta & ". ")
	End If

	If sclpGetValue ("mode") = "coltiles" Or sclpGetValue ("mode") = "tiles" Or sclpGetValue ("mode") = "mapped" Or sclpGetValue ("mode") = "sprites" Or sclpGetValue ("mode") = "text" Or sclpGetValue ("mode") = "sprites16" Then
		If sclpGetValue ("tsmap") = "" Then
			puts ("**ERROR** mode=tiles, coltiles, mapped, sprites, sprites16, text need tsmap")
			puts ("")
			puts ("Run mkts.exe with no params for help.")
			puts ("")
			End
		End If
		If sclpGetValue ("label") = "" Then
			puts ("**ERROR** mode=tiles, coltiles, mapped, sprites, sprites16, text need tsmap")
			puts ("")
			puts ("Run mkts.exe with no params for help.")
			puts ("")
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
	upsideDown = (sclpGetValue ("upsidedown") <> "")
	blackiszero = (sclpGetValue ("blackiszero") <> "")
	columns = (sclpGetValue ("columns") <> "")
	simplemeta = (sclpGetValue ("simple") <> "")
	supersimplemeta = (sclpGetValue ("supersimple") <> "")
	aseprite = (sclpGetValue ("aseprite") <> "")
	noskipempty = (sclpGetValue ("noskipempty") <> "")
	allbg = (sclpGetValue ("allbg") <> "")
	deinterlaced = (sclpGetValue ("deinterlaced") <> "")
	outputPalList = (sclpGetValue ("outputpallist") <> "")
	ignoreMapFile = (sclpGetValue ("ignoremapfile") <> "")
	
	' Do

	mainIdx = 0

	Select Case platform

		Case PLATFORM_SMS:
			Select Case sclpGetValue ("mode")
				Case "chars":
					smsDoChars img, pal (), xOrg, yOrg, wProc, hProc, max
				Case "text":
					smsDoTexts img, pal (), xOrg, yOrg, wProc, hProc, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, -1
				Case "tiles":
					smsDoTiles img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), max, -1
				Case "coltiles":
					smsDoColTiles img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), max, -1
				Case "mapped":
					smsDoTmaps img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, -1
				Case "sprites":
					smsDoSprites img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, sprxorg, spryorg, -1
				Case "sprites16":
					smsDoSprites16 img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, sprxorg, spryorg, -1
				Case "pals":
					smsDoPals pal (), sclpGetValue ("out"), sclpGetValue ("pals"), sclpGetValue ("label")
				Case else:
					Puts ("mode specified not available for this platform")
					End
			End Select

		Case PLATFORM_NES:
			Select Case sclpGetValue ("mode")
				Case "chars":
					nesDoChars img, pal (), xOrg, yOrg, wProc, hProc, max
				Case "text":
					nesDoTexts img, pal (), xOrg, yOrg, wProc, hProc, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, -1
				Case "tiles":
					nesDoTiles img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), max, -1
				Case "coltiles":
					nesDoColTiles img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), max, -1
				Case "mapped":
					nesDoTmaps img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, -1
				Case "sprites":
					nesDoSprites img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, sprxorg, spryorg, -1
				Case "sprites16":
					nesDoSprites16 img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, sprxorg, spryorg, -1
				Case "pals":
					nesDoPals pal (), sclpGetValue ("out"), sclpGetValue ("pals"), sclpGetValue ("label")
				Case else:
					Puts ("mode specified not available for this platform")
					End
			End Select

		Case PLATFORM_GB:
			Select Case sclpGetValue ("mode")
				Case "chars":
					gbDoChars img, pal (), xOrg, yOrg, wProc, hProc, max
				Case "text":
					gbDoTexts img, pal (), xOrg, yOrg, wProc, hProc, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, -1
				Case "tiles":
					gbDoTiles img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), max, -1
				Case "coltiles":
					gbDoColTiles img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), max, -1			
				Case "mapped":
					gbDoTmaps img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, -1
				Case "sprites":
					gbDoSprites img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, sprxorg, spryorg, -1
				Case "sprites16":
					gbDoSprites16 img, pal (), xOrg, yOrg, wProc, hProc, wMeta, hMeta, sclpGetValue ("tsmap"), sclpGetValue ("label"), offset, max, sprxorg, spryorg, -1
				Case else:
					Puts ("mode specified not available for this platform")
					End
			End Select

		Case PLATFORM_SG1000:
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

	End Select
End If

Select Case platform

	Case PLATFORM_SG1000:
		' Write binary or code depending on filename extension
		If sclpGetValue ("out") <> "bin" Then 
			filteredPuts ("Writing PGT and CGT to " & sclpGetValue ("out"))
			fOut = FreeFile
			Open sclpGetValue ("out") For Output as #fOut
			Print #fOut, "// Charset generated by mkts [sg1000 mode]"
			Print #fOut, "// Copyleft 2017 The Mojon Twins"
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

	Case PLATFORM_NES, PLATFORM_SMS:
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

	Case PLATFORM_GB:
		filteredPuts ("Writing data as C code to " & sclpGetValue ("out"))
		Kill sclpGetValue ("out")
		fOut = FreeFile
		Open sclpGetValue ("out") For Output as #fOut
		Print #fOut, "// Pattern data generated by mkts [gb mode]"
		Print #fOut, "// Copyleft 2017 The Mojon Twins"
		Print #fOut, ""
		Print #fOut, "const unsigned char " & sclpGetValue ("label") & " [] = {"
		ctr = 0: Print #fOut, "	";
		For i = 0 To mainIdx - 1
			Print #fOut, "0x" & Lcase (Hex (mainBin (i), 2));
			If i < mainIdx - 1 Then Print #fOut, ", ";
			ctr = (ctr + 1 And 7): If ctr = 0 And i < mainIdx - 1 Then Print #fOut, "": Print #fOut, "	";
		Next i
		Print #fOut, ""
		Print #fOut, "}"
		Print #fOut, ""
		Close #fOut

End Select
