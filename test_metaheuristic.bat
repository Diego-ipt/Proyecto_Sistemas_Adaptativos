@echo off
setlocal enabledelayedexpansion

:: Compilar el programa
g++ -o metaheuristic test_metaheuristic.cpp
if %ERRORLEVEL% neq 0 (
    echo Compilation failed. Exiting.
    exit /b 1
)

:: Definir los valores de N y M
set Ns=100 200
set Ms=300 600 800

:: Definir los umbrales (thresholds)
set thresholds=0.75 0.8 0.85

:: Crear archivo de salida
set "output_file=results_metaheuristic.csv"
echo N;M;Calidad Promedio;Desviación Estándar de Calidad;Tiempo Promedio para Mejor Solución (ms) > "%output_file%"

:: Función para ejecutar el programa y capturar calidad y tiempo
for %%threshold in (%thresholds%) do (
    for %%N in (%Ns%) do (
        for %%M in (%Ms%) do (
            for /L %%I in (1,1,10) do (
                set input_file=FFMS_all_instances\%%N-%%M-%%I.txt
                echo Ejecutando para N=%%N, M=%%M, instancia=%%I, threshold=%%threshold, archivo de entrada=!input_file!
                
                :: Ejecutar el programa y capturar solo las últimas dos líneas (calidad y tiempo)
                for /f "skip=2 tokens=1,2" %%a in ('metaheuristic prueba -i "!input_file!" -t 30 -th %%threshold') do (
                    set calidad=%%a
                    set tiempo=%%b
                    echo Calidad: !calidad!, Tiempo: !tiempo! ms
                    echo !calidad! >> temp_qual.txt
                    echo !tiempo! >> temp_time.txt
                )
            )
        )
    )
    
    :: Calcular los promedios y desviaciones
    call :calcular_estadisticas temp_qual.txt calidad_promedio calidad_std
    call :calcular_estadisticas temp_time.txt tiempo_promedio tiempo_std

    :: Guardar los resultados en el archivo CSV
    echo %%N;%%M;!calidad_promedio!;!calidad_std!;!tiempo_promedio! >> "%output_file%"
)

:: Función para calcular promedio y desviación estándar
:calcular_estadisticas
set "file=%1"
set "promedio_var=%2"
set "std_var=%3"
set sum=0
set count=0

for /f %%x in (%file%) do (
    set /a sum+=%%x
    set /a count+=1
)

:: Calcular el promedio
set /a promedio=sum/count

:: Calcular la desviación estándar (simplificado)
set varianza=0
for /f %%x in (%file%) do (
    set /a varianza+=((%%x-promedio)*(%%x-promedio))
)

set /a desviacion=sqrt(varianza/count)
set "%promedio_var%=%promedio%"
set "%std_var%=%desviacion%"


exit /b
pause
