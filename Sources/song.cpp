#include "Song.h"
#include <cctype>
using namespace std;

// Implementación de funciones auxiliares
string toLowerCase(const string& str) {
    string result = str;
    for (char& c : result) {
        c = tolower(c);
    }
    return result;
}

int safe_stoi(const string& str) {
    try {
        return stoi(str);
    } catch (const invalid_argument&) {
        return 0; // Valor predeterminado si la conversión falla
    }
}
