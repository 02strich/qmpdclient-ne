; The name of the installer
Name "QMPDClient"

; The file to write
OutFile "qmpdclient-installer.exe"

; The default installation directory
InstallDir $PROGRAMFILES\QMPDClient

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\QMPDClient" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "QMPDClient (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "qmpdclient.exe"
  File "mingwm10.dll"
  File "QtCore4.dll"
  File "QtGui4.dll"
  File "QtNetwork4.dll"
  File "growlnotify.exe"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\QMPDClient "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QMPDClient" "DisplayName" "QMPDClient"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QMPDClient" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QMPDClient" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QMPDClient" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\QMPDClient"
  CreateShortCut "$SMPROGRAMS\QMPDClient\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\QMPDClient\QMPDClient.lnk" "$INSTDIR\qmpdclient.exe" "" "$INSTDIR\qmpdclient.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QMPDClient"
  DeleteRegKey HKLM "Software\QMPDClient"

  ; Remove files and uninstaller
  Delete $INSTDIR\qmpdclient.exe
  Delete $INSTDIR\mingwm10.dll
  Delete $INSTDIR\QtCore4.dll
  Delete $INSTDIR\QtGui4.dll
  Delete $INSTDIR\QtNetwork4.dll
  Delete $INSTDIR\growlnotify.exe
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\QMPDClient\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\QMPDClient"
  RMDir "$INSTDIR"

SectionEnd
