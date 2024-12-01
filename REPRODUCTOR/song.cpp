#include "song.h"
#include <algorithm>
#include <stdexcept>  // Necesario para manejar las excepciones

// Función para convertir un string a minúsculas
string toLowerCase(const string& str) {
    string result = str;
    for (char& c : result) {
        c = tolower(c);
    }
    return result;
}

// Función segura para convertir un string a int

int safe_stoi(const string& str) {
    try {
        return std::stoi(str);  // Convierte la cadena a un entero
    } catch (const std::invalid_argument& e) {  // Captura una excepción de argumento inválido
        return 0;  // Si falla, devuelve 0 o algún valor por defecto
    } catch (const std::out_of_range& e) {  // Captura una excepción de rango
        return 0;
    }
}



