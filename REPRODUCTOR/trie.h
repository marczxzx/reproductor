#ifndef TRIE_H
#define TRIE_H

#include "song.h"
#include <unordered_map>
#include <memory>
#include <vector>

// Nodo del Trie
struct TrieNode {
    unordered_map<char, unique_ptr<TrieNode>> children;
    vector<Song*> songs;
};

// Estructura Trie para realizar búsquedas por prefijo en artist_name y track_name
class Trie {
public:
    Trie();
    void insert(const string& key, Song* song);
    vector<Song*> searchByPrefix(const string& prefix) const;

private:
    unique_ptr<TrieNode> root;
};

#endif // TRIE_H


