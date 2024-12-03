#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
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
// Función para agregar una nueva canción
Song createSong() {
    Song newSong;
    cout << "Ingresa el índice de la canción: ";
    cin >> newSong.index;
    cin.ignore();  // Ignorar el salto de línea

    cout << "Ingresa el nombre del artista: ";
    getline(cin, newSong.artist_name);

    cout << "Ingresa el nombre de la canción: ";
    getline(cin, newSong.track_name);

    cout << "Ingresa la popularidad (0-100): ";
    cin >> newSong.popularity;
    cin.ignore();  // Ignorar el salto de línea

    cout << "Ingresa el año de la canción: ";
    cin >> newSong.year;
    cin.ignore();  // Ignorar el salto de línea

    cout << "Ingresa el género: ";
    getline(cin, newSong.genre);

    return newSong;
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
/////////////////////////////////////////////////////////////////////////////
// Nodo del Árbol AVL basado en el año
struct AVLNodeYear {
    Song song;
    AVLNodeYear* left;
    AVLNodeYear* right;
    int height;

    AVLNodeYear(Song song) : song(song), left(nullptr), right(nullptr), height(1) {}
};

// Clase para manejar el Árbol AVL basado en el año
class AVLTreeYear {
public:
    AVLTreeYear() : root(nullptr) {}

    // Insertar canción en el árbol AVL ordenado por año
    void insert(Song song) {
        root = insert(root, song);
    }

    // Mostrar todas las canciones en orden por año de lo más reciente a lo más antiguo
    void displayByYear(int& count, int limit) {
        displayByYear(root, count, limit);
    }

private:
    AVLNodeYear* root;

    int height(AVLNodeYear* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    int getBalance(AVLNodeYear* node) {
        if (node == nullptr) return 0;
        return height(node->left) - height(node->right);
    }

    AVLNodeYear* rightRotate(AVLNodeYear* y) {
        AVLNodeYear* x = y->left;
        AVLNodeYear* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNodeYear* leftRotate(AVLNodeYear* x) {
        AVLNodeYear* y = x->right;
        AVLNodeYear* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    AVLNodeYear* insert(AVLNodeYear* node, Song song) {

        if (song.year > 2024) return node;

        if (node == nullptr) return new AVLNodeYear(song);

        if (song.year > node->song.year) {
            node->left = insert(node->left, song);
        } else {
            node->right = insert(node->right, song);
        }

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        if (balance > 1 && song.year > node->left->song.year) {
            return rightRotate(node);
        }
        if (balance < -1 && song.year <= node->right->song.year) {
            return leftRotate(node);
        }
        if (balance > 1 && song.year <= node->left->song.year) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && song.year > node->right->song.year) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void displayByYear(AVLNodeYear* node, int& count, int limit) {
        if (node == nullptr || count >= limit) return;

        displayByYear(node->left, count, limit);
    
        // Mostrar solo las canciones de 2024 o anteriores
        if (node->song.year <= 2024 && count < limit) {
            cout << node->song.artist_name << " - " << node->song.track_name << " (Año: " << node->song.year << ")" << endl;
            count++;
        }

        displayByYear(node->right, count, limit);
    }

};
/////////////////////////////////////////////////////////

// Clase para gestionar la biblioteca de canciones
class SongLibrary {
public:
    // Función para agregar una nueva canción a la biblioteca
    void addSong(const Song& newSong) {
        songs.push_back(newSong);

    // Insertar en el Trie de artistas y canciones
        artistTrie.insert(toLowerCase(newSong.artist_name), newSong);
        trackTrie.insert(toLowerCase(newSong.track_name), newSong);

    // Insertar en el árbol AVL basado en popularidad
        avlTree.insert(newSong);
        avlTreeYear.insert(newSong);  // Insertar en el AVL basado en año
    }

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

            if (year > 2024) continue;

            Song song = {index, artist_name, track_name, popularity, year, genre};
            songs.push_back(song);

            artistTrie.insert(toLowerCase(artist_name), song);
            trackTrie.insert(toLowerCase(track_name), song);

            // Insertar canción en el AVLTree basado en la popularidad
            avlTree.insert(song);
            avlTreeYear.insert(song);    // Basado en año
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
    void displayTopSongsByYear(int limit) {
        int count = 0;
        avlTreeYear.displayByYear(count, limit);
    }

private:
    vector<Song> songs;
    Trie artistTrie;
    Trie trackTrie;
    AVLTree avlTree; // Árbol AVL para canciones ordenadas por popularidad
    AVLTreeYear avlTreeYear; // Árbol AVL para canciones ordenadas por año
};

struct Node {
    Song song;
    Node* next;
    Node* prev;
    
    Node(Song s) : song(s), next(nullptr), prev(nullptr) {}
};

class Playlist {
public:
    Node* head;
    Node* tail;

    Playlist() : head(nullptr), tail(nullptr) {}

    // Agregar canción al final de la lista
    void addSongToPlaylist(Song new_song) {
        Node* new_node = new Node(new_song);
        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
    }

    // Eliminar canción de la lista
    void removeSongFromPlaylist(string song_name) {
        Node* current = head;
        while (current) {
            if (current->song.track_name == song_name) {
                if (current->prev) current->prev->next = current->next;
                if (current->next) current->next->prev = current->prev;
                if (current == head) head = current->next;
                if (current == tail) tail = current->prev;
                delete current;
                cout << "Canción eliminada exitosamente!" << endl;
                return;
            }
            current = current->next;
        }
        cout << "Canción no encontrada." << endl;
    }

   
    // Mostrar canciones de la playlist con número
    void displayPlaylist() {
        if (!head) {
            cout << "La playlist está vacía." << endl;
            return;
        }
        Node* current = head;
        int index = 1;  // Contador para mostrar el número de la canción
        while (current) {
            cout << index << ". " << current->song.artist_name << " - " << current->song.track_name << endl;
            current = current->next;
            index++;  // Incrementar el contador para la siguiente canción
        }
    }
    // Cambiar la canción de una posición actual a una nueva posición en la playlist
    void cambiar_orden(int posicion_actual, int nueva_posicion) {
        if (posicion_actual == nueva_posicion || !head) {
            cout << "Posiciones inválidas o son iguales." << endl;
            return;
        }

        Node* current = head;
        int index = 1;

        // Encontrar el nodo en la posición actual
        while (current && index < posicion_actual) {
            current = current->next;
            index++;
        }

        if (!current) {
            cout << "Posición actual fuera de rango." << endl;
            return;
        }

        // Remover el nodo de la lista
        if (current->prev) current->prev->next = current->next;
        if (current->next) current->next->prev = current->prev;
        if (current == head) head = current->next;
        if (current == tail) tail = current->prev;

        // Insertar el nodo en la nueva posición
        Node* new_position_node = head;
        index = 1;
        while (new_position_node && index < nueva_posicion) {
            new_position_node = new_position_node->next;
            index++;
        }

        // Si la nueva posición es al principio de la lista
        if (!new_position_node || nueva_posicion == 1) {
            current->next = head;
            if (head) head->prev = current;
            head = current;
            current->prev = nullptr;
            if (!tail) tail = head;  // Si era la única canción
        } else {
            // Insertar antes del nodo en la nueva posición
            current->next = new_position_node;
            current->prev = new_position_node->prev;
            if (new_position_node->prev) new_position_node->prev->next = current;
            new_position_node->prev = current;
        }

        cout << "Canción movida exitosamente a la nueva posición!" << endl;
    }
};

void showPlaylistMenu(Playlist& playlist, SongLibrary& library) {
    int sub_option;
    do {
        cout << "\nMenu de Playlist:" << endl;
        cout << "1. Agregar canción a la playlist" << endl;
        cout << "2. Eliminar canción de la playlist" << endl;
        cout << "3. Mostrar canciones en la playlist" << endl;
        cout << "4. Cambiar orden de canciones en la playlist" << endl;
        cout << "5. Volver al menú principal" << endl;
        cout << "Seleccione una opción: ";
        cin >> sub_option;
        cin.ignore();  // Limpiar el buffer

        if (sub_option == 1) {
            string track_prefix;
            cout << "Ingrese el nombre de la canción que desea agregar: ";
            getline(cin, track_prefix);

            vector<Song> track_results = library.searchByPrefix(track_prefix, false);
            if (track_results.empty()) {
                cout << "No se encontraron canciones con ese nombre." << endl;
            } else {
                cout << "Se encontraron " << track_results.size() << " canciones con ese nombre." << endl;
                cout << "Seleccione una opción:" << endl;
                cout << "1. Filtrar por nombre del artista" << endl;
                cout << "2. Mostrar todas las canciones con ese nombre" << endl;
                int sub_sub_option;
                cin >> sub_sub_option;
                cin.ignore();  // Limpiar el buffer

                if (sub_sub_option == 1) {
                    string artist_name;
                    cout << "Ingrese el nombre del artista: ";
                    getline(cin, artist_name);
                    bool found = false;
                    for (const auto& song : track_results) {
                        if (toLowerCase(song.artist_name) == toLowerCase(artist_name)) {
                            cout << "Canción encontrada: " << song.artist_name << " - " << song.track_name << endl;
                            playlist.addSongToPlaylist(song);  // Agregar a la playlist
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "No se encontraron canciones de ese artista con el mismo nombre." << endl;
                    }
                } else if (sub_sub_option == 2) {
                    // Mostrar las canciones encontradas con numeración
                    cout << "Seleccione el número de la canción que desea agregar a la playlist:" << endl;
                    for (int i = 0; i < track_results.size(); ++i) {
                        cout << i + 1 << ". " << track_results[i].artist_name << " - " << track_results[i].track_name << endl;
                    }

                    // Aquí también agregamos la opción de seleccionar una canción
                    int song_option;
                    cout << "Seleccione el número de la canción que desea agregar a la playlist: ";
                    cin >> song_option;
                    if (song_option >= 1 && song_option <= track_results.size()) {
                        playlist.addSongToPlaylist(track_results[song_option - 1]);  // Agregar canción seleccionada
                        cout << "Canción agregada exitosamente a la playlist!" << endl;
                    } else {
                        cout << "Opción inválida." << endl;
                    }
                }
            }
        } else if (sub_option == 2) {
            string track_to_remove;
            cout << "Ingrese el nombre de la canción que desea eliminar: ";
            getline(cin, track_to_remove);
            // Convertir el nombre de la canción a minúsculas para hacer la comparación insensible a mayúsculas
            transform(track_to_remove.begin(), track_to_remove.end(), track_to_remove.begin(), ::tolower);

            Node* current = playlist.head;
            vector<Node*> matching_songs;
            
            // Buscar canciones que coincidan (insensible a mayúsculas/minúsculas)
            while (current) {
                string song_name = current->song.track_name;
                transform(song_name.begin(), song_name.end(), song_name.begin(), ::tolower);

                if (song_name == track_to_remove) {
                    matching_songs.push_back(current);  // Guardamos las canciones que coinciden
                }
                current = current->next;
            }

            if (matching_songs.empty()) {
                cout << "No se encontró ninguna canción con ese nombre." << endl;
            } else if (matching_songs.size() == 1) {
                // Si hay solo una canción que coincide, la eliminamos directamente
                playlist.removeSongFromPlaylist(matching_songs[0]->song.track_name);
            } else {
                // Si hay más de una canción con el mismo nombre, pedimos el nombre del artista
                string artist_name;
                cout << "Se encontraron varias canciones con ese nombre. Ingrese el nombre del artista: ";
                getline(cin, artist_name);

                bool found = false;
                for (auto& song_node : matching_songs) {
                    if (toLowerCase(song_node->song.artist_name) == toLowerCase(artist_name)) {
                        playlist.removeSongFromPlaylist(song_node->song.track_name);
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    cout << "No se encontró una canción con el nombre del artista especificado." << endl;
                }
            }

        } else if (sub_option == 3) {
            playlist.displayPlaylist();  // Mostrar canciones en la playlist
        } else if (sub_option == 4) {
            // Opción para cambiar el orden de las canciones
            int posicion_actual, nueva_posicion;
            cout << "Ingrese la posición actual de la canción: ";
            cin >> posicion_actual;
            cout << "Ingrese la nueva posición de la canción: ";
            cin >> nueva_posicion;

            playlist.cambiar_orden(posicion_actual, nueva_posicion);  // Llamar a la función cambiar_orden
        }
    } while (sub_option != 5);
}

// Función para mostrar el menú interactivo
void showMenu(SongLibrary& library) {
    int option;
    Playlist userPlaylist;
    do {
        cout << "\nMenu de busqueda:" << endl;
        cout << "1. Buscar por canción" << endl;
        cout << "2. Buscar por artista" << endl;
        cout << "3. Mostrar canciones ordenadas por popularidad" << endl;
        cout << "4. Mostrar canciones ordenadas por año" << endl;
        cout << "5. Agregar una nueva canción" << endl;
        cout << "6. Crear nueva Playlist" << endl;
        cout << "7. Salir" << endl;
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
        } else if (option == 4) {
            int limit;
            cout << "¿Cuántas canciones deseas mostrar? ";
            cin >> limit;
            cout << "Canciones ordenadas por año (de lo más reciente a lo más antiguo):" << endl;
            library.displayTopSongsByYear(limit);
        } else if (option == 5) {
            Song newSong = createSong();
            library.addSong(newSong);
            cout << "Cancion agregada exitosamente!" << endl;
        } else if (option == 6) {
            cout << "¡Playlist creada exitosamente!" << endl;
            showPlaylistMenu(userPlaylist, library);
        }

    } while (option != 7);
}

int main() {
    SongLibrary library;
    library.loadSongs("/home/k4k4wate/Downloads/spotify_data.csv");

    showMenu(library);

    return 0;
}







