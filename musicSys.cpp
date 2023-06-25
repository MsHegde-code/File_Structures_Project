#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Function to split a string based on delimiter
vector<string> splitString(const string &str, char delimiter)
{
    vector<string> tokens;
    string token;
    size_t start = 0, end = 0;

    while ((end = str.find(delimiter, start)) != string::npos)
    {
        token = str.substr(start, end - start);
        tokens.push_back(token);
        start = end + 1;
    }

    tokens.push_back(str.substr(start));
    return tokens;
}

// Function to display all songs in the music system
void displayAllSongs(const string &filename)
{
    ifstream file(filename);
    string line;
    int songCount = 0;

    cout << "All Songs in the Music System:\n";
    cout << "----------------------------------\n";
    while (getline(file, line))
    {
        vector<string> songDetails = splitString(line, ',');
        cout << "Song Name: " << songDetails[0] << "\n";
        cout << "Artist: " << songDetails[1] << "\n";
        cout << "Duration: " << songDetails[2] << " seconds\n";

        songCount++;
    }

    cout << "Total Songs: " << songCount << "\n\n";
    file.close();
}

// Function to search for songs by artist
void searchByArtist(const string &filename)
{
    ifstream file(filename);
    string line, artist;
    bool found = false;

    cout << "Enter the artist name: ";
    cin.ignore();
    getline(cin, artist);

    cout << "\nSongs by Artist '" << artist << "':\n";
    while (getline(file, line))
    {
        vector<string> songDetails = splitString(line, ',');
        if (songDetails[1] == artist)
        {
            cout << "Song Name: " << songDetails[0] << "\n";
            cout << "Duration: " << songDetails[2] << " seconds\n";
            cout << "----------------------------------\n";
            found = true;
        }
    }

    if (!found)
    {
        cout << "No songs found for artist '" << artist << "'\n";
    }

    file.close();
}

// Function to add a new song to the music system
void addNewSong(const string &filename)
{
    ofstream file(filename, ios::app);
    string songName, artist, duration;

    cout << "Enter the song name: ";
    cin.ignore();
    getline(cin, songName);

    cout << "Enter the artist name: ";
    getline(cin, artist);

    cout << "Enter the duration in seconds: ";
    getline(cin, duration);

    file << songName << "," << artist << "," << duration << "\n";
    cout << "Song added successfully!\n\n";
    file.close();
}

// Function to modify a record in the music system
void modifyRecord(const string &filename)
{
    ifstream file(filename);
    ofstream tempFile("temp.txt");

    string line, songName, artist, duration;
    bool found = false;

    cout << "Enter the song name to modify: ";
    cin.ignore();
    getline(cin, songName);

    while (getline(file, line))
    {
        vector<string> songDetails = splitString(line, ',');
        if (songDetails[0] == songName)
        {
            cout << "Enter the new artist name: ";
            getline(cin, artist);

            cout << "Enter the new duration in seconds: ";
            getline(cin, duration);

            tempFile << songName << "," << artist << "," << duration << "\n";
            cout << "Record modified successfully!\n";
            found = true;
        }
        else
        {
            tempFile << line << "\n";
        }
    }

    if (!found)
    {
        cout << "No record found for song name '" << songName << "'\n";
    }

    file.close();
    tempFile.close();

    // Replace the original file with the modified file
    remove(filename.c_str());
    rename("temp.txt", filename.c_str());
}

int main()
{
    string filename = "music_system.txt";
    int choice;

    do
    {
        cout << "----- Music System Management -----\n";
        cout << "1. Display all songs\n";
        cout << "2. Search songs by artist\n";
        cout << "3. Add a new song\n";
        cout << "4. Modify a record\n";
        cout << "5. Exit\n";
        cout << "Enter your choice (1-5): ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            displayAllSongs(filename);
            break;
        case 2:
            searchByArtist(filename);
            break;
        case 3:
            addNewSong(filename);
            break;
        case 4:
            modifyRecord(filename);
            break;
        case 5:
            cout << "Exiting the program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 5);

    return 0;
}
