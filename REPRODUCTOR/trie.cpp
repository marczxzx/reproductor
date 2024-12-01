#include "trie.h"

// Constructor del Trie
Trie::Trie() : root(make_unique<TrieNode>()) {}

// Insertar una canción en el Trie
void Trie::insert(const string& key, Song* song) {
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
vector<Song*> Trie::searchByPrefix(const string& prefix) const {
    string lowerPrefix = toLowerCase(prefix);
    TrieNode* current = root.get();
    for (char ch : lowerPrefix) {
        if (!current->children.count(ch)) {
            return {}; // No hay coincidencias
        }
        current = current->children[ch].get();
    }
    return current->songs;
}


