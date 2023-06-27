// Added : updated deleteMusic() function such that associated artists are deleted
// Added : input validation
// Almost Done: No changes needed --> Imported from final1.cpp
// Should add: -> modify function
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <functional>

// Structure for Music details
struct Music
{
    int id;
    std::string name;
    std::string location;
};

// Structure for artist details
struct artist
{
    int id;
    std::string name;
    int MusicId;
};

// Function to calculate the hash value
int hashFunction(int key, int tableSize)
{
    std::hash<int> hasher;
    return hasher(key) % tableSize;
}

// Function to save Music details to file
void saveMusicDetails(const std::unordered_map<int, std::vector<Music>> &MusicHashTable)
{
    std::ofstream MusicFile("Musics.txt");
    if (MusicFile)
    {
        for (const auto &entry : MusicHashTable)
        {
            for (const Music &Music : entry.second)
            {
                MusicFile << Music.id << "," << Music.name << "," << Music.location << std::endl;
            }
        }
        MusicFile.close();
    }
    else
    {
        std::cerr << "Error opening Music file for writing." << std::endl;
    }
}

// Function to save artist details to file
void saveartistDetails(const std::unordered_map<int, std::vector<artist>> &artistHashTable)
{
    std::ofstream artistFile("artists.txt");
    if (artistFile)
    {
        for (const auto &entry : artistHashTable)
        {
            for (const artist &artist : entry.second)
            {
                artistFile << artist.id << "," << artist.name << "," << artist.MusicId << std::endl;
            }
        }
        artistFile.close();
    }
    else
    {
        std::cerr << "Error opening artist file for writing." << std::endl;
    }
}

// Function to save Music hash values and keys to file
void saveMusicHashDetails(const std::unordered_map<int, int> &MusicHashKey)
{
    std::ofstream MusicHashFile("Music_hash.txt");
    if (MusicHashFile)
    {
        for (const auto &entry : MusicHashKey)
        {
            MusicHashFile << entry.first << "," << entry.second << std::endl;
        }
        MusicHashFile.close();
    }
    else
    {
        std::cerr << "Error opening Music hash file for writing." << std::endl;
    }
}

// Function to save artist hash values and keys to file
void saveartistHashDetails(const std::unordered_map<int, int> &artistHashKey)
{
    std::ofstream artistHashFile("artist_hash.txt");
    if (artistHashFile)
    {
        for (const auto &entry : artistHashKey)
        {
            artistHashFile << entry.first << "," << entry.second << std::endl;
        }
        artistHashFile.close();
    }
    else
    {
        std::cerr << "Error opening artist hash file for writing." << std::endl;
    }
}

// Function to add a Music
void addMusic(std::unordered_map<int, std::vector<Music>> &MusicHashTable,
              std::unordered_map<int, int> &MusicHashKey, int tableSize)
{
    Music Music;
    std::cout << "Enter Music ID: ";
    std::cin >> Music.id;

    // Check if the Music ID already exists
    if (MusicHashKey.count(Music.id) > 0)
    {
        std::cout << "Music ID already exists. Please enter a unique ID." << std::endl;
        return;
    }

    std::cout << "Enter Music Name: ";
    std::cin.ignore();
    std::getline(std::cin, Music.name);
    std::cout << "Enter Music Recorded Location: ";
    std::getline(std::cin, Music.location);

    int hashValue = hashFunction(Music.id, tableSize);
    MusicHashTable[hashValue].push_back(Music);
    MusicHashKey[Music.id] = hashValue;

    // Save the details to the files
    saveMusicDetails(MusicHashTable);
    saveMusicHashDetails(MusicHashKey);
}

// Function to display all Musics
void displayMusics(const std::unordered_map<int, std::vector<Music>> &MusicHashTable)
{
    for (const auto &entry : MusicHashTable)
    {
        for (const Music &Music : entry.second)
        {
            std::cout << "Music ID: " << Music.id << std::endl;
            std::cout << "Music Name: " << Music.name << std::endl;
            std::cout << "Music Location: " << Music.location << std::endl;
            std::cout << std::endl;
        }
    }
}

// Function to add an artist
void addartist(std::unordered_map<int, std::vector<artist>> &artistHashTable,
               std::unordered_map<int, int> &artistHashKey, int tableSize,
               const std::unordered_map<int, std::vector<Music>> &MusicHashTable, std::unordered_map<int, int> &MusicHashKey)
{
    artist artist;
    std::cout << "Enter artist ID: ";
    std::cin >> artist.id;

    // Check if the artist ID already exists
    if (artistHashKey.count(artist.id) > 0)
    {
        std::cout << "artist ID already exists. Please enter a unique ID." << std::endl;
        return;
    }

    std::cout << "Enter artist Name: ";
    std::cin.ignore();
    std::getline(std::cin, artist.name);

    // Check if the entered Music ID exists
    int MusicId;
    std::cout << "Enter Music ID: ";
    std::cin >> MusicId;
    if (MusicHashKey.count(MusicId) == 0)
    {
        std::cout << "Music with ID " << MusicId << " does not exist." << std::endl;

        char choice;
        std::cout << "Do you want to display available Musics? (Y/N): ";
        std::cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            displayMusics(MusicHashTable);
        }

        return;
    }
    artist.MusicId = MusicId;

    int hashValue = hashFunction(artist.id, tableSize);
    artistHashTable[hashValue].push_back(artist);
    artistHashKey[artist.id] = hashValue;

    // Save the details to the files
    saveartistDetails(artistHashTable);
    saveartistHashDetails(artistHashKey);
}

// Function to search for a Music
void searchMusic(const std::unordered_map<int, std::vector<Music>> &MusicHashTable,
                 const std::unordered_map<int, int> &MusicHashKey, int tableSize)
{
    int MusicId;
    std::cout << "Enter Music ID to search: ";
    std::cin >> MusicId;

    auto it = MusicHashKey.find(MusicId);
    if (it != MusicHashKey.end())
    {
        int hashValue = it->second;
        const std::vector<Music> &Musics = MusicHashTable.at(hashValue);
        for (const Music &Music : Musics)
        {
            if (Music.id == MusicId)
            {
                std::cout << "Music ID: " << Music.id << std::endl;
                std::cout << "Music Name: " << Music.name << std::endl;
                std::cout << "Music Location: " << Music.location << std::endl;
                return;
            }
        }
    }

    std::cout << "Music not found." << std::endl;
}

// Function to search for an artist
void searchartist(const std::unordered_map<int, std::vector<artist>> &artistHashTable,
                  const std::unordered_map<int, int> &artistHashKey, int tableSize)
{
    int artistId;
    std::cout << "Enter artist ID to search: ";
    std::cin >> artistId;

    auto it = artistHashKey.find(artistId);
    if (it != artistHashKey.end())
    {
        int hashValue = it->second;
        const std::vector<artist> &artists = artistHashTable.at(hashValue);
        for (const artist &artist : artists)
        {
            if (artist.id == artistId)
            {
                std::cout << "artist ID: " << artist.id << std::endl;
                std::cout << "artist Name: " << artist.name << std::endl;
                std::cout << "Music ID: " << artist.MusicId << std::endl;
                return;
            }
        }
    }

    std::cout << "artist not found." << std::endl;
}

// Function to delete associated artists with Music (called automatically)
void deleteAssociatedartists(std::unordered_map<int, std::vector<artist>> &artistHashTable,
                             std::unordered_map<int, int> &artistHashKey,
                             int MusicId)
{
    for (auto &artistPair : artistHashTable)
    {
        std::vector<artist> &artists = artistPair.second;
        auto iter = artists.begin();
        while (iter != artists.end())
        {
            if (iter->MusicId == MusicId)
            {
                int artistId = iter->id;
                iter = artists.erase(iter);
                artistHashKey.erase(artistId);
            }
            else
            {
                ++iter;
            }
        }
    }
}

// Function to delete a Music
void deleteMusic(std::unordered_map<int, std::vector<Music>> &MusicHashTable,
                 std::unordered_map<int, int> &MusicHashKey,
                 std::unordered_map<int, std::vector<artist>> &artistHashTable,
                 std::unordered_map<int, int> &artistHashKey)
{
    int MusicId;
    std::cout << "Enter Music ID to delete: ";
    std::cin >> MusicId;

    auto it = MusicHashKey.find(MusicId);
    if (it != MusicHashKey.end())
    {
        int hashValue = it->second;
        std::vector<Music> &Musics = MusicHashTable[hashValue];
        for (auto iter = Musics.begin(); iter != Musics.end(); ++iter)
        {
            if (iter->id == MusicId)
            {
                Musics.erase(iter);
                MusicHashKey.erase(it);
                std::cout << "Music deleted successfully." << std::endl;

                // Delete associated artists
                deleteAssociatedartists(artistHashTable, artistHashKey, MusicId);

                // Save the updated details to the files
                saveMusicDetails(MusicHashTable);
                saveMusicHashDetails(MusicHashKey);
                saveartistDetails(artistHashTable);
                saveartistHashDetails(artistHashKey);
                return;
            }
        }
    }

    std::cout << "Music not found." << std::endl;
}

// Function to delete an artist
void deleteartist(std::unordered_map<int, std::vector<artist>> &artistHashTable,
                  std::unordered_map<int, int> &artistHashKey)
{
    int artistId;
    std::cout << "Enter artist ID to delete: ";
    std::cin >> artistId;

    auto it = artistHashKey.find(artistId);
    if (it != artistHashKey.end())
    {
        int hashValue = it->second;
        std::vector<artist> &artists = artistHashTable[hashValue];
        for (auto iter = artists.begin(); iter != artists.end(); ++iter)
        {
            if (iter->id == artistId)
            {
                artists.erase(iter);
                artistHashKey.erase(it);
                std::cout << "artist deleted successfully." << std::endl;

                // Save the updated details to the files
                saveartistDetails(artistHashTable);
                saveartistHashDetails(artistHashKey);
                return;
            }
        }
    }

    std::cout << "artist not found." << std::endl;
}

// Function to display all artists
void displayartists(const std::unordered_map<int, std::vector<artist>> &artistHashTable)
{
    for (const auto &entry : artistHashTable)
    {
        for (const artist &artist : entry.second)
        {
            std::cout << "artist ID: " << artist.id << std::endl;
            std::cout << "artist Name: " << artist.name << std::endl;
            std::cout << "Music ID: " << artist.MusicId << std::endl;
            std::cout << std::endl;
        }
    }
}

int main()
{
    int tableSize;
    std::cout << "Enter the hash table size: ";
    std::cin >> tableSize;

    std::unordered_map<int, std::vector<Music>> MusicHashTable;
    std::unordered_map<int, std::vector<artist>> artistHashTable;
    std::unordered_map<int, int> MusicHashKey;
    std::unordered_map<int, int> artistHashKey;

    int choice;
    do
    {
        std::cout << "------ Music Management System ------" << std::endl;
        std::cout << "1. Add Music" << std::endl;
        std::cout << "2. Add artist" << std::endl;
        std::cout << "3. Search Music" << std::endl;
        std::cout << "4. Search artist" << std::endl;
        std::cout << "5. Delete Music" << std::endl;
        std::cout << "6. Delete artist" << std::endl;
        std::cout << "7. Display Musics" << std::endl;
        std::cout << "8. Display artists" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            addMusic(MusicHashTable, MusicHashKey, tableSize);
            break;
        case 2:
            addartist(artistHashTable, artistHashKey, tableSize, MusicHashTable, MusicHashKey);
            break;
        case 3:
            searchMusic(MusicHashTable, MusicHashKey, tableSize);
            break;
        case 4:
            searchartist(artistHashTable, artistHashKey, tableSize);
            break;
        case 5:
            deleteMusic(MusicHashTable, MusicHashKey, artistHashTable, artistHashKey);
            break;
        case 6:
            deleteartist(artistHashTable, artistHashKey);
            break;
        case 7:
            displayMusics(MusicHashTable);
            break;
        case 8:
            displayartists(artistHashTable);
            break;
        case 0:
            std::cout << "Exiting..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }

        std::cout << std::endl;
    } while (choice != 0);

    return 0;
}