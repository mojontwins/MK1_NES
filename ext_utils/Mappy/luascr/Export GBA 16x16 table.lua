-- GBA 16x16 table

function main ()
 if mappy.msgBox ("GBA 16x16 table", "This will export the BG table and flip info of an 8x8 map made by 'Convert 16x16 blocks to 8x8' (as detailed in GBAMappy)\n\nRun the script (you will be prompted for a filename to save as)?", mappy.MMB_OKCANCEL, mappy.MMB_ICONQUESTION) == mappy.MMB_OK then

  local w = mappy.getValue(mappy.MAPWIDTH)
  local h = mappy.getValue(mappy.MAPHEIGHT)

  if (w == 0) then
   mappy.msgBox ("GBA 16x16 table", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
  else
 
   local isok,asname = mappy.fileRequester (".", "Textfiles (*.txt)", "*.txt", mappy.MMB_SAVE)
   if isok == mappy.MMB_OK then

    if (not (string.sub (string.lower (asname), -4) == ".txt")) then
     asname = asname .. ".txt"
    end

    outas = io.open (asname, "w")
    outas:write ("const unsigned short tilelookup[] = {\n");
    local blk = 0
    local maxblk = mappy.getValue (mappy.NUMBLOCKSTR)
    while blk < maxblk do
     local blkval = mappy.getBlockValue (blk, mappy.BLKBG)
     if mappy.getBlockValue (blk, mappy.BLKFLAG7) == 1 then
      blkval = blkval + 1024
     end
     if mappy.getBlockValue (blk, mappy.BLKFLAG8) == 1 then
      blkval = blkval + 2048
     end
     if blk == (maxblk-1) then
      outas:write (blkval)
     else
      outas:write (blkval..",")
      if (mappy.andVal (blk, 15) == 15) then
       outas:write ("\n")
      end
     end
     blk = blk + 1
    end
    outas:write ("\n};\n")
    outas:close ()

   end
  end
 end
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end
