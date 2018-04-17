-- Swap palette indexes

function main ()
 local w = mappy.getValue(mappy.MAPWIDTH)
 local h = mappy.getValue(mappy.MAPHEIGHT)

 if (w == 0) then
  mappy.msgBox ("Swap palette indexes", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
 else

  local isok,index1,index2 = mappy.doDialogue ("Swap palette indexes", "Enter index1,index2", "0,0", mappy.MMB_DIALOGUE2)
  if isok == mappy.MMB_OK then
   local a1,r1,g1,b1 = mappy.getValue (mappy.PALETTEARGB+index1)
   local a2,r2,g2,b2 = mappy.getValue (mappy.PALETTEARGB+index2)
   mappy.setValue (mappy.PALETTEARGB+index1, a2, r2, g2, b2)
   mappy.setValue (mappy.PALETTEARGB+index2, a1, r1, g1, b1)
   mappy.msgBox ("Swap palette indexes", "index "..tostring(index1).." and "..tostring(index2).." swapped successfully", mappy.MMB_OK, mappy.MMB_ICONINFO)
  end
 end
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end
