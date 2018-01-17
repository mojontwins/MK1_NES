Dim As String strIn, o, m
Dim As uByte bytes (255)
Dim As Integer i, indx, cmd, t, ct, j, x, y
? " Paste hex C string "
Line Input strIn
strIn = strIn & ","

o = ""
indx = 0
For i = 1 To Len (strIn)
	m = Mid (strIn, i, 1)
	If m ="," Then
		bytes (indx) = Val ("&H" & o)
		indx = indx + 1 
		o = ""
	ElseIf (m >= "0" And m <= "9") Or (m >= "a" And m <= "f") Then
		 o = o & m
	End if
Next i

x = 1
y = 1
i = 1
cls
While i < indx
	cmd = bytes (i) Shr 4
	If cmd = 0 Then
		' Repetition!
		t = bytes (i) And 15
		i = i + 1
		ct = bytes (i)
		i = i + 1
		For j = 1 To ct
			Locate y, x: Print Hex (t, 1)
			x = x + 1: If x = 17 Then x = 1: y = y + 1
		Next j
	ElseIf cmd = 15 Then
		' String!
		ct = (bytes (i) And 15) + 1
		i = i + 1
		For j = 1 To ct
			t = bytes (i)
			Locate y, x: Print Hex (t Shr 4, 1)
			x = x + 1: If x = 17 Then x = 1: y = y + 1
			Locate y, x: Print Hex (t And 15, 1)
			x = x + 1: If x = 17 Then x = 1: y = y + 1
			i = i + 1
		Next j
	Else
		Locate 15, 1: ? "WRONG COMMAND!": End
	End If
Wend
