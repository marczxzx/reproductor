#include "playlist.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Constructor de la clase Playlist
Playlist::Playlist(const string& name) : playlistName(name) {}

// Obtener el nombre de la playlist
string Playlist::getName() const {
    return playlistName;
}

// Agregar una canción a la playlist
void Playlist::addSong(Song* song) {
    songs.push_back(song);
    songMap.emplace(toLowerCase(song->track_name), song);
}

// Eliminar una canción de la playlist
void Playlist::removeSong(const string& track_name) {
    string lowerName = toLowerCase(track_name);
    auto range = songMap.equal_range(lowerName);

    if (distance(range.first, range.second) > 1) {
        cout << "Hay varias canciones llamadas \"" << track_name << "\". Ingresa el nombre del artista: ";
        string artist;
        getline(cin, artist);
        bool songFound = false;
        for (auto it = range.first; it != range.second; ++it) {
            if (toLowerCase(it->second->artist_name) == toLowerCase(artist)) {
                songs.remove(it->second);  // Eliminar de la lista
                songMap.erase(it);         // Eliminar de la estructura de datos
                cout << "Canción eliminada: " << it->second->track_name << " - " << it->second->artist_name << endl;
                songFound = true;
                break;
            }
        }
        if (!songFound) {
            cout << "No se encontró la canción con el artista especificado." << endl;
        }
    } else if (range.first != songMap.end()) {
        cout << "Canción eliminada: " << range.first->second->track_name << endl;
        songs.remove(range.first->second);
        songMap.erase(range.first);
    } else {
        cout << "Canción no encontrada." << endl;
    }
}

// Buscar una canción en la playlist
Song* Playlist::searchSong(const string& track_name) const {
    string lowerName = toLowerCase(track_name);
    auto range = songMap.equal_range(lowerName);

    if (distance(range.first, range.second) > 1) {
        cout << "Hay varias canciones con el nombre \"" << track_name << "\". Ingresa el nombre del artista: ";
        string artist;
        getline(cin, artist);
        for (auto it = range.first; it != range.second; ++it) {
            if (toLowerCase(it->second->artist_name) == toLowerCase(artist)) {
                return it->second;
            }
        }
        cout << "No se encontró la canción con el nombre y artista especificados." << endl;
        return nullptr;
    } else if (range.first != songMap.end()) {
        return range.first->second;
    }

    cout << "Canción no encontrada." << endl;
    return nullptr;
}

// Mostrar las canciones de la playlist
void Playlist::showPlaylist() const {
    cout << "Playlist: " << playlistName << endl;
    if (songs.empty()) {
        cout << "La playlist está vacía." << endl;
        return;
    }
    int i = 1;
    for (const auto& song : songs) {
        cout << i++ << ". " << song->track_name << " - " << song->artist_name << " (" << song->year << ")" << endl;
    }
}

// Cambiar el orden de una canción en la playlist
void Playlist::cambiar_orden(int posicion_actual, int nueva_posicion) {
    if (posicion_actual < 1 || nueva_posicion < 1) {
        cout << "Las posiciones deben ser mayores que cero." << endl;
        return;
    }

    // Convertir las posiciones a índices de la lista
    auto it_actual = songs.begin();
    advance(it_actual, posicion_actual - 1);

    if (it_actual == songs.end()) {
        cout << "Posición actual fuera de rango." << endl;
        return;
    }

    auto it_nueva = songs.begin();
    advance(it_nueva, nueva_posicion - 1);

    if (it_nueva == songs.end()) {
        cout << "Nueva posición fuera de rango." << endl;
        return;
    }

    // Mover la canción a la nueva posición
    Song* song = *it_actual;
    songs.erase(it_actual);
    songs.insert(it_nueva, song);

    cout << "Canción movida a la nueva posición: " << song->track_name << " - " << song->artist_name << endl;
}





