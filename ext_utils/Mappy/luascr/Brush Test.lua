-- Brush Test.lua

function main ()
 local w = mappy.getValue(mappy.MAPWIDTH)

 if (w == 0) then
  mappy.msgBox ("Brush Test", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
 else
 
  bnum = mappy.getValue (mappy.NUMBRUSHES)
  mappy.msgBox ("Brush Test", "This will create a new brush and rename it.\nMap name: "..mappy.getValue(mappy.MAPFILENAME).."\n\nNumber of brushes = "..bnum, mappy.MMB_OK, mappy.MMB_ICONINFO)
  
-- This will create a brush and copy the area described
  mappy.createBrush (bnum, 0, 0, 16, 16)
  mappy.renameBrush (bnum, "Brush Test");
  mappy.msgBox ("Brush Test", "New brush created, "..mappy.getBrushValue (bnum, mappy.BRUSHNAME), mappy.MMB_OK, mappy.MMB_ICONINFO)

  mappy.updateScreen ()
 
 end
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end
