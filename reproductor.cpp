#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <set>

using namespace std;

// Estructura para almacenar información de una canción
struct Song {
    int index;
    string artist_name;
    string track_name;
    int popularity;
    int year;
    string genre;

    // Sobrecarga de operadores para el set (árbol Red-Black)
    bool operator<(const Song& other) const {
        return track_name < other.track_name;  // Comparar por nombre de la canción
    }
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

// Estructura Trie para realizar búsquedas por prefijo en `artist_name` y `track_name`
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

// Clase para gestionar playlists usando un árbol Red-Black
class Playlist {
public:
    Playlist(const string& name) : playlist_name(name) {}

    // Agregar una canción a la playlist
    void addSong(const Song& song) {
        playlist.insert(song);  // Usamos el árbol Red-Black (set)
        cout << "Cancion agregada a la playlist: " << song.track_name << endl;
    }

    // Eliminar una canción de la playlist, pidiendo tanto el nombre de la canción como el nombre del autor
    void removeSong(const string& track_name, const string& artist_name) {
        for (auto it = playlist.begin(); it != playlist.end(); ++it) {
            if (toLowerCase(it->track_name) == toLowerCase(track_name) && 
                toLowerCase(it->artist_name) == toLowerCase(artist_name)) {
                cout << "Eliminando: " << it->track_name << " de " << it->artist_name << endl;
                playlist.erase(it);
                return;
            }
        }
        cout << "Cancion no encontrada en la playlist con el artista especificado." << endl;
    }

    // Buscar una canción en la playlist
    void searchSong(const string& track_name) const {
        for (const auto& song : playlist) {
            if (toLowerCase(song.track_name) == toLowerCase(track_name)) {
                cout << "Cancion encontrada en la playlist: " << song.track_name << endl;
                return;
            }
        }
        cout << "Cancion no encontrada en la playlist." << endl;
    }

    // Mostrar todas las canciones en la playlist
    void showPlaylist() const {
        cout << "\n--- Playlist: " << playlist_name << " ---" << endl;
        for (const auto& song : playlist) {
            cout << song.artist_name << " - " << song.track_name << " (" << song.year << ")" << endl;
        }
    }

private:
    string playlist_name;
    set<Song> playlist;  // Usamos set, que internamente es un árbol Red-Black
};

// Función para agregar una canción a la playlist filtrando por artista si hay múltiples coincidencias
void addSongToPlaylist(Playlist& playlist, SongLibrary& library, const string& track_name) {
    vector<Song*> matches = library.searchByTrackName(track_name);

    if (matches.empty()) {
        cout << "No se encontraron canciones con ese nombre." << endl;
        return;
    }

    if (matches.size() == 1) {
        playlist.addSong(*matches.front());
    } else {
        cout << "Se encontraron " << matches.size() << " canciones con ese nombre. Ingresa el nombre del artista para filtrar:" << endl;
        string artist_name;
        getline(cin, artist_name);

        // Convertir la entrada a minúsculas para no importar mayúsculas o minúsculas
        artist_name = toLowerCase(artist_name);

        // Buscar coincidencias con el nombre del artista ingresado
        for (const auto& song : matches) {
            if (toLowerCase(song->artist_name) == artist_name) {
                playlist.addSong(*song);
                return;
            }
        }
        cout << "No se encontraron canciones con ese nombre de artista." << endl;
    }
}

// Función para mostrar el menú de la playlist
void playlistMenu(Playlist& playlist, SongLibrary& library) {
    int choice;
    string input;

    do {
        cout << "\n--- Menu de Playlist ---\n";
        cout << "1. Agregar Cancion a la Playlist\n";
        cout << "2. Eliminar Cancion de la Playlist\n";
        cout << "3. Buscar Cancion en la Playlist\n";
        cout << "4. Mostrar Canciones en la Playlist\n";
        cout << "5. Salir\n";
        cout << "Elige una opcion: ";
        cin >> choice;
        cin.ignore();  // Limpiar el buffer de entrada

        switch (choice) {
            case 1: {
                cout << "Ingresa el nombre de la cancion: ";
                getline(cin, input);
                addSongToPlaylist(playlist, library, toLowerCase(input));
                break;
            }

            case 2: {
                cout << "Ingresa el nombre de la cancion a eliminar: ";
                string track_name;
                getline(cin, track_name);

                cout << "Ingresa el nombre del artista: ";
                string artist_name;
                getline(cin, artist_name);

                playlist.removeSong(toLowerCase(track_name), toLowerCase(artist_name));
                break;
            }

            case 3: {
                cout << "Ingresa el nombre de la cancion a buscar: ";
                getline(cin, input);
                playlist.searchSong(toLowerCase(input));
                break;
            }

            case 4: {
                playlist.showPlaylist();
                break;
            }

            case 5:
                cout << "Saliendo del menu de playlist..." << endl;
                break;

            default:
                cout << "Opcion invalida. Intenta nuevamente." << endl;
                break;
        }
    } while (choice != 5);
}


// Función para mostrar el menú principal
void showMenu(SongLibrary& library) {
    int choice;
    string input;

    do {
        cout << "\n--- Menu Principal ---\n";
        cout << "1. Buscar por Track ID\n";
        cout << "2. Buscar por Nombre del Artista\n";
        cout << "3. Buscar por Nombre de la Cancion\n";
        cout << "4. Crear una Playlist\n";
        cout << "5. Salir\n";
        cout << "Elige una opcion: ";
        cin >> choice;
        cin.ignore();  // Limpiar el buffer de entrada

        switch (choice) {
            case 1:
                cout << "Ingresa el Track ID: ";
                getline(cin, input);
                if (Song* song = library.searchByTrackId(input)) {
                    cout << song->artist_name << " - " << song->track_name << endl;
                } else {
                    cout << "Cancion no encontrada." << endl;
                }
                break;

            case 2:
                cout << "Ingresa el nombre del artista: ";
                getline(cin, input);
                for (const auto& song : library.searchByArtistName(input)) {
                    cout << song->artist_name << " - " << song->track_name << endl;
                }
                break;

            case 3:
                cout << "Ingresa el nombre de la cancion: ";
                getline(cin, input);
                for (const auto& song : library.searchByTrackName(input)) {
                    cout << song->artist_name << " - " << song->track_name << endl;
                }
                break;

            case 4:
                cout << "Ingresa el nombre de la playlist: ";
                getline(cin, input);
                {
                    Playlist playlist(input);
                    playlistMenu(playlist, library);  // Ir al menú de playlist
                }
                break;

            case 5:
                cout << "Saliendo..." << endl;
                break;

            default:
                cout << "Opcion invalida. Por favor, intenta nuevamente." << endl;
                break;
        }
    } while (choice != 5);
}

int main() {
    SongLibrary library;
    library.loadSongs("/home/k4k4wate/Downloads/spotify_data.csv");  // Cambia la ruta si es necesario
    showMenu(library);
    return 0;
}


