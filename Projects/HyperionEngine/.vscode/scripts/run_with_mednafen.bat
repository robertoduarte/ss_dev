@ECHO Off
SET EMULATOR_DIR=..\..\Emulators

if exist HyperionEngine.cue (
"%EMULATOR_DIR%\mednafen\mednafen.exe" HyperionEngine.cue
) else (
echo Please compile first !
)
