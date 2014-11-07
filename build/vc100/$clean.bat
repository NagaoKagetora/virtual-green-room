@echo off

del /Q "*.sdf"
del /Q /A:H "*.suo"
if exist "ipch" (
    rd /S /Q "ipch"
)
for /F %%a in ('dir /B /AD') do (
    if exist "%%a/Win32" (
        rd /S /Q "%%a/Win32"
    )
    if exist "%%a/x64" (
        rd /S /Q "%%a/x64"
    )
    if exist "%%a/GeneratedFiles" (
        rd /S /Q "%%a/GeneratedFiles"
    )
)

pause