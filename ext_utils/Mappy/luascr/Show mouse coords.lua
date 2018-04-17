-- Show mouse coords

function main ()
 if (mappy.getValue(mappy.MAPWIDTH) == 0) then
  mappy.msgBox ("Show mouse coords", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
 else

  local x = mappy.getValue (mappy.MOUSEBLOCKX)
  local y = mappy.getValue (mappy.MOUSEBLOCKY)
  mappy.msgBox("Show mouse coords", "This script should be run from a keypress, see 'Key Shortcuts' in the docs. Mouse is over block "..x..", "..y..". Useful for writing scripts which modify the map at mouse location...", mappy.MMB_OK, mappy.MMB_ICONINFO)

 end
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end

