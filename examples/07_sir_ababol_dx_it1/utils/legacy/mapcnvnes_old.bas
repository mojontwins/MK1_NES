' Map converter
' Cutrecode by na_th_an

' Estos programas son tela de optimizables, pero me da igual porque tengo un dual core.
' ¡OLE!

Sub WarningMessage ()
	Print "** WARNING **"
	Print "   MapCnv convierte un archivo raw de mappy (mapa.map, por ejemplo)"
	Print "   a un array directamente usable por los juegos de la churrera."
	Print "   Si metes mal los parámetros ocurrirán cosas divertidas."
	Print
End Sub

sub usage () 
	Print "** USO **"
	Print "   MapCnvNes mapa.map map_w map_h scr_w scr_h bolt prefix packed "
	
end sub

Dim As Integer map_w, map_h, scr_w, scr_h, bolt
Dim As Integer x, y, xx, yy, i, j, f, packed, ac, ct
Dim As Byte d
Dim As String o, prefix

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
	WarningMessage ()	
	usage ()
	end
End If

map_w = Val (Command (2))
map_h = Val (Command (3))
scr_w = Val (Command (4))
scr_h = Val (Command (5))
bolt = Val (Command (6))

prefix = Command (7)

if lcase(Command (8)) = "packed" then
	print lcase(command(8))
	packed = 1
else
	packed = 0
end if

Dim As Integer BigOrigMap (map_h * scr_h - 1, map_w * scr_w - 1)


' Leemos el mapa original

f = FreeFile
Open Command (1) for binary as #f

For y = 0 To (map_h * scr_h - 1)
	For x = 0 To (map_w * scr_w - 1)
		get #f , , d
		BigOrigMap (y, x) = d
	Next x
Next y

close f

' Construimos el nuevo mapa mientras rellenamos el array de cerrojos

open prefix & ".h" for output as #f

print #f, "// " & prefix & ".h"
print #f, "// Generado por MapCnvNes"
print #f, "// Copyleft 2013 The Mojon Twins"
print #f, " "

print #f, "const unsigned char " & prefix & " [] = {"

i = 0

for yy = 0 To map_h - 1
	for xx = 0 To map_w - 1
		o = "    "
		ac = 0
		ct = 0
		for y = 0 to scr_h - 1
			for x = 0 to scr_w - 1
				
				if BigOrigMap (yy * scr_h + y, xx * scr_w + x) = bolt then
					Bolts (i).x = x
					Bolts (i).y = y
					Bolts (i).np = yy * map_w + xx
					i = i + 1
				end if
				
				if packed = 0 then
					o = o + str (BigOrigMap (yy * scr_h + y, xx * scr_w + x))
					if yy < map_h - 1 Or xx < map_w - 1 Or y < scr_h -1 Or x < scr_w -1 then
						o = o + ", "
					end if
				else
					if ct = 0 then
						ac = BigOrigMap (yy * scr_h + y, xx * scr_w + x) * 16
					else
						ac = ac + BigOrigMap (yy * scr_h + y, xx * scr_w + x) 
						o = o + str (ac)
						
						if yy < map_h - 1 Or xx < map_w - 1 Or y < scr_h - 1 Or x < scr_w - 1 then
							o = o + ", "
						end if
					end if
					ct = 1 - ct
				end if
				
			next x
		next y		
		print #f, o
	next xx
	if yy < map_h - 1 then print #f, "    "
next yy
print #f, "};"

print #f, " "

'i = 32
' Escribimos el array de cerrojos
print #f, "//#define MAX_CERROJOS " + trim(str(i))
print #f, " "
print #f, "// np, xy"
if i > 0 Then
	print #f, "const unsigned char locks_" & prefix & " [] = {"
	
	for j = 0 to 31 'i - 1
		o = "    " + trim(str(bolts(j).np)) + ", " + trim(str(bolts(j).x * 16 + bolts(j).y))
		if j < 31 then o = o + ","
		print #f, o
	next j
	
	print #f, "};"
else
	print #f, "const unsigned char *locks_" & prefix & ";"
end if
print #f, " "
close f

if packed = 0 then 
	Print "Se escribió mapa.h con " + trim(str(map_h*map_w)) + " pantallas (" + trim(str(map_h*map_w*scr_h*scr_w)) + " bytes)."
else
	Print "Se escribió mapa.h con " + trim(str(map_h*map_w)) + " pantallas empaquetadas (" + trim(str(map_h*map_w*scr_h*scr_w / 2)) + " bytes)."
end if
Print "Se encontraron " + trim(str(i)) + " cerrojos."
print " "
end
