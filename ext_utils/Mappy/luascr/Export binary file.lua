-- Export binary file
-- Thanks to Jerzy Kut for the num_to_char function

function num_to_char ( number )
 return ( string.char ( math.mod ( math.mod ( number, 256 ) + 256, 256 ) ) )
end

function writeIntLSB ( file, number )
 file:write ( num_to_char( number )) -- x>>0
 file:write ( num_to_char( number / 256 )) -- x>>8
 file:write ( num_to_char( number / 65536 )) -- x>>16
 file:write ( num_to_char( number / 16777216 )) -- x>>24
end

function main ()
 if mappy.msgBox ("Export binary file", "This example script will export the current layer as a binary file (CDXMap format) (anims are replaced with block 0)\nThis is the same as the default .map format when you save a .map file\n\nRun the script (you will be prompted for a filename to save as)?", mappy.MMB_OKCANCEL, mappy.MMB_ICONQUESTION) == mappy.MMB_OK then

  local w = mappy.getValue(mappy.MAPWIDTH)
  local h = mappy.getValue(mappy.MAPHEIGHT)

  if (w == 0) then
   mappy.msgBox ("Export binary file", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
  else

   local isok,asname = mappy.fileRequester (".", "CDXMap files (*.map)", "*.map", mappy.MMB_SAVE)
   if isok == mappy.MMB_OK then

    if (not (string.sub (string.lower (asname), -4) == ".map")) then
     asname = asname .. ".map"
    end

    local isok,adjust = mappy.doDialogue ("Export binary file", "Adjust exported values by:", "-1", mappy.MMB_DIALOGUE1)
    if isok == mappy.MMB_OK then

     adjust = tonumber (adjust)
-- open file as binary
     outas = io.open (asname, "wb")
     writeIntLSB (outas, w)
     writeIntLSB (outas, h)
     local y = 0
     while y < h do
      local x = 0
      while x < w do
       local mapval = mappy.getBlockValue (mappy.getBlock (x, y), mappy.BLKBG)
       mapval = mapval + adjust
       if mapval < 0 then
        mapval = 0
       end
       writeIntLSB (outas, mapval)
       x = x + 1
      end
      y = y + 1
     end
     outas:close ()

    end
   end
  end
 end
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end
