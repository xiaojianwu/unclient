; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "UpdateNode Client"
#define MyAppVersion "1.0"
#define MyAppPublisher "UpdateNode UG (haftungsbeschr�nkt)"
#define MyAppURL "http://www.updatenode.com"
#define MyAppExeName "unclient.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{48EF0B65-2CD4-4372-BFCB-187C54D18BFC}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
OutputDir=..\installer
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes
CreateUninstallRegKey=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "..\package\unclient.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\package\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\package\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\package\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\package\QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\package\QtWebkit4.dll"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist
Source: "..\package\QtXml4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\package\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\package\vcredist_x86.exe"; DestDir: "{app}\3rdparty"; Flags: ignoreversion skipifsourcedoesntexist

[Run]
Filename: "{app}\3rdparty\vcredist_x86.exe"; Parameters: "/passive"; WorkingDir: "{app}\3rdparty"; StatusMsg: "Installing Micosoft's C/C++ runtime ..."; Flags: skipifdoesntexist