@ECHO Off
SET COMMON_DIR=..\Common

if not exist build.cue (
    echo "ISO missing, please build first."
)

if exist build.cue (
    "%COMMON_DIR%\mednafen\mednafen.exe" build.cue
)
