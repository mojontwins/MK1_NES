-- Set colourkey transparency

function main ()
 mappy.msgBox ("Set colourkey transparency", "This will change the colourkeys to the current still block which has the transparent colour in the top left pixel of its BG graphic", mappy.MMB_OK, mappy.MMB_ICONINFO)
 if (mappy.getValue(mappy.CURBLOCK) < 1) then
  mappy.msgBox ("Set colourkey transparency", "You need to load or create a map first, then select a still block which has the transparent colour in the top left pixel of its BG graphic (not block 0, currently selected still block is: "..mappy.getValue(mappy.CURBLOCK)..")", mappy.MMB_OK, mappy.MMB_ICONINFO)
 else
  local tcoli, tcola, tcolr, tcolg, tcolb
  if (mappy.getValue(mappy.BLOCKDEPTH) == 8) then
   tcoli = mappy.getPixel(0, 0, mappy.getBlockValue (mappy.getValue(mappy.CURBLOCK), mappy.BLKBG))
   tcola, tcolr, tcolg, tcolb = mappy.getValue (mappy.PALETTEARGB+tcoli)
  else
   tcoli = 0
   tcola, tcolr, tcolg, tcolb = mappy.getPixel(0, 0, mappy.getBlockValue (mappy.getValue(mappy.CURBLOCK), mappy.BLKBG))
  end
  mappy.setValue(mappy.TRANS8, tcoli)
  mappy.setValue(mappy.TRANSRED, tcolr)
  mappy.setValue(mappy.TRANSGREEN, tcolg)
  mappy.setValue(mappy.TRANSBLUE, tcolb)
  mappy.msgBox ("Set colourkey transparency", "Colourkeys set:\n\n8bit now index "..tcoli.."\n24bit now RGB "..tcolr..", "..tcolg..", "..tcolb, mappy.MMB_OK, mappy.MMB_ICONINFO)
 end
 mappy.updateScreen()
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end
