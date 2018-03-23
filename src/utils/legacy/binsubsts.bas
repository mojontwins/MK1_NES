' Binsubsts.bas

Sub usage 
	Print "$ binsubsts in.bin out.bin substs.txt"
End Sub

Dim As Integer fIn, fOut
Dim As uByte substs (255)
Dim As Integer i
Dim As uByte d

If Len (Command (3)) = 0 Then usage: End

Print "Reading substs @ " & Command (3)
fIn = Freefile
Open Command (3) For Input As #fIn
While Not Eof (fIn)
	input #fIn, substs (i)
	Print Hex (substs (i),2); " ";
	i = i + 1
	If (i And 15) = 0 Then Print
Wend
Print
Close #fIn

Print "Reading " & Command (1) & " & substituting into " & Command (2)

fIn = Freefile
Open Command (1) For Binary As #fIn
fOut = Freefile
Open Command (2) For Binary As #fOut

While Not Eof (fIn)
	Get #fIn, , d
	Put #fOut, , substs (d)
Wend

Close
Print "Done!"
