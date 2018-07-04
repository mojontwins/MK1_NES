' Simple GUI by Lothar Schirm - https://www.freebasic-portal.de/projekte/simple-gui-98.html

'Color constants:

Const	black = &H000000, blue = &H0000FF, green = &H00FF00, cyan = &H00FFFF, _
		red = &HFF0000, magenta = &HFF00FF, yellow = &HFFFF00, grey = &HAAAAAA, _
		white = &HFCCCCCC

Type Button
	x As Integer	'Position left top
	y As Integer
	w As Integer	'Height
	h As Integer	'Width
	text As String	'label
End Type

Type Label
	x As Integer	'Position left top
	y As Integer
	w As Integer	'Height
	h As Integer	'Width
	text As String	'label
End Type

Type TextBox
	x As Integer	'Position left top
	y As Integer
	w As Integer	'Height
	h As Integer	'Width
	text As String	'label
End Type

Type ListBox
	x As Integer			'Position left top
	y As Integer
	w As Integer			'Height
	h As Integer			'Width
	buffer(1e4) As String	'Buffer for items
	imax As Integer 		'Maximum index of items stored in the buffer
	offset As Integer		'Index of first item in the listbox when items are scrolled
	nmax as Integer 		'maximum number of items visible in the listbox
	index As Integer		'Selected index
End Type

Type DataGrid
	x As Integer				'Position left top
	y As Integer
	w As Integer				'Height
	h As Integer				'Width
	m As Integer				'Row number 0 to m
	n As Integer				'Column Number 0 To n
	colwidth(100) As Integer	'Buffer for columnwidths
	colpos(100) As Integer		'Left position of each column
	buffer(100, 100) As String	'Buffer for items
	index_row As Integer		'Index of selected row
	index_col As Integer		'Index of selected column
End Type

Declare Sub OpenWindow(w As Integer, h As Integer, title As String)
Declare Function Window_Event_Close() As Integer
Declare Sub Button_Draw(btn As Button, colour As UInteger)
Declare Function Button_New(x As Integer, y As Integer, w As Integer, h As Integer, Text As String) As Button
Declare Function Button_Event(btn As Button) As Integer
Declare Sub SetText(x As Integer, y As Integer, w As Integer, h As Integer, text As String, colour As UInteger = Color, bg As UInteger = white)
Declare Sub EditText(x As Integer, y As Integer, w As Integer, h As Integer, ByRef text As String, ReadOnly As Integer = 0)
Declare Function Label_New (x As Integer, y As Integer, w As Integer, h As Integer, text As String, fg As uInteger = Color, bg As uInteger = white) As Label
Declare Function TextBox_New(x As Integer, y As Integer, w As Integer, h As Integer, text As String) As TextBox
Declare Sub TextBox_SetText(ByRef tb As TextBox, text As String)
Declare Function TextBox_GetText(tb As Textbox) As String
Declare Sub TextBox_Edit(ByRef tb As Textbox, ReadOnly As Integer = 0)
Declare Function TextBox_Event(tb As TextBox) As Integer
Declare Function ListBox_New(x As Integer, y As Integer, w As Integer, h As Integer) As ListBox
Declare Sub DisplayItems(lb As ListBox)
Declare Sub ListBox_Add(ByRef lb As ListBox, item As String)
Declare Function ListBox_GetMaxIndex(lb As ListBox) As Integer
Declare Sub ListBox_SetItem(ByRef lb As Listbox, index As Integer, item As String)
Declare Sub ListBox_Clear(ByRef lb As ListBox)
Declare Function ListBox_GetIndex(lb As ListBox) As Integer
Declare Function ListBox_GetItem(lb As ListBox, index As Integer) As String
Declare Function ListBox_Event(ByRef lb As ListBox) As Integer
Declare Function DataGrid_New(x As Integer, y As Integer, m As Integer, n As Integer, cw() As Integer) As DataGrid
Declare Sub DataGrid_SetItem(ByRef dg As DataGrid, i As Integer, j As Integer, item As String)
Declare Sub DataGrid_Clear(ByRef dg As DataGrid)
Declare Sub DataGrid_GetIndexes(dg As DataGrid, ByRef i As Integer, ByRef j As Integer)
Declare Function DataGrid_GetItem(dg As DataGrid, i As Integer, j As Integer) As String
Declare Sub DataGrid_EditItem(ByRef dg As DataGrid, i As Integer, j As Integer, ReadOnly As Integer = 0)
Declare Function DataGrid_Event(ByRef dg As DataGrid) As Integer
