' nescnv
' by na_th_an

#include "png.bi"
#include "fbgfx.bi"
#include once "crt.bi"

#define RGBA_R( c ) ( CUInt( c ) Shr 16 And 255 )
#define RGBA_G( c ) ( CUInt( c ) Shr  8 And 255 )
#define RGBA_B( c ) ( CUInt( c )        And 255 )
#define RGBA_A( c ) ( CUInt( c ) Shr 24         )

Sub Usage () 
	Print "** USO **"
	Print "   nescnv in.png"
	Print "Genera tileset.chr"
	Print
End Sub

dim as any ptr img 			' will contain the image loaded from the PNG
dim as FB.Image ptr img2
dim as String filename1
dim as String filename2
dim as integer i, j, x, y, xx, yy, xxx, yyy,f
dim as byte myb

private sub libpng_error_callback cdecl _
	( _
		byval png as png_structp, _
		byval p as png_const_charp _
	)

	print "libpng failed to load the image (" & *p & ")"
	Puts "libpng failed to load the image (" & *p & ")"
	sleep
	end 1

end sub

function imageread_png _
	( byref filename as string, _
		byval bpp as integer _
	) as FB.IMAGE ptr
'	Puts("Execute imageread_png")
' Puts("Vars path:"+path+" filename:"+filename+ " bpp:")
	dim as ubyte header(0 to 7)

	dim as FILE ptr fp = fopen(  filename, "rb" )
	if( fp = NULL ) then
		Puts( "could not open image file " & filename)
		return NULL
	end if

	if( fread( @header(0), 1, 8, fp ) <> 8 ) then
		Puts("couldn't read header filename:"+ filename)
		fclose( fp )
		return NULL
	end if

	if( png_sig_cmp( @header(0), 0, 8 ) ) then
		Puts( "png_sig_cmp() failed")
		fclose( fp )
		return NULL
	end if

	dim as png_structp png = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, @libpng_error_callback, NULL )
	if( png = NULL ) then
		Puts( "png_create_read_struct() failed")
		fclose( fp )
		return NULL
	end if

	dim as png_infop info = png_create_info_struct( png )
	if( info = NULL ) then
		Puts( "png_create_info_struct() failed")
		fclose( fp )
		return NULL
	end if

	png_init_io( png, fp )
	png_set_sig_bytes( png, 8 )

	png_read_info( png, info )

	dim as integer w, h, bitdepth, channels, pixdepth, colortype, rowbytes
	w = png_get_image_width( png, info )
	h = png_get_image_height( png, info )
	bitdepth = png_get_bit_depth( png, info )
	channels = png_get_channels( png, info )
	pixdepth = bitdepth * channels
	colortype = png_get_color_type( png, info )

	print filename & ": " & w & "x" & h & " bitdepth=" & bitdepth & " pixdepth=" & pixdepth & " channels=" & channels & " ";

	select case( colortype )
	case PNG_COLOR_TYPE_RGB
		print "rgb"
	case PNG_COLOR_TYPE_RGB_ALPHA
		print "rgba"
	case PNG_COLOR_TYPE_GRAY
		print "grayscale"
	case else
		print "unsupported color type"
		return NULL
	end select

	dim as FB.IMAGE ptr img = imagecreate( w, h )
	dim as ubyte ptr dst = cptr( ubyte ptr, img + 1 )

	png_set_interlace_handling( png )
	png_read_update_info( png, info )

	rowbytes = png_get_rowbytes( png, info )
	dim as ubyte ptr src = callocate( rowbytes )

	for y as integer = 0 to h-1
		png_read_row( png, src, NULL )

		select case( colortype )
		case PNG_COLOR_TYPE_RGB
			imageconvertrow( src, 24, dst, bpp, w )
			dst += img->pitch
		case PNG_COLOR_TYPE_RGB_ALPHA
			select case( bpp )
			case 24, 32
				for i as integer = 0 to rowbytes-1 step 4
					'' FB wants: &hAARRGGBB, that is
					'' &hBB &hGG &hRR &hAA (little endian)
					''
					'' libpng provides &hAABBGGRR, that is
					'' &hRR &hGG &hBB &hAA (little endian)
					''
					'' so we need to copy AA/GG as-is, and
					'' swap RR/BB
					dst[0] = src[i+2]
					dst[1] = src[i+1]
					dst[2] = src[i+0]
					dst[3] = src[i+3]
					dst += 4
				next

			case 15, 16
				'' No alpha supported, only RGB will be used
				imageconvertrow( src, 32, dst, bpp, w )
				dst += img->pitch
			end select

		case PNG_COLOR_TYPE_GRAY
			select case( bpp )
			case 24, 32
				for i as integer = 0 to rowbytes-1
					*cptr( ulong ptr, dst ) = rgb( src[i], src[i], src[i] )
					dst += 4
				next
			case 15, 16
				for i as integer = 0 to rowbytes-1
					pset img, (i, y), rgb( src[i], src[i], src[i] )
				next
			case else
				'' 8 bpp and less require a proper global palette,
				'' which contains the colors used in the image
				'for i as integer = 0 to rowbytes-1
				'	pset img, (i, jinfo.output_scanline-1), src[i]
				'next
				memcpy( dst, src, rowbytes )
				dst += img->pitch
			end select
		end select
	next

	deallocate( src )

	png_read_end( png, info )
	png_destroy_read_struct( @png, @info, 0 )
	fclose( fp )

	function = img
end function

If Len (Command (1)) = 0  Then 
	Usage
	End
End If

' Primero cargo el archivo de imagen
screenres 640, 480, 32, , -1

filename1 = Command (1)
img = imageread_png ( filename1,32 )

if img then
'
else
	puts "Failed to load"
end if

f=freefile
open "tileset.chr" for binary as #f

x = 0
y = 0
for i = 0 to 511

	' write 8x8 lsbits (1 if colour = RED or BLUE)
	For yy = 0 to 7
		myb = 0
		For xx = 0 to 7
			If RGBA_R(point (x + xx, y + yy, img)) <> 0 Or RGBA_B(point (x + xx, y + yy, img)) <> 0 Then
				myb = myb Or (1 Shl (7 - xx))
			End If
		Next xx
		put #f,,myb
	next yy
	
	' write 8x8 msbits (1 if colour = GREEN or BLUE)
	For yy = 0 to 7
		myb = 0
		For xx = 0 to 7
			If RGBA_G(point (x + xx, y + yy, img)) <> 0 Or RGBA_B(point (x + xx, y + yy, img)) <> 0 Then
				myb = myb Or (1 Shl (7 - xx))
			End If
		Next xx
		put #f,,myb
	next yy
	
	x = x + 8
	If x = 256 Then 
		x = 0
		y = y + 8
	End If
next i

close (f)

puts "�Todo correcto!"
