#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <list>

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
        return stoi(str);
    } catch (const invalid_argument&) {
        return 0; // Valor predeterminado si la conversión falla
    }
}

// Nodo del Trie
struct TrieNode {
    unordered_map<char, unique_ptr<TrieNode>> children;
    vector<Song*> songs; // Lista de punteros a canciones que coinciden con este prefijo
};

// Estructura Trie para realizar búsquedas por prefijo en artist_name y track_name
class Trie {
public:
    Trie() : root(make_unique<TrieNode>()) {}

    // Insertar una canción en el Trie usando el nombre del artista o de la canción
    void insert(const string& key, Song* song) {
        string lowerKey = toLowerCase(key);
        TrieNode* current = root.get();
        for (char ch : lowerKey) {
            if (!current->children[ch]) {
                current->children[ch] = make_unique<TrieNode>();
            }
            current = current->children[ch].get();
        }
        current->songs.push_back(song);
    }

    // Búsqueda de canciones por prefijo
    vector<Song*> searchByPrefix(const string& prefix) const {
        string lowerPrefix = toLowerCase(prefix);
        TrieNode* current = root.get();
        for (char ch : lowerPrefix) {
            if (!current->children.count(ch)) {
                return {}; // No hay coincidencias
            }
            current = current->children[ch].get();
        }
        return current->songs; // Retorna todas las canciones que coincidan con el prefijo
    }

private:
    unique_ptr<TrieNode> root;
};

// Clase para gestionar la biblioteca de canciones
class SongLibrary {
public:
    // Cargar canciones desde el archivo CSV
    void loadSongs(const string& filename) {
        ifstream file(filename);
        string line;

        if (!file.is_open()) {
            cerr << "Error al abrir el archivo." << endl;
            return;
        }

        // Ignorar la primera línea (encabezados)
        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);
            string index_str, artist_name, track_name, track_id, popularity_str, year_str, genre;

            // Leer cada campo de acuerdo al orden en el archivo
            getline(ss, index_str, ',');
            getline(ss, artist_name, ',');
            getline(ss, track_name, ',');
            getline(ss, track_id, ',');
            getline(ss, popularity_str, ',');
            getline(ss, year_str, ',');
            getline(ss, genre, ',');

            // Convertir campos numéricos
            int index = safe_stoi(index_str);
            int popularity = safe_stoi(popularity_str);
            int year = safe_stoi(year_str);

            // Crear y almacenar la canción
            Song song = {index, artist_name, track_name, popularity, year, genre};
            songs.emplace(track_id, song);

            // Insertar en los tries usando nombres en minúsculas
            artistTrie.insert(toLowerCase(artist_name), &songs[track_id]);
            trackTrie.insert(toLowerCase(track_name), &songs[track_id]);
        }

        file.close();
    }

    // Búsqueda por track_id usando el unordered_map
    Song* searchByTrackId(const string& track_id) {
        if (songs.count(track_id)) {
            return &songs[track_id];
        }
        return nullptr;
    }

    // Búsqueda por nombre del artista usando Trie
    vector<Song*> searchByArtistName(const string& artist_name) const {
        return artistTrie.searchByPrefix(artist_name);
    }

    // Búsqueda por nombre de la canción usando Trie
    vector<Song*> searchByTrackName(const string& track_name) const {
        return trackTrie.searchByPrefix(track_name);
    }

private:
    unordered_map<string, Song> songs;
    Trie artistTrie;
    Trie trackTrie;
};

// Clase Playlist
class Playlist {
private:
    string playlistName;
    list<Song*> songs;
    unordered_multimap<string, Song*> songMap; // Permitir duplicados usando unordered_multimap

    // Convertir a minúsculas para comparaciones
    string toLowerCase(const string& str) const {
        string result = str;
        for (char& c : result) {
            c = tolower(c);
        }
        return result;
    }

public:
    Playlist(const string& name) : playlistName(name) {}

    // Mostrar el nombre de la playlist
    string getName() const {
        return playlistName;
    }

    // Agregar canción
    void addSong(Song* song) {
        songs.push_back(song);
        songMap.emplace(toLowerCase(song->track_name), song);
    }

    // Eliminar canción
    void removeSong(const string& track_name) {
        string lowerName = toLowerCase(track_name);
        auto range = songMap.equal_range(lowerName);

        if (distance(range.first, range.second) > 1) {
            cout << "Hay varias canciones llamadas \"" << track_name << "\". Ingresa el nombre del artista: ";
            string artist;
            getline(cin, artist);
            bool songFound = false;
            for (auto it = range.first; it != range.second; ++it) {
                if (toLowerCase(it->second->artist_name) == toLowerCase(artist)) {
                    songs.remove(it->second); // Eliminar de la lista
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
            songs.remove(range.first->second);
            songMap.erase(range.first);
            cout << "Canción eliminada: " << range.first->second->track_name << endl;
        } else {
            cout << "Canción no encontrada." << endl;
        }
    }

    // Buscar canción
    Song* searchSong(const string& track_name) {
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

    // Mostrar todas las canciones
    void showPlaylist() const {
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

    // Cambiar la canción de posición
    void cambiar_orden(int posicion_actual, int nueva_posicion) {
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
};

// Modificar la función para gestionar la playlist
void managePlaylist(SongLibrary& library) {
    string playlistName;
    cout << "Ingresa el nombre de la nueva playlist: ";
    getline(cin, playlistName);
    Playlist playlist(playlistName);

    int choice;
    string input;

    do {
        cout << "\n--- Menu de Playlist: " << playlist.getName() << " ---\n";
        cout << "1. Agregar Canción\n";
        cout << "2. Eliminar Canción\n";
        cout << "3. Mostrar Playlist\n";
        cout << "4. Buscar Canción\n";
        cout << "5. Cambiar Orden de Canciones\n";
        cout << "6. Salir\n";
        cout << "Ingresa tu elección: ";
        cin >> choice;
        cin.ignore();  // Limpiar el buffer de entrada

        switch (choice) {
            case 1: {
                cout << "Ingresa el nombre de la canción: ";
                getline(cin, input);
                vector<Song*> songs = library.searchByTrackName(input);
                if (!songs.empty()) {
                    cout << "Canciones encontradas: " << endl;
                    for (size_t i = 0; i < songs.size(); ++i) {
                        cout << i + 1 << ". " << songs[i]->track_name << " - " << songs[i]->artist_name << endl;
                    }
                    cout << "Ingresa el número de la canción que quieres agregar a la playlist: ";
                    int songIndex;
                    cin >> songIndex;
                    cin.ignore();  // Limpiar el buffer de entrada
                    if (songIndex > 0 && songIndex <= songs.size()) {
                        playlist.addSong(songs[songIndex - 1]);
                        cout << "Canción agregada a la playlist: " << songs[songIndex - 1]->track_name << endl;
                    } else {
                        cout << "Opción no válida." << endl;
                    }
                } else {
                    cout << "Canción no encontrada." << endl;
                }
                break;
            }
            case 2: {
                cout << "Ingresa el nombre de la canción a eliminar: ";
                getline(cin, input);
                playlist.removeSong(input);
                break;
            }
            case 3:
                playlist.showPlaylist();
                break;
            case 4: {
                cout << "Ingresa el nombre de la canción a buscar: ";
                getline(cin, input);
                Song* song = playlist.searchSong(input);
                if (song != nullptr) {
                    cout << "Canción encontrada: " << song->track_name << " - " << song->artist_name << endl;
                }
                break;
            }
            case 5: {
                int posicion_actual, nueva_posicion;
                cout << "Ingresa la posición actual de la canción: ";
                cin >> posicion_actual;
                cout << "Ingresa la nueva posición para la canción: ";
                cin >> nueva_posicion;
                playlist.cambiar_orden(posicion_actual, nueva_posicion);
                break;
            }
            case 6:
                cout << "Saliendo del menú de la playlist." << endl;
                break;
            default:
                cout << "Opción no válida." << endl;
        }
    } while (choice != 6);
}

int main() {
    SongLibrary library;
    library.loadSongs("/home/k4k4wate/Downloads/spotify_data.csv");

    managePlaylist(library);

    return 0;
}



