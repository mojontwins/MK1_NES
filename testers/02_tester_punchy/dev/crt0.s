; Startup code for cc65 and Shiru's NES library
; based on code by Groepaz/Hitmen <groepaz@gmx.net>, Ullrich von Bassewitz <uz@cc65.org>

; Old neslib integrated with FT2 by na_th_an

.if CNROM
NES_MAPPER				=3	;mapper number
NES_PRG_BANKS			=2	;number of 16K PRG banks, change to 2 for NROM256
NES_CHR_BANKS			=4	;number of 8K CHR banks
.else
NES_MAPPER				=0	;mapper number
NES_PRG_BANKS			=2	;number of 16K PRG banks, change to 2 for NROM256
NES_CHR_BANKS			=1	;number of 8K CHR banks
.endif
NES_MIRRORING			=0	;0 horizontal, 1 vertical, 8 four screen

FT_DPCM_OFF				=$ffc0	;samples offset, $c000 or higher, 64-byte steps
FT_SFX_STREAMS			=4	;number of sound effects played at once, can be 4 or less (faster)

.define FT_DPCM_ENABLE 	0	;zero to exclude all the DMC code
.define FT_SFX_ENABLE  	1	;zero to exclude all the sound effects code

.define SPEED_FIX		0	;zero if you want to handle PAL/NTSC speed difference by yourself

    .export _exit,__STARTUP__:absolute=1
    .export PAL_BUF, _PALUPDATE
	.import push0,popa,popax,_main;,zerobss;,copydata

; Linker generated symbols
	.import __RAM_START__   ,__RAM_SIZE__
	.import __ROM0_START__  ,__ROM0_SIZE__
	.import __STARTUP_LOAD__,__STARTUP_RUN__,__STARTUP_SIZE__
	.import	__CODE_LOAD__   ,__CODE_RUN__   ,__CODE_SIZE__
	.import	__RODATA_LOAD__ ,__RODATA_RUN__ ,__RODATA_SIZE__

    .include "zeropage.inc"


PPU_CTRL	=$2000
PPU_MASK	=$2001
PPU_STATUS	=$2002
PPU_OAM_ADDR=$2003
PPU_OAM_DATA=$2004
PPU_SCROLL	=$2005
PPU_ADDR	=$2006
PPU_DATA	=$2007
PPU_OAM_DMA	=$4014
PPU_FRAMECNT=$4017
DMC_FREQ	=$4010
CTRL_PORT1	=$4016
CTRL_PORT2	=$4017

OAM_BUF		=$0200
PAL_BUF		=$01c0

FRAMECNT1	=$00
FRAMECNT2	=$01
NTSCMODE	=$02
VRAMUPDATE	=$03
PAD_STATE	=$04	;2 bytes, one per controller
PAD_STATEP	=$06	;2 bytes
PAD_STATET	=$08	;2 bytes
FT_TEMP		=$0a	;7 bytes in zeropage
SCROLL_X	=$11
SCROLL_Y	=$12
PPU_CTRL_VAR=$13
PPU_MASK_VAR=$14
NAME_UPD_ADR=$15	;word
NAME_UPD_LEN=$17
PAL_PTR		=$18	;word
RAND_SEED	=$1a	;word
PALUPDATE	=$1c
_PALUPDATE  =$1c

TEMP		=$1d
SCROLL_X1   =$1e	;Added by mojon twins
PPU_CTRL_VAR1=$1f	;Added by mojon twins

PAD_BUF		=TEMP+1

PTR			=TEMP	;word
LEN			=TEMP+2	;word
NEXTSPR		=TEMP+4
SCRX		=TEMP+5
SCRY		=TEMP+6
SRC			=TEMP+7	;word
DST			=TEMP+9	;word

RLE_LOW		=TEMP
RLE_HIGH	=TEMP+1
RLE_TAG		=TEMP+2
RLE_BYTE	=TEMP+3


FT_BASE_ADR		=$0100	;page in RAM, should be $xx00

.define FT_THREAD      1;undefine if you call sound effects in the same thread as sound update
.define FT_PAL_SUPPORT	1   ;undefine to exclude PAL support
.define FT_NTSC_SUPPORT	1   ;undefine to exclude NTSC support

.segment "HEADER"

    .byte $4e,$45,$53,$1a
	.byte NES_PRG_BANKS
	.byte NES_CHR_BANKS
	.byte NES_MIRRORING|(NES_MAPPER<<4)
	.byte NES_MAPPER&$f0
	.res 8,0


.segment "STARTUP"

start:
_exit:

    sei
    ldx #$ff
    txs
    inx
    stx PPU_MASK
    stx DMC_FREQ
    stx PPU_CTRL		;no NMI

initPPU:
    bit PPU_STATUS
@1:
    bit PPU_STATUS
    bpl @1
@2:
    bit PPU_STATUS
    bpl @2

clearPalette:
	lda #$3f
	sta PPU_ADDR
	stx PPU_ADDR
	lda #$0f
	ldx #$20
@1:
	sta PPU_DATA
	dex
	bne @1

clearVRAM:
	txa
	ldy #$20
	sty PPU_ADDR
	sta PPU_ADDR
	ldy #$10
@1:
	sta PPU_DATA
	inx
	bne @1
	dey
	bne @1

clearRAM:
    txa
@1:
    sta $000,x
    sta $100,x
    sta $200,x
    sta $300,x
    sta $400,x
    sta $500,x
    sta $600,x
    sta $700,x
    inx
    bne @1

	lda #4
	jsr _pal_bright
	;jsr _pal_clear
	jsr _oam_clear

    ;jsr zerobss
	;jsr copydata

    lda #<(__RAM_START__+__RAM_SIZE__)
    sta	sp
    lda	#>(__RAM_START__+__RAM_SIZE__)
    sta	sp+1            ; Set argument stack ptr

	lda #%10000000
	sta <PPU_CTRL_VAR
	sta PPU_CTRL		;enable NMI
	lda #%00000110
	sta <PPU_MASK_VAR

waitSync3:
	lda <FRAMECNT1
@1:
	cmp <FRAMECNT1
	beq @1

detectNTSC:
	ldx #52				;blargg's code
	ldy #24
@1:
	dex
	bne @1
	dey
	bne @1

	lda PPU_STATUS
	and #$80
	sta <NTSCMODE

	jsr _ppu_off

	ldx #<MUSIC_data
	ldy #>MUSIC_data
	lda <NTSCMODE
	jsr FamiToneInit

	.if(FT_SFX_ENABLE)
	ldx #<sounds_data
	ldy #>sounds_data
	jsr FamiToneSfxInit
	.endif

	.if(!SPEED_FIX)
	lda #0
	sta <NTSCMODE
	.endif

	lda #$fd
	sta <RAND_SEED
	sta <RAND_SEED+1

	lda #0
	sta PPU_SCROLL
	sta PPU_SCROLL

	jmp _main			;no parameters

	.include "neslib.s"

.segment "RODATA"

MUSIC_data:
	.include "music.s"

	.if(FT_SFX_ENABLE)
sounds_data:
	.include "sounds.s"
	.endif

.segment "VECTORS"

    .word nmi	;$fffa vblank nmi
    .word start	;$fffc reset
   	.word irq	;$fffe irq / brk


.segment "CHARS"
	.if CNROM
	.incbin "tileset0.chr"
	.incbin "tileset1.chr"
	.incbin "tileset2.chr"
	.incbin "tileset3.chr"
	.else
	.incbin "tileset.chr"
	.endif
	