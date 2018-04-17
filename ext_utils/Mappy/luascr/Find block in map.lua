-- Find block in map

function main ()
 local w = mappy.getValue(mappy.MAPWIDTH)
 local h = mappy.getValue(mappy.MAPHEIGHT)

 if (w == 0) then
  mappy.msgBox ("Find block in map", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
 else

  mappy.msgBox ("Find block in map", "This will go to the next occurence of the currently selected block or anim", mappy.MMB_OK, mappy.MMB_ICONINFO)
    local blk = mappy.getValue (mappy.CURANIM)
    if (blk == -1) then
     blk = mappy.getValue (mappy.CURBLOCK)
    else
-- setBlock need anims in the format below (ie: anim 1 should be a value of -2)
     blk = -(blk+1)
    end

    local y = mappy.getValue (mappy.YMAPOFFSET)
    while y < h do
     local x = 0
     if (y == mappy.getValue (mappy.YMAPOFFSET)) then
      x = mappy.getValue (mappy.XMAPOFFSET) + 1
     end
     while x < w do
      if (mappy.getBlock (x, y) == blk) then
       mappy.msgBox ("Found block "..blk, "At "..x..", "..y, mappy.MMB_OK, mappy.MMB_ICONINFO)
       mappy.setValue (mappy.XMAPOFFSET, x)
       mappy.setValue (mappy.YMAPOFFSET, y)
       mappy.updateScreen()
       return
      end
      x = x + 1
     end
     y = y + 1
    end

    mappy.msgBox ("Find block in map", "No more blocks found, try moving to the top left of the map.", mappy.MMB_OK, mappy.MMB_ICONINFO)
    mappy.updateScreen()

 end
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end
