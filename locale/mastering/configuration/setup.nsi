;NSIS Modern User Interface
;Welcome/Finish Page Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include "gamedefinition.nsh"

;--------------------------------
;General

  ;Name and file
  Name "${TIKI_PRODUCT_NAME}"
  OutFile "${TIKI_OUTPUT_EXE}"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\${TIKI_PUBLISHER}\${TIKI_NAME}"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\${TIKI_PUBLISHER}\${TIKI_NAME}" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "The Game" SecGame

  SetOutPath "$INSTDIR"

  File "${TIKI_BINARY_PATH}\${TIKI_GAME_EXE}"

  SetOutPath "$INSTDIR\gamebuild"
  
  !include "..\output\scripts\install.nsh"
  
  ;Store installation folder
  WriteRegStr HKCU "Software\${TIKI_PUBLISHER}\${TIKI_NAME}" "" $INSTDIR

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecGame ${LANG_ENGLISH} "All files to play the Game."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecGame} $(DESC_SecGame)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  !include "..\output\scripts\uninstall.nsh"

  Delete "$INSTDIR\${TIKI_GAME_EXE}"
  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"
  RMDir "$INSTDIR\gamebuild"

  DeleteRegKey /ifempty HKCU "Software\${TIKI_PUBLISHER}\${TIKI_NAME}"

SectionEnd
