-- Export Flash actionscript
-- Thanks to Andre Michelle for the idea


function main ()
 if mappy.msgBox ("Export Flash actionscript", "This will export the current layer as an actionscript (anims are replaced with block 0)\n\nRun the script (you will be prompted for a filename to save as)?", mappy.MMB_OKCANCEL, mappy.MMB_ICONQUESTION) == mappy.MMB_OK then

  local w = mappy.getValue(mappy.MAPWIDTH)
  local h = mappy.getValue(mappy.MAPHEIGHT)

  if (w == 0) then
   mappy.msgBox ("Export Flash actionscript", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
  else

   local isok,asname = mappy.fileRequester (".", "Flash actionscript (*.as)", "*.as", mappy.MMB_SAVE)
   if isok == mappy.MMB_OK then

    if (not (string.sub (string.lower (asname), -3) == ".as")) then
     asname = asname .. ".as"
    end

    local isok,adjust = mappy.doDialogue ("Export Flash actionscript", "Adjust exported values by:", "0", mappy.MMB_DIALOGUE1)
    if isok == mappy.MMB_OK then

     adjust = tonumber (adjust)
     outas = io.open (asname, "w")
     outas:write ("var map = [\n");
     local y = 0
     while y < h do
      outas:write ("[")
      local x = 0
      while x < w do
       local mapval = mappy.getBlockValue (mappy.getBlock (x, y), mappy.BLKBG)
       mapval = mapval + adjust
       if mapval < 0 then
        mapval = 0
       end
       outas:write (tostring(mapval))
       if x < (w-1) then
        outas:write (",")
       else
        if y < (h-1) then
	 outas:write ("],\n")
	else
	 outas:write ("]\n")
	 end
       end
      x = x + 1
      end
      y = y + 1
     end
     outas:write ("]\n")
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
