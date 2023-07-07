// create the artist and music text files before running the code

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <string.h>
#include <unordered_map>
#include <functional>
using namespace std;

#define artistdata "artist.txt"
#define musicdata "music.txt"
// defining the records structures
struct artist
{
    int id;
    string name;
    string gender;
};

struct music
{
    int id;
    string name;
    int artistId;
};

int getHash_Value(int key, int tableSize)
{
    // creating a hash key
    hash<int> hasher;
    return hasher(key) % tableSize;
}

// Function to save artist details to file
void save_artist_info(const unordered_map<int, vector<artist>> &artist_Hash)
{
    ofstream afile(artistdata);
    if (afile)
    {
        for (const auto &entry : artist_Hash)
        {
            for (const artist &artist : entry.second)
            {
                afile << artist.id << "," << artist.name << "," << artist.gender << endl;
            }
        }
        afile.close();
    }
    else
    {
        cerr << "Error opening artist file for writing." << endl;
    }
}

// Function to save music details to file
void save_music_info(const unordered_map<int, vector<music>> &musicHashTable)
{
    ofstream mfile(musicdata);
    if (mfile)
    {
        for (const auto &entry : musicHashTable)
        {
            for (const music &music : entry.second)
            {
                mfile << music.id << "," << music.name << "," << music.artistId << endl;
            }
        }
        mfile.close();
    }
    else
    {
        cerr << "Error opening music file for writing." << endl;
    }
}

// Function to save artist hash values and keys to file
void save_artist_Hash_info(const unordered_map<int, int> &a_HashKey)
{
    ofstream artistHashFile("artist_hash.txt");
    if (artistHashFile)
    {
        for (const auto &entry : a_HashKey)
        {
            artistHashFile << entry.first << "," << entry.second << endl;
        }
        artistHashFile.close();
    }
    else
    {
        cerr << "Error opening artist hash file for writing." << endl;
    }
}

// Function to save music hash values and keys to file
void save_music_Hash_info(const unordered_map<int, int> &musicHashKey)
{
    ofstream musicHashFile("music_hash.txt");
    if (musicHashFile)
    {
        for (const auto &entry : musicHashKey)
        {
            musicHashFile << entry.first << "," << entry.second << endl;
        }
        musicHashFile.close();
    }
    else
    {
        cerr << "Error opening music hash file for writing." << endl;
    }
}

// Function to load artist data from file into the hash table
void loadArtistData(unordered_map<int, vector<artist>> &artist_Hash, unordered_map<int, int> &a_HashKey)
{
    ifstream artistFile(artistdata);
    if (artistFile)
    {
        artist_Hash.clear();
        a_HashKey.clear();
        artist artist;
        string line;
        while (getline(artistFile, line))
        {
            stringstream ss(line);
            string field;
            getline(ss, field, ',');
            artist.id = stoi(field);
            getline(ss, artist.name, ',');
            getline(ss, artist.gender);

            int hashValue = getHash_Value(artist.id, 10);
            artist_Hash[hashValue].push_back(artist);
            a_HashKey[artist.id] = hashValue;
        }
        artistFile.close();
    }
    else
    {
        cerr << "Error opening artist file for reading." << endl;
    }
}

// Function to load music data from file into the hash table
void loadMusicData(unordered_map<int, vector<music>> &musicHashTable, unordered_map<int, int> &musicHashKey)
{
    ifstream musicFile(musicdata);
    if (musicFile)
    {
        musicHashTable.clear();
        musicHashKey.clear();
        music music;
        string line;
        while (getline(musicFile, line))
        {
            stringstream ss(line);
            string field;
            getline(ss, field, ',');
            music.id = stoi(field);
            getline(ss, music.name, ',');
            getline(ss, field);
            music.artistId = stoi(field);

            int hashValue = getHash_Value(music.id, 10);
            musicHashTable[hashValue].push_back(music);
            musicHashKey[music.id] = hashValue;
        }
        musicFile.close();
    }
    else
    {
        cerr << "Error opening music file for reading." << endl;
    }
}

// Function to add a artist
void addartist(unordered_map<int, vector<artist>> &artist_Hash,
               unordered_map<int, int> &a_HashKey, int tableSize)
{
    artist artist;
    cout << "Enter artist ID: ";
    cin >> artist.id;

    // Check if the artist ID already exists
    if (a_HashKey.count(artist.id) > 0)
    {
        cout << "artist ID already exists. Please enter a unique ID." << endl;
        return;
    }

    cout << "Enter artist Name: ";
    cin.ignore();
    getline(cin, artist.name);
    cout << "Enter artist gender: ";
    getline(cin, artist.gender);

    int hashValue = getHash_Value(artist.id, tableSize);
    artist_Hash[hashValue].push_back(artist);
    a_HashKey[artist.id] = hashValue;

    // Save the details to the files
    save_artist_info(artist_Hash);
    save_artist_Hash_info(a_HashKey);
}

// Function to display all artists
void disp_Artists(const unordered_map<int, vector<artist>> &artist_Hash)
{
    for (const auto &entry : artist_Hash)
    {
        for (const artist &artist : entry.second)
        {
            cout << "artist ID: " << artist.id << endl;
            cout << "artist Name: " << artist.name << endl;
            cout << "artist gender: " << artist.gender << endl;
            cout << endl;
        }
    }
}

// Function to add an music
void addmusic(unordered_map<int, vector<music>> &musicHashTable,
              unordered_map<int, int> &musicHashKey, int tableSize,
              const unordered_map<int, vector<artist>> &artist_Hash, unordered_map<int, int> &a_HashKey)
{
    music music;
    cout << "Enter music ID: ";
    cin >> music.id;

    // Check if the music ID already exists
    if (musicHashKey.count(music.id) > 0)
    {
        cout << "music ID already exists. Please enter a unique ID." << endl;
        return;
    }

    cout << "Enter music Name: ";
    cin.ignore();
    getline(cin, music.name);

    // Check if the entered artist ID exists
    int artistId;
label1:
    cout << "Enter artist ID to link: ";
    cin >> artistId;
    if (a_HashKey.count(artistId) == 0)
    {
        cout << "artist with ID " << artistId << " does not exist." << endl;
        goto label1;
    }
    music.artistId = artistId;

    int hashValue = getHash_Value(music.id, tableSize);
    musicHashTable[hashValue].push_back(music);
    musicHashKey[music.id] = hashValue;

    // Save the details to the files
    save_music_info(musicHashTable);
    save_music_Hash_info(musicHashKey);
}

// Function to search for a artist
void searchartist(const unordered_map<int, vector<artist>> &artist_Hash,
                  const unordered_map<int, int> &a_HashKey, int tableSize)
{
    int artistId;
    cout << "Enter artist ID to search: ";
    cin >> artistId;

    auto it = a_HashKey.find(artistId);
    if (it != a_HashKey.end())
    {
        int hashValue = it->second;
        const vector<artist> &artists = artist_Hash.at(hashValue);
        for (const artist &artist : artists)
        {
            if (artist.id == artistId)
            {
                cout << "artist ID: " << artist.id << endl;
                cout << "artist Name: " << artist.name << endl;
                cout << "artist gender: " << artist.gender << endl;
                return;
            }
        }
    }

    cout << "artist not found." << endl;
}

// Function to search for an music
void search_Music_Record(const unordered_map<int, vector<music>> &musicHashTable,
                         const unordered_map<int, int> &musicHashKey, int tableSize)
{
    int musicId;
    cout << "Enter music ID to search: ";
    cin >> musicId;

    auto it = musicHashKey.find(musicId);
    if (it != musicHashKey.end())
    {
        int hashValue = it->second;
        const vector<music> &musics = musicHashTable.at(hashValue);
        for (const music &music : musics)
        {
            if (music.id == musicId)
            {
                cout << "music ID: " << music.id << endl;
                cout << "music Name: " << music.name << endl;
                cout << "artist ID: " << music.artistId << endl;
                return;
            }
        }
    }

    cout << "music not found." << endl;
}

// Function to delete associated musics with artist (called automatically)
void deleteAssociatedmusics(unordered_map<int, vector<music>> &musicHashTable,
                            unordered_map<int, int> &musicHashKey,
                            int artistId)
{
    for (auto &musicPair : musicHashTable)
    {
        vector<music> &musics = musicPair.second;
        auto iter = musics.begin();
        while (iter != musics.end())
        {
            if (iter->artistId == artistId)
            {
                int musicId = iter->id;
                iter = musics.erase(iter);
                musicHashKey.erase(musicId);
            }
            else
            {
                ++iter;
            }
        }
    }
}

// Function to delete a artist
void deleteartist(unordered_map<int, vector<artist>> &artist_Hash,
                  unordered_map<int, int> &a_HashKey,
                  unordered_map<int, vector<music>> &musicHashTable,
                  unordered_map<int, int> &musicHashKey)
{
    int artistId;
    cout << "Enter artist ID to delete: ";
    cin >> artistId;

    auto it = a_HashKey.find(artistId);
    if (it != a_HashKey.end())
    {
        int hashValue = it->second;
        vector<artist> &artists = artist_Hash[hashValue];
        for (auto iter = artists.begin(); iter != artists.end(); ++iter)
        {
            if (iter->id == artistId)
            {
                artists.erase(iter);
                a_HashKey.erase(it);
                cout << "artist deleted successfully." << endl;

                // Delete associated musics
                deleteAssociatedmusics(musicHashTable, musicHashKey, artistId);

                // Save the updated details to the files
                save_artist_info(artist_Hash);
                save_artist_Hash_info(a_HashKey);
                save_music_info(musicHashTable);
                save_music_Hash_info(musicHashKey);
                return;
            }
        }
    }

    cout << "artist not found." << endl;
}

// Function to delete an music
void deletemusic(unordered_map<int, vector<music>> &musicHashTable,
                 unordered_map<int, int> &musicHashKey)
{
    int musicId;
    cout << "Enter music ID to delete: ";
    cin >> musicId;

    auto it = musicHashKey.find(musicId);
    if (it != musicHashKey.end())
    {
        int hashValue = it->second;
        vector<music> &musics = musicHashTable[hashValue];
        for (auto iter = musics.begin(); iter != musics.end(); ++iter)
        {
            if (iter->id == musicId)
            {
                musics.erase(iter);
                musicHashKey.erase(it);
                cout << "music deleted successfully." << endl;

                // Save the updated details to the files
                save_music_info(musicHashTable);
                save_music_Hash_info(musicHashKey);
                return;
            }
        }
    }

    cout << "music not found." << endl;
}

// Function to display all musics
void displaymusics(const unordered_map<int, vector<music>> &musicHashTable)
{
    for (const auto &entry : musicHashTable)
    {
        for (const music &music : entry.second)
        {
            cout << "music ID: " << music.id << endl;
            cout << "music Name: " << music.name << endl;
            cout << "artist ID: " << music.artistId << endl;
            cout << endl;
        }
    }
}

// to display the file contents
void displayFileContents(const string &filePath)
{
    ifstream file(filePath);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            cout << line << endl;
        }
        file.close();
    }
    else
    {
        cout << "Error opening file: " << filePath << endl;
    }
}
int main()
{
    int tableSize = 10;
    // cout << "Enter the hash table size: ";
    // cin >> tableSize;

    unordered_map<int, vector<artist>> artist_Hash;
    unordered_map<int, vector<music>> musicHashTable;
    unordered_map<int, int> a_HashKey;
    unordered_map<int, int> musicHashKey;

    loadArtistData(artist_Hash, a_HashKey);
    loadMusicData(musicHashTable, musicHashKey);
    int choice;
    do
    {
        cout << "#####"
             << "##### Music Record Management System ########" << endl;
        cout << "##\t\t"
             << "1. Add Artist"
             << "\t\t\t##" << endl;
        cout << "##\t\t"
             << "2. Add music"
             << "\t\t\t##" << endl;
        cout << "##\t\t"
             << "3. Search artist"
             << "\t\t##" << endl;
        cout << "##\t\t"
             << "4. Search music"
             << "\t\t\t##" << endl;
        cout << "##\t\t"
             << "5. Delete artist"
             << "\t\t##" << endl;
        cout << "##\t\t"
             << "6. Delete music"
             << "\t\t\t##" << endl;
        cout << "##\t\t"
             << "7. Display artists"
             << "\t\t##" << endl;
        cout << "##\t\t"
             << "8. Display musics"
             << "\t\t##" << endl;
        cout << "##\t\t"
             << "0. Exit"
             << "\t\t\t\t##" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addartist(artist_Hash, a_HashKey, tableSize);
            break;
        case 2:
            addmusic(musicHashTable, musicHashKey, tableSize, artist_Hash, a_HashKey);
            break;
        case 3:
            searchartist(artist_Hash, a_HashKey, tableSize);
            break;
        case 4:
            search_Music_Record(musicHashTable, musicHashKey, tableSize);
            break;
        case 5:
            deleteartist(artist_Hash, a_HashKey, musicHashTable, musicHashKey);
            break;
        case 6:
            deletemusic(musicHashTable, musicHashKey);
            break;
        case 7:
            disp_Artists(artist_Hash);
            break;
        case 8:
            displaymusics(musicHashTable);
            break;
        case 0:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }

        cout << endl;
    } while (choice != 0);

    return 0;
}