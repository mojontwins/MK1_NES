-- Read textfile
-- Reads a texfile of ASCII digits (0 to 9) into current layer
-- columns must be smaller than map width

function main ()

  local w = mappy.getValue(mappy.MAPWIDTH)
  local h = mappy.getValue(mappy.MAPHEIGHT)

  if (w == 0) then
   mappy.msgBox ("Read textfile", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
  else
 
   local isok,asname = mappy.fileRequester (".", "Textfile (*.txt)", "*.txt", mappy.MMB_OPEN)
   if isok == mappy.MMB_OK then

    mappy.copyLayer(mappy.getValue(mappy.CURLAYER),mappy.MPY_UNDO)

    infile = io.open (asname, "r")
    local y = 0
    while y < h do
     local x = 0
     while x < w do
      local blk = tonumber (infile:read (1))
      if (blk ~= nil) then
       mappy.setBlock (x, y, blk)
       x = x + 1
      else
       x = w
      end
     end
     y = y + 1
    end
    infile:close ()

    mappy.updateScreen()

   end
  end
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end
