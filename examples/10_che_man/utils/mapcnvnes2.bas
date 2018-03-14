' Map converter
' convertes raw headerles .MAP into useable code

sub usage () 
	Print "Usage:"
	Print 
	Print "$ MapCnvNes in.map out.h w h tlock prefix [offset] [packed|zerostrungpacked]"
end sub

Function inCommand (a As String) As Integer
	Dim As Integer i
	i = 1
	While (Command (i) <> "")
		If Command (i) = a Then Return -1
		i = i + 1
	Wend
	Return 0
End Function

Dim As Integer map_w, map_h, scr_w, scr_h, bolt, locksI
Dim As Integer x, y, xx, yy, i, j, f, packed, ac, ct, npant, iddecos, decosTT, offset, bx, by, cx, cy, rp0, bytecnt
Dim As Byte d, dp
Dim As String o, prefix
Dim Shared As Integer BigOrigMap (1024, 1024)
Dim As uByte decos (127, 127)
Dim As uByte decosXY (127, 127), XY (127)
Dim As uByte decosO (127, 127)
Dim As uByte decosI (127)
Dim As uByte decosOI (127)
Dim As Integer founddecos, mapPants, decoT, decoCT, decosize

Type MyBolt
	np As Integer
	x As Integer
	y As Integer
End Type

Dim As MyBolt Bolts (100)

Print "MK1 v1.0 mapcnvnes2 ~ ";

if Command (6) = "" THen
	usage ()
	end
End If

map_w = Val (Command (3))
map_h = Val (Command (4))
scr_w = 16
scr_h = 12
bolt = Val (Command (5))

mapPants = map_w * map_h

For i = 0 To mapPants - 1
	decosI (i) = 0
	decosOI (i) = 0
Next i
locksI = 0

prefix = Command (6)

For i = 7 To 8
	If Val (Command (i)) Then offset = Val (Command (i))
Next i
If offset Then Print "Offset " & offset & " ~ ";

If inCommand ("packed") Then
	packed = 1
ElseIf inCommand ("zerostrungpacked")  Then
	packed = 2
Else
	packed = 0
End If

' Leemos el mapa original

Print "Reading MAP (" & (map_h*scr_h*map_w*scr_w) & " bytes) ~ ";

f = FreeFile
Open Command (1) for binary as #f

For y = 0 To (map_h * scr_h - 1)
	For x = 0 To (map_w * scr_w - 1)
		get #f , , d
		'?x, y, d, offset
		BigOrigMap (y, x) = d - offset
	Next x
Next y

close f

' Construimos el nuevo mapa mientras rellenamos el array de cerrojos

open Command (2) for output as #f

print #f, "// " & prefix & ".h"
print #f, "// Generated with mapcnvnes2.exe"
print #f, "// Copyleft 2013, 2017 The Mojon Twins"
print #f, ""
Print #f, "// Map Size Is " & map_w & "x" & map_h
Print #f, "// Screen Size Is " & scr_w & "x" & scr_h
print #f, ""

if packed < 2 Then print #f, "const unsigned char map_" & prefix & " [] = {"

locksI = 0: decosTT = 0
bytecnt = 0

for yy = 0 To map_h - 1
	for xx = 0 To map_w - 1
		npant = yy * map_w + xx
		bx = xx * scr_w
		by = yy * scr_h
		
		o = "	"
		ac = 0
		ct = 0
		if packed = 2 Then Print #f, "const unsigned char scr_" & prefix & "_" & lcase (hex (npant, 2)) & " [] = {"
		for y = 0 to scr_h - 1
			for x = 0 to scr_w - 1
				cx = bx + x
				cy = by + y
				
				if BigOrigMap (cy, cx) = bolt then
					Bolts (locksI).x = x
					Bolts (locksI).y = y
					Bolts (locksI).np = yy * map_w + xx
					locksI = locksI + 1
				end if
				
				If packed <> 0 Then
					If BigOrigMap (cy, cx) > 15 Then
						' Write to decos
						If Not founddecos Then
							founddecos = -1
							Print "Found decos ~ ";
						End If
						decosXY (nPant, decosI (nPant)) = cx * 16 + cy
						decos (nPant, decosI (nPant)) = BigOrigMap (cy, cx)
						decosI (nPant) = decosI (nPant) + 1
						' Reset to previous (so there's more repetitions)
						BigOrigMap (cy, cx) = dp
					End If
				End If

				if packed = 0 then
					o = o + str (BigOrigMap (cy, cx))
					bytecnt = bytecnt + 1
					if yy < map_h - 1 Or xx < map_w - 1 Or y < scr_h -1 Or x < scr_w -1 then
						o = o + ", "
					end if
				elseif packed = 1 Then
					If ct = 0 then
						ac = BigOrigMap (cy, cx) * 16
					Else
						ac = ac + BigOrigMap (cy, cx) 
						o = o + "0x" & Lcase (Hex (ac, 2))
						bytecnt = bytecnt + 1
						
						if yy < map_h - 1 Or xx < map_w - 1 Or y < scr_h - 1 Or x < scr_w - 1 then
							o = o + ", "
						end if
					End if
					ct = 1 - ct
				else
					If ct = 0 then
						ac = BigOrigMap (cy, cx) * 16
						' special
						If ac = 0 Then
							If x < scr_w - 1 Then
								If BigOrigMap (cy, cx + 1) = 0 Then
									rp0 = 0
									cx = cx + 1
									While BigOrigMap (cy, cx) = 0
										rp0 = rp0 + 1
										cx = cx + 1
										ct = 1 - ct
										If cx = scr_w Then Exit While
									Wend
									' Write zerocodeds
									o = o + "0x" + Hex (rp0, 2) ' max = 0f means 1+15 zeroes
									bytecnt = bytecnt + 1
									if yy < map_h - 1 Or xx < map_w - 1 Or y < scr_h - 1 Or x < scr_w - 1 then
										o = o + ", "
									end if
									' Where are we?
									If cx < scr_w Then
										If ct = 0 then 
											ac = 16 * BigOrigMap (cy, cx) 
										Else 
											ac = BigOrigMap (cy, cx)
										End If
									End If
								End If
							End If
						End If
					Else
						ac = ac + BigOrigMap (cy, cx) 
						o = o + "0x" & Lcase (Hex (ac, 2))
						bytecnt = bytecnt + 1
						
						if yy < map_h - 1 Or xx < map_w - 1 Or y < scr_h - 1 Or x < scr_w - 1 then
							o = o + ", "
						end if
					End if
					ct = 1 - ct
				end if
				
				dp = BigOrigMap (cy, cx)
			next x
		next y		
		print #f, o
		if packed = 2 Then Print #f, "};"
	next xx
	if map_w <> 1 And yy < map_h - 1 then print #f, "    "
next yy
print #f, "};"

print #f, " "

Print " Wrote ";
If packed = 1 Then Print "packed ";
If packed = 2 Then Print "zerostrungpacked ";
Print "MAP (" & bytecnt & " bytes) ~ ";

' Process decos
If founddecos Then 
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
	Print #f, "const unsigned char * const map_" & prefix & "_decos [] = {"
	For y = 0 To map_H - 1
		Print #f, "	";
		For x = 0 To map_W - 1
			nPant = x + y * map_W
			If decosOI (nPant) Then
				Print #f, "map_" & prefix & "_decos_" & Lcase (Hex (nPant, 2));
			Else 
				Print #f, "0";
			End If
			decosize = decosize + 2
			If x < map_W - 1 Or y < map_H - 1 Then Print #f, ", ";
		Next x
		Print #f, ""
	Next y
	Print #f, "};"
	Print #f, ""
	Print #f, "// Total decorations size in bytes is " & decosize
	Print #f, ""

	Print "Wrote decos (" & decosize & " bytes) ~ ";
End If

' Escribimos el array de cerrojos

if locksI > 0 Then
	Print #f, "// Locks"
	Print #f, "// Format: NP YX ..."
	print #f, ""
	print #f, "#define N_BOLTS_" & prefix & " " & locksI
	print #f, ""
	print #f, "const unsigned char map_" & prefix & "_locks [] = {"
	Print #f, "	";
	for j = 0 to locksI - 1
		Print #f, "0x" & Lcase (Hex(bolts(j).np, 2)) + ", " + "0x" & Lcase (Hex(bolts(j).y * 16 + bolts(j).x, 2));
		if j < locksI - 1 then Print #f, ", ";
	next j
	Print #f, ""
	print #f, "};"
else
	print #f, ""
	print #f, "#define N_BOLTS_" & prefix & " " & locksI
end if
print #f, " "
close f

Print " Wrote bolts (" & locksI & " bytes) ~ ";

Print "DONE!"
