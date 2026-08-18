#define MyAppName "Forge"
#define MyAppVersion "0.1.0"
#define MyAppPublisher "Suprim Ojha"
#define MyAppExeName "forge.exe"

[Setup]
AppId={{B8E6D9B5-6F1D-4E3E-9D77-FORGE01000001}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}

DefaultDirName={autopf}\Forge
DefaultGroupName=Forge

OutputDir=..\dist
OutputBaseFilename=Forge-{#MyAppVersion}-x64

ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

Compression=lzma
SolidCompression=yes

WizardStyle=modern

UninstallDisplayName=Forge
Uninstallable=yes

[Files]
Source: "..\build\forge.exe"; DestDir: "{app}"; Flags: ignoreversion

[Tasks]
Name: "addtopath"; Description: "Add Forge to the system PATH"; GroupDescription: "Additional options:"; Flags: checkedonce

[Code]
procedure AddToPath();
var
  OldPath: string;
  NewPath: string;
begin
  if not RegQueryStringValue(
    HKEY_LOCAL_MACHINE,
    'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
    'Path',
    OldPath
  ) then
    OldPath := '';

  if Pos(ExpandConstant('{app}'), OldPath) = 0 then
  begin
    if OldPath <> '' then
      NewPath := OldPath + ';' + ExpandConstant('{app}')
    else
      NewPath := ExpandConstant('{app}');

    RegWriteStringValue(
      HKEY_LOCAL_MACHINE,
      'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
      'Path',
      NewPath
    );
  end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if (CurStep = ssPostInstall) and WizardIsTaskSelected('addtopath') then
    AddToPath();
end;

[Icons]
Name: "{group}\Forge"; Filename: "{app}\forge.exe"

[Run]
Filename: "{cmd}"; Parameters: "/K forge --help"; WorkingDir: "{app}"; Flags: postinstall skipifsilent