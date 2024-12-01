#include "song_library.h"
#include "menu.h"

int main() {
    SongLibrary library;
    library.loadSongs("/home/k4k4wate/Downloads/spotify_data.csv");

    mainMenu(library);
    return 0;
}


