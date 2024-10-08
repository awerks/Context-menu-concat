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
    echo [HKEY_CLASSES_ROOT\Directory\Background\shell\Concatenate Videos]
    echo @="Concatenate Videos"
    echo "Icon"="%fileDir%\\concat.exe"
    echo.
    echo [HKEY_CLASSES_ROOT\Directory\Background\shell\Concatenate Videos\command]
    echo @="%fileDir%\\concat.exe \"%%1\""
) > "%regFile%"

regedit.exe /s "%regFile%"
del "%regFile%"

endlocal
