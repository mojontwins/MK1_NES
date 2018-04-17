-- Anim Test.lua

function main ()

 local animtypes = {
  "AN_NONE",
  "AN_LOOPF",
  "AN_LOOPR",
  "AN_ONCE",
  "AN_ONCEH",
  "AN_PPFF",
  "AN_PPRR",
  "AN_PPRF",
  "AN_PPFR",
  "AN_ONCES"
 }

 local w = mappy.getValue(mappy.MAPWIDTH)
 local h = mappy.getValue(mappy.MAPHEIGHT)

 if (w == 0) then
  mappy.msgBox ("Anim Test", "You need to load or create a map first", mappy.MMB_OK, mappy.MMB_ICONINFO)
 else
 
  local anum = mappy.getValue(mappy.NUMBLOCKANIM)
  mappy.msgBox ("Anim Test", "This will create a new anim, use 'test.fmp' for the map.\nMap name: "..mappy.getValue(mappy.MAPFILENAME).."\n\nNumber of anims = "..anum, mappy.MMB_OK, mappy.MMB_ICONINFO)
  
  mappy.createAnim (anum)
  mappy.setAnimFrame (anum, mappy.ANMREFFRAME, 66)
  mappy.insertAnimFrame (anum, 0, 66)
  mappy.insertAnimFrame (anum, 1, 67)
  mappy.insertAnimFrame (anum, 2, 68)
  mappy.insertAnimFrame (anum, 3, 72)
  mappy.insertAnimFrame (anum, 3, 71)
  mappy.insertAnimFrame (anum, 3, 70)
  mappy.insertAnimFrame (anum, 3, 0)
  mappy.setAnimFrame (anum, 3, 1)
  mappy.cutAnimFrame (anum, 3)
  mappy.setAnimValue (anum, mappy.ANMDELAY, 4);
  mappy.setAnimValue (anum, mappy.ANMTYPE, mappy.AN_ONCE);
  mappy.msgBox ("Anim Test", "New anim created, anim "..anum.." numframes = "..mappy.numAnimFrames (anum)..". Delay = "..mappy.getAnimValue (anum, mappy.ANMDELAY)..", type: "..animtypes[mappy.getAnimValue (anum, mappy.ANMTYPE)+1].." ("..mappy.getAnimValue (anum, mappy.ANMTYPE)..")", mappy.MMB_OK, mappy.MMB_ICONINFO)

  mappy.updateScreen ()
 
 end
end

test, errormsg = pcall( main )
if not test then
    mappy.msgBox("Error ...", errormsg, mappy.MMB_OK, mappy.MMB_ICONEXCLAMATION)
end
