' This code is based upon the original compressor (writen in VBScript)
' by tokumaru. I just translated it to freebasic 'cause it wouldn't work
' in my toolchain in its original form

' Plus this code is portable.

sub usage
	Print "$ lzsscomp in.bin out.bin|out.h"
	Print ""
	Print "Output format is based on file extension. If .bin is provided, binary output"
	Print "will be produced; C code output otherwise."
end sub



Dim Shared As Integer dictCount (65535)
Dim Shared As Integer dictCode (65535)
Dim Shared As Integer dictPointer (65535)
Dim Shared As Integer currentSlot

Dim Shared As Integer bestLength (16383)
Dim Shared As Integer bestData (16383)

Dim Shared As Integer flagBuffer
Dim Shared As Integer flagPosition
Dim Shared As Integer dictStep

Dim Shared As uByte inputBinary (16383)
Dim Shared As uByte outputData (16383)
Dim Shared As Integer inputDataSize, outputDataPosition, outputLength

If Command (2) = "" Then usage: End
If Right (Command (2), 4) <> ".bin" And Right (Command (2), 2) <> ".h" Then usage: End

Sub updateDictionary (currentPosition As Integer)
	Dim As Integer currentCode, currentSlot
	
	If currentPosition - 2048 >= 0 Then
		currentCode = inputBinary (currentPosition - 2048) + inputBinary (currentPosition - 2047) * 256
		currentSlot = currentCode
		dictCount (currentSlot) = dictCount (currentSlot) - 1
		
		'Find the slot that points to the start of the sliding window
		While dictPointer(currentSlot) <> (currentPosition - 2048)
			currentSlot = (currentSlot + dictStep) And 65535
		Wend
		
		dictCode (currentSlot) = -1
		dictPointer (currentSlot) = -1
	End If

	currentCode = inputBinary (currentPosition) + inputBinary (currentPosition + 1) * 256
	currentSlot = currentCode

	dictCount (currentSlot) = dictCount (currentSlot) + 1
	' Find the next free slot
	While dictCode (currentSlot) > -1
		currentSlot = (currentSlot + dictStep) And 65535
	Wend
	dictCode (currentSlot) = currentCode
	dictPointer (currentSlot) = currentPosition
End Sub

Sub FindBestMatch (currentPosition As Integer)
	Dim As Integer currentCode, currentSlot, matchCount, currentLength, previousPosition, matchNotFinished

	'Assume that the best is an uncompressed character
	bestLength(currentPosition) = 1
	bestData(currentPosition) = inputBinary (currentPosition)

	currentCode = inputBinary (currentPosition) + inputBinary (currentPosition + 1) * 256
	currentSlot = currentCode

	matchCount = dictCount (currentCode)

	While matchCount > 0 'While there are occurrences of the pair in the dictionary
		currentLength = 2
		
		'Find the next slot with this code
		While (dictCode (currentSlot) <> currentCode) 'Or (DictPointer(CurrentSlot) = -1)
			currentSlot = (currentSlot + dictStep) And 65535
		Wend
		
		previousPosition = dictPointer (currentSlot)
		matchNotFinished = -1
		
		While matchNotFinished
			If (currentPosition + currentLength) < inputDataSize Then 'If there's still data to be read
				If inputBinary (previousPosition + currentLength) = inputBinary (currentPosition + currentLength) Then
					currentLength = currentLength + 1
					If currentLength = 16 Then matchNotFinished = 0
				Else
					matchNotFinished = 0
				End If
			Else
				matchNotFinished = 0
			End If
		Wend
		
		If currentLength > bestLength (currentPosition) Then
			bestLength (currentPosition) = currentLength
			bestData(currentPosition) = previousPosition
		End If
		matchCount = matchCount - 1
		currentSlot = (currentSlot + dictStep) And 65535
	Wend
End Sub

Sub writeByte (b As uByte)
	outputData (outputDataPosition) = b
	'? "Write ( " & outputDataPosition & " ) = " & b
	outputDataPosition = outputDataPosition + 1
	If outputDataPosition > outputLength Then outputLength = outputDataPosition
End Sub

Sub writeByteAt (postn As Integer, b As uByte)
	outputData (postn) = b
	If outputLength < postn + 1 Then outputLength = postn + 1
	'outputDataPosition = postn + 1
End Sub

Sub outputFlag (flagValue As Integer)
	If (flagBuffer And 256) > 0 Then 'If the buffer is empty
		flagPosition = outputDataPosition 'Remember the position
		writeByte (0) 'Write dummy value
	End If

	flagBuffer = (flagBuffer * 2) Or (flagValue And 1) 'Add the flag to the buffer

	If (flagBuffer And 65536) > 0 Then 'If the buffer is full
		'outputData (flagPosition) = flagBuffer And 255 'Write it to the correct position
		writeByteAt (flagPosition, flagBuffer And 255)
		flagBuffer = 256 'Clear the buffer
	End If
End Sub

Sub parseGreedy
	Dim As Integer currentPosition

	currentPosition = 0

	While currentPosition <= inputDataSize - 1 'UBound (bestLength)
		If bestLength (currentPosition) = 1 Then
			outputFlag 0
			writeByte bestData (currentPosition)
		Else
			outputFlag 1
			If (bestLength (currentPosition) < 6) And (currentPosition - bestData (currentPosition) <= 32) Then
				writeByte 128 + (bestLength (currentPosition) - 2) * 32 + (currentPosition - bestData (currentPosition)) - 1
			Else
				bestData (currentPosition) = (bestLength (currentPosition) - 1) * 2048 + (currentPosition - bestData (currentPosition)) - 1
				writeByte (bestData (currentPosition) And 65280) \ 256
				writeByte bestData (currentPosition) And 255
			End If
		End If
		currentPosition = currentPosition + bestLength (currentPosition)
	Wend
End Sub

Sub parseOptimal
	Dim As Integer currentPosition, currentBlock, bestBlock, newLength

	currentPosition = inputDataSize - 1 'UBound (bestLength)

	While currentPosition >= 0
		bestBlock = currentPosition
		For currentBlock = currentPosition - 1 To currentPosition - 15 Step -1
			If currentBlock >= 0 Then
				If currentBlock + bestLength (currentBlock) >= currentPosition + 1 Then bestBlock = currentBlock
			End If
		Next

		newLength = currentPosition - bestBlock + 1

		If (newLength = 1) And (bestLength (bestBlock) > 1) Then
			currentBlock = bestBlock
			While bestLength (currentBlock) > 1
				currentBlock = bestData (currentBlock)
			Wend
			bestData (bestBlock) = bestData (currentBlock)
		End If
		bestLength (bestBlock) = newLength Or 256
		currentPosition = bestBlock - 1
	Wend

	currentPosition = 0

	While currentPosition <= InputDataSize - 1 ' UBound (bestLength)
		If (bestLength (currentPosition) And 256) > 0 Then
			bestLength (currentPosition) = bestLength (currentPosition) And 255

			If bestLength (currentPosition) = 1 Then
				outputFlag 0
				writeByte bestData (currentPosition)
			Else
				outputFlag 1
				If (bestLength(currentPosition) < 6) And (currentPosition - bestData (currentPosition) <= 32) Then
					writeByte 128 + (bestLength (currentPosition) - 2) * 32 + (currentPosition - bestData (currentPosition)) - 1
				Else
					bestData (currentPosition) = (bestLength (currentPosition) - 1) * 2048 + (currentPosition - bestData(currentPosition)) - 1
					writeByte (bestData (currentPosition) And 65280) \ 256
					writeByte bestData (currentPosition) And 255
				End If
			End If
		End If
		currentPosition = currentPosition + 1
	Wend
End Sub

Dim As Integer currentPosition
Dim As Integer fIn, fOut, i, ctr

'Set InputData = New ByteStream
'InputData.LoadFromFile InputFile
fIn = FreeFile
Open Command (1) For Binary As #fIn
inputDataSize = 0
While Not Eof (fIn)
	Get #fIn, , inputBinary (inputDataSize)
	inputDataSize = inputDataSize + 1
Wend
Close #fIn

Print "Input size = " & inputDataSize;

DictStep = 4369 ' 1

For currentSlot = 0 To 65535
	dictCount (currentSlot) = 0
	dictCode (currentSlot) = -1
	dictPointer (currentSlot) = -1
Next

flagBuffer = 256

For currentPosition = 0 To inputDataSize - 2
	findBestMatch currentPosition
	updateDictionary currentPosition
Next

'If the last byte does not belong to the end of a match, it can only be encoded as uncompressed
bestLength (currentPosition) = 1
bestData (currentPosition) = inputBinary (CurrentPosition)

' Set OutputData = New ByteStream
outputDataPosition = 0
outputLength = 0

'Generate the output
'ParseGreedy '(5763 bytes)
parseOptimal '(5729 bytes) Maybe the parsing isn't optimal?

'Mark the end of the stream
outputFlag 1
writeByte 0

'Output flags that may still be in the buffer
If (flagBuffer And 256) = 0 Then
	While (flagBuffer And 65536) = 0
		flagBuffer = flagBuffer * 2
	Wend
	outputData (FlagPosition) = FlagBuffer And 255
End If

'OutputData.SaveToFile OutputFile
'Set OutputData = Nothing

Kill Command (2)
fOut = FreeFile

If Right (Command (2), 4) = ".bin" Then
	Open Command (2) For Binary As #fOut
	For i = 0 To outputLength - 1
		Put #fOut, , outputData (i)
	Next i
	Close #fOut
ElseIf Right (Command (2), 2) = ".h" Then
	Open Command (2) For Output As #fOut
	ctr = 0
	Print #fOut, "const unsigned char " & Left (Command (2), Len (Command (2)) - 2) & " [" & outputLength & "] = {": Print #fOut, "	";
	For i = 0 To outputLength - 1
		Print #fOut, "0x" & Lcase (Hex (outputData (i), 2));
		If i < outputLength - 1 Then Print #fOut, ", ";
		ctr = ctr + 1: If ctr = 16 And i < outputLength - 1 Then ctr = 0: Print #fOut, "": Print #fOut, "	";
	Next i
	Print #fOut, ""
	Print #fOut, "};"
	Close #fOut
End If
	
Print " . . DONE, output size = " & outputLength
