' mscmk1.bas
' Mojontwins Script Compiler for MK1_NES / AGNES
' Copyleft 2018 by The Mojon Twins

' $ fbc mscmk1.bas mtparser.bas

#include "mtparser.bi"

Const LIST_WORDS_SIZE = 128
Const LIST_ALIAS_SIZE = 255
Const MACRO_SIZE = 32
Const MAX_MACROS = 31

ReDim Shared As String script (0)
ReDim Shared As Integer addresses (0)
Dim Shared As Integer clausulasUsed (255)
Dim Shared As Integer actionsUsed (255)
Dim Shared As uByte myBin (16383)
Dim As uInteger maxClausuleSize
Dim Shared lP (LIST_WORDS_SIZE) As String
Dim Shared As String listAlias (LIST_ALIAS_SIZE) 
Dim Shared As String decoInclude
Dim Shared As Integer doIncludeDecos, decoFetch
Dim Shared As Byte decorated (255)
Dim Shared As Integer maxScr
Dim Shared As String macros (MAX_MACROS, MACRO_SIZE)
Dim Shared As Integer macrosPtr
Dim Shared As Integer maxItem
Dim Shared As Integer itemSetx, itemSety, itemSetStep, itemSetOr, itemFlag, slotFlag
Dim Shared As Integer itemSelectClr, itemSelectC1, itemSelectC2, itemEmpty
Dim Shared As Integer debug

Sub usage
	Print "Usage:"
	Print
	Print "$ mscmk1.exe input.spt output.h maxpants [debug]"
End Sub

Function inCommand (a As String) As Integer
	Dim As Integer i
	i = 1
	While (Command (i) <> "")
		If Command (i) = a Then Return -1
		i = i + 1
	Wend
	Return 0
End Function

Sub resetScript (n As Integer)
	Dim As Integer i
	For i = 0 To n: script (i) = "": Next i
	For i = 0 To 16383: myBin (i) = 0: Next i
	For i = 0 To maxScr: decorated (i) = 0: Next i
End Sub

Function pVal (s As String) As Integer
	Dim As Integer res, i
	If Left (s, 1) = "#" Then
		res = 128 + Val (Right (s, Len (s) - 1))
	ElseIf Left (s, 1) = "$" Then
		For i = 0 To LIST_ALIAS_SIZE
			If s = listAlias (i) Then res = i: Exit For
		Next i
	Else
		res = Val (s)
	End If

	Return res
End Function

Function pValL (s As String) As Integer
	Dim As Integer res, i

	If s = "SLOT_SELECTED" Then 
		res = slotFlag
	ElseIf s = "ITEM_SELECTED" Then
		res = itemFlag
	ElseIf Left (s, 1) = "#" Then
		res = pVal (Right (s, Len (s) - 1))
	ElseIf Left (s, 1) = "$" Then
		For i = 0 To LIST_ALIAS_SIZE
			If s = listAlias (i) Then res = i: Exit For
		Next i
	Else
		res = -1
	End If

	Return res
End Function

Function pValR (s As String) As Integer
	Dim As Integer res, i
	If s = "SLOT_SELECTED" Then 
		res = 128 + slotFlag
	ElseIf s = "ITEM_SELECTED" Then
		res = 128 + itemFlag
	ElseIf Left (s, 1) = "#" Then
		res = 128 + Val (Right (s, Len (s) - 1))
	ElseIf Left (s, 1) = "$" Then
		For i = 0 To LIST_ALIAS_SIZE
			If s = listAlias (i) Then res = 128 + i: Exit For
		Next i
	Else
		res = Val (s)
	End If
	pValR = res
End Function

Sub ProcessItems (f As Integer)
	Dim terminado As Integer
	Dim linea As String
	terminado = 0
	While Not terminado
		Line input #f, linea
		linea = Trim (linea, Any chr (32) + chr (9))
		parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
		Select Case Ucase (lP (0))
			Case "SLOT_FLAG":
				slotFlag = val (lP (1))
			Case "ITEM_FLAG":
				itemFlag = val (lP (1))
			Case "LOCATION":
				itemSetX = val (lP (1))
				itemSetY = val (lP (2))
			Case "DISPOSITION":
				itemSetStep = val (lP (2))
				If lP (1) = "HORZ" then itemSetOr = 0 Else ItemSetOr = 1
			Case "SELECTOR":
				itemSelectClr = val (lP (1))
				itemSelectC1 = val (lP (2))
				itemSelectC2 = val (lP (3))
			Case "EMPTY":
				itemEmpty = val (lP (1))
			Case "SIZE":
				maxItem = val (lP (1))
			Case "END":
				terminado = Not 0
		End Select
		If eof (f) Then terminado = Not 0
	Wend
End Sub

Sub addAlias (nameA As String, flag As Integer)
	listAlias (flag) = nameA
End Sub

Sub addMacro ()
	Dim As Integer i
	For i = 0 To MACRO_SIZE
		macros (macrosPtr, i) = lP (i)
	Next i
	If macrosPtr < MAX_MACROS Then macrosPtr = macrosPtr + 1 Else Print "** WARNING ** MAX_MACROS reached! Next will overwrite last."
End Sub

Sub ProcessAlias (f As Integer)
	Dim terminado As Integer
	Dim linea As String
	terminado = 0
	While Not terminado
		Line input #f, linea
		linea = Trim (linea, Any chr (32) + chr (9))
		parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
		If lP (0) = "END" Then terminado = Not 0
		If Left (lP (0), 1) = "$" Then
			AddAlias lP (0), pValR (lP (1))
		ElseIf Left (lP (0), 1) = "%" Then
			AddMacro
		End If
		If eof (f) Then terminado = Not 0
	Wend
End Sub

Sub lPshiftLeft (first As Integer)
	Dim As Integer i
	For i = first To LIST_WORDS_SIZE
		lP (i - 1) = lP (i)
	Next i
End Sub

Function strictMscNumeric (word As String) As Integer
	Dim As Integer res, i
	Dim As String m
	If word = "" Then
		res = 0
	ElseIf Left (word, 1) = "$" Or Left (word, 1) = "#" Then
		res = -1
	Else
		res = -1
		For i = 1 To Len (word)
			m = Mid (word, i, 1)
			If m < "0" Or m > "9" Then
				res = 0: Exit For
			End If
		Next i
	End If

	Return res
End Function

Sub fixTokens
	Dim As Integer i 

	' Fixes:
	i = 0: While i < LIST_WORDS_SIZE
		If lP (i) = "" Then 
			Exit While
		ElseIf lP (i) = "FLAG" Then
			If Len (lP (i + 1)) Then
				If Left (lP (i + 1), 1) <> "$" Then
					lP (i + 1) = "#" & lP (i + 1)
				End If
				lPshiftLeft i + 1
			End If
		Else 
			i = i + 1
		End If
	Wend

	i = 0: While i < LIST_WORDS_SIZE
		If lP (i) = "" Then 
			Exit While
		ElseIf lP (i) = "SET" Or lP (i) = "INC" Or lP (i) = "DEC" Then
			If Not strictMscNumeric (lP (i + 1)) Then
				lP (i) = lP (i) & "_" & lP (i + 1)
				lPshiftLeft i + 2
			End If
		End If
		i = i + 1
	Wend
End Sub

Sub fixEquals (position As Integer)
	If lP (position) = "=" Then lPshiftLeft (position + 1)
End Sub

Function procesaClausulas (f As Integer) As String
	' Read clausules from 'f' and outputs a string

	Dim As String linea
	Dim As String clausulas, clausula

	Dim As Integer sc_terminado, estado
	Dim As Integer longitud
	Dim As Integer ai, itt, dF, opCode, lvalue, rvalue, i, value
	Dim As Integer fzx1, fzy1, fzx2, fzy2

	longitud = 0
	clausulas = ""

	' Add decorations
	If doIncludeDecos Then

		' Open file
		dF = FreeFile
		Open decoInclude For Input as #dF
			
			' Fetch screen
			sc_terminado = 0
			estado = 0
			While Not sc_terminado And Not Eof (dF)
				Line input #dF, linea
				linea = Trim (linea, Any chr (32) + chr (9))
				If linea <> "" Then
					parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
					If lP (0) = "ENTERING" and val (lP (2)) = decoFetch Then
						estado = 1
						sc_terminado = Not 0
					End If
				End If
			Wend

			If estado = 1 Then
				? "Decos " & decoFetch

				' Add a new clausule IF TRUE
				clausula = chr (&HF0)
				clausulasUsed (&HF0) = -1

				' Then
				clausula = clausula + Chr (255)

				' fetch section DECORATIONS
				sc_terminado = 0
				While Not sc_terminado And Not Eof (dF)
					Line input #dF, linea
					linea = Trim (linea, Any chr (32) + chr (9))
					parseTokenizeString linea, lP (), ",;()" & chr (9), "'"

					if lP(0) = "DECORATIONS" Then
						' Parse
						clausula = clausula + Chr (&HF4)
						actionsUsed (&HF4) = -1

						do
							Line input #dF, linea
							linea = Trim (linea, Any chr (32) + chr (9))
							if ucase (linea)="END" then sc_terminado = -1: exit do
							parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
							' X,Y,T -> XY T
							clausula = clausula + Chr ((Val(lP(0)) Shl 4) + (Val(lP(1)) And 15)) + Chr (Val(lP(2)))
						loop

						' End
						clausula = clausula + Chr (&HFF)
					End If
				Wend

				' End 
				clausula = clausula + Chr (&HFF)

				' Add length
				clausula = Chr (len (clausula)) + clausula

				' Add clausule
				clausulas = clausulas + clausula
			End If
		Close dF		
	End If

	sc_terminado = 0
	estado = 0
	clausula = ""
	If debug Then Print

	If f Then
		While Not sc_terminado And Not Eof (f)
			Line Input #f, linea
			linea = Trim (linea, Any Chr (32) + Chr (9))

			parseCleanTokens lP ()
			parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
			If lP (0) = "" Then Continue While
			
			fixTokens

			If debug Then 
				Print parseGlueTokens (lP ())
			End If

			If estado <> 1 Then
				' Conditions

				Select Case Ucase (lP (0))
					Case "IF":
						' Which condition?

						If Len (lP (1)) Then

							If Left (lP (1), 1) = "$" Or Left (lP (1), 1) = "#" Then
								' 0  1    2  3
								' IF FLAG OP RVALUE
								lvalue = pValL (lP (1))
								If lvalue = -1 Then Print "ERROR - Wrong lvalue": End

								Select Case lP (2)
									Case "=": opCode = &H10
									Case "<": opCode = &H11
									Case ">": opCode = &H12
									Case "<>", "!=": opCode = &H13
									Case Else:
										Print "ERROR - Wrong operand": End
								End Select

								clausula = clausula + Chr (opCode) + Chr (lvalue) + Chr (pValR (lP (3)))
								clausulasUsed (opCode) = -1							
							Else
								Select Case Ucase (lP (1))

									' Conditions about position
									Case "PLAYER_TOUCHES":
										clausula = clausula + chr (&H20) + chr (pValR (lP (2))) + chr (pValR (lP (3)))
										clausulasUsed (&H20) = -1
									Case "PLAYER_IN_X":
										clausula = clausula + chr (&H21) + chr (val (lP (2))) + chr (val (lP (3)))
										clausulasUsed (&H21) = -1
									Case "PLAYER_IN_X_TILES":
										fzx1 = val (lP (2)) * 16 - 15
										If fzx1 < 0 Then fzx1 = 0
										fzx2 = val (lP (3)) * 16 + 15
										If fzx2 > 255 Then fzx2 = 255
										clausula = clausula + chr (&H21) + chr (fzx1) + chr (fzx2)
										clausulasUsed (&H21) = -1
									Case "PLAYER_IN_Y":
										clausula = clausula + chr (&H22) + chr (val (lP (2))) + chr (val (lP (3)))
										clausulasUsed (&H22) = -1
									Case "PLAYER_IN_Y_TILES":
										fzx1 = val (lP (2)) * 16 - 15
										If fzx1 < 0 Then fzx1 = 0
										fzx2 = (val (lP (3)) + 1) * 16 + 15
										If fzx2 > 191 Then fzx2 = 191
										clausula = clausula + chr (&H22) + chr (fzx1) + chr (fzx2)
										clausulasUsed (&H22) = -1
									Case "NPANT":
										' 0  1     2    3
										' IF NPANT [OP] 10'
										If lP (2) <> "=" And lP (2) <> "!=" And lP (3) <> "<>" Then
											lP (3) = lP (2): lP (2) = "="
										End If
										Select Case lP (2)
											Case "=": opCode = &H50
											Case "<>", "!=": opCode = &H51
											Case Else
												Print "ERROR - Wrong operand": End
										End Select
										clausula = clausula + chr (opCode) + chr (pValR (lP (3)))
										clausulasUsed (opCode) = -1
									Case "NPANT_NOT"
										clausula = clausula + chr (&H51) + chr (pValR (lP (2)))
										clausulasUsed (&H51) = -1
									Case "LEVEL"
										Select Case lP (2)
											Case "=": opCode = &H80
											Case "<>", "!=": OpCode = &H81
											Case Else
												Print "ERROR - Wrong operand": End
										End Select
										clausula = clausula + chr (opCode) + chr (pValR (lP (3)))
										clausulasUsed (opCode) = -1

									' Conditions about enemies killed
									Case "ALL_ENEMIES_DEAD":
										clausula = clausula + chr (&H30)
										clausulasUsed (&H30) = -1
									Case "ENEMIES_KILLED_EQUALS":
										clausula = clausula + chr (&H31) + chr (pValR (lP (2)))
										clausulasUsed (&H31) = -1
									Case "ENEMIES_KILLED", "ENEMS_KILLED":
										clausula = clausula + chr (&H31) + chr (pValR (lP (3)))
										clausulasUsed (&H31) = -1

									' Conditions based on recent actions
									Case "JUST_PUSHED":
										clausula = clausula + chr (&H60)
										clausulasUsed (&H60) = -1
									Case "JUST_INTERACTED"
										clausula = clausula + chr (&H61)
										clausulasUsed (&H61) = -1
									Case "ARG"
										Select Case lP (2)
											Case "=":
												value = pValR (lP (3))
												opCode = &H62
											Case "~":
												value = pValL (lP (3))
												If value = -1 Then Print "ERROR - Wrong lvalue in ARG ~": End
												value = 128 + value
												opCode = &H63
											Case Else
												Print "ERROR - Wrong operand": End
										End Select
										clausula = clausula + chr (opCode) + chr (value)
										clausulasUsed (opCode) = -1

									' Conditions based upon # of collectable objects
									Case "PLAYER_HAS_OBJECTS"
										clausula = clausula + chr (&H40)
										clausulasUsed (&H61) = -1
									Case "OBJECT_COUNT"
										If lP(2) <> "=" Then Print "ERROR - Wrong operand": End
										clausula = clausula + chr (&H41) + chr (pValR (lP (3)))
										clausulasUsed (&H41) = -1

									' Conditions based on the timer
									Case "TIMER"
										Select Case lP (2)
											Case ">=": opCode = &H70
											Case "<=": opCode = &H71
											Case "=": opCode = &H72
											Case "<>", "!=": opCode = &H73
											Case Else
												Print "ERROR - Wrong operand": End
										End Select
										clausula = clausula + Chr (opCode) + Chr (pValR (lP (3)))
										clausulasUsed (opCode) = -1

									' T
									Case "TRUE"
										clausula = clausula + chr (&HF0)
										clausulasUsed (&HF0) = -1
								End Select
							End If

						End If
					Case "THEN":
						' Close conditions; changes state
						If clausula = "" Then 
							? "ERROR - THEN without IFs": End
						End If
						clausula = clausula + Chr (255)					
						estado = 1

					Case "END":
						' And end in state 0 closes this section
						If estado = 0 Then sc_terminado = -1

				End Select

			Else
				' Actions
				Select Case Ucase (lP (0))

					' Setters
					Case "SET":
						' 0   1      2 3
						' SET lvalue = rvalue
						lvalue = pValL (lP (1))
						If lvalue = -1 Then Print "ERROR - Wrong lvalue": End
						rValue = pValR (lP (3))
						If lP (2) <> "=" Then Print "ERROR - Syntax error in SET": End
						clausula = clausula + Chr (&H1) + Chr (lvalue) + Chr (rvalue)
						actionsUsed (&H1) = -1
					Case "SET_TILE"
						' 0        1 2 3 4'
						' SET_TILE x y = t'
						If lP (3) <> "=" Then Print "ERROR - Syntax error in SET_TILE": End
						clausula = clausula + Chr (&H20) + Chr (pValR (lP (1))) + Chr (pValR (lP (2))) + Chr (pValR (lP (4)))
						actionsUsed (&H20) = -1
					Case "SET_SAFE"
					    ' 0        1    2 3
						' SET_SAFE HERE
						' SET_SAFE np   x y
						If lP (1) = "HERE" Then
							clausula = clausula + Chr (&HEA)
							actionsUsed (&HEA) = -1
						Else
							clausula = clausula + Chr (&HE9) + Chr (pValR (lP (1))) + Chr (pValR (lP (2))) + Chr (pValR (lP (3)))
							actionsUsed (&HE9) = -1
						End If
					Case "SET_FIRE_ZONE":
						clausula = clausula + Chr (&H51) + Chr (Val (lP (1))) + Chr (Val (lP (2))) + Chr (Val (lP (3))) + Chr (Val (lP (4)))
						actionsUsed (&H51) = -1
					Case "SET_FIRE_ZONE_TILES":
						fzx1 = pValR (lP (1)) * 16 - 7: If fzx1 < 0 Then fzx1 = 0
						fzy1 = (pValR (lP (2)) + 1) * 16 - 15: If fzy1 < 0 Then fzy1 = 0
						fzx2 = pValR (lP (3)) * 16 + 15: If fzx2 > 255 Then fzx2 = 255
						fzy2 = (pValR (lP (4)) + 1) * 16 + 15: If fzy2 > 207 Then fzy2 = 207
						clausula = clausula + Chr (&H51) + Chr (fzx1) + Chr (fzy1) + Chr (fzx2) + Chr (fzy2)
						actionsUsed (&H51) = -1
					Case "SETX", "SET_X":
						fixEquals 1
						clausula = clausula + Chr (&H6B) + Chr (pValR (lP (1)))
						actionsUsed (&H6B) = -1
					Case "SETX", "SET_Y":
						fixEquals 1
						clausula = clausula + Chr (&H6A) + Chr (pValR (lP (1)))
						actionsUsed (&H6A) = -1
					Case "SETXY", "SET_XY":
						fixEquals 1
						clausula = clausula + Chr (&H68) + Chr (pValR (lP (1))) + Chr (pValR (lP (2)))
						actionsUsed (&H68) = -1
					Case "SET_TIMER"
						fixEquals 1
						clausula = clausula + Chr (&H70) + Chr (pValR (lP (1)))
						actionsUsed (&H70) = -1

					' Maths
					Case "INC", "DEC", "ADD", "SUB"
						' 0  1      2
						' OP lvalue rvalue
						lValue = pValL (lP (1))
						If lValue = -1 Then Print "ERROR - Wrong lvalue": End
						rValue = pValR (lP (2))
						Select Case Ucase (lP (0))
							Case "INC", "ADD": opCode = &H10
							Case "DEC", "SUB": opCode = &H11
						End Select
						clausula = clausula + Chr (opCode) + Chr (lValue) +  Chr (rValue)
						actionsUsed (opCode) = -1
					Case "SWAP"
						clausula = clausula + Chr (&H14) + Chr (pVal (lP (1))) + Chr (pVal (lP (2)))
						actionsUsed (&H14) = -1
					Case "FLIPFLOP"
						clausula = clausula + Chr (&H15) + Chr (pVal (lP (1)))
						actionsUsed (&H15) = -1

					Case "INC_LIFE"
						clausula = clausula + Chr (&H30) + Chr (pValR (lP (1)))
						actionsUsed (&H30) = -1
					Case "DEC_LIFE"
						clausula = clausula + Chr (&H31) + Chr (pValR (lP (1)))
						actionsUsed (&H31) = -1
					Case "RECHARGE"
						clausula = clausula + Chr (&HE2)
						actionsUsed (&HE2) = -1

					Case "INC_OBJECTS"
						clausula = clausula + Chr (&H40) + Chr (pValR (lP (2)))
						actionsUsed (&H40) = -1
					Case "DEC_OBJECTS"
						clausula = clausula + Chr (&H41) + Chr (pValR (lP (2)))
						actionsUsed (&H41) = -1
					
					' Printers
					Case "PRINT_TILE_AT"
						' 0              1  2  3 4
						' PRINT_TILE_AT (x, y) = t
						' 0             1  2  3 
						' PRINT_TILE_AT x, y, t
						If lP (3) = "=" Then lP (3) = lP (4)
						clausula = clausula + Chr (&H50) + Chr (pValR (lP (1))) + Chr (pValR (lP (2))) + Chr (pValR (lP (3)))
						clausulasUsed (&H50) = -1
					Case "SHOW"
						clausula = clausula + Chr (&HE1)
						actionsUsed (&HE1) = -1
					Case "TEXT"
						clausula = clausula + Chr (&HE3) + Chr (Len (lP (1)))
						For ai = 1 To Len (lP (1))
							If Mid (lP (1), ai, 1) = "_" Then
								clausula = clausula + Chr (32)
							Else
								clausula = clausula + Mid (lP (1), ai, 1)
							End If
						Next ai
						clausula = clausula + Chr (0)
						actionsUsed (&HE3) = -1
					Case "TEXTBOX"
						clausula = clausula + Chr (&HED) + Chr (pValR (lP (1)))
						actionsUsed (&HED) = -1
					Case "DECORATIONS"
						clausula = clausula + Chr (&HF4)
						actionsUsed (&HF4) = -1
						Do
							Line Input #f, linea
							linea = Trim (linea, Any Chr (32) + Chr (9))
							If Ucase (linea) = "END" Then Exit Do
							parseCleanTokens lP ()
							parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
							' X Y T -> XY T
							clausula = clausula + Chr ((Val (lP(0)) Shl 4) + (Val (lP (1)) And 15)) + Chr (Val (lP (2)))
						Loop
						clausula = clausula + Chr (&HFF)

					' Screen invalidation
					Case "REDRAW"
						clausula = clausula + Chr (&H6E)
						actionsUsed (&H6E) = -1
					Case "REENTER"
						clausula = clausula + Chr (&H6F)
						actionsUsed (&H6F) = -1
					Case "WARP_TO"
						clausula = clausula + Chr (&H6D) + Chr (pValR (lP (1))) + Chr (pval (lP (2))) + Chr (pval (lP (3)))
						actionsUsed (&H6D) = -1
					
					' Timer
					Case "TIMER_START"
						clausula = clausula + Chr (&H71)
						actionsUsed (&H71) = -1
					Case "TIMER_STOP"
						clausula = clausula + Chr (&H72)
						actionsUsed (&H72) = -1

					' Spritey things
					Case "ADD_CONTAINER"
						clausula = clausula + Chr (&H86) + Chr (Val (lP (2)) + 16 * (1 + Val (lP (3)))) + Chr (128 + pVal (lP (1))) 
						actionsUsed (&H86) = -1
					Case "ADD_SPRITE"
						clausula = clausula + Chr (&H86) + Chr (Val (lP (2)) + 16 * (1 + Val (lP (3)))) + Chr (pVal (lP (1))) 
						actionsUsed (&H86) = -1

					' Misc
					Case "FLICKER"
						clausula = clausula + Chr (&H32)
						actionsUsed (&H32) = -1
					Case "EXTERN"
						clausula = clausula + Chr (&HE4) + Chr (Val (lP (1)))
						actionsUsed (&HE4) = -1
					Case "PAUSE", "DELAY"
						clausula = clausula + Chr (&HE5) + Chr (pValR (lP (1)))
						actionsUsed (&HE5) = -1

					' OGT
					Case "MUSIC"
						If lP (1) = "OFF" Or lP (1) = "STOP" Then
							clausula = clausula + Chr (&HE6) + Chr (&HFF)
							actionsUsed (&HE6) = -1
						Else
							clausula = clausula + Chr (&HE6) + Chr (pValR (lP (1)))
							actionsUsed (&HE6) = -1
						End If
					Case "SOUND"
						clausula = clausula + Chr (&HE0) + Chr (pValR (lP (1)))
						actionsUsed (&HE0) = -1

					' End conditions
					Case "GAME", "GAME_OVER"
						clausula = clausula + Chr (&HF0)
						actionsUsed (&HF0) = -1
					Case "WIN", "WIN_GAME"
						clausula = clausula + Chr (&HF1)
						actionsUsed (&HF1) = -1
					Case "BREAK"
						clausula = clausula + Chr (&HF2)
						actionsUsed (&HF2) = -1

					' Finally
					Case "END":
						clausula = clausula + Chr (&HFF)
						clausula = Chr (len (clausula)) + clausula
						
						If debug Then
							For i = 1 To Len (clausula) 
								Print Hex (Asc (Mid (clausula, i, 1)), 2); " ";
							Next i
							Print 
						End If

						clausulas = clausulas + clausula
						estado = 0
						clausula = ""
				End Select
			End If
		Wend
	End If

	If clausulas <> "" Then clausulas = clausulas + Chr (255)
	procesaClausulas = clausulas
End Function

maxClausuleSize = 0
itemEmpty = -1

Dim As Integer fIn, fOut
Dim As Integer scriptCount, nSections, killing
Dim As Integer maxidx, i, j, binPt, clin
Dim As Integer keepGoing, sDone
Dim As String thisLevelConstantName, linea, clausulas

Print "MK1 v1.0 mscmk1 1.0 ~ ";
If Len (Command (3)) = 0 Then usage: End
debug = inCommand ("debug")

fIn = FreeFile
Open Command (1) For Input As #fIn
fOut = FreeFile
Open Command (2) For Output As #fOut

maxScr = Val (Command (3)) - 1
maxidx = 2 * Val (Command (3))
ReDim script (maxidx + 5)
ReDim addresses (maxidx + 5)

' Output code & data

Print #fOut, "// NES MK1 v1.0"
Print #fOut, "// Copyleft Mojon Twins 2013, 2015, 2017, 2018"
Print #fOut, ""
Print #fOut, "// " & Command (2) & " - script data and interpreter"
Print #fOut, "// generated by mscMK1 1.0 by the Mojon Twins"
Print #fOut, ""

scriptCount = 0
nSections = 0
killing = 0

Print "Parsing " & Command (2) & " ~ ";

keepGoing = -1
sDone = 0
decoInclude = ""
thisLevelConstantName = ""
resetScript maxidx + 5

While keepGoing
	If Eof (fIn) Then
		keepGoing = 0
		linea = ""
		lP (0) = "END_OF_LEVEL"
		killing = -1
	Else
		Line Input #fIn, linea
		linea = Trim (linea, Any Chr (32) + Chr (9))
		parseCleanTokens lP ()
		parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
	End If

	doIncludeDecos = 0

	Select Case lP (0)
		Case "LEVELID":
			thisLevelConstantName = lP (1)
			? "L" & scriptCount & " = " & thisLevelConstantName & " ~ ";
		Case "INC_DECORATIONS":
			decoInclude = lP (1)
			? "L" & scriptCount & " decos @ " & decoInclude & " ~ ";
		Case "ITEMSET":
			ProcessItems fIn
		Case "DEFALIAS":
			processAlias fIn
		Case "ENTERING":
			sDone = -1
			If lP (1) <> "GAME" And lP (1) <> "ANY" And decoInclude <> "" Then
				doIncludeDecos = -1
				decoFetch = Val (lP (2))
				decorated (decoFetch) = -1
			End If
			clausulas = procesaClausulas (fIn)
			parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
			If lP (1) = "GAME" Then
				script (maxidx) = clausulas
			ElseIf lP (1) = "ANY" Then
				script (maxidx + 1) = clausulas
			Else
				i = 2
				While i < LIST_WORDS_SIZE
					If lP (i) = "" Then Exit While
						script (2 * Val (lP (i))) = clausulas
					i = i + 1
				Wend
				nSections = nSections + 1
			End If
		Case "PRESS_FIRE"
			sDone = -1
			clausulas = procesaClausulas (fIn)
			parseTokenizeString linea, lP (), ",;()" & chr (9), "'"
			If lP (2) = "ANY" Then
				script (maxidx + 2) = clausulas
			Else
				i = 3
				While i < LIST_WORDS_SIZE
					If lP (i) = "" Then Exit While
						script (1 + 2 * Val (lP (i))) = clausulas
					i = i + 1
				Wend
				nSections = nSections + 1
			End If
		Case "ON_TIMER_OFF"
			sDone = -1
			script (maxidx + 3) = procesaClausulas (fIn)
			nSections = nSections + 1
		Case "PLAYER_GETS_COIN"
			sDone = -1
			script (maxidx + 4) = procesaClausulas (fIn)
			nSections = nSections + 1
		Case "PLAYER_KILLS_ENEMY"
			sDone = -1
			script (maxidx + 5) = procesaClausulas (fIn)
			nSections = nSections + 1
		Case "END_OF_LEVEL"
			' Decorate screens without explicit ENTERING SCREEN n if needed
			If decoInclude <> "" Then
				For i = 0 To maxScr
					If Not decorated (i) Then
						doIncludeDecos = -1
						decoFetch = i 
						clausulas = procesaClausulas (0)
						script (2 * i) = clausulas
						nSections = nSections + 1
					End If 
				Next i 
			End If

			If killing Then 	
				? "Finished. Sections: " & nSections & " ~ ";
			Else
				? "Level finished. Sections: " & nSections & " ~ ";
			End If

			If sDone Then
				' Write and reset
				binPt = (maxidx + 6) * 2
				For i = 0 To maxidx + 5
					If Len (script (i)) = 0 Then
						addresses (i) = 0
					Else
						addresses (i) = binPt
					End If 

					For j = 1 To Len (script (i))
						myBin (binPt) = Asc (Mid (script (i), j, 1))
						binPt = binPt + 1
					Next j

					myBin (i * 2) = addresses (i) And 255
					myBin (i * 2 + 1) = addresses (i) Shr 8
				Next i

				Print #fOut, "const unsigned char script_pool_" & scriptCount & " [] = {"
				
				j = 0: clin = 0
				Print #fOut, "/*            0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F */"
				Print #fOut, "/* " & hex (clin, 4) & " */ ";
				For i = 0 To binPt - 1
					Print #fOut, "0x" & Lcase (Hex (myBin (i), 2));
					j = j + 1: If j = 16 Then j = 0
					If i < binPt - 1 Then 
						Print #fOut, ", ";
						If j = 0 Then clin = clin + 1: Print #fOut, "": Print #fOut, "/* " & hex (clin, 4) & " */ ";
					End If
				Next i

				Print #fOut, ""
				Print #fOut, "};"
				Print #fOut, ""

				scriptCount = scriptCount + 1
				resetScript (maxidx + 5)
			End If

			sDone = 0
			decoInclude = ""
			thisLevelConstantName = ""
			nSections = 0
	End Select
Wend

' Write script pool index

Print #fOut, "const unsigned char * const script_pool [] = {"
For i = 0 To scriptCount - 1
	Print #fOut, "    script_pool_" & i;
	If i < scriptCount - 1 Then Print #fOut, ", " Else Print #fOut, ""
Next i
Print #fOut, "};"
Print #fOut, ""

' Write interpreter

Print #fOut, "#ifdef CLEAR_FLAGS"
Print #fOut, "void msc_clear_flags (void) {"
Print #fOut, "    memfill (flags, 0, MAX_FLAGS);"
Print #fOut, "}"
Print #fOut, "#endif"
Print #fOut, ""
Print #fOut, "unsigned char read_byte (void) {"
Print #fOut, "    return *script ++;"
Print #fOut, "}"
Print #fOut, ""
Print #fOut, "unsigned char read_vbyte (void) {"
Print #fOut, "    sc_c = *script ++;"
Print #fOut, "    if (sc_c & 128) return flags [sc_c & 127];"
Print #fOut, "    return sc_c;"
Print #fOut, "}"
Print #fOut, ""
Print #fOut, "void readxy (void) {"
Print #fOut, "    sc_x = read_vbyte ();"
Print #fOut, "    sc_y = read_vbyte ();"
Print #fOut, "}"
If actionsUsed (&H68) Or actionsUsed (&H6A) Or actionsUsed (&H6B) Or actionsUsed (&H6C) Or actionsUsed (&H6D) Then
	Print #fOut, ""
	Print #fOut, "void reloc_player (void) {"
	Print #fOut, "    prx = read_vbyte () << 4;        px = prx << FIXBITS;"
	Print #fOut, "    pry = (read_vbyte () << 4) + 16; py = pry << FIXBITS;"
	Print #fOut, "    player_stop ();"
	Print #fOut, "}"
End If
Print #fOut, ""
Print #fOut, "void run_script (unsigned char whichs) {"
Print #fOut, "    // read address offset from index"
Print #fOut, "    gp_gen = (unsigned char *) script_pool [level] + whichs + whichs;"
Print #fOut, "    rda = *gp_gen ++; rdb = *gp_gen;"
Print #fOut, "    script_result = 0;"
Print #fOut, "    sc_continuar = 0;"
Print #fOut, "    if (!(rda | rdb)) return;"
Print #fOut, "    script = (unsigned char *) script_pool [level] + rda + (rdb << 8);"
Print #fOut, ""
Print #fOut, "    while ((sc_c = read_byte ()) != 0xff) {"
Print #fOut, "        next_script = script + sc_c;"
Print #fOut, "        sc_terminado = sc_continuar = 0;"
Print #fOut, "        while (!sc_terminado) {"
Print #fOut, "            switch (read_byte ()) {"

' Conditions with IF FLAG

If clausulasUsed (&H10) Then
	' IF FLAG x = y
	' &H10 x y
	Print #fOut, "                case 0x10: readxy (); sc_terminado = (flags [sc_x] != sc_y); break;"
End If
If clausulasUsed (&H11) Then
	' IF FLAG x < y
	' &H11 x y
	Print #fOut, "                case 0x11: readxy (); sc_terminado = (flags [sc_x] >= sc_y); break;"
End If
If clausulasUsed (&H12) Then
	' IF FLAG x > y
	' &H12 x y
	Print #fOut, "                case 0x12: readxy (); sc_terminado = (flags [sc_x] <= sc_y); break;"
End If
If clausulasUsed (&H13) Then
	' IF FLAG x != y
	' &H13 x y
	Print #fOut, "                case 0x13: readxy (); sc_terminado = (flags [sc_x] == sc_y); break;"
End If

' Conditions about POSITION

If clausulasUsed (&H20) Then
	' IF PLAYER_TOUCHES (x, y)
	' &H20 x y
	Print #fOut, "                case 0x20: readxy (); sc_x <<= 4; sc_y = 16 + (sc_y << 4); sc_terminado = (!(prx + 7 >= sc_x && prx <= sc_x + 15 && pry + 15 >= sc_y && pry <= sc_y + 15)); break;"
End If
If clausulasUsed (&H21) Then
	' IF PLAYER_IN_X ('x1', 'x2')
	' &H21 'x' 'y'
	Print #fOut, "                case 0x21: sc_terminado = (!(prx >= read_byte () && prx <= read_byte ())); break;"
End If
If clausulasUsed (&H22) Then
	' IF PLAYER_IN_Y ('y1', 'y2')
	' &H22 'x' 'y'
	Print #fOut, "                case 0x22: sc_terminado = (!(pry >= read_byte () && pry <= read_byte ())); break;"
End If

If clausulasUsed (&H50) Then
	' IF NPANT = n
	' &H50 n
	Print #fOut, "                case 0x50: sc_terminado = (n_pant != read_vbyte ()); break;"
End If
If clausulasUsed (&H51) Then
	' IF NPANT <> n
	' &H51 n
	Print #fOut, "                case 0x51: sc_terminado = (n_pant == read_vbyte ()); break;"
End If

If clausulasUsed (&H80) Then
	' IF LEVEL = n
	' &H80 n
	Print #fOut, "                case 0x80: sc_terminado = (level != read_vbyte ()); break;"
End If
If clausulasUsed (&H80) Then
	' IF LEVEL <> n
	' &H81 n
	Print #fOut, "                case 0x81: sc_terminado = (level == read_vbyte ()); break;"
End If

' Conditions about enemies DEAD

If clausulasUsed (&H30) Then
	' IF ALL_ENEMIES_DEAD
	' &H30
	Print #fOut, "                case 0x30: sc_terminado = (pkilled != c_max_enems); break;"	
End If
If clausulasUsed (&H31) Then
	' IF ENEMIES_KILLED_EQUALS n
	' IF ENEMIES_KILLED = n
	' IF ENEMS_KILLED = n
	' &H31 n
	Print #fOut, "                case 0x31: sc_terminado = (pkilled != read_vbyte ()); break;"
End If

' Conditions based on recent actions

If clausulasUsed (&H60) Then
	' IF JUST_PUSHED
	' &60
	Print #fOut, "                case 0x60: sc_terminado = (!just_pushed); break;"
End If
If clausulasUsed (&H61) Then
	' IF JUST_INTERACTED
	' &61
	Print #fOut, "                case 0x61: sc_terminado = (!just_interacted); break;"
End If
If clausulasUsed (&H62) Then
	' IF ARG = n
	' &62 n
	Print #fOut, "                case 0x62: sc_terminado = (script_arg != read_vbyte ()); break;"
End If
If clausulasUsed (&H63) Then
	' IF ARG ~ 'n'
	' &63 'n'
	Print #fOut, "                case 0x63: sc_terminado = (script_arg != read_byte ()); break;"
End If

' Conditions based on collectable objects count
If clausulasUsed (&H40) Then
	' IF PLAYER_HAS_OBJECTS
	' &H40
	Print #fOut, "                case 0x40: sc_terminado = (pobjs == 0); break;"
End If
If clausulasUsed (&H41) Then
	' IF OBJECT_COUNT = n
	' &H41 n
	Print #fOut, "                case 0x41: sc_terminado = (pobjs != read_vbyte ()); break;"
End If

' Conditions based on TIMER
If clausulasUsed (&H70) Then
	' IF TIMER >= n
	' &H70 n
	Print #fOut, "                case 0x70: sc_terminado = (timer < read_vbyte ()); break;"
End If
If clausulasUsed (&H71) Then
	' IF TIMER <= n
	' &H71 n
	Print #fOut, "                case 0x71: sc_terminado = (timer > read_vbyte ()); break;"
End If
If clausulasUsed (&H72) Then
	' IF TIMER = n
	' &H72 n
	Print #fOut, "                case 0x72: sc_terminado = (timer != read_vbyte ()); break;"
End If
If clausulasUsed (&H73) Then
	' IF TIMER <> n
	' &H73 n
	Print #fOut, "                case 0x73: sc_terminado = (timer == read_vbyte ()); break;"
End If

' T
If clausulasUsed (&HF0) Then
	' IF TRUE
	' &HF0
	Print #fOut, "                case 0xf0: break;"
End If

' End conditions

Print #fOut, "                case 0xff: sc_terminado = sc_continuar = 1; break;"
Print #fOut, "            }"
Print #fOut, "        }"
Print #fOut, ""
Print #fOut, "        if (sc_continuar) {"
Print #fOut, "            fire_script_success |= sc_continuar;"
Print #fOut, "            sc_terminado = 0;"
Print #fOut, "            while (!sc_terminado) {"
Print #fOut, "                switch (read_byte ()) {"

' Actions

' Setter actions

If actionsUsed (&H1) Then
	' SET #x = y
	' &H11 x y
	Print #fOut, "                    case 0x01: readxy (); flags [sc_x] = sc_y; break;"
End If

If actionsUsed (&H20) Then
	' SET_TILE x y t
	' &H20 x y t
	Print #fOut, "                    case 0x20: readxy (); _x = sc_x; _y = sc_y; _t = read_vbyte (); map_set (); break;"
End If

If actionsUsed (&H51) Then
	' SET_FIRE_ZONE x1 y1 x2 y2
	' &H51 'x1' 'y1' 'x2' 'y2'
	Print #fOut, "                    case 0x51: f_zone_ac = 1; fzx1 = read_byte (); fzy1 = read_byte (); fzx2 = read_byte (); fzy2 = read_byte (); break;"
End If

If actionsUsed (&H68) Then
	' SET_XY x y
	' &H68 x y
	Print #fOut, "                    case 0x68: reloc_player (); break;"
End If
If actionsUsed (&H6A) Then
	' SET_Y x
	' &H6A x
	Print #fOut, "                    case 0x6a: py = read_vbyte () << 10; player_stop (); break;"
End If
If actionsUsed (&H6B) Then
	' SET_X x
	' &H6B x
	Print #fOut, "                    case 0x6b: px = read_vbyte () << 10; player_stop (); break;"
End If

If actionsUsed (&H70) Then
	' SET_TIMER n
	' &H70 n
	Print #fOut, "                    case 0x70: timer = read_vbyte (); break;"
End If

If actionsUsed (&HE9) Then
	' SET SAFE n, x, y
	' &HE9 n x y
	Print #fOut, "                    case 0xe9: n_pant_safe = read_vbyte (); px_safe = read_vbyte () << 10; py_safe = read_vbyte () << 10; break;"
End If
If actionsUsed (&HEA) Then
	' SET SAFE HERE
	' &HEA
	Print #fOut, "                    case 0xea: player_register_safe_spot (); break;"
End If

' Maths

If actionsUsed (&H10) Then
	' INC #n x
	' &H10 n x
	Print #fOut, "                    case 0x10: readxy (); flags [sc_x] += sc_y; break;"
End If
If actionsUsed (&H11) Then
	' DEC #n x
	' &H11 n x
	Print #fOut, "                    case 0x11: readxy (); flags [sc_x] -= sc_y; break;"
End If

If actionsUsed (&H14) Then
	' SWAP #x #y
	' &H14 x y
	Print #fOut, "                    case 0x14: readxy (); rda = flags [sc_x]; flags [sc_x] = flags [sc_y]; flags [sc_y] = rda; break;"
End If

If actionsUsed (&H15) Then
	' FLIPFLOP #n
	' &H15 n
	Print #fOut, "                    case 0x15: sc_x = read_vbyte (); flags [sc_x] = 1 - flags [sc_x]; break;"
End If

' Objects and life

If actionsUsed (&H30) Then
	' INC_LIFE n
	' &H30 n
	Print #fOut, "                    case 0x30: plife += read_vbyte (); break;"
End If
If actionsUsed (&H31) Then
	' DEC_LIFE n
	' &H31 n
	Print #fOut, "                    case 0x31: plife -= read_vbyte (); break;"
End If
If actionsUsed (&H30) Then
	' RECHARGE
	' &HE2
	Print #fOut, "                    case 0x30: plife = PLAYER_LIFE; break;"
End If
If actionsUsed (&H40) Then
	' INC_OBJECTS n
	' &H40 n
	Print #fOut, "                    case 0x40: pobjs += read_vbyte (); break;"
End If
If actionsUsed (&H41) Then
	' DEC_OBJECTS n
	' &H41 n
	Print #fOut, "                    case 0x41: pobjs -= read_vbyte (); break;"
End If

' Printer

If actionsUsed (&HE1) Then
	' SHOW
	' &HE1
	Print #fOut, "                    case 0xe1: ppu_waitnmi (); clear_update_list (); break;"
End If

If actionsUsed (&HE3) Then
	' TEXT "text"
	' &HE3 length characters* 0
	Print #fOut, "                    case 0xe3: _x = LINE_OF_TEXT_X; _y = LINE_OF_TEXT; gp_gen = script; sc_n = read_byte (); script += (sc_n + 1); pr_ul_str (); break;"
End If

If actionsUsed (&HED) Then
	' TEXTBOX n
	' &HED n
	Print #fOut, "                    case 0xed: gp_gen = custom_texts [read_vbyte ()]; textbox_do (); break;"
End If

If actionsUsed (&H50) Then
	' PRINT_TILE_AT x, y, n
	' &H50 x y n
	Print #fOut, "                    case 0x50: readxy (); _x = sc_x; _y = sc_y; _t = read_vbyte (); update_list_tile (); break;"
End If

If actionsUsed (&HF4) Then
	' DECORATIONS {xy, t}* END
	' &HF4 {xy, t}* &HFF
	Print #fOut, "                    case 0xf4: while (0xff != (sc_x = read_byte ())) { _x = sc_x >> 4; _y = sc_x & 15; _t = read_byte (); map_set (); ppu_waitnmi (); clear_update_list (); } break;"
End If
' Screen invalidation

If actionsUsed (&H6D) Then
	' WARP_TO n x y
	' &H6D n x y
	Print #fOut, "                    case 0x6d: n_pant = read_vbyte (); on_pant = 0xfe; reloc_player (); break;"
End If
If actionsUsed (&H6E) then
	' REDRAW
	' &H6E
	Print #fOut, "                    case 0x6e: ppu_waitnmi (); clear_update_list (); ppu_off (); rdx = 0; rdy = TOP_ADJUST; for (gpit = 0; gpit < 192; gpit ++) { _t = map_buff [gpit]; draw_tile (); _x = (_x + 2) & 31; if (!_x) _y += 2; } break;"
End If
If actionsUsed (&H6F) Then
	' REENTER
	' &H6F
	Print #fOut, "                    case 0x6f: on_pant = 0xfe; break;"
End If

' Timer

If actionsUsed (&H71) Then
	' TIMER_START
	' &H71
	Print #fOut, "                    case 0x71: timer_on = 1; break;"
End If
If actionsUsed (&H72) Then
	' TIMER_STOP
	' &H71
	Print #fOut, "                    case 0x72: timer_on = 0; break;"
End If

' Spritey things

If actionsUsed (&H86) Then
	' ADD_CONTAINER / ADD_SPRITE f x y
	' &H86 'f' 'x' 'y'
	Print #fOut, "                    case 0x86: sc_x = read_byte (); sc_y = read_byte (); containers_add (); break;"
End If

' Misc

If actionsUsed (&H32) Then
	' FLICKER
	' &H32
	Print #fOut, "                    case 0x32: pstate = EST_PARP; pctstate = 100; break;"
End If
If actionsUsed (&HE4) Then
	' EXTERN n
	' &HE4 n
	Print #fOut, "                    case 0xe4: do_extern_action (read_byte ()); break;"
End If
If actionsUsed (&HE5) Then
	' PAUSE n
	' &HE5 n
	Print #fOut, "                    case 0xe5: delay (read_vbyte ()); break;"
End If

' Ogt

If actionsUsed (&HE0) Then
	' SOUND n
	' &HE0 n
	Print #fOut, "                    case 0xe0: sfx_play (read_vbyte (), 1); break;"
End If
If actionsUsed (&HE6) Then
	' MUSIC n
	' &HE6 n
	Print #fOut, "                    case 0xe6: sc_n = read_vbyte (); if (sc_n == 0xff) music_stop () else music_play (sc_n); break;"
End If

' Exit conditions'

If actionsUsed (&HF0) Then
	' GAME OVER
	' &HF0
	Print #fOut, "                    case 0xf0: game_over = 1; return;"
End If
If actionsUsed (&HF1) Then
	' WIN GAME
	' &HF1
	Print #fOut, "                    case 0xf1: script_result = 1; return;"
End If
If actionsUsed (&HF2) Then
	' BREAK
	' &HF2
	Print #fOut, "                    case 0xf2: return;"
End If

' End actions

Print #fOut, "                    case 0xff: sc_terminado = 1; break;"
Print #fOut, "                }"
Print #fOut, "            }"
Print #fOut, "        }"
Print #fOut, "        script = next_script;"
Print #fOut, "    }"
Print #fOut, "}"

Close

Print "DONE!"
