Sub strToArr128 (mstr As String, arr () As uByte)
	Dim As Integer i
	For i = 0 To 127
		arr (i) = 0
	Next i
	For i = 1 To Len (mstr)
		arr (i - 1) = Asc (Mid (mstr, i, 1))
	Next i
End Sub

Dim as Integer x, y, f1, f2, i, j
Dim as uByte d, arr (127), map_w, map_h, nenems

' enexpander.bas
If Len(command (1)) = 0 or Len(command (2)) = 0 Then ?"enexpander enems.ene output.ene":?:End

' read / write header

f1 = Freefile
Open Command (1) For Binary as #f1
f2 = Freefile
Open Command (2) For Binary as #f2

' Dummy read 256 bytes
For i = 1 To 256
	Get #f1, , d
Next i

' Write correct filenames

strToArr128 "mapa.map", arr ()
For i = 0 To 127
	Put #f2, , arr (i)
Next i
strToArr128 "mappy.bmp", arr ()
For i = 0 To 127
	Put #f2, , arr (i)
Next i

' Read/modify headers

' map_w, map_h, scr_w, scr_h, nenems
get #f1, , map_w
put #f2, , map_w
get #f1, , map_h
put #f2, , map_h
get #f1, , d
d = 16: put #f2, , d
get #f1, , d
d = 12: put #f2, , d
get #f1, , nenems
put #f2, , nenems

' Read / Modify / Write enems

For i = 1 To (nenems * map_w * map_h)
	' t x y xx yy n s1 s2
	get #f1, , d ' t
	put #f2, , d
	get #f1, , d ' x
	put #f2, , d
	get #f1, , d ' y
	d = d + 1: put #f2, , d
	get #f1, , d ' xx
	put #f2, , d
	get #f1, , d ' yy
	d = d + 1: put #f2, , d
	get #f1, , d ' n
	put #f2, , d
	get #f1, , d ' s1
	put #f2, , d
	get #f1, , d ' s2
	put #f2, , d
Next i

' Read / Write hotspots

For i = 1 To (map_w * map_h)
	' xx t
	get #f1, , d ' xx
	put #f2, , d
	get #f1, , d ' t
	put #f2, , d
Next i

Close

? "DONE!"
