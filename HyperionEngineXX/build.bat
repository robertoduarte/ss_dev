@ECHO Off
SET PROJECT_DIR=%~dp0
SET "PROJECT_DIR=%PROJECT_DIR:\=/%"

SET COMMON_DIR=%PROJECT_DIR%../Common/
SET PATH=%COMMON_DIR%msys_trimmed/usr/bin;%PATH%

SET YAUL_INSTALL_ROOT=%PROJECT_DIR%yaul/sh2eb-elf
SET YAUL_PROG_SH_PREFIX=
SET YAUL_ARCH_SH_PREFIX=sh2eb-elf
SET YAUL_ARCH_M68K_PREFIX=m68keb-elf
SET YAUL_BUILD_ROOT=%PROJECT_DIR%/yaul/libyaul
SET YAUL_BUILD=build
SET YAUL_CDB=0
SET YAUL_OPTION_DEV_CARTRIDGE=0
SET YAUL_OPTION_MALLOC_IMPL=tlsf
SET YAUL_OPTION_SPIN_ON_ABORT=1
SET YAUL_OPTION_BUILD_GDB=0
SET YAUL_OPTION_BUILD_ASSERT=1
SET SILENT=1
SET MAKE_ISO_XORRISO=%COMMON_DIR%msys_trimmed/usr/bin/xorriso

IF NOT EXIST %PROJECT_DIR%yaul/ (
  @REM Extract latest libyaul gcc and copy libwinpthread-1.dll so that intellisense works on vscode
  unzip %COMMON_DIR%sh2eb-elf.zip -d %PROJECT_DIR%yaul/
  cp %COMMON_DIR%msys_trimmed/usr/bin/libwinpthread-1.dll %PROJECT_DIR%yaul/sh2eb-elf/bin/

  @REM Get the libyaul version used at the time of creation of this project from git repository and compile,
  @REM it will also install necessary files into the previous extracted gcc folder.
  cd %PROJECT_DIR%yaul/
  git clone https://github.com/ijacquez/libyaul.git
  cd libyaul
  git checkout 4816a73104b8e5c8a322488f0fc2eabe07ad982a
  cmd /c make install
  cd %PROJECT_DIR%
)

IF EXIST *.iso (
  del /Q *.iso
)

IF EXIST *.cue (
  del /Q *.cue
)

IF EXIST build/ (
  del /Q build
)

make
