import os
import csv

# Directorio con los archivos .txt
directory = "./Ejecuciones/MeModel/th0.85"

# Archivo CSV de salida
output_csv = "hybrid-0.85.csv"

def leer_ultima_linea(filepath):
    """Lee la última línea significativa de un archivo."""
    with open(filepath, 'r') as file:
        lines = file.readlines()
        return lines[-1].strip()  # Devuelve la última línea como texto

def procesar_archivos_y_guardar_en_csv(directory, output_csv):
    """Procesa todos los archivos .txt en un directorio y guarda las últimas líneas en un CSV."""
    resultados = []

    # Recorre todos los archivos en el directorio
    for filename in os.listdir(directory):
        if filename.endswith(".salida_0"):
            filepath = os.path.join(directory, filename)
            try:
                ultima_linea = leer_ultima_linea(filepath)
                # Agregar nombre del archivo y datos a la lista de resultados
                resultados.append([filename, *ultima_linea.split()])
            except Exception as e:
                print(f"Error al procesar {filepath}: {e}")

    # Escribe los resultados en un archivo CSV
    archivo_existe = os.path.exists(output_csv)
    with open(output_csv, 'a', newline='') as csvfile:
        writer = csv.writer(csvfile)
        # Si el archivo no existía, escribe el encabezado
        if not archivo_existe:
            writer.writerow(["Archivo", "Calidad", "Tiempo"])
        # Escribe los datos
        writer.writerows(resultados)

    print(f"Resultados guardados en {output_csv}")

if __name__ == "__main__":
    procesar_archivos_y_guardar_en_csv(directory, output_csv)
