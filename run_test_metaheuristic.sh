#!/bin/bash

# Compile the program
g++ -o metaheuristic metaheuristic.cpp
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi

# Define the sets of values for N and M
Ns=(100 200)
Ms=(300 600 800)

# Define the threshold values
thresholds=(0.75 0.8 0.85)

# Initialize arrays to store qualities and times to best solutions
declare -A qualities
declare -A times_to_best

# Function to run the program and capture quality and time to best solution
run_program() {
    local input_file=$1
    local threshold=$2
    local output=$(./metaheuristic -i "$input_file" -t 30 -th "$threshold")
    local quality=$(echo "$output" | sed -n '1p')
    local time_to_best=$(echo "$output" | sed -n '2p')
    echo "$quality" "$time_to_best"
}

# Iterate over each threshold value
for threshold in "${thresholds[@]}"; do
    # Initialize arrays for the current threshold
    declare -A qualities
    declare -A times_to_best

    # Iterate over each pair of N and M values
    for N in "${Ns[@]}"; do
        for M in "${Ms[@]}"; do
            for instance in $(seq -w 001 100); do
                input_file="./FFMS_all_instances/${N}-${M}-${instance}.txt"
                echo "Ejecutando para N=$N, M=$M, instancia=$instance, threshold=$threshold, archivo de entrada=$input_file"
                
                # Run the program and capture qualities and times to best solutions
                result=$(run_program "$input_file" "$threshold")
                quality=$(echo "$result" | awk '{print $1}')
                time_to_best=$(echo "$result" | awk '{print $2}')
                
                # Store the results
                key="$N-$M"
                qualities["$key"]+="$quality "
                times_to_best["$key"]+="$time_to_best "
            done
        done
    done

    # Function to calculate the average of an array of numbers
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

    # Function to calculate the standard deviation of an array of numbers
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

    # Create the CSV file and add headers
    output_file="results_metaheuristic_${threshold}.csv"
    echo "N;M;Calidad Promedio;Desviación Estándar de Calidad;Tiempo Promedio para Mejor Solución (ms);Desviación Estándar de Tiempo para Mejor Solución (ms)" > "$output_file"

    # Calculate and display the average quality and time to best solution for each pair of sets
    for N in "${Ns[@]}"; do
        for M in "${Ms[@]}"; do
            key="$N-$M"
            quals=(${qualities["$key"]})
            times_best=(${times_to_best["$key"]})
            
            avg_quality=$(calculate_average "${quals[*]}")
            std_dev_quality=$(calculate_std_dev "${quals[*]}")
            avg_time_to_best=$(calculate_average "${times_best[*]}")
            std_dev_time_to_best=$(calculate_std_dev "${times_best[*]}")
            
            echo "N=$N, M=$M: Calidad Promedio = $avg_quality, Desviación Estándar de Calidad = $std_dev_quality, Tiempo Promedio para Mejor Solución = $avg_time_to_best ms, Desviación Estándar de Tiempo para Mejor Solución = $std_dev_time_to_best ms"
            
            # Save the results to the CSV file
            echo "$N;$M;$avg_quality;$std_dev_quality;$avg_time_to_best;$std_dev_time_to_best" >> "$output_file"
        done
    done
done