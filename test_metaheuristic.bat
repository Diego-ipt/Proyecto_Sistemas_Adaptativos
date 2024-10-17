@echo off
setlocal enabledelayedexpansion

REM Compilar el programa 
g++ -o metaheuristic metaheuristic.cpp 

rem Directorio donde está el archivo de prueba específico
set "ARCHIVO=FFMS_all_instances\100-300-001.txt"

REM Crear el archivo CSV y agregar encabezados (si no existe)
if exist results_metaheuristic.csv (
    del results_metaheuristic.csv
)

REM añadir encabezado de los campos al csv
echo N;M;tiempo_ejecucion(ms);calidad_solucion > results_metaheuristic.csv

rem Umbral de prueba (puedes cambiar este valor si es necesario)
set THRESHOLD=0.75

rem Obtener el nombre del archivo sin la extensión
set "FILENAME=100-300-001"

rem Extraer los valores de N, M e I del nombre del archivo
for /f "tokens=1-3 delims=-" %%a in ("!FILENAME!") do (
    set N=%%a
    set M=%%b
    set I=%%c
)

rem Mostrar qué archivo se está procesando
echo Ejecutando prueba con archivo: %ARCHIVO% (N=%N%, M=%M%, I=%I%)

rem Ejecutar el programa y almacenar los outputs en un txt
metaheuristic -i %ARCHIVO% -th %THRESHOLD% > temp_output.txt

for /f "tokens=3" %%x in ('findstr "Tiempo de Ejecución" temp_output.txt') do (
    set tiempo_ejecucion=%%x
)
for /f "tokens=3" %%x in ('findstr "Calidad" temp_output.txt') do (
    set calidad_solucion=%%x
)

rem Guardar los resultados en el archivo CSV
echo %N%;%M%;%tiempo_ejecucion%;%calidad_solucion% >> results_metaheuristic.csv

rem Eliminar el archivo temporal
del temp_output.txt

timeout /t 10

rem Fin del script
echo Todas las pruebas han sido ejecutadas y los resultados se han agregado al archivo CSV.

endlocal