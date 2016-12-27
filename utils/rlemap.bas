Const SCR_W = 16
Const SCR_H = 12

Const STATE_REP = 1
Const STATE_STRING = 2

Sub usage
	Print "$ rlemap.exe in.map out.h w h tlock prefix [scrsizes]"
End Sub

Function even (i As Integer) As Integer 
	Return ((i And 1) = 0)
End Function

Dim As Integer x, y, xx, yy, nPant, mapW, mapH, i, j, dat, f, mapsize, decosize, scrsize
Dim As Integer mapWtiles, mapPants, tLock, locksI, state, tileStrI, decoT, decoCt, XYct
Dim As Integer counter
Dim As String o, prefix
Dim As uByte d, dp
'Redim As uByte m (0, 0)
'Redim As uByte decos (0, 0), decosI (0)
'Redim As uByte mOut (0, 0), mOutI (0)
Dim As uByte locks (63), tilestr (32)

' Sorry, but this is acting weird
Dim As uByte m (127, SCR_W * SCR_H - 1)
Dim As uByte decos (127, 127)
Dim As uByte decosXY (127, 127), XY (127)
Dim As uByte decosO (127, 127)
Dim As uByte decosI (127)
Dim As uByte decosOI (127)
Dim As uByte mOut (127, SCR_W * SCR_H - 1)
Dim As uByte mOutI (127)

If Command (6) = "" Then usage: End

Print "RLEMAP ... ";

mapW = Val (Command (3))
mapH = Val (Command (4))
mapWtiles = mapW * SCR_W
mapPants = mapW * mapH
tLock = Val (Command (5))
prefix = Command (6)

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
Next i
locksI = 0

f = Freefile
Open Command (1) For Binary as #f
i = 0: dp = 0
While Not Eof (f)
	' Read from file
	Get #f, , d
	' Screen coordinates
	xx = (i \ SCR_W) Mod  mapW
	yy = i \ (SCR_W * SCR_H * mapW)
	' Tile coordinates
	x = i Mod SCR_W
	y = (i \ mapWtiles) Mod SCR_H
	' screen number
	nPant = xx + yy * mapW
	' Is d a decoration' 
	If d > 16 Then
		' Write to decos
		decosXY (nPant, decosI (nPant)) = x * 16 + y
		decos (nPant, decosI (nPant)) = d
		decosI (nPant) = decosI (nPant) + 1
		' Reset to previous (so there's more repetitions)
		d = dp
	End If
	' Is d a lock' 
	If d = tLock Then
		locks (locksI) = nPant: locksI = locksI + 1
		locks (locksI) = x * 16 + y: locksI = locksI + 1
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
For nPant = 0 To mapPants - 1
	state = STATE_REP
	dp = 99: i = 0: counter = 0: tileStrI = 0
	' "================ SCREEN " & nPant & "==================="
	While i < SCR_W * SCR_H
		d = m (nPant, i)
		' "FOUND:"; d
		if d = dp Then
			' ">>SAME!"
			' Same tile as previous
			If state = STATE_REP Then
				' " * NEXT SAME"
				counter = counter + 1
				' "COUNTER " & counter
			Else
				' " * FIRST SAME"
				' "tileStrI = " & tileStrI
				' We were filling a string.
				
				If tileStrI = 0 Then
					' Initial
					counter = 1
					' "(Write nothing 0)"
				ElseIf tileStrI = 1 Then
					' String only has the same tile which is now being accounted as a repetition
					' Clean tileStrI, account for repetition.
					tileStrI = 0
					counter = 2
					' "(Write nothing 1)"
				ElseIf even (tileStrI) Then
					' "String has even (containing prev)"
					' String containing current tile repetition has an even number of tiles thus
					' is writable.
					' Write string, reset repetition counter
					mOut (nPant, mOutI (nPant)) = &HF0 + (tileStrI \ 2) - 1: mOutI (nPant) = mOutI (nPant) + 1
					For j = 0 To tileStrI - 1 Step 2
						dat = tileStr (j) * 16 + tileStr (j + 1)
						mOut (nPant, mOutI (nPant)) = dat: mOutI (nPant) = mOutI (nPant) + 1
					Next j
					' "(Write string -> " & (tileStrI\2) & ")"
					counter = 1
				Else
					' "String has odd (containing prev) -> remove prev!"
					' String containing current tile repetition has an odd number of tiles. Remove
					' Last tile, account for it, write string.
					tileStrI = tileStrI - 1
					mOut (nPant, mOutI (nPant)) = &HF0 + (tileStrI \ 2) - 1: mOutI (nPant) = mOutI (nPant) + 1
					For j = 0 To tileStrI - 1 Step 2
						dat = tileStr (j) * 16 + tileStr (j + 1)
						mOut (nPant, mOutI (nPant)) = dat: mOutI (nPant) = mOutI (nPant) + 1
					Next j
					' "(Write string -> " & (tileStrI\2) & ")"
					counter = 2
				End If
				state = STATE_REP
				
				' "COUNTER = " & counter
			End If
		Else
			' ">>DIFFERENT!"
			If state = STATE_REP Then
				' " * FIRST DIFFERENT"
				' "COUNTER = " & counter
				' First different tile! Write rep
				tileStrI = 0
				If counter > 0 Then
					If counter < 4 Then
					'If counter <> counter then
						' Too few for a repetition, adding them to a string saves bytes.
						For j = 1 to counter
							tileStr (tileStrI) = dp
							tileStrI = tileStrI + 1
						Next j
					Else
						' "Write REP dp = " & dp & ", counter = " & counter
						mOut (nPant, mOutI (nPant)) = dp: mOutI (nPant) = mOutI (nPant) + 1
						mOut (nPant, mOutI (nPant)) = counter: mOutI (nPant) = mOutI (nPant) + 1
					End If
				End If
				state = STATE_STRING
				tileStr (tileStrI) = d
				tileStrI = tileStrI + 1
			Else
				' Next different tile. Write to tileStr
				' " * NEXT DIFFERENT"
				' Has tileStr 32 tiles already' 
				If tileStrI = 32 Then
					' "string is 32 tiles long, writing"
					' Write string, 0xFF = Full string (32 bytes), 32 = (0xF+1)*2
					mOut (nPant, mOutI (nPant)) = &HFF: mOutI (nPant) = mOutI (nPant) + 1
					For j = 0 To 31 Step 2
						dat = tileStr (j) * 16 + tileStr (j + 1)
						mOut (nPant, mOutI (nPant)) = dat: mOutI (nPant) = mOutI (nPant) + 1
					Next j
					' Reset
					tileStrI = 0
				End If
				
				tileStr (tileStrI) = d: tileStrI = tileStrI + 1
			End If
		End if	
		dp = d
		i = i + 1
	Wend
	' Done with the screen, write what's on
	If state = STATE_REP Then
		' "Write REP (last)"
		mOut (nPant, mOutI (nPant)) = dp: mOutI (nPant) = mOutI (nPant) + 1
		mOut (nPant, mOutI (nPant)) = counter: mOutI (nPant) = mOutI (nPant) + 1
	Else
		If Not even (tileStrI) Then tileStr (tileStrI) = 0: tileStrI = tileStrI + 1
		mOut (nPant, mOutI (nPant)) = &HF0 + (tileStrI \ 2) - 1: mOutI (nPant) = mOutI (nPant) + 1
		For j = 0 To tileStrI - 1 Step 2
			dat = tileStr (j) * 16 + tileStr (j + 1)
			mOut (nPant, mOutI (nPant)) = dat: mOutI (nPant) = mOutI (nPant) + 1
		Next j
		' "(Write string (last) -> " & (tileStrI\2) & ")"
	End If		
	
	' Optimization: IF screen takes more than 96 bytes, store uncompressed
	If mOutI (nPant) > 96 Then
		mOutI (nPant) = 0
		For i = 0 To 191 Step 2
			mOut (nPant,  i \ 2) = 16 * m (nPant, i) + m (nPant, i + 1)
		Next i
	End If
Next nPant

' Process decos
For nPant = 0 To mapPants - 1
	If decosI (nPant) Then
		For i = 0 To decosI (nPant) - 1
			decoT = decos (nPant, i)
			If decoT <> &Hff Then
				decoCT = 1
				XY (0) = decosXY (nPant, i)
				' Find more:
				For j = i + 1 To decosI (nPant) - 1
					If decos (nPant, i) = decos (nPant, j) Then
						' Found! DESTROY!
						XY (decoCT) = decosXY (nPant, j)
						decoCT = decoCT + 1
						decos (nPant, j) = &Hff
					End If
				Next j
				If decoCT = 1 Then
					' T | 128, XY
					decosO (nPant, decosOI (nPant)) = decoT Or 128: decosOI (nPant) = decosOI (nPant) + 1
					decosO (nPant, decosOI (nPant)) = XY (0): decosOI (nPant) = decosOI (nPant) + 1
				Else
					' T N XY XY XY XY...
					decosO (nPant, decosOI (nPant)) = decoT: decosOI (nPant) = decosOI (nPant) + 1
					decosO (nPant, decosOI (nPant)) = decoCT: decosOI (nPant) = decosOI (nPant) + 1
					For j = 0 To decoCT - 1
						decosO (nPant, decosOI (nPant)) = XY (j): decosOI (nPant) = decosOI (nPant) + 1
					Next j
				End If
			End If
		Next i
	End If
Next nPant

' Write output
f = Freefile
Open Command (2) For Output As #f
Print #f, "// RLEPacked(TM) map"
Print #f, "// generated by rlemap.exe"
Print #f, "// Copyleft 2015 The Mojon Twins"
Print #f, ""
Print #f, "// Map Size Is " & mapW & "x" & mapH
Print #f, "// Screen Size Is " & SCR_W & "x" & SCR_H
Print #f, ""
Print #f, "#define N_BOLTS_" & Ucase (prefix) & " " & (locksI \ 2)
Print #f, ""

mapsize = 0

' Write screens
Print #f, "// Compressed map structure, screens"
Print #f, "// Format: [SCR_SIZE, [ T COUNT | 0xF0 + COUNT T1 T2 T3 T4 ... ] | 0, 96 packed bytes]"
Print #f, ""
For nPant = 0 To mapPants - 1

	If mOutI (nPant) = 2 Then
		' Empty screen!
		Print #f, "// scr_" & prefix & "_" & Lcase (Hex (nPant, 2)) & " is empty."
		scrsize = 0
	Else
		Print #f, "const unsigned char scr_" & prefix & "_" & Lcase (Hex (nPant, 2)) & " [] = { ";
		Print #f, "0x" & Lcase (Hex (mOutI (nPant), 2)) & ", ";
		
		If mOutI (nPant) = 0 Then mOutI (nPant) = 96
			
		mapsize = mapsize + 1
		scrsize = 1
		For j = 0 To mOutI (nPant) - 1
			Print #f, "0x" & Lcase (Hex (mOut (nPant, j), 2));
			mapsize = mapsize + 1
			scrsize = scrsize + 1
			If j < mOutI (nPant) - 1 Then Print #f, ", ";
		Next j
		Print #f, " };"
	End If
	If Command (7) = "scrsizes" Then Print #f, "// Screen size = " & scrsize & " bytes."
Next nPant
Print #f, ""

' Write map index
Print #f, "// Compressed map array"
Print #f, ""
Print #f, "const unsigned char *map_" & prefix & " [] = {"
For y = 0 To mapH - 1
	Print #f, "	";
	For x = 0 To mapW - 1
		nPant = x + y * mapW
		If mOutI (nPant) = 2 Then
			Print #f, "0";
		Else
			Print #f, "scr_" & prefix & "_" & Lcase (Hex (nPant, 2));
		End If
		mapsize = mapsize + 2
		If x < mapW - 1 Or y < mapH - 1 Then Print #f, ", ";
	Next x
	Print #f, ""
Next y
Print #f, "};"
Print #f, ""
Print #f, "// Total map size in bytes is " & mapsize
Print #f, ""

' Write decos
decosize = 0
Print #f, "// Decorations"
Print #f, "// Format: [T N XY XY XY XY... (T < 128) | T XY (T >= 128)]"
Print #f, ""
For nPant = 0 To mapPants - 1
	If decosOI (nPant) Then
		Print #f, "const unsigned char map_" & prefix & "_decos_" & Lcase (Hex (nPant, 2)) & " [] = { ";
		For i = 0 To decosOI (nPant) - 1
			Print #f, "0x" & Lcase (Hex (decosO (nPant, i), 2)) & ", " ;
			decosize = decosize + 1
		Next i
		Print #f, "0x00 }; "
	End If
Next nPant
Print #f, ""
Print #f, "const unsigned char *map_" & prefix & "_decos [] = {"
For y = 0 To mapH - 1
	Print #f, "	";
	For x = 0 To mapW - 1
		nPant = x + y * mapW
		If decosOI (nPant) Then
			Print #f, "map_" & prefix & "_decos_" & Lcase (Hex (nPant, 2));
		Else 
			Print #f, "0";
		End If
		decosize = decosize + 2
		If x < mapW - 1 Or y < mapH - 1 Then Print #f, ", ";
	Next x
	Print #f, ""
Next y
Print #f, "};"
Print #f, ""
Print #f, "// Total decorations size in bytes is " & decosize
Print #f, ""

' Write locks
If locksI Then
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

' Exit cleanly
Close #f
Erase m, decos, decosI

Print " DONE!"
