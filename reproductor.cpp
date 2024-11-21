/* #include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <algorithm>

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

// Función para mostrar el menú y realizar las búsquedas
void showMenu(SongLibrary& library) {
    int choice;
    string input;

    do {
        cout << "\n--- Menu de Busqueda ---\n";
        cout << "1. Buscar por Track ID\n";
        cout << "2. Buscar por Nombre del Artista\n";
        cout << "3. Buscar por Nombre de la Cancion\n";
        cout << "4. Salir\n";
        cout << "Elige una opcion: ";
        cin >> choice;
        cin.ignore(); // Limpiar el buffer de entrada

        switch (choice) {
            case 1:
                cout << "Ingresa el Track ID: ";
                getline(cin, input);
                if (Song* song = library.searchByTrackId(input)) {
                    cout << "Cancion encontrada: " << song->track_name << " - " << song->artist_name << " (" << song->year << ")" << endl;
                } else {
                    cout << "Track ID no encontrado." << endl;
                }
                break;

            case 2:
                cout << "Ingresa el Nombre del Artista: ";
                getline(cin, input);
                for (const auto& song : library.searchByArtistName(toLowerCase(input))) {
                    cout << song->track_name << " (" << song->year << ") - Popularidad: " << song->popularity << endl;
                }
                break;

            case 3:
                cout << "Ingresa el Nombre de la Cancion: ";
                getline(cin, input);
                for (const auto& song : library.searchByTrackName(toLowerCase(input))) {
                    cout << song->artist_name << " - " << song->track_name << " (" << song->year << ") - Popularidad: " << song->popularity << endl;
                }
                break;

            case 4:
                cout << "Saliendo del programa." << endl;
                break;

            default:
                cout << "Opcion invalida. Por favor, intenta nuevamente." << endl;
                break;
        }
    } while (choice != 4);
}

int main() {
    SongLibrary library;
    library.loadSongs("spotify_data.csv");

    showMenu(library);

    return 0;
}
*/

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <map> // Para el Red-Black Tree

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////
// PARA COMPILAR CON g++17
//g++ -std=c++17 -o n_reproductor n_reproductor.cpp
////////////////////////////////////////////////////////////////////////////////////////

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

// Clase Playlist utilizando un Red-Black Tree (std::map)
class Playlist {
public:
    // Añadir una canción a la playlist
    void addSong(int key, Song* song) {
        if (songs.count(key)) {
            cout << "La cancion con la clave " << key << " ya esta en la playlist." << endl;
            return;
        }
        songs[key] = song;
        cout << "Cancion insertada en la playlist: " << song->track_name << endl;
    }

    // Eliminar una canción de la playlist
    void removeSong(int key) {
        if (!songs.count(key)) {
            cout << "La cancion con la clave " << key << " no esta en la playlist." << endl;
            return;
        }
        cout << "Cancion eliminada de la playlist: " << songs[key]->track_name << endl;
        songs.erase(key);
    }

    // Imprimir todas las canciones de la playlist en orden
    void printPlaylist() const {
        if (songs.empty()) {
            cout << "La playlist esta vacia." << endl;
            return;
        }
        cout << "\n--- Playlist ---" << endl;
        for (const auto& [key, song] : songs) {
            cout << key << ": " << song->track_name << " - " << song->artist_name
                 << " (" << song->year << ") - Popularidad: " << song->popularity << endl;
        }
    }

private:
    map<int, Song*> songs; // Red-Black Tree (std::map)
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

// Función para mostrar el menú y realizar las búsquedas y gestiones de playlist
void showMenu(SongLibrary& library, Playlist& playlist) {
    int choice;
    string input;

    do {
        cout << "\n--- Menu ---\n";
        cout << "1. Buscar por Track ID\n";
        cout << "2. Buscar por Nombre del Artista\n";
        cout << "3. Buscar por Nombre de la Cancion\n";
        cout << "4. Insertar Cancion a la Playlist\n";
        cout << "5. Eliminar Cancion de la Playlist\n";
        cout << "6. Mostrar Playlist\n";
        cout << "7. Salir\n";
        cout << "Elige una opcion: ";
        cin >> choice;
        cin.ignore(); // Limpiar el buffer de entrada

        switch (choice) {
            case 1: {
                cout << "Ingresa el Track ID: ";
                getline(cin, input);
                if (Song* song = library.searchByTrackId(input)) {
                    cout << "Cancion encontrada: " << song->track_name << " - " << song->artist_name << endl;
                } else {
                    cout << "Track ID no encontrado." << endl;
                }
                break;
            }
            case 2: {
                cout << "Ingresa el Nombre del Artista: ";
                getline(cin, input);
                for (const auto& song : library.searchByArtistName(toLowerCase(input))) {
                    cout << song->track_name << " (" << song->year << ") - Popularidad: " << song->popularity << endl;
                }
                break;
            }
            case 3: {
                cout << "Ingresa el Nombre de la Cancion: ";
                getline(cin, input);
                for (const auto& song : library.searchByTrackName(toLowerCase(input))) {
                    cout << song->artist_name << " - " << song->track_name << endl;
                }
                break;
            }
            case 4: {
                cout << "Ingresa el Track ID de la Cancion a Insertar: ";
                getline(cin, input);
                if (Song* song = library.searchByTrackId(input)) {
                    playlist.addSong(song->index, song);
                } else {
                    cout << "Track ID no encontrado." << endl;
                }
                break;
            }
            case 5: {
                int id;
                cout << "Ingresa el indice de la cancion a eliminar: ";
                cin >> id;
                playlist.removeSong(id);
                break;
            }
            case 6:
                playlist.printPlaylist();
                break;
            case 7:
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opcion no valida. Intenta de nuevo." << endl;
        }
    } while (choice != 7);
}

// Función principal
int main() {
    SongLibrary library;
    Playlist playlist;

    string filename = "spotify_data.csv";
    library.loadSongs(filename);

    showMenu(library, playlist);
    return 0;
}
