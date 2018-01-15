' Map converter
' Cutrecode by na_th_an

' Estos programas son tela de optimizables, pero me da igual porque tengo un dual core.
' ?OLE!

sub usage () 
	Print "** USO **"
	Print "   MapCnvNes mapa.map map_w map_h scr_w scr_h bolt prefix [offset] [packed|zerostrungpacked]"
	
end sub

Dim As Integer map_w, map_h, scr_w, scr_h, bolt
Dim As Integer x, y, xx, yy, i, j, f, packed, ac, ct, npant, iddecos, decosTT, offset, bx, by, cx, cy, rp0
Dim As Byte d
Dim As String o, prefix
Dim As Integer BigOrigMap (255, 255)
Dim As uByte decos (255, 127), decosOn (255 - 1)


Type MyBolt
	np As Integer
	x As Integer
	y As Integer
End Type

Dim As MyBolt Bolts (100)

if 	Command (1) = "" Or _
	Val (Command (2)) <= 0 Or _
	Val (Command (3)) <= 0 Or _
	Val (Command (4)) <= 0 Or _
	Val (Command (5)) <= 0 Or _
	Val (Command (6)) <= 0 Then
	
	usage ()
	end
End If

map_w = Val (Command (2))
map_h = Val (Command (3))
scr_w = Val (Command (4))
scr_h = Val (Command (5))
bolt = Val (Command (6))

prefix = Command (7)

offset = Val (Command (8))

if lcase(Command (9)) = "packed" Or lcase(Command (8)) = "packed" then
	packed = 1
elseif lcase (Command (9)) = "zerostrungpacked" Or lcase (Command (8)) = "zerostrungpacked" Then
	packed = 2
Else
	packed = 0
end if


' Leemos el mapa original

f = FreeFile
Open Command (1) for binary as #f

For y = 0 To (map_h * scr_h - 1)
	For x = 0 To (map_w * scr_w - 1)
		get #f , , d
		BigOrigMap (y, x) = d - offset
	Next x
Next y

close f

' Construimos el nuevo mapa mientras rellenamos el array de cerrojos

open prefix & ".h" for output as #f

print #f, "// " & prefix & ".h"
print #f, "// Generated with MapCnvNes"
print #f, "// Copyleft 2013 The Mojon Twins"
print #f, " "

if packed < 2 Then print #f, "const unsigned char " & prefix & " [] = {"

i = 0: decosTT = 0

for yy = 0 To map_h - 1
	for xx = 0 To map_w - 1
		npant = yy * map_w + xx
		bx = xx * scr_w
		by = yy * scr_h
		
		o = "	"
		ac = 0
		ct = 0
		iddecos = 0: decosOn (npant) = 0
		if packed = 2 Then Print #f, "const unsigned char scr_" & prefix & "_" & lcase (hex (npant, 2)) & " [] = {"
		for y = 0 to scr_h - 1
			for x = 0 to scr_w - 1
				cx = bx + x
				cy = by + y
				
				if BigOrigMap (cy, cx) = bolt then
					Bolts (i).x = x
					Bolts (i).y = y
					Bolts (i).np = yy * map_w + xx
					i = i + 1
				end if
				
				If BigOrigMap (cy, cx) > 15 Then
					' Write to decos
					decos (npant, iddecos) = y + x * 16
					iddecos = iddecos + 1
					decos (npant, iddecos) = BigOrigMap (cy, cx)
					iddecos = iddecos + 1
					decosOn (npant) = iddecos
					BigOrigMap (cy, cx) = 0
					decosTT = decosTT + 1
				End If

				if packed = 0 then
					o = o + str (BigOrigMap (cy, cx))
					if yy < map_h - 1 Or xx < map_w - 1 Or y < scr_h -1 Or x < scr_w -1 then
						o = o + ", "
					end if
				elseif packed = 1 Then
					If ct = 0 then
						ac = BigOrigMap (cy, cx) * 16
					Else
						ac = ac + BigOrigMap (cy, cx) 
						o = o + "0x" & Lcase (Hex (ac, 2))
						
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
									if yy < map_h - 1 Or xx < map_w - 1 Or y < scr_h - 1 Or x < scr_w - 1 then
										o = o + ", "
									end if
									' Where are we?
									If cx < scr_w Then
										If ct = 0 then 
											ac = 16 * BigOrigMap (cy, cx) 
										Else 
											ac = BigOrigMap (cy, cx)s
										End If
									End If
								End If
							End If
						End If
					Else
						ac = ac + BigOrigMap (cy, cx) 
						o = o + "0x" & Lcase (Hex (ac, 2))
						
						if yy < map_h - 1 Or xx < map_w - 1 Or y < scr_h - 1 Or x < scr_w - 1 then
							o = o + ", "
						end if
					End if
					ct = 1 - ct
				end if
				
			next x
		next y		
		print #f, o
		if packed = 2 Then Print #f, "};"
	next xx
	if yy < map_h - 1 then print #f, "    "
next yy
print #f, "};"

print #f, " "

' Write decos
If decosTT > 0 Then
	For i = 0 To (map_h * map_w) - 1
		If decosOn (i) > 0 Then
			Print #f, "const unsigned char " & prefix & "_decos_" & Hex (i, 2) & " [] = { ";
			For j = 0 To decosOn (i) - 1
				Print #f, "0x" & Lcase (Hex (decos (i, j))) & ", " ;
			Next j
			Print #f, "0xff };"
		End If
	Next i
	Print #f, ""
	Print #f, "const unsigned char *" & prefix & "_decos [] = {"
	Print #f, "	";
	j = 0
	For i = 0 To (map_h * map_w) - 1
		If j Then Print #f, ", ";
		j = -1
		If decosOn (i) > 0 Then
			Print #f, prefix & "_decos_" & Hex (i, 2);
		Else
			Print #f, "0";
		End If
	Next i
	Print #f, ""
	Print #f, "};"
	Print #f, ""
Else
	Print #f, "const unsigned char **" & prefix & "_decos = 0;"
	Print #f, ""
End If

i = 32
' Escribimos el array de cerrojos
print #f, "//#define MAX_CERROJOS " + trim(str(i))
print #f, " "
print #f, "// np, xy"
if i > 0 Then
	print #f, "const unsigned char locks_" & prefix & " [] = {"
	
	for j = 0 to i - 1
		o = "	" + "0x" & Lcase (Hex(bolts(j).np, 2)) + ", " + "0x" & Lcase (Hex(bolts(j).x * 16 + bolts(j).y, 2))
		if j < i - 1 then o = o + ","
		print #f, o
	next j
	
	print #f, "};"
else
	print #f, "const unsigned char *cerrojos;"
end if
print #f, " "
close f

if packed = 0 then 
	Print "mapcnvnes, unpacked mode, " + trim(str(map_h*map_w)) + " screens (" + trim(str(map_h*map_w*scr_h*scr_w)) + " bytes) written."
else
	Print "mapcnvnes, packed mode, " + trim(str(map_h*map_w)) + " screens (" + trim(str(map_h*map_w*scr_h*scr_w / 2)) + " bytes) written."
end if
