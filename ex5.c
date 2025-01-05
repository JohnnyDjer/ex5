/******************
Name: Jonathan Djerassi    
ID: 303013098
Assignment: ex5
*******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OP 1

// Define `strdup` for C99 compatibility
char *strdup(const char *str) {
    size_t len = strlen(str) + 1;
    char *copy = (char *)malloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }
    return copy;
}
// Struct Definitions
typedef struct Song {
    char *title;
    char *artist;
    int year;
    int streams;
    char *lyrics;
    int index;
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

    while (OP) {
        printf("Please Choose:\n");
        printf("    1. Watch playlists\n");
        printf("    2. Add playlist\n");
        printf("    3. Remove playlist\n");
        printf("    4. exit\n");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');  // Clear buffer
            continue;
        }
        while (getchar() != '\n');  // Clear leftover characters


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
    printf("Enter playlist's name:\n");
    if (!fgets(name, sizeof(name), stdin)) {
        printf("Failed to read input.\n");
        return;
    }

    // Remove the trailing newline and carriage return characters
    size_t len = strlen(name);
    if (len > 0 && (name[len - 1] == '\n' || name[len - 1] == '\r')) {
        name[len - 1] = '\0';  // Remove the last character (either '\n' or '\r')
    }

    // Remove any trailing spaces from the name
    len = strlen(name);
    while (len > 0 && name[len - 1] == ' ') {
        name[len - 1] = '\0'; // Remove the last space
        len--;
    }

    // Remove any leading spaces from the name
    int start = 0;
    while (name[start] == ' ') {
        start++;
    }

    // Create a new name starting from the first non-space character
    char cleanedName[256];
    strcpy(cleanedName, &name[start]);

    // Proceed with the playlist creation using cleanedName
    Playlist **temp = (Playlist **)realloc(*playlists, (*playlistCount + 1) * sizeof(Playlist *));
    if (temp == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    *playlists = temp;
    Playlist *newPlaylist = createPlaylist(cleanedName);
    if (newPlaylist == NULL) {
        printf("Failed to create playlist!\n");
        return;
    }

    (*playlists)[*playlistCount] = newPlaylist;
    (*playlistCount)++;
}

void removePlaylist(Playlist ***playlists, int *playlistCount) {
    while (OP) {
        displayPlaylists(*playlists, *playlistCount);  // Show playlist menu

        if (*playlistCount == 0) {

            return;  // Exit if no playlists exist
        }

        printf("Choose a playlist to remove: ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');  // Clear buffer
            continue;  // Restart the loop
        }
        getchar();  // Clear newline

        if (choice == *playlistCount + 1) {  // Back to main menu option
            return;
        }

        if (choice < 1 || choice > *playlistCount) {  // Invalid option
            printf("Invalid option. Please try again.\n");
            continue;  // Restart the loop
        }

        // Remove the selected playlist
        int index = choice - 1;
        freePlaylist((*playlists)[index]);

        for (int i = index; i < *playlistCount - 1; i++) {
            (*playlists)[i] = (*playlists)[i + 1];
        }

        (*playlistCount)--;
        *playlists = (Playlist **)realloc(*playlists, (*playlistCount) * sizeof(Playlist *));
        printf("Playlist removed successfully.\n");
    }
}

void displayPlaylists(Playlist **playlists, int playlistCount) {
    if (playlistCount == 0) {
        printf("Choose a playlist:\n");
        printf("1. Back to main menu\n");

        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please try again.\n");
            while (getchar() != '\n');  // Clear input buffer
            return;  // Exit the function
        }

        while (getchar() != '\n');  // Clear leftover characters

        if (choice == 1) {
            return;  // Back to main menu
        } else {
            printf("Invalid option. Returning to main menu.\n");
            return;
        }
    }

    printf("Choose a playlist:\n");
    for (int i = 0; i < playlistCount; i++) {
        printf("%d. %s\n", i + 1, playlists[i]->name);
    }
    printf("%d. Back to main menu\n", playlistCount + 1);

    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please try again.\n");
        while (getchar() != '\n');  // Clear input buffer
        return;
    }

    while (getchar() != '\n');  // Clear leftover characters

    if (choice == playlistCount + 1) {
        return;  // Back to main menu
    } else if (choice >= 1 && choice <= playlistCount) {
        playlistMenu(playlists[choice - 1], playlists, playlistCount);
    } else {
        printf("Invalid option. Returning to main menu.\n");
    }
}

void playlistMenu(Playlist *playlist, Playlist **playlists, int playlistCount) {
    int choice;
    printf("playlist %s:\n", playlist->name);
    while (OP) {

        printf("    1. Show Playlist\n");
        printf("    2. Add Song\n");
        printf("    3. Delete Song\n");
        printf("    4. Sort\n");
        printf("    5. Play\n");
        printf("    6. exit\n");

       if(scanf("%d", &choice)!=1) {
           getchar(); // Clear newline
           while(getchar()!='\n');
           continue;
       }
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
                    printf("Current Playlist:\n");
                    for (int i = 0; i < playlist->songCount; i++) {
                        Song *song = playlist->songs[i];
                        printf("%d. Title: %s\n   Artist: %s\n   Released: %d\n   Streams: %d\n",
                               i + 1, song->title, song->artist, song->year, song->streams);
                    }

                    printf("Choose a song to delete, or 0 to quit: ");
                    int songChoice;
                    scanf("%d", &songChoice);
                    getchar(); // Clear newline

                    if (songChoice == 0) {
                        break; // Exit without deleting
                    } else if (songChoice >= 1 && songChoice <= playlist->songCount) {
                        deleteSong(playlist, songChoice - 1); // Perform the deletion
                    } else {
                        printf("Invalid option. Please try again.\n");
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

         
        }
    }
}

// Song Functions
void addSong(Playlist *playlist) {
    char title[256], artist[256], lyrics[1024];
    int year;

    // Prompt and read song title
    printf("Enter song's details\n");
    printf("Title:\n");

    // Clear input buffer before using fgets after scanf
    while (getchar() != '\n');  // Clear any leftover newline in the buffer

    if (fgets(title, sizeof(title), stdin) == NULL) {
        printf("Failed to read title.\n");
        return;
    }
    title[strcspn(title, "\n")] = '\0';  // Remove newline character if present

    // Check if title is empty
    if (strlen(title) == 0) {
        printf("Title cannot be empty.\n");
        return;
    }

    // Read artist name
    printf("Artist:\n");
    if (fgets(artist, sizeof(artist), stdin) == NULL) {
        printf("Failed to read artist.\n");
        return;
    }
    artist[strcspn(artist, "\n")] = '\0';  // Remove newline character if present

    // Check if artist is empty
    if (strlen(artist) == 0) {
        printf("Artist cannot be empty.\n");
        return;
    }

    // Read release year
    printf("Year of release:\n");
    if (scanf("%d", &year) != 1) {
        printf("Invalid input for year.\n");
        while (getchar() != '\n'); // Clear the buffer
        return;
    }
    getchar();  // Clear newline from input buffer

    // Check for valid year input
    if (year <= 0) {
        printf("Invalid year entered.\n");
        return;
    }

    // Read lyrics
    printf("Lyrics:\n");
    if (fgets(lyrics, sizeof(lyrics), stdin) == NULL) {
        printf("Failed to read lyrics.\n");
        return;
    }
    lyrics[strcspn(lyrics, "\n")] = '\0';  // Remove newline character if present

    // Create song object and add it to playlist
    Song *song = (Song *)malloc(sizeof(Song));
    if (!song) {
        printf("Memory allocation failed!\n");
        return;
    }

    song->title = strdup(title);
    song->artist = strdup(artist);
    song->year = year;
    song->streams = 0;
    song->lyrics = strdup(lyrics);
    song->index = playlist->songCount;

    // Ensure reallocation of songs array works
    playlist->songs = (Song **)realloc(playlist->songs, (playlist->songCount + 1) * sizeof(Song *));
    if (!playlist->songs) {
        printf("Memory allocation failed!\n");
        free(song->title);
        free(song->artist);
        free(song->lyrics);
        free(song);
        return;
    }

    playlist->songs[playlist->songCount] = song;
    playlist->songCount++;
    printf("Song added successfully!\n");
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
    while (OP) {
        printf("Choose a song to play, or 0 to quit: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 0) {
            return;
        } else if (choice >= 1 && choice <= playlist->songCount) {
            playSong(playlist->songs[choice - 1]);
        } else {

        }
    }
}

void playSong(Song *song) {
    song->streams++;
    printf("Now playing %s:\n$ %s $\n\n", song->title, song->lyrics);
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

    if (songA->streams == songB->streams) {
        return songA->index - songB->index; // Maintain original order
    }
    return songA->streams - songB->streams;
}

int compareByStreamsDesc(const void *a, const void *b) {
    Song *songA = *(Song **)a;
    Song *songB = *(Song **)b;

    if (songA->streams == songB->streams) {
        return songA->index - songB->index; // Maintain original order
    }
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
