-- Solid rectangle
-- Sets an area of the map to a specified block

function main ()
 local w = mappy.getValue(mappy.MAPWIDTH)
 local h = mappy.getValue(mappy.MAPHEIGHT)

 if (w == 0) then
  mappy.msgBox ("Solid rectangle", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
 else

  local isok,rx,ry,rw,rh = mappy.doDialogue ("Solid rectangle", "Enter x,y,width,height", "0,0,10,10", mappy.MMB_DIALOGUE2)
  if isok == mappy.MMB_OK then

   rx = tonumber (rx)
   ry = tonumber (ry)
   rw = tonumber (rw)
   rh = tonumber (rh)

   if (rx < 0 or (rx+rw) > w or ry < 0 or (ry+rh) > h) then
    mappy.msgBox ("Solid rectangle", "A value was out of bounds, rectangle not drawn", mappy.MMB_OK, mappy.MMB_ICONINFO)
   else

    local blk = mappy.getValue (mappy.CURANIM)
    if (blk == -1) then
     blk = mappy.getValue (mappy.CURBLOCK)
    else
-- setBlock need anims in the format below (ie: anim 1 should be a value of -2)
     blk = -(blk+1)
    end

    mappy.copyLayer(mappy.getValue(mappy.CURLAYER),mappy.MPY_UNDO)

    local y = ry
    while y < (ry+rh) do
     local x = rx
     while x < (rx+rw) do
      mappy.setBlock (x, y, blk)
      x = x + 1
     end
     y = y + 1
    end

    mappy.updateScreen()

   end
  end
 end
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end
