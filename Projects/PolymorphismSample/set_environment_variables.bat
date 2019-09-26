@ECHO Off
SET TOOLCHAIN_DIR=../../Toolchain
SET PATH=%TOOLCHAIN_DIR%/Compiler/SH_COFF/sh-coff/bin;%PATH%
SET PATH=%TOOLCHAIN_DIR%/Compiler/SH_COFF/sh-coff/sh-coff/bin;%PATH%
SET PATH=%TOOLCHAIN_DIR%/Compiler/SH_COFF/Other Utilities;%PATH%
SET PATH=%TOOLCHAIN_DIR%/msys32/usr/bin;%PATH%
SET PATH=%TOOLCHAIN_DIR%/Other;%PATH%

SET C_INCLUDE_PATH=%TOOLCHAIN_DIR%/Compiler/SH_COFF/sh-coff/sh-coff/include;%C_INCLUDE_PATH%