#ifndef SONGLIBRARY_H
#define SONGLIBRARY_H

#include <unordered_map>
#include "Trie.h"

class SongLibrary {
public:
    void loadSongs(const string& filename);
    Song* searchByTrackId(const string& track_id);
    vector<Song*> searchByArtistName(const string& artist_name) const;
    vector<Song*> searchByTrackName(const string& track_name) const;

private:
    unordered_map<string, Song> songs;
    Trie artistTrie;
    Trie trackTrie;
};

#endif // SONGLIBRARY_H
