'

Sub usage
	Print "usage"
	Print
	Print "$ flipenems in.ene out.ene map_w map_h scr_w scr_h nenems flipHorz flipVert"
End Sub

Sub PutString (fH As Integer, s As String)
	Dim As uByte d
	Dim As Integer i
	For i = 1 To Len (s)
		d = Asc (Mid (s, i, 1))
		Put #fH, , d
	Next i
End Sub

Type Enem
	t As uByte
	x As uByte
	y As uByte
	xx As uByte
	yy As uByte
	n As uByte
	s1 As uByte
	s2 As uByte
End Type

Type HS
	x As uByte
	y As uByte
	t As uByte
End Type

Dim As Integer map_w, map_h, scr_w, scr_h
Dim As Integer flipHorz, flipVert, map_x0, map_x1, map_y0, map_y1, map_x, map_y, x, y, x0, y0, x1, y1, sy, sx
Dim As Integer nenems, fIn, fOut, n, nPant
Dim As String header
Dim As Enem enems (255, 7)
Dim As HS hs (255)

If Command (7) = "" Then
	usage
	End
End If

' Parse flippage

flipHorz = (Command (9) = "flipHorz" Or Command (8) = "flipHorz")
flipVert = (Command (9) = "flipVert" Or Command (8) = "flipVert")

' Parse sizes

map_w = Val (Command (3))
map_h = Val (Command (4))
scr_w = Val (Command (5))
scr_h = Val (Command (6))
nenems = Val (Command (7))

' Boundaries

If flipHorz Then
	map_x0 = map_w - 1
	map_x1 = 0
	sx = -1
Else
	map_x0 = 0
	map_x1 = map_w - 1
	sx = 1
End If

If flipVert Then
	map_y0 = map_h - 1
	map_y1 = 0
	sy = -1
Else
	map_y0 = 0
	map_y1 = map_h - 1
	sy = 1
End If

? "Y " & map_y0 & " TO " & map_y1
? "X " & map_x0 & " TO " & map_x1
' Open files

fIn = FreeFile
Open Command (1) For Binary As #fIn
fOut = FreeFile
Open Command (2) For Binary As #fOut

' Do header
header = Input (261, fIn)
PutString fOut, header

' Do Screens. First read
'? "ENEMS"
For map_y = 0 To map_h - 1
	For map_x = 0 To map_w - 1
		nPant = map_y * map_w + map_x
		'? "=== " & nPant
		For n = 0 To nenems - 1	
			Get #fIn, , enems (nPant, n).t
			Get #fIn, , enems (nPant, n).x
			Get #fIn, , enems (nPant, n).y
			Get #fIn, , enems (nPant, n).xx
			Get #fIn, , enems (nPant, n).yy
			Get #fIn, , enems (nPant, n).n
			Get #fIn, , enems (nPant, n).s1
			Get #fIn, , enems (nPant, n).s2
			'? "    " & enems (nPant, n).t & " (" & enems (nPant, n).x & ", " & enems (nPant, n).y & ")"
Next n, map_x, map_y

'? "HOTSPOTS"
For map_y = 0 To map_h - 1
	For map_x = 0 To map_w - 1
		nPant = map_y * map_w + map_x
		'? "=== " & nPant
		Get #fIn, , hs (nPant).x
		Get #fIn, , hs (nPant).y
		Get #fIn, , hs (nPant).t
		'? "    " & hs (nPant).x & ", " & y  & " ->  "& hs (nPant).t 
Next map_x, map_y

' Now write.

For map_y = map_y0 To map_y1 Step sy
	For map_x = map_x0 To map_x1 Step sx
		nPant = map_y * map_w + map_x
		? "-> " & nPant
		For n = 0 To nenems - 1
			If flipHorz Then 
				enems (nPant, n).x = scr_w - 1 - enems (nPant, n).x
				enems (nPant, n).xx = scr_w - 1 - enems (nPant, n).xx
			End If
			If flipVert Then 
				enems (nPant, n).y = scr_h - 1 - enems (nPant, n).y
				enems (nPant, n).yy = scr_h - 1 - enems (nPant, n).yy
			End If
			
			Put #fOut, , enems (nPant, n).t
			Put #fOut, , enems (nPant, n).x
			Put #fOut, , enems (nPant, n).y
			Put #fOut, , enems (nPant, n).xx
			Put #fOut, , enems (nPant, n).yy
			Put #fOut, , enems (nPant, n).n
			Put #fOut, , enems (nPant, n).s1
			Put #fOut, , enems (nPant, n).s2
Next n, map_x, map_y

For map_y = map_y0 To map_y1 Step sy
	For map_x = map_x0 To map_x1 Step sx
		nPant = map_y * map_w + map_x
		
		If flipHorz Then hs (nPant).x = scr_w - 1 - hs (nPant).x
		If flipVert Then hs (nPant).y = scr_h - 1 - hs (nPant).y
		
		Put #fOut, , hs (nPant).x
		Put #fOut, , hs (nPant).y
		Put #fOut, , hs (nPant).t
Next map_x, map_y

Close fIn, fOut
