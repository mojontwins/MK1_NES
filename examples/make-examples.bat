@echo off
set path=..\..\cc65_2.13.2\bin\;%path%

echo Making 00_tester_sideview
cd 00_tester_sideview\dev
call compile.bat > nul
cd ..\..\

echo Making 01_sgt_helmet_training_day
cd 01_sgt_helmet_training_day\dev
call compile.bat > nul
cd ..\..\

echo Making 02_jet_paco
cd 02_jet_paco\dev
call compile.bat > nul
cd ..\..\

echo Making 03_cheril_of_the_bosque_v1
cd 03_cheril_of_the_bosque_v1\dev
call compile.bat > nul
cd ..\..\

echo Making 04_d'veel'ng
cd 04_d'veel'ng\dev
call compile.bat > nul
cd ..\..\

echo Making 05_cheril_the_goddess_v1
cd 05_cheril_the_goddess_v1\dev
call compile.bat > nul
cd ..\..\

echo Making 06_lala_prologue
cd 06_lala_prologue\dev
call compile.bat > nul
cd ..\..\

echo Making 07_sir_ababol_dx_it1
cd 07_sir_ababol_dx_it1\dev
call compile.bat > nul
cd ..\..\

echo Making 08_sgt_helmet_training_day_MK1_1.0_port
cd 08_sgt_helmet_training_day_MK1_1.0_port\dev
call compile.bat > nul
cd ..\..\

echo DONE!
