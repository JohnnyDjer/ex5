/******************
Name: Jonathan Djerassi    
ID: 303013098
Assignment: ex5
*******************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Struct Definitions
typedef struct Song {
    char *title;
    char *artist;
    int year;
    int streams;
    char *lyrics;
} Song;

typedef struct Playlist {
    char *name;
    Song **songs;
    int songCount;
} Playlist;

// Function Prototypes
void mainMenu();
Playlist *createPlaylist(const char *name);
void addPlaylist(Playlist ***playlists, int *playlistCount);
void removePlaylist(Playlist ***playlists, int *playlistCount);
void displayPlaylists(Playlist **playlists, int playlistCount);
void playlistMenu(Playlist *playlist, Playlist **playlists, int playlistCount);
void addSong(Playlist *playlist);
void displayPlaylist(Playlist *playlist);
void playSong(Song *song);
void deleteSong(Playlist *playlist, int songIndex);
void sortPlaylist(Playlist *playlist);
void freeSong(Song *song);
void freePlaylist(Playlist *playlist);
void cleanup(Playlist **playlists, int playlistCount);

// Comparison Function Prototypes
int compareByYear(const void *a, const void *b);
int compareByStreamsAsc(const void *a, const void *b);
int compareByStreamsDesc(const void *a, const void *b);
int compareByTitle(const void *a, const void *b);

int main() {
    mainMenu();
    return 0;
}

// Main Menu
void mainMenu() {
    Playlist **playlists = NULL;
    int playlistCount = 0;
    int choice;

    while (1) {
        printf("Please Choose:\n");
        printf("    1. Watch playlists\n");
        printf("    2. Add playlist\n");
        printf("    3. Remove playlist\n");
        printf("    4. exit\n");
        scanf("%d", &choice);
        getchar(); // Clear newline

        switch (choice) {
            case 1:
                displayPlaylists(playlists, playlistCount);
                break;
            case 2:
                addPlaylist(&playlists, &playlistCount);
                break;
            case 3:
                removePlaylist(&playlists, &playlistCount);
                break;
            case 4:
                cleanup(playlists, playlistCount);
                printf("Goodbye!\n");
                return;
            default:
                printf("Invalid option\n");
        }
    }
}

// Playlist Functions
Playlist *createPlaylist(const char *name) {
    Playlist *playlist = (Playlist *)malloc(sizeof(Playlist));
    playlist->name = strdup(name);
    playlist->songs = NULL;
    playlist->songCount = 0;
    return playlist;
}

void addPlaylist(Playlist ***playlists, int *playlistCount) {
    char name[256];
    printf("Enter playlist's name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    *playlists = (Playlist **)realloc(*playlists, (*playlistCount + 1) * sizeof(Playlist *));
    (*playlists)[*playlistCount] = createPlaylist(name);
    (*playlistCount)++;

}

void removePlaylist(Playlist ***playlists, int *playlistCount) {
    if (*playlistCount == 0) {
        printf("No playlists available to remove.\n");
        return;
    }

    displayPlaylists(*playlists, *playlistCount);
    printf("Choose a playlist to remove: ");
    int choice;
    scanf("%d", &choice);
    getchar(); // Clear newline

    if (choice < 1 || choice > *playlistCount) {
        printf("Invalid option\n");
        return;
    }

    int index = choice - 1;
    freePlaylist((*playlists)[index]);

    for (int i = index; i < *playlistCount - 1; i++) {
        (*playlists)[i] = (*playlists)[i + 1];
    }

    *playlistCount -= 1;
    *playlists = (Playlist **)realloc(*playlists, *playlistCount * sizeof(Playlist *));
    printf("Playlist removed successfully.\n");
}

void displayPlaylists(Playlist **playlists, int playlistCount) {
    if (playlistCount == 0) {
        printf("No playlists available.\n");
        return;
    }

    printf("Choose a playlist:\n");
    for (int i = 0; i < playlistCount; i++) {
        printf("%d. %s\n", i + 1, playlists[i]->name);
    }
    printf("%d. Back to main menu\n", playlistCount + 1);

    int choice;
    scanf("%d", &choice);
    getchar(); // Clear newline

    if (choice == playlistCount + 1) {
        return; // Back to main menu
    } else if (choice >= 1 && choice <= playlistCount) {
        playlistMenu(playlists[choice - 1],playlists, playlistCount);
    } else {
        printf("Invalid option\n");
    }
}

void playlistMenu(Playlist *playlist, Playlist **playlists, int playlistCount) {
    int choice;
    while (1) {
        printf("playlist %s:\n", playlist->name);
        printf("    1. Show Playlist\n");
        printf("    2. Add Song\n");
        printf("    3. Delete Song\n");
        printf("    4. Sort\n");
        printf("    5. Play\n");
        printf("    6. exit\n");
        scanf("%d", &choice);
        getchar(); // Clear newline

        switch (choice) {
            case 1:

                displayPlaylist(playlist);
                break;
            case 2:
                addSong(playlist);
                break;
            case 3:
                if (playlist->songCount == 0) {
                    printf("No songs to delete.\n");
                } else {
                    // Display the current playlist before asking for deletion
                    displayPlaylist(playlist);
                    printf("choose a song to delete, or 0 to quit: ");
                    int songChoice;
                    scanf("%d", &songChoice);
                    getchar(); // Clear newline
                    if (songChoice >= 1 && songChoice <= playlist->songCount) {
                        deleteSong(playlist, songChoice - 1);
                    } else if (songChoice != 0) {
                        printf("Invalid option\n");
                    }
                }
            break;
            case 4:
                sortPlaylist(playlist);

                break;
            case 5:
                for (int i = 0; i < playlist->songCount; i++) {
                    playSong(playlist->songs[i]);
                }
                break;
            case 6:
                displayPlaylists(playlists, playlistCount);  // Go back to the playlist selection
                return  ;

            default:
                printf("Invalid option\n");
        }
    }
}

// Song Functions
void addSong(Playlist *playlist) {
    char title[256], artist[256], lyrics[1024];
    int year;
    printf("Enter song's details\n");
    printf("Title:\n");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0';

    printf("Artist:\n");
    fgets(artist, sizeof(artist), stdin);
    artist[strcspn(artist, "\n")] = '\0';

    printf("Year of release:\n");
    scanf("%d", &year);
    getchar(); // Clear newline

    printf("Lyrics:\n");
    fgets(lyrics, sizeof(lyrics), stdin);
    lyrics[strcspn(lyrics, "\n")] = '\0';

    Song *song = (Song *)malloc(sizeof(Song));
    song->title = strdup(title);
    song->artist = strdup(artist);
    song->year = year;
    song->streams = 0;
    song->lyrics = strdup(lyrics);

    playlist->songs = (Song **)realloc(playlist->songs, (playlist->songCount + 1) * sizeof(Song *));
    playlist->songs[playlist->songCount] = song;
    playlist->songCount++;

}

void displayPlaylist(Playlist *playlist) {
    if (playlist->songCount == 0) {
        printf("Playlist is empty.\n");
        return;
    }

    for (int i = 0; i < playlist->songCount; i++) {
        Song *song = playlist->songs[i];
        printf("%d. Title: %s\n   Artist: %s\n   Released: %d\n   Streams: %d\n",
               i + 1, song->title, song->artist, song->year, song->streams);
    }

    int choice;
    while (1) {
        printf("Choose a song to play, or 0 to quit: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 0) {
            return;
        } else if (choice >= 1 && choice <= playlist->songCount) {
            playSong(playlist->songs[choice - 1]);
        } else {
            printf("Invalid option\n");
        }
    }
}

void playSong(Song *song) {
    song->streams++;
    printf("Now playing %s :\n♪ %s ♪\n\n", song->title, song->lyrics);
}

void deleteSong(Playlist *playlist, int songIndex) {
    freeSong(playlist->songs[songIndex]);

    for (int i = songIndex; i < playlist->songCount - 1; i++) {
        playlist->songs[i] = playlist->songs[i + 1];
    }

    playlist->songCount--;
    playlist->songs = (Song **)realloc(playlist->songs, playlist->songCount * sizeof(Song *));
    printf("Song deleted successfully.\n");
}

void sortPlaylist(Playlist *playlist) {
    int choice;
    printf("choose:\n");
    printf("1. sort by year\n");
    printf("2. sort by streams - ascending order\n");
    printf("3. sort by streams - descending order\n");
    printf("4. sort alphabetically\n");

    scanf("%d", &choice);
    getchar();  // Clear newline

    switch (choice) {
        case 1:
            qsort(playlist->songs, playlist->songCount, sizeof(Song *), compareByYear);
            printf("sorted\n");
            break;
        case 2:
            qsort(playlist->songs, playlist->songCount, sizeof(Song *), compareByStreamsAsc);
            printf("sorted\n");
            break;
        case 3:
            qsort(playlist->songs, playlist->songCount, sizeof(Song *), compareByStreamsDesc);
            printf("sorted\n");
            break;
        case 4:
            qsort(playlist->songs, playlist->songCount, sizeof(Song *), compareByTitle);
            printf("sorted\n");
            break;
        default:
            printf("Invalid option\n");
            return;
    }
}

// Comparison Functions
int compareByYear(const void *a, const void *b) {
    Song *songA = *(Song **)a;
    Song *songB = *(Song **)b;
    return songA->year - songB->year;
}

int compareByStreamsAsc(const void *a, const void *b) {
    Song *songA = *(Song **)a;
    Song *songB = *(Song **)b;
    return songA->streams - songB->streams;
}

int compareByStreamsDesc(const void *a, const void *b) {
    Song *songA = *(Song **)a;
    Song *songB = *(Song **)b;
    return songB->streams - songA->streams;
}

int compareByTitle(const void *a, const void *b) {
    Song *songA = *(Song **)a;
    Song *songB = *(Song **)b;
    return strcmp(songA->title, songB->title);
}

// Memory Management
void freeSong(Song *song) {
    free(song->title);
    free(song->artist);
    free(song->lyrics);
    free(song);
}

void freePlaylist(Playlist *playlist) {
    for (int i = 0; i < playlist->songCount; i++) {
        freeSong(playlist->songs[i]);
    }
    free(playlist->songs);
    free(playlist->name);
    free(playlist);
}

void cleanup(Playlist **playlists, int playlistCount) {
    for (int i = 0; i < playlistCount; i++) {
        freePlaylist(playlists[i]);
    }
    free(playlists);
}
