-- Merge layers
-- Merges ayer l1 and layer l2 to layer l3

function main ()
 local w = mappy.getValue(mappy.MAPWIDTH)
 local h = mappy.getValue(mappy.MAPHEIGHT)

 if (w == 0) then
  mappy.msgBox ("Merge layers", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
 else

  mappy.msgBox ("Merge layers", "Merges two layers, in the next dialogue enter the source layers and the destination layer separated by commas. All layers must exist, the destination can be one of the source layers. Highest block number is used. You can undo this operation with ctrl+z if destination is current layer.", mappy.MMB_OK, mappy.MMB_ICONINFO)
  local isok,l1,l2,l3 = mappy.doDialogue ("Merge layers", "Enter layer1,layer2,destlayer", "0,1,0", mappy.MMB_DIALOGUE2)
  if isok == mappy.MMB_OK then

   l1 = tonumber (l1)
   l2 = tonumber (l2)
   l3 = tonumber (l3)

   mappy.copyLayer(mappy.getValue(mappy.CURLAYER),mappy.MPY_UNDO)

   local y = 0
   while y < h do
    local x = 0
    while x < w do
     local b1 = mappy.getBlock (x, y, l1)
     local b2 = mappy.getBlock (x, y, l2)
     if b2 > b1 and b2 ~= 0 then
      b1 = b2
     end
     mappy.setBlock (x, y, b1, l3)
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
