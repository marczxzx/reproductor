#ifndef SONG_H
#define SONG_H

#include <string>
using namespace std;

// Estructura para almacenar información de una canción
struct Song {
    int index;
    string artist_name;
    string track_name;
    int popularity;
    int year;
    string genre;
};

// Función para convertir un string a minúsculas
string toLowerCase(const string& str);

// Función segura para convertir un string a int
int safe_stoi(const string& str);

#endif // SONG_H


