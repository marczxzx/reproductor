#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

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

// Nodo del Árbol AVL
struct AVLNode {
    Song song;
    AVLNode* left;
    AVLNode* right;
    int height;
    
    AVLNode(Song song) : song(song), left(nullptr), right(nullptr), height(1) {}
};

// Clase para manejar el Árbol AVL
class AVLTree {
public:
    AVLTree() : root(nullptr) {}

    // Insertar canción en el árbol AVL
    void insert(Song song) {
        root = insert(root, song);
    }

    // Mostrar todas las canciones en orden por popularidad
    void displayInOrder(int& count, int limit) {
        displayInOrder(root, count, limit);
    }

    // Obtener el nodo raíz
    AVLNode* getRoot() {
        return root;
    }

private:
    AVLNode* root;

    int height(AVLNode* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    int getBalance(AVLNode* node) {
        if (node == nullptr) return 0;
        return height(node->left) - height(node->right);
    }

    AVLNode* rightRotate(AVLNode* y) {
        if (!y || !y->left) return y;
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right) + 1);
        x->height = max(height(x->left), height(x->right) + 1);

        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        if (!x || !x->right) return x;
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right) + 1);
        y->height = max(height(y->left), height(y->right) + 1);

        return y;
    }

    AVLNode* insert(AVLNode* node, Song song) {
        if (node == nullptr) return new AVLNode(song);

        if (song.popularity < node->song.popularity) {
            node->left = insert(node->left, song);
        } else {
            node->right = insert(node->right, song);
        }

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && song.popularity < node->left->song.popularity) {
            return rightRotate(node);
        }
        if (balance < -1 && song.popularity > node->right->song.popularity) {
            return leftRotate(node);
        }
        if (balance > 1 && song.popularity > node->left->song.popularity) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && song.popularity < node->right->song.popularity) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void displayInOrder(AVLNode* node, int& count, int limit) {
        if (node == nullptr || count >= limit) return;

        displayInOrder(node->left, count, limit);
        if (count < limit && node->song.popularity > 0) {
            cout << node->song.artist_name << " - " << node->song.track_name << " (Popularity: " << node->song.popularity << ")" << endl;
            count++;
        }
        displayInOrder(node->right, count, limit);
    }
};

// Nodo del Trie
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    vector<Song> songs;  // Almacena canciones que coinciden con el prefijo
};

// Clase Trie
class Trie {
public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word, const Song& song) {
        TrieNode* current = root;
        for (char c : word) {
            c = tolower(c);
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->songs.push_back(song);
    }

    vector<Song> searchByPrefix(const string& prefix) const {
        TrieNode* current = root;
        vector<Song> result;

        for (char c : prefix) {
            c = tolower(c);
            if (current->children.find(c) == current->children.end()) {
                return result;
            }
            current = current->children[c];
        }

        return collectSongs(current);
    }

private:
    TrieNode* root;

    vector<Song> collectSongs(TrieNode* node) const {
        vector<Song> result;
        if (!node) return result;

        result.insert(result.end(), node->songs.begin(), node->songs.end());

        for (auto& child : node->children) {
            vector<Song> childSongs = collectSongs(child.second);
            result.insert(result.end(), childSongs.begin(), childSongs.end());
        }

        return result;
    }
};

// Clase para gestionar la biblioteca de canciones
class SongLibrary {
public:
    void loadSongs(const string& filename) {
        ifstream file(filename);
        string line;

        if (!file.is_open()) {
            cerr << "Error al abrir el archivo." << endl;
            return;
        }

        getline(file, line);

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
            songs.push_back(song);

            artistTrie.insert(toLowerCase(artist_name), song);
            trackTrie.insert(toLowerCase(track_name), song);

            // Insertar canción en el AVLTree basado en la popularidad
            avlTree.insert(song);
        }

        file.close();
    }

    vector<Song> searchByPrefix(const string& prefix, bool searchArtist) const {
        if (searchArtist) {
            return artistTrie.searchByPrefix(prefix);
        } else {
            return trackTrie.searchByPrefix(prefix);
        }
    }

    void displayTopSongs(int limit) {
        int count = 0;
        avlTree.displayInOrder(count, limit);
    }

    void sortByPopularity() {
        int limit;
        cout << "¿Cuántas canciones deseas mostrar? ";
        cin >> limit;
        cout << "Canciones ordenadas por popularidad:" << endl;

        int count = 0;
        avlTree.displayInOrder(count, limit);  // Mostrar las canciones ordenadas
    }

private:
    vector<Song> songs;
    Trie artistTrie;
    Trie trackTrie;
    AVLTree avlTree; // Árbol AVL para canciones ordenadas por popularidad
};

// Función para mostrar el menú interactivo
// Función para mostrar el menú interactivo
void showMenu(SongLibrary& library) {
    int option;
    do {
        cout << "\nMenu de busqueda:" << endl;
        cout << "1. Buscar por canción" << endl;
        cout << "2. Buscar por artista" << endl;
        cout << "3. Mostrar canciones ordenadas por popularidad" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> option;
        cin.ignore();

        if (option == 1) {
            string track_prefix;
            cout << "Ingrese el nombre de la canción: ";
            getline(cin, track_prefix);

            vector<Song> track_results = library.searchByPrefix(track_prefix, false);
            if (track_results.empty()) {
                cout << "No se encontraron canciones con ese nombre." << endl;
            } else {
                cout << "Se encontraron " << track_results.size() << " canciones con ese nombre." << endl;
                cout << "Seleccione una opción:" << endl;
                cout << "1. Filtrar por nombre del artista" << endl;
                cout << "2. Mostrar todas las canciones con ese nombre" << endl;
                int sub_option;
                cin >> sub_option;
                cin.ignore();  // Limpiar el buffer
                if (sub_option == 1) {
                    string artist_name;
                    cout << "Ingrese el nombre del artista: ";
                    getline(cin, artist_name);
                    bool found = false;
                    for (const auto& song : track_results) {
                        if (toLowerCase(song.artist_name) == toLowerCase(artist_name)) {
                            cout << song.artist_name << " - " << song.track_name << endl;
                            found = true;
                        }
                    }
                    if (!found) {
                        cout << "No se encontraron canciones de ese artista con el mismo nombre." << endl;
                    }
                } else if (sub_option == 2) {
                    for (const auto& song : track_results) {
                        cout << song.artist_name << " - " << song.track_name << endl;
                    }
                }
            }
        } else if (option == 2) {
            string artist_prefix;
            cout << "Ingrese el prefijo del artista: ";
            getline(cin, artist_prefix);
            vector<Song> results = library.searchByPrefix(artist_prefix, true);
            if (results.empty()) {
                cout << "No se encontraron artistas." << endl;
            } else {
                for (const auto& song : results) {
                    cout << song.artist_name << " - " << song.track_name << " (Popularidad: " << song.popularity << ")" << endl;
                }
            }
        } else if (option == 3) {
            library.sortByPopularity();
        }

    } while (option != 4);
}

int main() {
    SongLibrary library;
    library.loadSongs("/home/k4k4wate/Downloads/spotify_data.csv");

    showMenu(library);

    return 0;
}


