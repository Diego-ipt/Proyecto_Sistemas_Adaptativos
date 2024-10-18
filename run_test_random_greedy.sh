#!/bin/bash

# Compilar el programa
g++ -o test_greedy test_greedy.cpp

# Definir los conjuntos de valores N y M
Ns=(100 200)
Ms=(300 600 800)

# Definir el valor del umbral
threshold=0.75
alpha=0 #para elegir promedio simple o ponderado, puede ser entre 0 y 1
# Inicializar arrays para almacenar los tiempos de ejecución y las calidades
declare -A execution_times
declare -A qualities

# Función para ejecutar el programa y capturar el tiempo de ejecución y la calidad
run_program() {
    local input_file=$1
    local threshold=$2
    local alpha=$3
    local output=$(./test_greedy -i "$input_file" -th "$threshold" -alpha "$alpha" )
    local execution_time=$(echo "$output" | sed -n '1p')
    local quality=$(echo "$output" | sed -n '2p')
    echo "$execution_time" "$quality"
}

# Recorrer cada par de valores N y M
for N in "${Ns[@]}"; do
    for M in "${Ms[@]}"; do
        for instance in $(seq -w 001 100); do
            input_file="./FFMS_all_instances/${N}-${M}-${instance}.txt"
            echo "Ejecutando para N=$N, M=$M, instancia=$instance, archivo de entrada=$input_file"
            
            # Ejecutar el programa y capturar los tiempos de ejecución y las calidades
            result=$(run_program "$input_file" "$threshold" "$alpha")
            execution_time=$(echo "$result" | awk '{print $1}')
            quality=$(echo "$result" | awk '{print $2}')
            
            # Almacenar los resultados
            execution_times["$N-$M"]+="$execution_time "
            qualities["$N-$M"]+="$quality "
        done
    done
done


# Función para calcular la media de un array de números
calculate_average() {
    local numbers=($1)
    local sum=0
    local count=${#numbers[@]}
    for num in "${numbers[@]}"; do
        sum=$(awk "BEGIN {print $sum + $num}")
    done
    local average=$(awk "BEGIN {print $sum / $count}")
    echo "$average"
}

calculate_std_dev() {
    local numbers=($1)
    local mean=$(calculate_average "$1")
    local sum_sq_diff=0
    local count=${#numbers[@]}
    for num in "${numbers[@]}"; do
        sum_sq_diff=$(awk "BEGIN {print $sum_sq_diff + ($num - $mean)^2}")
    done
    local variance=$(awk "BEGIN {print $sum_sq_diff / $count}")
    local std_dev=$(awk "BEGIN {print sqrt($variance)}")
    echo "$std_dev"
}

# Calcular y mostrar la media del tiempo de ejecución y la calidad para cada par de conjuntos
for N in "${Ns[@]}"; do
    for M in "${Ms[@]}"; do
        exec_times=(${execution_times["$N-$M"]})
        quals=(${qualities["$N-$M"]})
        
        avg_exec_time=$(calculate_average "${exec_times[*]}")
        std_dev_exec_time=$(calculate_std_dev "${exec_times[*]}")
        avg_quality=$(calculate_average "${quals[*]}")
        std_dev_quality=$(calculate_std_dev "${quals[*]}")
        echo "N=$N, M=$M: Tiempo de Ejecución Promedio = $avg_exec_time ms, Desviación Estándar = $std_dev_exec_time ms, Calidad Promedio = $avg_quality, Desviación Estándar = $std_dev_quality"
    done
done