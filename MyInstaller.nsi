!include "MUI2.nsh"
!include "LogicLib.nsh"

; Output file name
OutFile "DriveRelayInstaller.exe"

; Default installation directory
InstallDir "$PROGRAMFILES64\DriveRelay"

; Request admin privileges
RequestExecutionLevel admin

; Include an icon for the installer
Icon "ressources\icons\prog.ico"
; Set the installer logo (this is the image shown in the setup dialog)
!define MUI_INSTALLEDICON "ressources\icons\installer_logo.bmp"

; Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "LICENSE.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

; Custom page for desktop shortcut option
!insertmacro MUI_PAGE_CUSTOMFUNCTION_SHOW ShowCustomPage

; Variables for custom page
Var CHECK_DESKTOP_SHORTCUT

; Section for installing files
Section "DriveRelay" SEC01
    SetOutPath "$INSTDIR\bin"
    File "bin\DriveRelay.exe"
    File "bin\HIDRELAY.dll"
    File "ressources\icons\iconmanual.bmp"
    File "ressources\icons\logo.bmp"
    File "ressources\icons\prog.bmp"
    File "ressources\icons\test.bmp"
    File "ressources\icons\prog.ico"

    ; Create Start Menu shortcut
    CreateDirectory "$SMPROGRAMS\DriveRelay"
    CreateShortCut "$SMPROGRAMS\DriveRelay\DriveRelay.lnk" "$INSTDIR\bin\DriveRelay.exe" "$INSTDIR\bin\prog.ico"

    ; Create Desktop shortcut if checked
    ${If} ${CHECK_DESKTOP_SHORTCUT}
        CreateShortCut "$DESKTOP\DriveRelay.lnk" "$INSTDIR\bin\DriveRelay.exe" "$INSTDIR\bin\prog.ico"
    ${EndIf}
SectionEnd

; Section for uninstallation
Section "Uninstall"
    Delete "$DESKTOP\DriveRelay.lnk"
    Delete "$SMPROGRAMS\DriveRelay\DriveRelay.lnk"
    RMDir /r "$INSTDIR"
    RMDir "$SMPROGRAMS\DriveRelay"
SectionEnd

; Custom page functions
Function ShowCustomPage
    ; Create the custom page
    !insertmacro MUI_INSTALLOPTIONS_EXTRACT "CustomPage.ini"
FunctionEnd

; Define a custom page to include a checkbox for desktop shortcut
[CustomPage.ini]
[Settings]
NumFields=1

[Field 1]
Type=Checkbox
Text="Create a shortcut on the desktop"
Flags=NOTIFY
Left=0
Right=100%
Top=0
Bottom=12

; Save the selected option to the variable
Function un.onSelChange
    ${If} ${NSIS_CHECKBOX}
        StrCpy $CHECK_DESKTOP_SHORTCUT "1"
    ${Else}
        StrCpy $CHECK_DESKTOP_SHORTCUT "0"
    ${EndIf}
FunctionEnd
