@echo off
setlocal

set "fileDir=%cd%"
set "regFile=%fileDir%\AddConcatenateVideos.reg"
set "fileDir=%fileDir:\=\\%"

if not exist "%fileDir%\\concat.exe" (
    echo concat.exe not found. Compiling...
    
    gcc concat.c -o3 -o "concat.exe"
    
    if errorlevel 1 (
        echo Compilation failed. Exiting...
        exit /b 1
    )
) else (
    echo concat.exe found. No compilation needed.
)

(
    echo Windows Registry Editor Version 5.00
    echo.
    echo [HKEY_CLASSES_ROOT\Directory\shell\Concatenate Videos\command]
    echo @="%fileDir%\\concat.exe \"%%1\""
    echo [HKEY_CLASSES_ROOT\Directory\shell\Concatenate Videos]
    echo @="Concatenate this folder"
    echo "Icon"="%fileDir%\\concat.exe"
    echo [HKEY_CLASSES_ROOT\Directory\Background\shell\Concatenate Videos]
    echo @="Concatenate files in this folder"
    echo "Icon"="%fileDir%\\concat.exe"
    echo [HKEY_CLASSES_ROOT\Directory\Background\shell\Concatenate Videos\command]
    echo @="%fileDir%\\concat.exe \"%%V\""
) > "%regFile%"

regedit.exe /s "%regFile%"
del "%regFile%"

set "sendToFolder=%APPDATA%\Microsoft\Windows\SendTo"
set "shortcutFile=%sendToFolder%\Concatenate videos.lnk"

%SYSTEMROOT%\System32\WindowsPowerShell\v1.0\powershell.exe -command "$s=(New-Object -COMObject WScript.Shell).CreateShortcut('%shortcutFile%'); $s.TargetPath='%fileDir%\\concat.exe'; $s.Save()"

endlocal
