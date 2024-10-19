# Proyecto_Sistemas_Adaptativos

Integrantes:
Diego Ignacio Perez Torres
Antonia Renata Montero López
Carlos Andrés Venegas Aguayo

para compilar greedy:
g++ -o test_greedy test_greedy.cpp
para ejecutar:
./test_greedy -i <instancia_problema> -th <threshold> -alpha <alpha>

El parámetro alpha es opcional. Si le otorgamos un valor a alpha, entonces nuestro greedy será aleatorizado, sino, alpha tomará valor 1 y será determinista.

para compilar metaheuristica de trayectoria (se ocupó una combinación de Simulated Annealing e Iterated Greedy Algorithm):
g++ -o test_metaheuristic test_metaheuristic.cpp
para ejecutar:
./test_metaheuristic nombreMetaheuristica -i <instancia_problema> -t tiempoMaxSegundos -th <threshold>

Para ejecutar estos algoritmos, tanto Greedy como Metaheuristica, se necesita un dataset. En este caso se está utilizando un dataset para el FFMSP, ubicado en la carpeta FFMS_all_instances. Se consideraron thresholds de 0.75, 0.8, 0.85.

Los archivos run_test_random_greedy.sh y run_test_greedy.sh entregarán la información de la ejecución de las instancias para calcular la media de la calidad y su desviación estándar para cada N y M del dataset.

El archivo run_test_metaheuristic.sh entregará la información de la ejecución de cada instancia para calcular el tiempo de ejecución promedio, desviación estándar de la solución, y la media de la mejor solución encontrada para cada N y M del dataset.

Para ejecutar los archivos .sh:
./nombre_archivo.sh

Para otorgar permisos para ejecutar .sh:.
chmod +x nombre_archivo.sh

