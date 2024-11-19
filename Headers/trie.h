#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <memory>
#include <vector>
#include "Song.h"

struct TrieNode {
    unordered_map<char, unique_ptr<TrieNode>> children;
    vector<Song*> songs; // Lista de punteros a canciones que coinciden con este prefijo
};

class Trie {
public:
    Trie();
    void insert(const string& key, Song* song);
    vector<Song*> searchByPrefix(const string& prefix) const;

private:
    unique_ptr<TrieNode> root;
};

#endif // TRIE_H
