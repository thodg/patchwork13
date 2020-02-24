
;------   patchwork13.nsi   ------

;--------------------------------
;General

  !include "MUI.nsh"

  OutFile "patchwork13-0.1b.exe"

  ;Name and file
  Name "Patchwork13! version 0.1b"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\Patchwork13"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Patchwork13" ""

;--------------------------------
;Variables

  Var MUI_TEMP
  Var STARTMENU_FOLDER

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages
  !insertmacro MUI_PAGE_LICENSE "License.txt"
  !insertmacro MUI_PAGE_DIRECTORY

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Patchwork13"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER
  
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer functions
Function .onInit
  ;Prevent multiple instances
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "Patchwork13SetupMutex") i .r1 ?e'
  Pop $R0
  StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "Patchwork13! Setup is already running"
    Abort
FunctionEnd


;--------------------------------
;Installer Sections

Section "pw13" Sec_pw13
  SetDetailsPrint textonly
  DetailPrint "Installing pw13 ..."
  SetDetailsPrint listonly

  ;Store installation folder
  WriteRegStr HKCU "Software\Patchwork13" "" $INSTDIR

  SetOutPath "$INSTDIR\bin"
  SetOverwrite on

  !include "pw13.i.dll.nsh"

  File "libs\libxml2-2.dll"
SectionEnd


Section "pw13_std" Sec_pw13_std
  SectionIn 1

  SetDetailsPrint textonly
  DetailPrint "Installing pw13_std ..."
  SetDetailsPrint listonly

  SetOutPath "$INSTDIR\lib"

  !include "pw13_std.i.dll.nsh"
SectionEnd


Section "pw13_sdl" Sec_pw13_sdl
  SectionIn 2

  SetDetailsPrint textonly
  DetailPrint "Installing pw13_sdl ..."
  SetDetailsPrint listonly

  SetOutPath "$INSTDIR\bin"
  File "libs\SDL.dll"

  SetOutPath "$INSTDIR\lib"
  !include "pw13_sdl.i.dll.nsh"
SectionEnd


Section "pw13_sound" Sec_pw13_sound
  SectionIn 2

  SetDetailsPrint textonly
  DetailPrint "Installing pw13_sound ..."
  SetDetailsPrint listonly

  SetOutPath "$INSTDIR\lib"
  !include "pw13_sound.i.dll.nsh"
SectionEnd


Section "cluster" Sec_cluster
  SectionIn 3

  SetDetailsPrint textonly
  DetailPrint "Installing cluster ..."
  SetDetailsPrint listonly

  SetOutPath "$INSTDIR\bin"
  File "libs\pthreadGC2.dll"
  !include "cluster.i.dll.nsh"
SectionEnd


Section "pw13_gtk" Sec_pw13_gtk
  SectionIn 4

  SetDetailsPrint textonly
  DetailPrint "Installing pw13_gtk ..."
  SetDetailsPrint listonly

  SetOutPath "$INSTDIR"
  File "..\pw13_gtk\pw13_toolbox.glade"
  File "..\pw13_gtk\pw13_quit.glade"
  File "..\pw13_gtk\pw13_tb_cluster_pref.glade"
  File "..\pw13_gtk\pw13_cluster_servers.glade"
  File "..\pw13_gtk\pw13_patchwork.glade"
  File "..\pw13_gtk\pw13_pw_close.glade"
  File "..\pw13_gtk\pw13_patch.glade"

  SetOutPath "$INSTDIR\bin"
  File "..\pw13_gtk\src\pw13_gtk.exe"
  File "libs\libglade-2.0-0.dll"
  File "libs\libxml2.dll"

  SetOutPath "$INSTDIR\lib"
  !include "pw13_gtk.i.dll.nsh"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  ;Create shortcuts
  CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Patchwork13.lnk" "$INSTDIR\bin\pw13_gtk.exe"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_END

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd


Section "" SecEnd

  SetDetailsPrint textonly
  DetailPrint "Finished !"
  SetDetailsPrint listonly
SectionEnd


;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ; pw13
  !include "pw13.u.dll.nsh"
  Delete "$INSTDIR\bin\libxml2-2.dll"

  ; pw13_std
  !include "pw13_std.u.dll.nsh"

  ; pw13_sdl
  !include "pw13_sdl.u.dll.nsh"
  Delete "$INSTDIR\bin\SDL.dll"

  ; pw13_sound
  !include "pw13_sound.u.dll.nsh"

  ; cluster
  !include "cluster.u.dll.nsh"
  Delete "$INSTDIR\bin\pthreadGC2.dll"

  ; pw13_gtk
  !include "pw13_sdl.u.dll.nsh"
  Delete "$INSTDIR\bin\pw13_gtk.exe"
  Delete "$INSTDIR\bin\libglade-2.0-0.dll"
  Delete "$INSTDIR\bin\libxml2.dll"

  RMDir "$INSTDIR\bin"
  Delete "$INSTDIR\pw13_toolbox.glade"
  Delete "$INSTDIR\pw13_quit.glade"
  Delete "$INSTDIR\pw13_tb_cluster_pref.glade"
  Delete "$INSTDIR\pw13_cluster_servers.glade"
  Delete "$INSTDIR\pw13_patchwork.glade"
  Delete "$INSTDIR\pw13_pw_close.glade"
  Delete "$INSTDIR\pw13_patch.glade"

  RMDir "$INSTDIR\lib"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
    
  Delete "$SMPROGRAMS\$MUI_TEMP\Sequence 3D.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall.lnk"
  
  ;Delete empty start menu parent diretories
  StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"
 
  startMenuDeleteLoop:
	ClearErrors
    RMDir $MUI_TEMP
    GetFullPathName $MUI_TEMP "$MUI_TEMP\.."
    
    IfErrors startMenuDeleteLoopDone
  
    StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
  startMenuDeleteLoopDone:

  DeleteRegKey /ifempty HKCU "Software\Sequence3D"

SectionEnd
