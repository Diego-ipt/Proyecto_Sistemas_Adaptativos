import os
import csv
from collections import defaultdict

# Directorio con los archivos .csv
csv_directory = "./results_export/resultadosIndividuales"  # Cambia esta ruta según tus necesidades

# Directorio de salida para los archivos combinados
output_directory = "./results_export"
os.makedirs(output_directory, exist_ok=True)

def crear_csv_por_threshold(csv_directory, output_directory):
    """
    Crea un archivo .csv por cada threshold, combinando los resultados de BRKGA, GREEDY y MHT.
    """
    # Diccionario para agrupar datos por threshold
    threshold_data = defaultdict(lambda: defaultdict(dict))

    # Leer todos los archivos en el directorio
    for filename in os.listdir(csv_directory):
        if filename.endswith(".csv"):
            # Identificar algoritmo y threshold
            parts = filename.split("-")
            algoritmo = parts[0].upper()  # BRKGA, GREEDY, etc.
            threshold = parts[1].replace(".csv", "")  # Ejemplo: 0.75

            filepath = os.path.join(csv_directory, filename)
            try:
                with open(filepath, 'r') as csvfile:
                    reader = csv.reader(csvfile)
                    next(reader)  # Saltar encabezado
                    for row in reader:
                        archivo = row[0]
                        calidad = row[1]  # Ignorar el tiempo
                        # Guardar calidad en la estructura
                        threshold_data[threshold][archivo][algoritmo] = calidad
            except Exception as e:
                print(f"Error al procesar {filepath}: {e}")

    # Crear un archivo .csv por cada threshold
    for threshold, archivos in threshold_data.items():
        output_filepath = os.path.join(output_directory, f"threshold-{threshold}.csv")
        with open(output_filepath, 'w', newline='') as outfile:
            writer = csv.writer(outfile)
            # Escribir encabezado
            writer.writerow(["Archivo", "BRKGA", "GREEDY", "MHT"])
            # Escribir datos por archivo
            for archivo, algoritmos in archivos.items():
                writer.writerow([
                    archivo,
                    algoritmos.get("BRKGA", ""),
                    algoritmos.get("GREEDY", ""),
                    algoritmos.get("MHT", "")
                ])
        print(f"Archivo generado: {output_filepath}")

if __name__ == "__main__":
    crear_csv_por_threshold(csv_directory, output_directory)
