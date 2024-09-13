#include <iostream>
#include <vector>
#include <cmath>
// Función para calcular la media normal
int sumatotal(const std::vector<double>& datos) {
    double suma = 0.0;

    // Sumar todos los elementos
    for (double valor : datos) {
        suma += valor;
    }

    // Calcular la media
    return suma;
}
/*
int main() {
    std::vector<double> datos = {64,62,63}; // Ejemplo de datos

    int suma = sumatotal(datos);
    int punto1=suma/ datos.size();//puntos medio (promedios)
    int punto2=(suma/ datos.size()+32)%64;//puntos medio (promedios)

    std::cout << "El punto medio entre los datos es: " << punto1<<" y "<< punto2;

    return 0;
}*/