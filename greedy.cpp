#include "distancias_hamilton_calidad.cpp"
#include "promedio_modificado.cpp"

std::tuple<std::string, int> greedyHeuristicFFMS(const std::vector<std::string>& dataset, const std::unordered_map<std::string,int>& substring_to_index, 
const std::unordered_map<int, std::string>& index_to_substring, int threshold){

    // matriz de posiciones de los substrings
    std::vector<std::vector<int>> positions;

    // Dividir los strings en substrings y mapearlos a posiciones
    for(const auto& str: dataset){
        positions.push_back(compressString(str, substring_to_index));
    }
    // Calcular los promedios de las columnas
    std::vector<int> column_means = calculateColumnMeans(positions);
    //
    std::string solution_string;
    for(int pos: column_means){
        solution_string += getSubstringByPosition(index_to_substring, pos);
    }
    // Evaluar la calidad de la solución
    int max_distance = 0;
    std::string farthest_string;
    for( const auto& str : dataset){
        int total_distance = 0;
        for (size_t i = 0; i + 3<= solution_string.size(); i += 3) {
            string substring_solution = solution_string.substr(i, 3);
            string substring_str = str.substr(i, 3);
            total_distance += hammingDistance(substring_solution, substring_str);
        }
        if (total_distance > max_distance){
            max_distance = total_distance;
            farthest_string = str;
        }
        // Si satisface el umbral, se devuelve la solución
        if (max_distance >= threshold){
            return std::make_tuple(solution_string, max_distance);
        }
    }

    return std::make_tuple(solution_string, max_distance);

}
