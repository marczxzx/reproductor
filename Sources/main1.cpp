#include <iostream>
#include "SongLibrary.h"

using namespace std;

void showMenu(SongLibrary& library);

int main() {
    SongLibrary library;
    library.loadSongs("spotify_data.csv");

    showMenu(library);

    return 0;
}

void showMenu(SongLibrary& library) {
    int choice;
    string input;

    do {
        cout << "\n--- Menu de Busqueda ---\n";
        cout << "1. Buscar por Track ID\n";
        cout << "2. Buscar por Nombre del Artista\n";
        cout << "3. Buscar por Nombre de la Cancion\n";
        cout << "4. Salir\n";
        cout << "Elige una opcion: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Ingresa el Track ID: ";
                getline(cin, input);
                if (Song* song = library.searchByTrackId(input)) {
                    cout << "Cancion encontrada: " << song->track_name << " - " << song->artist_name << " (" << song->year << ")" << endl;
                } else {
                    cout << "Track ID no encontrado." << endl;
                }
                break;

            case 2:
                cout << "Ingresa el Nombre del Artista: ";
                getline(cin, input);
                for (const auto& song : library.searchByArtistName(toLowerCase(input))) {
                    cout << song->track_name << " (" << song->year << ") - Popularidad: " << song->popularity << endl;
                }
                break;

            case 3:
                cout << "Ingresa el Nombre de la Cancion: ";
                getline(cin, input);
                for (const auto& song : library.searchByTrackName(toLowerCase(input))) {
                    cout << song->artist_name << " - " << song->track_name << " (" << song->year << ") - Popularidad: " << song->popularity << endl;
                }
                break;

            case 4:
                cout << "Saliendo del programa." << endl;
                break;

            default:
                cout << "Opcion invalida. Por favor, intenta nuevamente." << endl;
                break;
        }
    } while (choice != 4);
}
