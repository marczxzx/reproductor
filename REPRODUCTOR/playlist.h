#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "song.h"
#include <list>
#include <unordered_map>

class Playlist {
private:
    string playlistName;
    list<Song*> songs;
    unordered_multimap<string, Song*> songMap;

public:
    Playlist(const string& name);
    string getName() const;
    void addSong(Song* song);
    void removeSong(const string& track_name);
    Song* searchSong(const string& track_name) const;
    void showPlaylist() const;
    void cambiar_orden(int posicion_actual, int nueva_posicion);
};

#endif // PLAYLIST_H


