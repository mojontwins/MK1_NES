-- Example Placer script
-- called when the mouse moves and a button pressed that is set to Placer.lua

function main ()
 local x = mappy.getValue (mappy.MOUSEBLOCKX)
 local y = mappy.getValue (mappy.MOUSEBLOCKY)

 local blk = mappy.getValue (mappy.CURANIM)
 if (blk == -1) then
  blk = mappy.getValue (mappy.CURBLOCK)
 else
-- setBlock need anims in the format below (ie: anim 1 should be a value of -2)
  blk = -(blk+1)
 end

-- Place a '+' of blocks at current position
 if x > 0 then
  mappy.setBlock (x-1, y, blk)
 end
 if y > 0 then
  mappy.setBlock (x, y-1, blk)
 end
 mappy.setBlock (x, y, blk)
 if x < (mappy.getValue (mappy.MAPWIDTH)-1) then
  mappy.setBlock (x+1, y, blk)
 end
 if y < (mappy.getValue (mappy.MAPHEIGHT)-1) then
  mappy.setBlock (x, y+1, blk)
 end
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end

