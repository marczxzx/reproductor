#include "Trie.h"

Trie::Trie() : root(make_unique<TrieNode>()) {}

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

vector<Song*> Trie::searchByPrefix(const string& prefix) const {
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
