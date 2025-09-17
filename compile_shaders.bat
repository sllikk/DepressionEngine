@echo off

rem Путь к компилятору GLSL (укажи путь, если он не в системе)
set GLSLC=glslc.exe

rem Папки
set SHADER_DIR=shaders\
set OUTPUT_DIR=compiled_shaders\

rem Создаём выходную папку, если её нет
if not exist %OUTPUT_DIR% (
    mkdir %OUTPUT_DIR%
)

rem Цветной вывод
powershell -Command "Write-Host 'Compiling .vert shaders...' -ForegroundColor Cyan"
for %%f in (%SHADER_DIR%*.vert) do (
    powershell -Command "Write-Host '  Compiling %%~nxf' -ForegroundColor Green"
    %GLSLC% "%%f" -o "%OUTPUT_DIR%%%~nf.vert.spv"
)

powershell -Command "Write-Host 'Compiling .frag shaders...' -ForegroundColor Cyan"
for %%f in (%SHADER_DIR%*.frag) do (
    powershell -Command "Write-Host '  Compiling %%~nxf' -ForegroundColor Green"
    %GLSLC% "%%f" -o "%OUTPUT_DIR%%%~nf.frag.spv"
)

powershell -Command "Write-Host 'Compiling .comp shaders...' -ForegroundColor Cyan"
for %%f in (%SHADER_DIR%*.comp) do (
    powershell -Command "Write-Host '  Compiling %%~nxf' -ForegroundColor Green"
    %GLSLC% "%%f" -o "%OUTPUT_DIR%%%~nf.comp.spv"
)

powershell -Command "Write-Host 'Success!' -ForegroundColor Yellow"
pause
