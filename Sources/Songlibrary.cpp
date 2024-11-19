#include "SongLibrary.h"
#include <fstream>
#include <sstream>
#include <iostream>

void SongLibrary::loadSongs(const string& filename) {
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

Song* SongLibrary::searchByTrackId(const string& track_id) {
    if (songs.count(track_id)) {
        return &songs[track_id];
    }
    return nullptr;
}

vector<Song*> SongLibrary::searchByArtistName(const string& artist_name) const {
    return artistTrie.searchByPrefix(artist_name);
}

vector<Song*> SongLibrary::searchByTrackName(const string& track_name) const {
    return trackTrie.searchByPrefix(track_name);
}
