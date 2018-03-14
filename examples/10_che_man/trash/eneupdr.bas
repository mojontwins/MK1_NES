Type EnemyIn
	t As uByte
	x As uByte
	y As uByte
	xx As uByte
	yy As uByte
	n As uByte
	s1 As uByte
	s2 As uByte
End Type

Type HotSpot
	x as uByte
	y as uByte
	t as uByte
End Type

Dim As EnemyIn enems (255)
Dim As HotSpot h (255)
Dim As uByte d, mapW, mapH, xt, yt
Dim As Integer i, iHi, iHf, iHs, iVi, iVf, iVs, index, x, y
Dim As String dummy

If Command (2) = "" Then
	Print "eneupdater in.ene out.ene"
	System
End If

' First read
Open Command (1) For Binary as #1

' Skip header
dummy = Input (256, 1)

' Read...
Get #1, , mapW
Get #1, , mapH

Get #1, , d: Get #1, , d: Get #1, , d

' And get all the enemies
For i = 0 To (mapW * mapH * 3) - 1
	Get #1, , enems (i).t
	Get #1, , enems (i).x
	Get #1, , enems (i).y
	Get #1, , enems (i).xx
	Get #1, , enems (i).yy
	Get #1, , enems (i).n
	Get #1, , enems (i).s1
	Get #1, , enems (i).s2
	'Print enems (i).t;" (";enems (i).x;",";enems (i).y;") - (";enems (i).xx;",";enems (i).yy;") ";enems (i).n;" ";enems (i).s1;enems (i).s2
Next i

' Hotspots
For i = 0 To (mapW * mapH) - 1
	Get #1, , h (i).x
	Get #1, , h (i).y
	Get #1, , h (i).t
	
Next i

Close

' Now flip...
if Command (3) = "horz" or Command (3) = "both" Then
	iHi = mapW - 1: iHf = 0: iHs = -1
Else
	iHi = 0: iHf = mapW - 1: iHs = 1
End If

If Command (3) = "vert" or Command (3) = "both" Then
	iVi = mapH - 1: iVf = 0: iVs = -1
Else
	iVi = 0: iVf = mapH - 1: iVs = 1
End If

Open Command (2) For Binary as #1
Put #1, , dummy
Put #1, , mapW
Put #1, , mapH

d = 16: Put #1, , d 
d = 12: Put #1, , d
d = 3: Put #1, , d

for y = iVi To iVf Step iVs
	For x = iHi To iHf Step iHs
		index = 3 * (x + y * mapW)
		For i = 0 To 2
			d = enems (index + i).t
			Select Case d
				'Case 1, 2, 3, 4: d = &H10 + (d - 1)
				'Case 4: d = &H23
				'Case 6: d = &H34
				'Case 7: d = &H70
				case &H10, &H11, &H12: d = d - &HF 
				case &H23: d = 4
			End Select

			Put #1, , d
			
			If Command (3) = "horz" Or Command (3) = "both" Then
				d = 15 - enems (index + i).x			
			Else
				d = enems (index + i).x
			End if
			Put #1, , d
			
			If Command (3) = "vert" Or Command (3) = "both" Then
				d = 11 - enems (index + i).y	
			Else
				d = enems (index + i).y
			End if
			Put #1, , d
			
			If Command (3) = "horz" Or Command (3) = "both" Then
				d = 15 - enems (index + i).xx
			Else
				d = enems (index + i).xx
			End if
			Put #1, , d
			
			If Command (3) = "vert" Or Command (3) = "both" Then
				d = 11 - enems (index + i).yy
			Else
				d = enems (index + i).yy
			End if
			Put #1, , d
			
			Put #1, , enems (index + i).n
			Put #1, , enems (index + i).s1
			Put #1, , enems (index + i).s2
		Next i
	Next x
Next y

' Hotspots
for y = iVi To iVf Step iVs
	For x = iHi To iHf Step iHs
		index = x + y * mapW
		xt = h (index).x
		yt = h (index).y
		If Command (3) = "horz" Or Command (3) = "both" Then
			xt = 15 - xt
		End If
		If Command (3) = "vert" Or Command (3) = "both" Then
			yt = 11 - yt
		End If
		
		Put #1, , xt
		Put #1, , yt
		Put #1, , h (index).t
		'Print d, h(index).t
	Next x
Next y

Close
Print "OK"
