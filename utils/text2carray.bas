Dim As String text
Dim As Integer i
Dim As Integer c

If Command (1) = "" Then
	Print "Enter all your text..."
	Line Input text
Else
	Print Command (1)
	text = Command (1)
End If

c = 0
For i = 1 To Len (text)
	Print "0x" & Lcase (Hex (Asc (Mid (text, i, 1)), 2));
	If i < Len (text) Then ? ", ";
	c = c + 1: If c = 16 Then ?: c = 0
Next i
