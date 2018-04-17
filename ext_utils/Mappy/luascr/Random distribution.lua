-- Random Distribution Script
-- Robert Edwards 2003

-- This script is in the public domain and can be used for any purpose
-- I just hope its useful as an example of binary IO in lua for mappy

shifttab = {}
shifttab[0] = 1
shifttab[1] = 256
shifttab[2] = 65536
shifttab[3] = 16777216

function ShowError(message)
    mappy.msgBox("Error ...", message, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end

function ShowMessage(message)
    mappy.msgBox("Message ...", message, mappy.MMB_OK, mappy.MMB_ICONNONE)
end

function ReadInt( file )
    acc = 0
    
    for i = 0,3 do
        a = string.byte( file:read(1) )
        acc = acc + (a * shifttab[i])
    end
    
    return acc
end

function ReadShort( file )
    acc = 0

    for i = 0,1 do
        a = string.byte( file:read(1) )
        acc = acc + (a * shifttab[i])
    end

    return acc
end

function ReadChar( file )
    a = string.byte( file:read(1) )
    return a
end


function main()

    if mappy.msgBox("Random Distribution Plugin", "This will create a semi-random map based upon an input 8-bit TGA bitmap, high index = high density of current block", mappy.MMB_OKCANCEL, mappy.MMB_ICONQUESTION ) == mappy.MMB_OK then
        isok, srcfile = mappy.fileRequester(".","Targa Bitmaps(*.tga)","*.TGA",mappy.MMB_OPEN )
        if isok then
            file = io.open( srcfile, "r+b" )
            idsize=ReadChar(file)

            cmaptype = ReadChar(file)
            if cmaptype ~= 0 and cmaptype ~= 1 then
                error("Incorrect type of targa file")
            end

            type = ReadChar(file)
            if type ~= 3 and type ~= 1 then
                error("Incorrect type of targa file")
            end
            
            -- skip the colormap info and origin info
            file:read(4+5)


            xsize = ReadShort(file)
            if mappy.getValue(mappy.MAPWIDTH) ~= xsize then
                error("Bitmap is wrong width")
            end
            
            
            ysize = ReadShort(file)
            if mappy.getValue(mappy.MAPHEIGHT) ~= ysize then
                error("Bitmap is wrong height")
            end

            bpp = ReadChar(file)
            if bpp ~= 8 then
                error("Incorrect color depth")
            end

            --ignore the image descriptor byte
            file:read(1)

            -- ignore the file identification
            file:read(idsize)
            
            if cmaptype == 1 then
                -- skip colour palette
                file:read(768)
		end

            if type == 1 then
                y = ysize-1
            else
                y = 0
            end

            mappy.copyLayer(mappy.getValue(mappy.CURLAYER),mappy.MPY_UNDO)

            cblock = mappy.getValue(mappy.CURBLOCK)
            for i=0,(ysize-1) do
                for x=0,(xsize-1) do
				io.write ("x=",x," y=",y,"\n")
                    rnd = ReadChar(file) / 256
                    if math.random() < rnd then
                        mappy.setBlock(x,y,cblock)
                    end
                end
                if type == 1 then
                    y = y - 1
                else
                    y = y + 1
                end
            end
            
            file:close()

            mappy.updateScreen()

        end
    end

end

test, errormsg = pcall( main )
if not test then
    ShowError(errormsg)
end
