-- Set palette index

function main ()
 local w = mappy.getValue(mappy.MAPWIDTH)
 local h = mappy.getValue(mappy.MAPHEIGHT)

 if (w == 0) then
  mappy.msgBox ("Set palette index", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
 else
  local isok,index,a,r,g,b = mappy.doDialogue ("Set palette index", "Enter index,alpha,red,green,blue", "0,255,0,0,0", mappy.MMB_DIALOGUE2)
  if isok == mappy.MMB_OK then
   mappy.setValue (mappy.PALETTEARGB+index, a, r, g, b)
   mappy.msgBox ("Set palette index", "index "..tostring(index).." set successfully", mappy.MMB_OK, mappy.MMB_ICONINFO)
  end
 end
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end
