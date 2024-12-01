#ifndef SONG_LIBRARY_H
#define SONG_LIBRARY_H

#include "song.h"
#include <vector>

using namespace std;

class SongLibrary {
private:
    vector<Song> songs;  // Vector que almacena las canciones

public:
    // Cargar canciones desde un archivo CSV
    void loadSongs(const string& filename);

    // Buscar canción por track_id
    Song* searchByTrackId(const string& track_id);

    // Buscar canción por nombre de canción
    vector<Song*> searchByTrackName(const string& track_name);

    // Buscar canción por nombre de artista
    vector<Song*> searchByArtistName(const string& artist_name);

};

#endif // SONG_LIBRARY_H

