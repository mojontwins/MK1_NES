' Para comprimir las pantallas.
' Primero las leo

Dim as integer x, y, i, n, c, idx, ctr
Dim as uByte d, dold
Dim as uByte map(2, 256)
dim as uByte rle(2, 239)
Dim As uByte pal(47)
Dim As String palString

' Read pal
Open "pallist.txt" For Input as #1
	Line Input #1, palString
	For i = 1 To Len(palString)
		pal (i-1) = val (Mid (palString, i, 1))
	Next i
Close #1

open "fixed.map" for binary as #1

for i = 0 to 2
	for n = 0 to 239
		get #1,,d
		map (i, n) = d
	next n
next i

close #1

' Compresión:

' Leo "d"
' Si bit 7 = 0
' imprimo d
' Si bit 7 = 1
' cuenta = d and 127 y leo otro byte que imprimo cuenta veces.

open "screens.h" for output as #1

For i = 0 to 2
	print #1, "const unsigned char scr_rle_" & trim(str(i)) & " [] = {"
	n = 1
	idx = 0
	dold = map (i, 0)
	ctr = 1
	
	While n < 240
		d = map (i, n)
		if dold<>d or ctr = 127 then
			' write old
			if ctr = 1 then
				rle (i, idx) = dold
				idx = idx + 1				
			else
				rle (i, idx) = 128 + ctr
				idx = idx + 1
				rle (i, idx) = dold
				idx = idx + 1
				dold = d
				ctr = 1
			end if
			dold = d
		else
			ctr = ctr + 1
		end if
		n=n+1
	Wend
	
	if ctr = 1 then
		rle (i, idx) = dold
		idx = idx + 1				
	else
		rle (i, idx) = 128 + ctr
		idx = idx + 1
		rle (i, idx) = dold
		idx = idx + 1
		dold = d
		ctr = 1
	end if
	dold = d

	ctr = 0
	print #1, "    ";
	for n = 0 to idx - 1
		ctr = ctr + 1
		print #1, "0x" & hex (rle (i, n), 2);
		if ctr = 16 or n = idx - 1 then
			ctr=0
			print #1, ","
			
			print #1, "    ";
		else
			print #1, ", ";
		end if
	next n
	
	' attributes
	ctr = 0
	for y = 0 to 7
		for x = 0 to 7
			d = 0
			d = d + pal (map(i, 16 * (2 * y) + (2 * x)))
			d = d + pal (map(i, 16 * (2 * y) + (2 * x + 1))) Shl 2
			d = d + pal (map(i, 16 * (2 * y + 1) + (2 * x))) Shl 4
			d = d + pal (map(i, 16 * (2 * y + 1) + (2 * x + 1))) Shl 6
			ctr = ctr + 1
			print #1, "0x" & hex (d, 2);
			if ctr = 16  then
				
				ctr = 0
				if x = 7 and y = 7 then 
					print #1, ""
				else
					print #1, ","
				endif
				
				print #1, "    ";
			else
				print #1, ", ";
			end if
		next x
	next y
	print #1, "};"
Next i

close #1
