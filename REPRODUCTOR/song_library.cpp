#include "song_library.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>  // Para sort

using namespace std;

// Cargar canciones desde el archivo CSV
void SongLibrary::loadSongs(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        return;
    }

    string line;
    getline(file, line);  // Leer la primera línea (encabezados)
    
    while (getline(file, line)) {
        stringstream ss(line);
        string index, artist_name, track_name, popularity, year, genre;

        getline(ss, index, ',');
        getline(ss, artist_name, ',');
        getline(ss, track_name, ',');
        getline(ss, popularity, ',');
        getline(ss, year, ',');
        getline(ss, genre, ',');

        Song song;
        song.index = safe_stoi(index);  // Convierte string a int de forma segura
        song.artist_name = artist_name;
        song.track_name = track_name;
        song.popularity = safe_stoi(popularity);
        song.year = safe_stoi(year);
        song.genre = genre;

        songs.push_back(song);  // Agregar la canción al vector de canciones
    }

    file.close();
    cout << "Canciones cargadas exitosamente desde " << filename << endl;
}

Song* SongLibrary::searchByTrackId(const string& track_id) {
    for (auto& song : songs) { // song es una referencia a un objeto Song
        if (song.index == safe_stoi(track_id)) {  // Compara con 'index', no con 'track_name'
            return &song;  // Devuelve la dirección del objeto Song
        }
    }
    return nullptr;  // Si no se encuentra, devuelve nullptr
}


vector<Song*> SongLibrary::searchByTrackName(const string& trackName) {
    string lowerTrackName = toLowerCase(trackName);
    vector<Song*> matches;
    
    for (auto& song : songs) {
        string lowerSongName = toLowerCase(song.track_name);
        
        // Compara el nombre de la canción en minúsculas
        if (lowerSongName.find(lowerTrackName) != string::npos) {
            matches.push_back(&song);  // Agrega la canción a los resultados
        }
    }

    // Si se encontraron canciones, las ordenamos por popularidad
    if (!matches.empty()) {
        sort(matches.begin(), matches.end(), [](Song* a, Song* b) {
            return a->popularity > b->popularity;
        });
    } else {
        cout << "No se encontraron canciones con ese nombre." << endl;
    }

    return matches;
}


// Buscar canción por nombre de artista
vector<Song*> SongLibrary::searchByArtistName(const string& artist_name) {
    vector<Song*> result;
    string lowerArtistName = toLowerCase(artist_name);
    for (auto& song : songs) {
        if (toLowerCase(song.artist_name).find(lowerArtistName) != string::npos) {
            result.push_back(&song);
        }
    }
    return result;
}
///////////////////////////
// Ordenar por popularidad

void SongLibrary::ordenarPorPopularidad() {
    std::sort(songs.begin(), songs.end(), [](const Song& a, const Song& b) {
        return a.popularity > b.popularity;
    });

    for (const Song& song : songs) {
        cout << song.track_name << " - " << song.artist_name << " (" << song.popularity << ")" << endl;
    }
}
void SongLibrary::ordenarPorAnio() {
    std::sort(songs.begin(), songs.end(), [](const Song& a, const Song& b) {
        return a.year > b.year;
    });

    for (const Song& song : songs) {
        cout << song.track_name << " - " << song.artist_name << " (" << song.year << ")" << endl;
    }
}

void SongLibrary::filtrarPorGenero(const string& genero) {
    for (const Song& song : songs) {
        if (toLowerCase(song.genre) == toLowerCase(genero)) {
            cout << song.track_name << " - " << song.artist_name << " (" << song.genre << ")" << endl;
        }
    }
}

//////////////////////
// Implementación de getSongsSortedByPopularity
vector<Song> SongLibrary::getSongsSortedByPopularity() {
    std::vector<Song> sortedSongs = songs; // Copia de objetos
    std::sort(sortedSongs.begin(), sortedSongs.end(), [](const Song& a, const Song& b) {
        return a.popularity > b.popularity;
    });
    return sortedSongs;
}

// Implementación de getSongsSortedByYear
vector<Song> SongLibrary::getSongsSortedByYear() {
    std::vector<Song> sortedSongs = songs; // Copia de objetos
    std::sort(sortedSongs.begin(), sortedSongs.end(), [](const Song& a, const Song& b) {
        return a.year > b.year;
    });
    return sortedSongs;
}


