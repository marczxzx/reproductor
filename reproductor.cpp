#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>
#include <stack>
#include <queue>
#include <functional>

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////
// PARA COMPILAR CON g++17
// g++ -std=c++17 -o n_reproductor n_reproductor.cpp
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

    vector<Song*> searchByPrefix(const string& prefix) const {
        string lowerPrefix = toLowerCase(prefix);
        TrieNode* current = root.get();
        for (char ch : lowerPrefix) {
            if (!current->children.count(ch)) {
                return {};
            }
            current = current->children[ch].get();
        }
        return current->songs;
    }

private:
    unique_ptr<TrieNode> root;
};

// Clase Playlist
class Playlist {
public:
    void addSong(int key, Song* song) {
        if (songs.count(key)) {
            cout << "La canción con la clave " << key << " ya está en la playlist." << endl;
            return;
        }
        songs[key] = song;
        insertionOrder.push_back(song);
        cout << "Canción insertada en la playlist: " << song->track_name << endl;
    }

    void removeSong(int key) {
        if (!songs.count(key)) {
            cout << "La canción con la clave " << key << " no está en la playlist." << endl;
            return;
        }
        Song* song = songs[key];
        songs.erase(key);
        insertionOrder.erase(remove(insertionOrder.begin(), insertionOrder.end(), song), insertionOrder.end());
        cout << "Canción eliminada de la playlist: " << song->track_name << endl;
    }

    void showByPopularity() const {
        if (songs.empty()) {
            cout << "La playlist está vacía." << endl;
            return;
        }
        vector<Song*> sortedSongs = insertionOrder;
        sort(sortedSongs.begin(), sortedSongs.end(), [](Song* a, Song* b) {
            return a->popularity > b->popularity;
        });
        printSongs(sortedSongs, "Orden por Popularidad");
    }

    void showReverseInsertion() const {
        if (insertionOrder.empty()) {
            cout << "La playlist está vacía." << endl;
            return;
        }
        vector<Song*> reversedOrder = insertionOrder;
        reverse(reversedOrder.begin(), reversedOrder.end());
        printSongs(reversedOrder, "Orden de Inserción Inverso");
    }

    void showByYear(int year = -1) const {
        vector<Song*> filteredSongs;
        for (const auto& song : insertionOrder) {
            if (year == -1 || song->year == year) {
                filteredSongs.push_back(song);
            }
        }
        sort(filteredSongs.begin(), filteredSongs.end(), [](Song* a, Song* b) {
            return a->year < b->year;
        });
        string title = year == -1 ? "Orden por Fecha (Year)" : "Canciones del Año " + to_string(year);
        printSongs(filteredSongs, title);
    }

    void printPlaylist() const {
        if (songs.empty()) {
            cout << "La playlist está vacía." << endl;
            return;
        }
        printSongs(insertionOrder, "Orden de Inserción");
    }

private:
    map<int, Song*> songs;
    vector<Song*> insertionOrder;

    void printSongs(const vector<Song*>& songsList, const string& title) const {
        cout << "\n--- " << title << " ---" << endl;
        int index = 1;
        for (const Song* song : songsList) {
            cout << index++ << ": " << song->track_name << " - " << song->artist_name
                 << " (" << song->year << ") - Popularidad: " << song->popularity << endl;
        }
    }
};

// Clase SongLibrary
class SongLibrary {
public:
    void loadSongs(const string& filename) {
        ifstream file(filename);
        string line;

        if (!file.is_open()) {
            cerr << "Error al abrir el archivo." << endl;
            return;
        }

        getline(file, line); // Ignorar encabezados

        while (getline(file, line)) {
            stringstream ss(line);
            string index_str, artist_name, track_name, track_id, popularity_str, year_str, genre;

            getline(ss, index_str, ',');
            getline(ss, artist_name, ',');
            getline(ss, track_name, ',');
            getline(ss, track_id, ',');
            getline(ss, popularity_str, ',');
            getline(ss, year_str, ',');
            getline(ss, genre, ',');

            int index = safe_stoi(index_str);
            int popularity = safe_stoi(popularity_str);
            int year = safe_stoi(year_str);

            Song song = {index, artist_name, track_name, popularity, year, genre};
            songs.emplace(track_id, song);

            artistTrie.insert(toLowerCase(artist_name), &songs[track_id]);
            trackTrie.insert(toLowerCase(track_name), &songs[track_id]);
        }

        file.close();
    }

    Song* searchByTrackId(const string& track_id) {
        if (songs.count(track_id)) {
            return &songs[track_id];
        }
        return nullptr;
    }

    vector<Song*> searchByArtistName(const string& artist_name) const {
        return artistTrie.searchByPrefix(artist_name);
    }

    vector<Song*> searchByTrackName(const string& track_name) const {
        return trackTrie.searchByPrefix(track_name);
    }

private:
    unordered_map<string, Song> songs;
    Trie artistTrie;
    Trie trackTrie;
};

// Menú principal
void showMenu(SongLibrary& library, Playlist& playlist) {
    int choice;
    string input;

    do {
        cout << "\n--------------- Menu ---------------\n";
        cout << "1. Buscar por Track ID\n";
        cout << "2. Buscar por Nombre del Artista\n";
        cout << "3. Buscar por Nombre de la Canción\n";
        cout << "4. Insertar Cancion a la Playlist\n";
        cout << "5. Eliminar Cancion de la Playlist\n";
        cout << "6. Mostrar Playlist\n";
        cout << "7. Mostrar Playlist por Popularidad\n";
        cout << "8. Mostrar Playlist en Orden Inverso\n";
        cout << "9. Mostrar Playlist por Fecha\n";
        cout << "10. Salir\n";
        cout << "Elige una opcion: ";
        cin >> choice;
        cout << endl;
        cout << "---------------------------------------------\n";
        
        cin.ignore();

        switch (choice) {
            case 1: {
                cout << "Ingresa el Track ID: ";
                getline(cin, input);
                Song* song = library.searchByTrackId(input);
                if (song) {
                    cout << "Cancion encontrada: " << song->track_name << " - " << song->artist_name << endl;
                } else {
                    cout << "Track ID no encontrado." << endl;
                }
                break;
            }
            case 2: {
                cout << "Ingresa el Nombre del Artista: ";
                getline(cin, input);
                auto songs = library.searchByArtistName(input);
                for (const auto& song : songs) {
                    cout << song->track_name << " (" << song->year << ") - Popularidad: " << song->popularity << endl;
                }
                break;
            }
            case 3: {
                cout << "Ingresa el Nombre de la Cancion: ";
                getline(cin, input);
                auto songs = library.searchByTrackName(input);
                for (const auto& song : songs) {
                    cout << song->artist_name << " - " << song->track_name << endl;
                }
                break;
            }
            case 4: {
                cout << "Ingresa el Nombre de la Cancion: ";
                getline(cin, input);
                auto songs = library.searchByTrackName(input);
                if (songs.empty()) {
                    cout << "No se encontraron canciones con ese nombre." << endl;
                    break;
                }
                if (songs.size() == 1) {
                    playlist.addSong(songs[0]->index, songs[0]);
                } else {
                    cout << "Hay varias canciones con ese nombre. Selecciona una:" << endl;
                    for (size_t i = 0; i < songs.size(); ++i) {
                        cout << i + 1 << ": " << songs[i]->track_name << " - " << songs[i]->artist_name
                             << " (" << songs[i]->year << ") - Popularidad: " << songs[i]->popularity << endl;
                    }
                    size_t selection;
                    cin >> selection;
                    cin.ignore();
                    if (selection > 0 && selection <= songs.size()) {
                        playlist.addSong(songs[selection - 1]->index, songs[selection - 1]);
                    } else {
                        cout << "Seleccion no valida." << endl;
                    }
                }
                break;
            }
            case 5: {
                cout << "Ingresa el indice de la cancion a eliminar: ";
                int id;
                cin >> id;
                cin.ignore();
                playlist.removeSong(id);
                break;
            }
            case 6:
                playlist.printPlaylist();
                break;
            case 7:
                playlist.showByPopularity();
                break;
            case 8:
                playlist.showReverseInsertion();
                break;
            case 9: {
                cout << "Ingresa la Fecha (year o -1 para todas): ";
                int year;
                cin >> year;
                cin.ignore();
                playlist.showByYear(year);
                break;
            }
            case 10:
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }
    } while (choice != 10);
}

// Función principal
int main() {
    SongLibrary library;
    Playlist playlist;

    library.loadSongs("spotify_data.csv");
    showMenu(library, playlist);

    return 0;
}
