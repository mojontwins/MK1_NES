;RLE decompressor by Shiru (NESASM version)
;uses 4 bytes in zero page
;decompress data from an address in X/Y to PPU_DATA

RLE_LOW		equ $00
RLE_HIGH	equ RLE_LOW+1
RLE_TAG		equ RLE_HIGH+1
RLE_BYTE	equ RLE_TAG+1


unrle
	stx RLE_LOW
	sty RLE_HIGH
	ldy #0
	jsr rle_byte
	sta RLE_TAG
.1
	jsr rle_byte
	cmp RLE_TAG
	beq .2
	sta PPU_DATA
	sta RLE_BYTE
	bne .1
.2
	jsr rle_byte
	cmp #0
	beq .4
	tax
	lda RLE_BYTE
.3
	sta PPU_DATA
	dex
	bne .3
	beq .1
.4
	rts

rle_byte
	lda [RLE_LOW],y
	inc RLE_LOW
	bne .1
	inc RLE_HIGH
.1
	rts