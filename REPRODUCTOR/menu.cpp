#include "menu.h"
#include <iostream>
#include <vector>
#include <algorithm> // Para min()

using namespace std;

// Función para gestionar la playlist
void managePlaylist(SongLibrary& library) {
    string playlistName;
    cout << "Ingresa el nombre de la nueva playlist: ";
    getline(cin, playlistName);
    Playlist playlist(playlistName);

    int choice;
    string input;

    do {
        cout << "\n--- Menú de Playlist: " << playlist.getName() << " ---\n";
        cout << "1. Agregar Canción\n";
        cout << "2. Eliminar Canción\n";
        cout << "3. Mostrar Playlist\n";
        cout << "4. Buscar Canción\n";
        cout << "5. Cambiar Orden de Canciones\n";
        cout << "6. Salir\n";
        cout << "Ingresa tu elección: ";
        cin >> choice;
        cin.ignore();  // Limpiar el buffer de entrada

        switch (choice) {
            case 1: {
                cout << "Ingresa el nombre de la canción: ";
                getline(cin, input);
                vector<Song*> songs = library.searchByTrackName(input);
                if (!songs.empty()) {
                    cout << "Canciones encontradas: " << endl;
                    for (size_t i = 0; i < songs.size(); ++i) {
                        cout << i + 1 << ". " << songs[i]->track_name << " - " << songs[i]->artist_name << endl;
                    }
                    cout << "¿Deseas agregar una canción de las 10 más populares o de todas las canciones?\n";
                    cout << "1. Ver las 10 más populares\n";
                    cout << "2. Ver todas las canciones\n";
                    cout << "Ingresa tu opción: ";
                    int opcion;
                    cin >> opcion;
                    cin.ignore();  // Limpiar el buffer de entrada

                    if (opcion == 1) {
                        size_t limit = min(songs.size(), static_cast<size_t>(10));
                        cout << "Mostrando las " << limit << " canciones más populares:\n";
                        for (size_t i = 0; i < limit; ++i) {
                            cout << i + 1 << ". " << songs[i]->track_name << " - " << songs[i]->artist_name << endl;
                        }

                        cout << "Ingresa el número de la canción que quieres agregar a la playlist: ";
                        int songIndex;
                        cin >> songIndex;
                        cin.ignore();  // Limpiar el buffer de entrada
                        if (songIndex > 0 && songIndex <= limit) {
                            playlist.addSong(songs[songIndex - 1]);
                            cout << "Canción agregada a la playlist: " << songs[songIndex - 1]->track_name << endl;
                        } else {
                            cout << "Opción no válida." << endl;
                        }
                    } else if (opcion == 2) {
                        cout << "Mostrando todas las canciones:\n";
                        for (size_t i = 0; i < songs.size(); ++i) {
                            cout << i + 1 << ". " << songs[i]->track_name << " - " << songs[i]->artist_name << " (" << songs[i]->year << ")" << endl;
                        }

                        cout << "Ingresa el número de la canción que quieres agregar a la playlist: ";
                        int songIndex;
                        cin >> songIndex;
                        cin.ignore();  // Limpiar el buffer de entrada
                        if (songIndex > 0 && songIndex <= songs.size()) {
                            playlist.addSong(songs[songIndex - 1]);
                            cout << "Canción agregada a la playlist: " << songs[songIndex - 1]->track_name << endl;
                        } else {
                            cout << "Opción no válida." << endl;
                        }
                    } else {
                        cout << "Opción no válida." << endl;
                    }
                } else {
                    cout << "Canción no encontrada." << endl;
                }
                break;
            }
            case 2: {
                cout << "Ingresa el nombre de la canción a eliminar: ";
                getline(cin, input);
                playlist.removeSong(input);
                break;
            }
            case 3:
                playlist.showPlaylist();
                break;
            case 4: {
                cout << "Ingresa el nombre de la canción a buscar: ";
                getline(cin, input);
                Song* song = playlist.searchSong(input);
                if (song != nullptr) {
                    cout << "Canción encontrada: " << song->track_name << " - " << song->artist_name << endl;
                } else {
                    cout << "Canción no encontrada." << endl;
                }
                break;
            }
            case 5: {
                int posicion_actual, nueva_posicion;
                cout << "Ingresa la posición actual de la canción: ";
                cin >> posicion_actual;
                cout << "Ingresa la nueva posición para la canción: ";
                cin >> nueva_posicion;
                cin.ignore(); // Limpiar el buffer de entrada
                playlist.cambiar_orden(posicion_actual, nueva_posicion);
                break;
            }
            case 6:
                cout << "Saliendo del menú de la playlist." << endl;
                break;
            default:
                cout << "Opción no válida." << endl;
        }
    } while (choice != 6);
}

int mainMenu(SongLibrary& library) {
    int choice;

    do {
        cout << "\n--- Menú Principal ---\n";
        cout << "1. Buscar canción por índice\n";
        cout << "2. Buscar canción por nombre de canción\n";
        cout << "3. Buscar canción por nombre de artista\n";
        cout << "4. Crear una nueva playlist\n";
        cout << "5. Ordenar canciones por popularidad\n";
        cout << "6. Ordenar canciones por año\n";
        cout << "7. Filtrar canciones por género\n";
        cout << "8. Salir\n";
        cout << "Ingresa tu elección: ";
        cin >> choice;
        cin.ignore(); // Limpiar el buffer de entrada

        string input;
        switch (choice) {
            case 1: {
                cout << "Ingresa el track_id: ";
                getline(cin, input);
                Song* song = library.searchByTrackId(input);
                if (song) {
                    cout << "Canción encontrada: " << song->track_name << " - " << song->artist_name 
                         << " (" << song->year << ")" << endl;
                } else {
                    cout << "No se encontró ninguna canción con ese track_id." << endl;
                }
                break;
            }
            case 2: {
                cout << "Ingresa el nombre de la canción: ";
                getline(cin, input);
                vector<Song*> songs = library.searchByTrackName(input);
                if (!songs.empty()) {
                    cout << "¿Deseas ver las 10 canciones más populares o todas las canciones?\n";
                    cout << "1. Ver las 10 más populares\n";
                    cout << "2. Ver todas las canciones\n";
                    cout << "Ingresa tu opción: ";
                    int opcion;
                    cin >> opcion;
                    cin.ignore();  // Limpiar el buffer de entrada

                    if (opcion == 1) {
                        size_t limit = min(songs.size(), static_cast<size_t>(10));
                        cout << "Mostrando las " << limit << " canciones más populares:\n";
                        for (size_t i = 0; i < limit; ++i) {
                            cout << i + 1 << ". " << songs[i]->track_name << " - " << songs[i]->artist_name << " (" << songs[i]->year << ")" << endl;
                        }
                    } else if (opcion == 2) {
                        cout << "Mostrando todas las canciones:\n";
                        for (size_t i = 0; i < songs.size(); ++i) {
                            cout << i + 1 << ". " << songs[i]->track_name << " - " << songs[i]->artist_name << " (" << songs[i]->year << ")" << endl;
                        }
                    } else {
                        cout << "Opción no válida." << endl;
                    }
                } else {
                    cout << "No se encontraron canciones con ese nombre." << endl;
                }
                break;
            }
            case 3: {
                cout << "Ingresa el nombre del artista: ";
                getline(cin, input);
                vector<Song*> songs = library.searchByArtistName(input);
                if (!songs.empty()) {
                    cout << "Canciones encontradas del artista " << input << ":\n";
                    for (size_t i = 0; i < songs.size(); ++i) {
                        cout << i + 1 << ". " << songs[i]->track_name << " - " << songs[i]->artist_name << " (" << songs[i]->year << ")" << endl;
                    }
                } else {
                    cout << "No se encontraron canciones de ese artista." << endl;
                }
                break;
            }
            case 4:
                managePlaylist(library);
                break;
            case 5: {
                int cantidad;
                cout << "¿Cuántas canciones quieres ver? Ingresa un número: ";
                cin >> cantidad;
                cin.ignore(); // Limpiar el buffer de entrada

                vector<Song> cancionesPopulares = library.getSongsSortedByPopularity();  // Asume que tienes una función que devuelve las canciones ordenadas por popularidad

    // Mostrar solo la cantidad de canciones que el usuario quiere ver, pero no más que el total disponible
                size_t limit = min(cantidad, static_cast<int>(cancionesPopulares.size()));
                cout << "Mostrando las canciones más populares (" << limit << " canciones):\n";
                for (size_t i = 0; i < limit; ++i) {
                    cout << i + 1 << ". " << cancionesPopulares[i].track_name << " - " << cancionesPopulares[i].artist_name<<endl;
                }
                break;
            }
            case 6: {
                int cantidad;
                cout << "¿Cuántas canciones quieres ver? Ingresa un número: ";
                cin >> cantidad;
                cin.ignore(); // Limpiar el buffer de entrada

                vector<Song> cancionesPorAnio = library.getSongsSortedByYear();  // Asume que tienes una función que devuelve las canciones ordenadas por año

    // Mostrar solo la cantidad de canciones que el usuario quiere ver, pero no más que el total disponible
                size_t limit = min(cantidad, static_cast<int>(cancionesPorAnio.size()));
                cout << "Mostrando canciones ordenadas por año (" << limit << " canciones):\n";
                for (size_t i = 0; i < limit; ++i) {
                    cout << i + 1 << ". " << cancionesPorAnio[i].track_name << " - " << cancionesPorAnio[i].artist_name << " (" << cancionesPorAnio[i].year << ")" << endl; 
                }
            break;
        }
            case 7: {
                cout << "Ingresa el género a filtrar: ";
                getline(cin, input);
                library.filtrarPorGenero(input);
                break;
            }
            case 8:
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opción no válida." << endl;
        }
    } while (choice != 8);

    return 0;
}

