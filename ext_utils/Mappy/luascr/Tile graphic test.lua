-- Tile graphic test


function main ()
 if mappy.msgBox ("Graphic test", "This is a test LUA script which generates a new block and graphic and shades it. In 8bit, the palette is set to greyscale. This script has no useful function and is provided so it can be modified for a purpose (see the text file luascr\\Tile graphic test.lua).\n\nRun the script?", mappy.MMB_OKCANCEL, mappy.MMB_ICONQUESTION) == mappy.MMB_OK then

  local w = mappy.getValue(mappy.BLOCKWIDTH)
  local h = mappy.getValue(mappy.BLOCKHEIGHT)
  local depth = mappy.getValue(mappy.BLOCKDEPTH)

  if (w == 0) then
   mappy.msgBox ("Graphic test", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
  else

   local p = 0
   local a,r,g,b
   while p < 256 do
-- a,r,g,b = mappy.getValue((mappy.PALETTEARGB)+p)
-- io.write("old index #",p," = ",a,",",r,",",g,",",b,"\n")
    a = 255
    r = p
    g = p
    b = p
-- io.write("rgb=",r,g,b,"\n")
    mappy.setValue((mappy.PALETTEARGB)+p, a, r, g, b)
    p = p + 1
   end

   local newblock = mappy.createBlock (1)
   local newgraphic = mappy.createGraphic (1)
   mappy.setBlockValue (newblock, mappy.BLKBG, newgraphic);
   mappy.setBlockValue (newblock, mappy.BLKUSER1, 123);
   mappy.setBlockValue (newblock, mappy.BLKFLAG1, 1);
   local y = 0
   while y < h do
    local x = 0
    while x < w do
     local pval = (x*(128/w))+(y*(128/h))
     if depth == 8 then
      mappy.setPixel (x, y, newgraphic, pval);
     else
      mappy.setPixel (x, y, newgraphic, 255, pval, pval, pval);
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
