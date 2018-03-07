@echo off
set path=..\..\cc65_2.13.2\bin\;%path%

for /F %%d in ('dir /ad /b') do (
	echo Building %%d . . .
	cd %%d\dev
	call compile.bat > nul
	cd ..\..\
	echo DONE!
)
