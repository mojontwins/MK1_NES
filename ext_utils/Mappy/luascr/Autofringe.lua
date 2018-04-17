-- Autofringe Script
-- Robert Edwards 2003

-- This script is in the public domain and can be used for any purpose
-- I just hope its useful.


function ShowError(message)
    mappy.msgBox("Error ...", message, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end

function ShowMessage(message)
    mappy.msgBox("Message ...", message, mappy.MMB_OK, mappy.MMB_ICONNONE)
end

function TestBlock(i, j, testfor)
    toff = 0

    if mappy.getBlock(i,j) == testfor then
        return 0
    end

    if j-1 > 0  then
        if mappy.getBlock(i,j-1) == testfor then
            toff = mappy.orVal(1, toff)
        end
    end
    
    if j+1 < mappy.getValue(mappy.MAPHEIGHT) then
        if mappy.getBlock(i,j+1) == testfor then
            toff = mappy.orVal(2, toff)
        end
    end
    
    if i-1 > 0  then
        if mappy.getBlock(i-1,j) == testfor then
            toff = mappy.orVal(4, toff)
        end
    end

    if i+1 < mappy.getValue(mappy.MAPWIDTH) then
        if mappy.getBlock(i+1,j) == testfor then
            toff = mappy.orVal(8, toff)
        end
    end
    
    if mappy.andVal(toff,15) == 0 then
        -- this is a could be a corner piece
        -- so check the corners
        
        if i-1 > 0 then
        
            if j-1 > 0 then
                if mappy.getBlock(i-1,j-1) == testfor then
                    toff = mappy.orVal(16,toff)
                end
            end
            
            if j+1 < mappy.getValue(mappy.MAPHEIGHT) then
                if mappy.getBlock(i-1,j+1) == testfor then
                    toff = mappy.orVal(32,toff)
                end
            end
        
        end
        if i+1 < mappy.getValue(mappy.MAPWIDTH) then

            if j-1 > 0 then
                if mappy.getBlock(i+1,j-1) == testfor then
                    toff = mappy.orVal(64,toff)
                end
            end

            if j+1 < mappy.getValue(mappy.MAPHEIGHT) then
                if mappy.getBlock(i+1,j+1) == testfor then
                    toff = mappy.orVal(128,toff)
                end
            end

        end
        
        if toff == 16 then
            return 8
        end
        if toff == 32 then
            return 6
        end
        if toff == 64 then
            return 7
        end
        if toff == 128 then
            return 5
        end

    else
        -- try to identify the type of piece
        if toff == 2 then
            return 12
        end

        if toff == 1 then
            return 11
        end
        
        if toff == 4 then
            return 9
        end
        
        if toff == 8 then
            return 10
        end
        
        if toff == mappy.andVal(toff,10) then
            return 4
        end
        
        if toff == mappy.andVal(toff,6) then
            return 3
        end
        
        if toff == mappy.andVal(toff,9) then
            return 2
        end
        
        if toff == mappy.andVal(toff,5) then
            return 1
        end

    end
    
    return 0
end

function main()

    if mappy.msgBox("AutoFringe", "This will automatically fringe the selected block", mappy.MMB_OKCANCEL, mappy.MMB_ICONQUESTION ) == mappy.MMB_OK then

        mappy.copyLayer(mappy.getValue(mappy.CURLAYER),mappy.MPY_UNDO)

        cur_block = mappy.getValue(mappy.CURBLOCK)
        w,h = mappy.getValue(mappy.MAPWIDTH),mappy.getValue(mappy.MAPHEIGHT)

        for j = 0,(h-1) do
            for i= 0,(w-1) do
                a = TestBlock(i,j, cur_block)
                if a > 0 then
                    mappy.setBlock( i,j,cur_block+a)
                end
            end
        end

        mappy.updateScreen()

    end

end

test, errormsg = pcall( main )
if not test then
    ShowError(errormsg)
end
