@echo off
setlocal enabledelayedexpansion

REM Compilar el programa
g++ -o test_greedy test_greedy.cpp

REM Directorio donde están los archivos de prueba
set "CARPETA=FFMS_all_instances"

REM Crear el archivo CSV y agregar encabezados (si no existe)
if exist results_greedy.csv (
    del results_greedy.csv
)

REM añadir encabezado de los campos al csv
echo N;M;tiempo_ejecucion(ms);calidad_solucion > results_greedy.csv

REM Umbral de prueba (puedes cambiar este valor si es necesario)
set THRESHOLD=0.75
set ALPHA=0.1

REM Recorrer todos los archivos con la estructura N-M-I.txt en la carpeta
for %%f in (%CARPETA%\*.txt) do (
    REM Obtener el nombre del archivo sin la extensión
    set "FILENAME=%%~nf"
    
    REM Extraer los valores de N, M e I del nombre del archivo
    for /f "tokens=1-3 delims=-" %%a in ("!FILENAME!") do (
        set N=%%a
        set M=%%b
        set I=%%c
        
        REM Mostrar qué archivo se está procesando
        echo Ejecutando prueba con archivo: %%f (N=!N!, M=!M!, I=!I!)
        
        REM Ejecutar el programa y guardar directamente en el CSV
        test_greedy -i %%f -th !THRESHOLD! -alpha !ALPHA!
    )
)

REM Fin del script
echo Todas las pruebas han sido ejecutadas y los resultados se han agregado al archivo CSV.
pause
endlocal
