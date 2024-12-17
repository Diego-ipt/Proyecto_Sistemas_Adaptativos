import os
import csv
from collections import defaultdict

# Directorio con los archivos .csv
csv_directory = "./results_export/resultadosPorTh"  # Cambia esta ruta según tus necesidades

# Archivo de salida en formato .dat
output_dat = "results.dat"

def procesar_csv_y_crear_dat(csv_directory, output_dat):
    """
    Procesa archivos .csv y genera un archivo .dat con el formato deseado.
    """
    resultados = defaultdict(list)  # Almacena resultados agrupados por (N, M, t)

    # Recorre todos los archivos en el directorio
    for filename in os.listdir(csv_directory):
        if filename.endswith(".csv"):
            filepath = os.path.join(csv_directory, filename)
            # Extraer umbral (threshold) del nombre del archivo
            threshold = filename.split("-")[1].replace(".csv", "")  # Ejemplo: threshold-0.75.csv
            try:
                with open(filepath, 'r') as csvfile:
                    reader = csv.reader(csvfile)
                    next(reader)  # Saltar el encabezado
                    for row in reader:
                        # Extraer N, M, e I del nombre del archivo de la primera columna
                        archivo = row[0]
                        nmi_parts = archivo.split("-")
                        nstr = nmi_parts[0]  # N
                        length = nmi_parts[1]  # M

                        # Extraer calidad de los algoritmos
                        brkga = row[1]  # Columna BRKGA
                        greedy = row[2]  # Columna GREEDY
                        mht = row[3]  # Columna MHT

                        # Almacenar los resultados por clave (N, M, threshold)
                        resultados[(nstr, length, threshold)].append((mht, greedy, brkga))
            except Exception as e:
                print(f"Error al procesar {filepath}: {e}")

    # Escribir en el archivo .dat
    with open(output_dat, 'w') as datfile:
        # Escribir encabezado
        datfile.write("inst\tnstr\tlength\tt\tMHT\tGREEDY\tBRKGA\n")
        # Escribir cada línea agrupada por N, M, t
        for (nstr, length, t), datos in resultados.items():
            for i, (mht, greedy, brkga) in enumerate(datos):
                inst = i % 100 + 1  # Reiniciar contador cada 100 instancias
                datfile.write(f"{inst}\t{nstr}\t{length}\t{t}\t{mht}\t{greedy}\t{brkga}\n")

    print(f"Archivo .dat generado en {output_dat}")

if __name__ == "__main__":
    procesar_csv_y_crear_dat(csv_directory, output_dat)
