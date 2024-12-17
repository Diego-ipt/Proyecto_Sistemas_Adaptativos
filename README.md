# Proyecto_Sistemas_Adaptativos

Integrantes:
Diego Ignacio Perez Torres
Antonia Renata Montero López
Carlos Andrés Venegas Aguayo

para compilar greedy: g++ -o test_greedy test_greedy.cpp para ejecutar: ./test_greedy -i <instancia_problema> -th -alpha

El parámetro alpha es opcional. Si le otorgamos un valor a alpha, entonces nuestro greedy será aleatorizado, sino, alpha tomará valor 1 y será determinista.

para compilar metaheuristica de trayectoria (se ocupó una combinación de Simulated Annealing e Iterated Greedy Algorithm): g++ -o test_metaheuristic test_metaheuristic.cpp para ejecutar: ./test_metaheuristic nombreMetaheuristica -i <instancia_problema> -t tiempoMaxSegundos -th

Para ejecutar estos algoritmos, tanto Greedy como Metaheuristica, se necesita un dataset. En este caso se está utilizando un dataset para el FFMSP, ubicado en la carpeta FFMS_all_instances. Se consideraron thresholds de 0.75, 0.8, 0.85.

Los archivos run_test_random_greedy.sh y run_test_greedy.sh entregarán la información de la ejecución de las instancias para calcular la media de la calidad y su desviación estándar para cada N y M del dataset.

El archivo run_test_metaheuristic.sh entregará la información de la ejecución de cada instancia para calcular el tiempo de ejecución promedio, desviación estándar de la solución, y la media de la mejor solución encontrada para cada N y M del dataset.

Para ejecutar los archivos .sh: ./nombre_archivo.sh

Para otorgar permisos para ejecutar .sh:. chmod +x nombre_archivo.sh

####### para brkgaAPI
Plain driver: a sample driver and Decoder class that make use of the BRKGA API. Additional
documentation can be found in the source file api-usage.cpp.

To recompile and run the program, type:
	cd ./examples/api-usage
	make clean
	make
	./api-usage
	
To modify any of the BRKGA parameters, edit the file api-usage.cpp and recompile the program.


Para la metaheurística híbrida se utilizó una combinación de la metaheurística de trayectoria (MHT) y una metaheurística poblacional (MHP). No se utilizó brkga como MHP, sino que se creó una solución con componentes poblacionales. 

##Para compilar metaheuristica híbrida:
g++ -o merge_model merge_model.cpp

##Para ejecutar: 
./merge_model -alg <algorithm_name> -input <inputFileName> -th <threshold> -time <time> [optional parameters]

[optional_parameters]: 
  -pop_size <val>    	Population size
  -pm <val>         	Parameter pm
  -elite <val>		Number of elite individuals
  -max_error <num>	Number of maximum errors allowed
  -temp_pert <val>    	Temperature perturbation
  -temp_leap <val>    	Temperature leap
  -cool_rate <val>   	Cooling rate
  -heat_rate <val>      Heat rate
  -tuning <val>         Enable tuning mode
  -iter_max <num>       Number of maximum interations
  -N_parents <num>      Number of Parents for crossover
  -n_part <num>        	Number of partitions in crossover



  Los resultados de todos los algoritmos descritos anteriormente están en la carpeta ./results_export. Los logs de tuning están en la carpeta ./tuning_logs
