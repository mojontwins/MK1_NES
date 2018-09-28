Const SCR_W = 16
Const SCR_H = 12

Const STATE_REP = 1
Const STATE_STRING = 2

Sub usage
	Print "Usage:"
	Print 
	Print "$ rle53mapMK1.exe in.map out.h w h tlock prefix [offset] [scrsizes] [nodecos] [bin]"
End Sub

Function even (i As Integer) As Integer 
	Return ((i And 1) = 0)
End Function

Function inCommand (a As String) As Integer
	Dim As Integer i
	i = 1
	While (Command (i) <> "")
		If Command (i) = a Then Return -1
		i = i + 1
	Wend
	Return 0
End Function

Dim As Integer x, y, xx, yy, nPant, mapW, mapH, i, j, dat, f, fBin, mapsize, decosize, scrsize, founddecos
Dim As Integer printscrsizes, nodecos, offset, binmode
Dim As Integer mapWtiles, mapPants, tLock, locksI, state, tileStrI, decoT, decoCt, YXct
Dim As Integer counter, totalBytes, value
Dim As String o, prefix
Dim As uByte d, dp
'Redim As uByte m (0, 0)
'Redim As uByte decos (0, 0), decosI (0)
'Redim As uByte mOut (0, 0), mOutI (0)
Dim As uByte locks (63), tilestr (32)

' Sorry, but this is acting weird
Dim As uByte m (127, SCR_W * SCR_H - 1)
Dim As uByte decos (127, 127)
Dim As uByte decosYX (127, 127), YX (127)
Dim As uByte decosO (127, 127)
Dim As uByte decosI (127)
Dim As uByte decosOI (127)
Dim As uByte mOut (127, SCR_W * SCR_H - 1)
Dim As uByte mOutI (127)
Dim As String cMapAmalgam (127)
Dim As Integer scrSizes (127)
Dim As Integer scrMaps (127)
Dim As Integer screensum, n, cMapI, realPant, ctr
Dim As Integer scrOffsets (127)

Print "MK1 v1.0 rle53mapMK1 ~ ";

If Command (6) = "" Then usage: End

mapW = Val (Command (3))
mapH = Val (Command (4))
mapWtiles = mapW * SCR_W
mapPants = mapW * mapH
tLock = Val (Command (5))
prefix = Command (6)

'printscrsizes = (Command (7) = "printscrsizes" Or Command (8) = "printscrsizes")
'nodecos = (Command (7) = "nodecos" Or Command (8) = "nodecos")
printscrsizes = inCommand ("scrsizes")
nodecos = inCommand ("nodecos")
binmode = inCommand ("bin")

If binmode Then nodecos = -1

For i = 7 To 9
	If Val (Command (i)) Then offset = Val (Command (i))
Next i
If offset Then Print "Offset " & offset & " ~ ";

' Read map to big array
'Redim m (mapPants - 1, SCR_W * SCR_H - 1)
' Max 64 decos as per screen
'Redim decos (mapPants - 1, 127)
'Redim decosI (mapPants - 1)
'Redim mOut (mapPants - 1, SCR_W * SCR_H - 1)
'Redim mOutI (mapPants - 1)

' Reset indexes (not needed, but good practice).
For i = 0 To mapPants - 1
	decosI (i) = 0
	decosOI (i) = 0
	mOutI (i) = 0
	cMapAmalgam (i) = ""
	scrOffsets (i) = 0
Next i
locksI = 0

Print "Reading MAP (" & (maph*SCR_H*mapw*SCR_W) & " bytes) ~ ";

f = Freefile
Open Command (1) For Binary as #f
i = 0: dp = 0

founddecos = 0
decosize = 0
While Not Eof (f)
	' Read from file
	Get #f, , d
	' Offset
	d = d - offset
	' Screen coordinates
	xx = (i \ SCR_W) Mod  mapW
	yy = i \ (SCR_W * SCR_H * mapW)
	' Tile coordinates
	x = i Mod SCR_W
	y = (i \ mapWtiles) Mod SCR_H
	' screen number
	nPant = xx + yy * mapW
	' Is d a decoration' 
	If d >= 32 Then
		' Write to decos
		If Not founddecos Then 
			founddecos = -1
			Print "Found decos ~ ";
		End If
		decosYX (nPant, decosI (nPant)) = y * 16 + x
		decos (nPant, decosI (nPant)) = d
		decosI (nPant) = decosI (nPant) + 1
		' Reset to previous (so there's more repetitions)
		d = dp
	End If
	' Is d a lock' 
	If d = tLock Then
		locks (locksI) = nPant: locksI = locksI + 1
		locks (locksI) = y * 16 + x: locksI = locksI + 1
	End if	
	' Write to big array
	m (nPant, x + y * SCR_W) = d
	' Previous one
	dp = d
	' Counter
	i = i + 1
Wend
Close #f

' Process screens

Print "Compressing ~ ";
totalBytes = 0
For nPant = 0 To mapPants - 1
	d = m (nPant, 0): n = 1: cMapI = 0
	cMapAmalgam (nPant) = ""
	screensum = 0
	For i = 1 To SCR_W*SCR_H-1
		screensum = screensum + m (nPant, i)
		' Different: write, substitute
		If m (nPant, i) <> d Or n = 8 Then
			mOut (nPant, cMapI) = (d And 31) Or ((n - 1) Shl 5)
			'?Hex (mOut (nPant, cMapI), 2) & " ";
			cMapAmalgam (nPant) = cMapAmalgam (nPant) & Hex (mOut (nPant, cMapI), 2)
			cMapI = cMapI + 1
			n = 0
		End If
		d = m (nPant, i): n = n + 1
	Next i
	mOut (nPant, cMapI) = (d And 31) Or ((n - 1) Shl 5)
	'?Hex (mOut (nPant, cMapI), 2) & " ";
	cMapAmalgam (nPant) = cMapAmalgam (nPant) & Hex (mOut (nPant, cMapI), 2)
	cMapI = cMapI + 1

	' Process decos
	If Not nodecos Then
		If decosI (nPant) Then
			For i = 0 To decosI (nPant) - 1
				decoT = decos (nPant, i)
				If decoT <> &Hff Then
					decoCT = 1
					YX (0) = decosYX (nPant, i)
					' Find more:
					For j = i + 1 To decosI (nPant) - 1
						If decos (nPant, i) = decos (nPant, j) Then
							' Found! DESTROY!
							YX (decoCT) = decosYX (nPant, j)
							decoCT = decoCT + 1
							decos (nPant, j) = &Hff
						End If
					Next j
					If decoCT = 1 Then
						' T | 128, YX
						decosO (nPant, decosOI (nPant)) = decoT Or 128: decosOI (nPant) = decosOI (nPant) + 1
						decosO (nPant, decosOI (nPant)) = YX (0): decosOI (nPant) = decosOI (nPant) + 1
					Else
						' T N YX YX YX YX...
						decosO (nPant, decosOI (nPant)) = decoT: decosOI (nPant) = decosOI (nPant) + 1
						decosO (nPant, decosOI (nPant)) = decoCT: decosOI (nPant) = decosOI (nPant) + 1
						For j = 0 To decoCT - 1
							decosO (nPant, decosOI (nPant)) = YX (j): decosOI (nPant) = decosOI (nPant) + 1
						Next j
					End If
				End If
			Next i

			If decosOI (nPant) Then
				screensum = screensum + 1
				For i = 0 To decosOI (nPant) - 1
					mOut (nPant, cMapI) = decosO (nPant, i)
					cMapAmalgam (nPant) = cMapAmalgam (nPant) & Hex (mOut (nPant, cMapI), 2)
					cMapI = cMapI + 1
					decosize = decosize + 1
				Next i
			End If

		End If

		mOut (nPant, cMapI) = 0
		cMapAmalgam (nPant) = cMapAmalgam (nPant) & Hex (mOut (nPant, cMapI), 2)
		cMapI = cMapI + 1
		decosize = decosize + 1
	End If

	realPant = nPant

	' Detect empty screen
	If screensum = 0 Then 
		realPant = 255: cMapI = 0
	Else
		' Search for repeated screens
		For j = 0 To nPant - 1
			If cMapAmalgam (j) = cMapAmalgam (nPant) Then
				realPant = j
				cMapI = 0
				Exit For
			End If
		Next j
	End If

	scrSizes (nPant) = cMapI
	scrMaps (nPant) = realPant '' Fixe here
	scrOffsets (nPant) = totalBytes
	totalBytes = totalBytes + cMapI
Next nPant

' Write output
f = Freefile
Open Command (2) For Output As #f
Print #f, "// rle53mapMK1(TM) map " & Command (2)
Print #f, "// Generated by rle53mapMK1.exe"
Print #f, "// Copyleft 2015, 2017, 2018 The Mojon Twins"
Print #f, ""
Print #f, "// Map Size Is " & mapW & "x" & mapH
Print #f, "// Screen Size Is " & SCR_W & "x" & SCR_H
Print #f, ""

Print "Writing "; 

mapsize = 0

If binmode Then
	Print "[bin] ";

	Print #f, "// [binmode] - This file contains only constants."
	Print #f, "// Actual map index + map binary + locks is in " + Command (2) + ".bin"
	Print #f, ""

	Print #f, "#define MAP_" & prefix & "_MAP_W        " & mapW 
	Print #f, "#define MAP_" & prefix & "_MAP_H        " & mapH
	Print #f, "#define MAP_" & prefix & "_MAP_SIZE     " & mapPants
	Print #f, "#define MAP_" & prefix & "_MAP_BYTES    0x" & Hex (mapPants * 2 + totalBytes, 4)

	Print #f, ""

	fBin = Freefile
	Open Command (2) + ".bin" For Binary As #fBin

	' First write an index, little endian
	For i = 0 To mapPants - 1
		value = scrOffsets (i) + mapPants * 2
		d = value And 255: Put #fBin, , d
		d = value Shr 8: Put #fBin, , d
		mapsize = mapsize + 2
	Next i
End If

Print "~ ";

' Write screens
If Not binmode Then
	Print #f, "// Compressed map structure, screens in RLE53, byte = NNNNNRRR, repeat R times tile #N"
	Print #f, "// Decos follow each screen, Format: [T N YX YX YX YX... (T < 128) | T YX (T >= 128)]"
	
	Print #f, ""
End If

For nPant = 0 To mapPants - 1
	If scrMaps (nPant) = 255 Then
		If Not binmode Then Print #f, "// Screen " & Lcase (Hex (nPant, 2)) & " is empty."
	ElseIf scrSizes (nPant) Then
		If Not binmode Then Print #f, "const unsigned char scr_" & prefix & "_" & Lcase (Hex (nPant, 2)) & " [] = {";		
		For i = 0 To scrSizes (nPant) - 1
			If binmode Then 
				d = mOut (nPant, i)
				Put #fBin, , d
			Else
				Print #f, "0x" & Lcase (Hex (mOut (nPant, i), 2));				
			End If
			If Not binmode Then If i < scrSizes (nPant) - 1 Then Print #f, ", ";
		Next i
		If Not binmode Then Print #f, "};"
		If printscrsizes And Not binmode Then Print #f, "// Size = " & scrSizes (nPant) & " bytes."
		mapsize = mapsize + scrSizes (nPant)
	Else
		If Not binmode Then Print #f, "// Screen " & Lcase (Hex (nPant, 2)) & " is a copy of screen " & Lcase (Hex (scrMaps (nPant), 2)) & "."
	End If
Next nPant
Print #f, ""

' Write map index
If Not binmode Then
	Print #f, "// Compressed map array"
	Print #f, ""
	Print #f, "const unsigned char * const map_" & prefix & " [] = {"
	ctr = 0
	For nPant = 0 To mapPants - 1
		If ctr = 0 Then Print #f, "	";
		If scrMaps (nPant) = 255 Then
			Print #f, Space (Len ("scr_" & prefix & "_")) & " 0";
		Else
			Print #f, "scr_" & prefix & "_" & Lcase (Hex (scrMaps (nPant), 2));
		Endif
		If nPant < mapPants - 1 Then Print #f, ", ";
		ctr = ctr + 1: If ctr = mapW And nPant < mapPants - 1 Then ctr = 0: Print #f, ""
		mapsize = mapsize + 2
	Next nPant
	Print #f, ""
	Print #f, "};"
	Print #f, ""
	Print #f, "// Total map size in bytes is " & mapsize
	If decosize Then Print #f, "// From which, decorations size in bytes is " & decosize

	If Not nodecos Then Print "Wrote decos (" & decosize & " bytes) ~ ";

	Print #f, ""
End If

Print "Wrote MAP (" & mapsize & " bytes) ~ ";

' Write locks
If locksI Then
	If binmode Then
		Print #f, "#define N_BOLTS_" & Ucase (prefix) & " " & (locksI \ 2)
		Print #f, ""
		Print #f, "#define MAP_" & prefix & "_BOLTS_OFFSET 0x" & Hex (mapPants * 2 + totalBytes, 4)
		Print #f, "#define MAP_" & prefix & "_BOLTS_BYTES  " & locksI
		For i = 0 To locksI - 1
			d = locks (i): Put #fBin, , d
		Next i
	Else
		Print #f, "// Locks"
		Print #f, "// Format: NP YX ..."
		print #f, ""
		Print #f, "#define N_BOLTS_" & Ucase (prefix) & " " & (locksI \ 2)
		print #f, ""
		Print #f, "const unsigned char map_" & prefix & "_locks [] = {"
		Print #f, "	";
		For i = 0 To locksI - 1
			Print #f, "0x" & Lcase (Hex (locks (i), 2));
			If i < locksI - 1 Then Print #f, ", ";
		Next i
		Print #f, ""
		Print #f, "};"
		Print #f, ""
	End If		
else
	Print #f, "#define N_BOLTS_" & Ucase (prefix) & " 0"	
End If

Print "Wrote locks (" & locksI & " bytes) ~ ";

' Exit cleanly
Close #f
Erase m, decos, decosI

Print " DONE!"
