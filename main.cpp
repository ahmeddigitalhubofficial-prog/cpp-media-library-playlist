#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ================= MediaItem (Base Class) =================

class MediaItem
{
protected:
    int id;
    string title;
    bool favorite;

public:
    MediaItem(int i, string t)
    {
        id = i;
        title = t;
        favorite = false;
    }

    virtual ~MediaItem() {}

    int getID()
    {
        return id;
    }

    string getTitle()
    {
        return title;
    }

    bool isFavorite()
    {
        return favorite;
    }

    void addToFavorite()
    {
        favorite = true;
    }
    void removeFavorite()
{
    favorite = false;
}

    virtual void display() = 0;

    virtual string saveToFile() = 0;

    virtual string getType() = 0;

    virtual string getCreator() = 0;

    virtual string getDuration() = 0;

    virtual string getRatingText() = 0;
};

// ================= Song (Derived Class) =================

class Song : public MediaItem
{
private:
    string artist;
    string duration;

public:
    Song(int id, string title, string artist, string duration)
        : MediaItem(id, title)
    {
        this->artist = artist;
        this->duration = duration;
    }

    void display() override
    {
        cout << "\n[Song]\n";
        cout << "ID: " << id << endl;
        cout << "Title: " << title << endl;
        cout << "Artist: " << artist << endl;
        cout << "Duration: " << duration << endl;

        if (favorite)
            cout << "Favorite: Yes\n";
    }

    string saveToFile() override
    {
        return "SONG," + to_string(id) + "," +
               title + "," +
               artist + "," +
               duration + "," +
               to_string(favorite);
    }

    string getType() override
    {
        return "Song";
    }

    string getCreator() override
    {
        return artist;
    }

    string getDuration() override
    {
        return duration;
    }

    string getRatingText() override
    {
        return "-";
    }
};

// ================= Movie (Derived Class) =================

class Movie : public MediaItem
{
private:
    string director;
    string duration;
    double rating;

public:
    Movie(int id, string title, string director, string duration, double rating)
        : MediaItem(id, title)
    {
        this->director = director;
        this->duration = duration;
        this->rating = rating;
    }

    void display() override
    {
        cout << "\n[Movie]\n";
        cout << "ID: " << id << endl;
        cout << "Title: " << title << endl;
        cout << "Director: " << director << endl;
        cout << "Duration: " << duration << endl;
        cout << "Rating: " << rating << endl;

        if (favorite)
            cout << "Favorite: Yes\n";
    }

    string saveToFile() override
    {
        return "MOVIE," + to_string(id) + "," +
               title + "," +
               director + "," +
               duration + "," +
               to_string(rating) + "," +
               to_string(favorite);
    }

    string getType() override
    {
        return "Movie";
    }

    string getCreator() override
    {
        return director;
    }

    string getDuration() override
    {
        return duration;
    }

    string getRatingText() override
    {
        ostringstream value;
        value << fixed << setprecision(1) << rating;
        return value.str();
    }
};

// ================= Node =================

class Node
{
public:
    MediaItem* data;
    Node* next;
    Node* prev;

    Node(MediaItem* item)
    {
        data = item;
        next = NULL;
        prev = NULL;
    }
};

// ================= MediaList (Doubly Linked List) =================

class MediaList
{
private:
    Node* head;
    Node* tail;
    Node* current;

public:
    MediaList()
    {
        head = NULL;
        tail = NULL;
        current = NULL;
    }

    ~MediaList()
    {
        Node* temp = head;
        while (temp != NULL)
        {
            Node* nextNode = temp->next;
            delete temp->data;
            delete temp;
            temp = nextNode;
        }
    }

    void insert(MediaItem* item)
    {
        Node* newNode = new Node(item);

        if (head == NULL)
        {
            head = tail = current = newNode;
            return;
        }

        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;

        // Ensure current pointer is initialized properly if it was empty
        if (current == NULL) {
            current = head;
        }
    }

    void displayAll()
    {
        if (head == NULL)
        {
            cout << "\nList is Empty\n";
            return;
        }

        Node* temp = head;

        while (temp != NULL)
        {
            temp->data->display();

            if (temp == current) {
                cout << "-> [Currently Active Track]\n";
            }
            cout << "------------------\n";
            temp = temp->next;
        }
    }

    void searchByTitle(string title)
    {
        Node* temp = head;
        bool found = false;

        while (temp != NULL)
        {
            if (temp->data->getTitle() == title)
            {
                cout << "\nFound Successfully\n";
                temp->data->display();
                found = true;
            }
            temp = temp->next;
        }

        if (!found) {
            cout << "\nNot Found\n";
        }
    }

    void deleteByID(int id)
    {
        Node* temp = head;

        while (temp != NULL)
        {
            if (temp->data->getID() == id)
            {
                if (temp == current)
                {
                    if (current->next != NULL) {
                        current = current->next;
                    } else {
                        current = current->prev;
                    }
                }

                if (temp == head)
                {
                    head = head->next;
                    if (head)
                        head->prev = NULL;
                    else
                        tail = NULL;
                }
                else if (temp == tail)
                {
                    tail = tail->prev;
                    if (tail)
                        tail->next = NULL;
                    else
                        head = NULL;
                }
                else
                {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
                }

                delete temp->data;
                delete temp;

                cout << "\nDeleted Successfully\n";
                return;
            }

            temp = temp->next;
        }

        cout << "\nID Not Found\n";
    }

    void nextItem()
    {
        if (current == NULL)
        {
            cout << "\nNo Items\n";
            return;
        }

        if (current->next == NULL)
        {
            cout << "\nLast Item Reached\n";
            return;
        }

        current = current->next;
        cout << "\nCurrent Item:\n";
        current->data->display();
    }

    void previousItem()
    {
        if (current == NULL)
        {
            cout << "\nNo Items\n";
            return;
        }

        if (current->prev == NULL)
        {
            cout << "\nFirst Item Reached\n";
            return;
        }

        current = current->prev;
        cout << "\nCurrent Item:\n";
        current->data->display();
    }

    void showCurrent()
    {
        if (current == NULL)
        {
            cout << "\nNo Current Item\n";
            return;
        }

        current->data->display();
    }

    void addFavorite(int id)
    {
        Node* temp = head;

        while (temp != NULL)
        {
            if (temp->data->getID() == id)
            {
                temp->data->addToFavorite();
                cout << "\nAdded To Favorites\n";
                return;
            }

            temp = temp->next;
        }

        cout << "\nID Not Found\n";
    }
    void removeFavorite(int id)
{
    Node* temp = head;

    while(temp != NULL)
    {
        if(temp->data->getID() == id)
        {
            temp->data->removeFavorite();
            cout << "\nRemoved From Favorites\n";
            return;
        }

        temp = temp->next;
    }

    cout << "\nID Not Found\n";
}
    void showSongs()
    {
    Node* temp = head;

    while(temp != NULL)
    {
        Song* s =
        dynamic_cast<Song*>(temp->data);

        if(s != NULL)
        {
            s->display();
            cout << "------------------\n";
        }

        temp = temp->next;
    }
}
void showMovies()
{
    Node* temp = head;

    while(temp != NULL)
    {
        Movie* m =
        dynamic_cast<Movie*>(temp->data);

        if(m != NULL)
        {
            m->display();
            cout << "------------------\n";
        }

        temp = temp->next;
    }
}
    void showFavorites()
    {
        Node* temp = head;
        bool found = false;

        while (temp != NULL)
        {
            if (temp->data->isFavorite())
            {
                temp->data->display();
                cout << "------------------\n";
                found = true;
            }

            temp = temp->next;
        }

        if (!found)
            cout << "\nNo Favorites Yet\n";
    }

    void saveData(bool showMessage = true)
    {
        ofstream file("media.txt");

        Node* temp = head;

        while (temp != NULL)
        {
            file << temp->data->saveToFile() << endl;
            temp = temp->next;
        }

        file.close();
        if (showMessage)
            cout << "\nSaved Successfully\n";
    }

    bool loadData()
    {
        ifstream file("media.txt");

        if (!file)
            return false;

        string line;
        int loadedCount = 0;

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            vector<string> fields;
            string field;
            stringstream parser(line);

            while (getline(parser, field, ','))
                fields.push_back(field);

            try
            {
                MediaItem* item = NULL;

                if (fields.size() == 6 && fields[0] == "SONG")
                {
                    item = new Song(stoi(fields[1]), fields[2],
                                    fields[3], fields[4]);

                    if (stoi(fields[5]) == 1)
                        item->addToFavorite();
                }
                else if (fields.size() == 7 && fields[0] == "MOVIE")
                {
                    item = new Movie(stoi(fields[1]), fields[2],
                                     fields[3], fields[4],
                                     stod(fields[5]));

                    if (stoi(fields[6]) == 1)
                        item->addToFavorite();
                }

                if (item != NULL)
                {
                    insert(item);
                    loadedCount++;
                }
            }
            catch (...)
            {
                cout << "\nSkipped an invalid line in media.txt\n";
            }
        }

        if (loadedCount > 0)
        {
            cout << ">> [System Info] Loaded " << loadedCount
                 << " items from media.txt!\n";
            return true;
        }

        return false;
    }
    bool idExists(int id)
{
    Node* temp = head;

    while(temp != NULL)
    {
        if(temp->data->getID() == id)
            return true;

        temp = temp->next;
    }

    return false;
}
int getCount()
{
    int count = 0;

    Node* temp = head;

    while(temp != NULL)
    {
        count++;
        temp = temp->next;
    }

    return count;
}

void showCompactPlaylist(const string& type, bool favoritesOnly = false)
{
    Node* temp = head;
    bool found = false;

    string tableTitle;
    if (favoritesOnly)
        tableTitle = "FAVORITES PLAYLIST";
    else if (type == "Movie")
        tableTitle = "MOVIES PLAYLIST";
    else
        tableTitle = "SONGS PLAYLIST";

    cout << "\n+---------------------------------------------------------------------------------------------------+\n";
    cout << "| " << left << setw(97) << tableTitle << " |\n";
    cout << "+------+-----+----------------------+----------------------+--------------------+--------+----------+\n";
    cout << "| Type | ID  | Title                | Artist / Director    | Duration           | Rating | Favorite |\n";
    cout << "+------+-----+----------------------+----------------------+--------------------+--------+----------+\n";

    while (temp != NULL)
    {
        bool correctType = type.empty() || temp->data->getType() == type;
        bool correctFavorite = !favoritesOnly || temp->data->isFavorite();

        if (correctType && correctFavorite)
        {
            cout << "| " << left << setw(4) << temp->data->getType()
                 << " | " << right << setw(3) << temp->data->getID()
                 << " | " << left << setw(20) << temp->data->getTitle().substr(0, 20)
                 << " | " << setw(20) << temp->data->getCreator().substr(0, 20)
                 << " | " << setw(18) << temp->data->getDuration().substr(0, 18)
                 << " | " << setw(6) << temp->data->getRatingText()
                 << " | " << setw(8) << (temp->data->isFavorite() ? "Yes *" : "No")
                 << " |\n";
            found = true;
        }

        temp = temp->next;
    }

    if (!found)
        cout << "| No " << left << setw(91)
             << (favoritesOnly ? "favorite media available."
                               : type + "s available.")
             << "|\n";

    cout << "+------+-----+----------------------+----------------------+--------------------+--------+----------+\n";
}
};

// ================= MediaHubManager =================

class MediaHubManager
{
private:
    MediaList mediaList;

public:
    bool loadSavedData()
    {
        return mediaList.loadData();
    }

    void loadDefaultData()
    {
        // 5 Default Songs pre-existing in the system
        mediaList.insert(new Song(1, "Blinding Lights", "The Weeknd", "3 min 20 sec"));
        mediaList.insert(new Song(2, "Shape of You", "Ed Sheeran", "233 sec"));
        mediaList.insert(new Song(3, "Stay", "The Kid LAROI", "2 min 21 sec"));
        mediaList.insert(new Song(4, "As It Was", "Harry Styles", "167 sec"));
        mediaList.insert(new Song(5, "Someone Like You", "Adele", "4 min 45 sec"));

        // 5 Default Movies pre-existing in the system
        mediaList.insert(new Movie(6, "Inception", "Christopher Nolan", "2 hr 28 min 12 sec", 8.8));
        mediaList.insert(new Movie(7, "The Dark Knight", "Christopher Nolan", "2 hr 32 min", 9.0));
        mediaList.insert(new Movie(8, "Interstellar", "Christopher Nolan", "2 hr 49 min 5 sec", 8.7));
        mediaList.insert(new Movie(9, "Avatar", "James Cameron", "2 hr 42 sec", 7.9));
        mediaList.insert(new Movie(10, "The Matrix", "Lana Wachowski", "2 hr 16 min", 8.7));

        cout << ">> [System Info] Default Media Hub Loaded Successfully with 10 Items!\n";
    }

    void addSong()
    {
        int id;
        string title;
        string artist;
        string duration;

        cout << "Enter ID: ";
        while(!(cin >> id))
        {
        cin.clear();
        cin.ignore(1000,'\n');

        cout << "Invalid ID\n";
        cout << "Enter ID: ";
        }        cin.ignore();
if(mediaList.idExists(id))
{
    cout << "\nID Already Exists\n";
    return;
}
        cout << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Artist: ";
        getline(cin, artist);

        cout << "Enter Duration (e.g., 3 min 20 sec / 200 sec): ";
        getline(cin, duration);

        mediaList.insert(new Song(id, title, artist, duration));
        mediaList.saveData(false);
        cout << "\nSong Added Successfully\n";
        showSongsPlaylist();
    }
    void showSongs()
{
    mediaList.showSongs();
}
    void showMovies()
{
    mediaList.showMovies();
}
    void showSongsPlaylist()
    {
        mediaList.showCompactPlaylist("Song");
    }

    void showMoviesPlaylist()
    {
        mediaList.showCompactPlaylist("Movie");
    }

    void showFavoritesPlaylist()
    {
        mediaList.showCompactPlaylist("", true);
    }

    void showAllPlaylists()
    {
        showMoviesPlaylist();
        showSongsPlaylist();
        showFavoritesPlaylist();
    }

    void displayOrganizedPlaylists()
    {
        showAllPlaylists();
    }
    void addMovie()
    {
        int id;
        string title;
        string director;
        string duration;
        double rating;

        cout << "Enter ID: ";
        while(!(cin >> id))
{
    cin.clear();
    cin.ignore(1000,'\n');

    cout << "Invalid ID\n";
    cout << "Enter ID: ";
}
        cin.ignore();
if(mediaList.idExists(id))
{
    cout << "\nID Already Exists\n";
    return;
}
        cout << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Director: ";
        getline(cin, director);

        cout << "Enter Duration (e.g., 2 hr 15 min 30 sec / 2 hr 10 min): ";
        getline(cin, duration);

        cout << "Enter Rating: ";
        while(!(cin >> rating))
        {
        cin.clear();
        cin.ignore(1000,'\n');

        cout << "Invalid Rating\n";
        cout << "Enter Rating: ";
        }
        mediaList.insert(new Movie(id, title, director, duration, rating));
        mediaList.saveData(false);
        cout << "\nMovie Added Successfully\n";
        showMoviesPlaylist();
    }

    void displayAll()
    {
        mediaList.displayAll();
    }

void search()
{
    string title;

    cin.ignore();

    cout << "Enter Title: ";
    getline(cin, title);

    mediaList.searchByTitle(title);
}
    void deleteItem()
    {
        int id;
        showAllPlaylists();
        cout << "Enter ID: ";
        while(!(cin >> id))
{
    cin.clear();
    cin.ignore(1000,'\n');

    cout << "Invalid ID\n";
    cout << "Enter ID: ";
}

        mediaList.deleteByID(id);
        mediaList.saveData(false);
    }

    void nextItem()
    {
        mediaList.nextItem();
    }

    void previousItem()
    {
        mediaList.previousItem();
    }

    void currentItem()
    {
        mediaList.showCurrent();
    }

    void addFavorite()
    {
        int id;
        showAllPlaylists();
        cout << "Enter ID: ";
        while(!(cin >> id))
{
    cin.clear();
    cin.ignore(1000,'\n');

    cout << "Invalid ID\n";
    cout << "Enter ID: ";
}

        mediaList.addFavorite(id);
        mediaList.saveData(false);
    }
void removeFavorite()
{
    int id;

    showAllPlaylists();
    cout << "Enter ID: ";
    while(!(cin >> id))
{
    cin.clear();
    cin.ignore(1000,'\n');

    cout << "Invalid ID\n";
    cout << "Enter ID: ";
}

    mediaList.removeFavorite(id);
    mediaList.saveData(false);
}

    void showFavorites()
    {
        mediaList.showFavorites();
    }
    void showCount()
{
    cout << "\nTotal Items: "
         << mediaList.getCount()
         << endl;
}

    void saveData(bool showMessage = true)
    {
        mediaList.saveData(showMessage);
    }
};

// ================= Users =================

class UserManager
{
public:
    void saveName(const string& name, const string& role)
    {
        ofstream file("users.txt", ios::app);

        if (!file)
        {
            cout << "\nCould not open users.txt\n";
            return;
        }

        file << role << "," << name << endl;
    }
};

int readChoice()
{
    int choice;

    while (!(cin >> choice))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "   Invalid input! Enter a number: ";
    }

    return choice;
}

void showAdminMenu(const string& name)
{
    cout << "\n+---------------------------------------+\n";
    cout << "|              ADMIN MENU               |\n";
    cout << "+---------------------------------------+\n";
    cout << "   Welcome, " << name << "\n";
    cout << "+---------------------------------------+\n";
    cout << "|  [1] [+] Add Song                     |\n";
    cout << "|  [2] [+] Add Movie                    |\n";
    cout << "|  [3] [-] Delete By ID                 |\n";
    cout << "|  [4] [=] Display All                  |\n";
    cout << "|  [5] [#] Total Media Count            |\n";
    cout << "|  [6] [S] Save Media To TXT            |\n";
    cout << "|  [0] [X] Logout                       |\n";
    cout << "+---------------------------------------+\n";
    cout << "   Enter Choice: ";
}

void showUserMenu(const string& name)
{
    cout << "\n+---------------------------------------+\n";
    cout << "|               USER MENU               |\n";
    cout << "+---------------------------------------+\n";
    cout << "   Welcome, " << name << "\n";
    cout << "+---------------------------------------+\n";
    cout << "|  [ 1] [=] Display All                 |\n";
    cout << "|  [ 2] [?] Search By Title             |\n";
    cout << "|  [ 3] [@] Show Current Item           |\n";
    cout << "|  [ 4] [>] Next Item                   |\n";
    cout << "|  [ 5] [<] Previous Item               |\n";
    cout << "|  [ 6] [*] Add To Favorites            |\n";
    cout << "|  [ 7] [*] Show Favorites              |\n";
    cout << "|  [ 8] [-] Remove Favorite             |\n";
    cout << "|  [ 9] [M] Show Songs Only             |\n";
    cout << "|  [10] [F] Show Movies Only            |\n";
    cout << "|  [ 0] [X] Logout                      |\n";
    cout << "+---------------------------------------+\n";
    cout << "   Enter Choice: ";
}

void runAdminMenu(MediaHubManager& manager, const string& name)
{
    int choice;

    do
    {
        showAdminMenu(name);
        choice = readChoice();
        clearScreen();

        switch (choice)
        {
        case 1: manager.addSong(); break;
        case 2: manager.addMovie(); break;
        case 3: manager.deleteItem(); break;
        case 4: manager.displayOrganizedPlaylists(); break;
        case 5: manager.showCount(); break;
        case 6: manager.saveData(); break;
        case 0: cout << "\nLogged out successfully.\n"; break;
        default: cout << "\nInvalid Choice\n";
        }
    }
    while (choice != 0);
}

void runUserMenu(MediaHubManager& manager, const string& name)
{
    int choice;

    do
    {
        showUserMenu(name);
        choice = readChoice();
        clearScreen();

        switch (choice)
        {
        case 1: manager.displayOrganizedPlaylists(); break;
        case 2: manager.search(); break;
        case 3: manager.currentItem(); break;
        case 4: manager.nextItem(); break;
        case 5: manager.previousItem(); break;
        case 6: manager.addFavorite(); break;
        case 7: manager.showFavoritesPlaylist(); break;
        case 8: manager.removeFavorite(); break;
        case 9: manager.showSongs(); break;
        case 10: manager.showMovies(); break;
        case 0: cout << "\nLogged out successfully.\n"; break;
        default: cout << "\nInvalid Choice\n";
        }
    }
    while (choice != 0);
}

// ================= MAIN =================

int main()
{
    MediaHubManager manager;
    UserManager users;

    if (!manager.loadSavedData())
    {
        manager.loadDefaultData();
        manager.saveData(false);
    }

    int roleChoice;

    do
    {
        cout << "\n+---------------------------------------+\n";
        cout << "|           MEDIA HUB SYSTEM            |\n";
        cout << "+---------------------------------------+\n";
        cout << "|  [1] [A] Login As Admin               |\n";
        cout << "|  [2] [U] Login As User                |\n";
        cout << "|  [0] [X] Exit                         |\n";
        cout << "+---------------------------------------+\n";
        cout << "   Enter Choice: ";
        roleChoice = readChoice();
        clearScreen();

        if (roleChoice == 1 || roleChoice == 2)
        {
            string name;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "   Enter Your Name: ";
            getline(cin, name);

            if (name.empty())
            {
                cout << "\nName cannot be empty.\n";
                continue;
            }

            if (roleChoice == 1)
            {
                users.saveName(name, "ADMIN");
                runAdminMenu(manager, name);
            }
            else
            {
                users.saveName(name, "USER");
                runUserMenu(manager, name);
            }
        }
        else if (roleChoice == 0)
            cout << "\nGood Bye, and thank you!\n";
        else
            cout << "\nInvalid Choice\n";
    }
    while (roleChoice != 0);

    return 0;
}
