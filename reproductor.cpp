#include <iostream>
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


int main() {
    SongLibrary library;
    library.loadSongs("spotify_data.csv");

    showMenu(library);

    return 0;
}
