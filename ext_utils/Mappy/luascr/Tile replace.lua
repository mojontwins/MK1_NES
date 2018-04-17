function main ()
 local w = mappy.getValue(mappy.MAPWIDTH)
 local h = mappy.getValue(mappy.MAPHEIGHT)

 if (w == 0) then
  mappy.msgBox ("Tile replace", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
 else

  local isok,t1,t2 = mappy.doDialogue ("Remplazar", "Introduce tile1,tile2", "0,1", mappy.MMB_DIALOGUE2)
  if isok == mappy.MMB_OK then

   t1 = tonumber (t1)
   t2 = tonumber (t2)

   -- mappy.copyLayer(mappy.getValue(mappy.CURLAYER),mappy.MPY_UNDO)
   local y = 0
   while y < h do
    local x = 0
    while x < w do
     local b = mappy.getBlock (x, y)

     if b == t1 then
     	mappy.setBlock (x, y, t2)
     end
     
     x = x + 1
    end
    y = y + 1
   end

   mappy.updateScreen()

  end
 end
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end
