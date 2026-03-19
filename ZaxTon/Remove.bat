batch
@echo off
echo Pulizia file temporanei Unreal in corso...

for %%d in (Intermediate Saved .vs Binaries DerivedDataCache Backup) do (
    if exist "%%d" (
        echo Eliminazione di %%d...
        rmdir /s /q "%%d"
    )
)

for %%f in (*.sln .vsconfig *.htm) do (
    if exist "%%f" (
        echo Eliminazione di %%f...
        del /q "%%f"
    )
)

echo Pulizia completata!
powershell -c "[console]::beep(1000,300)"
pause