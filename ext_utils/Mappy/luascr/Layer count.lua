-- Layer Count

lnum = 0

function main ()
 local w = mappy.getValue(mappy.MAPWIDTH)
 local h = mappy.getValue(mappy.MAPHEIGHT)

 if (w == 0) then
  mappy.msgBox ("Layer Count", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
 else

  mappy.msgBox ("Layer Count", "Number of layers in map = "..lnum, mappy.MMB_OK, mappy.MMB_ICONINFO)

 end
end

function countlayer ()
 mappy.getBlock(0,0,lnum)
end

while 1 do
 lnum = lnum + 1
 test, errormsg = pcall( countlayer )
 if not test then
  break
 end
 if (lnum == 8) then
  break;
 end
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end
