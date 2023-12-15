
/*-------------------------------------------
   Program: Library
   Course: CS 251, Fall 2023, UIC
   Author: MaryJo Santos
   Description: Implemented a simplified library management system that allows the 
                system to store and edit book titles, isbns, and statuses
  ------------------------------------------- */


// You cannot use any libraries beyond the ones we have given below.
// You must use output streams, file streams, vectors, and strings.
// You are not allowed to use any other containers or objects.
// You may not use structs, pointers, or dynamic memory allocations.

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

using namespace std;

// Function prototypes for all the glorious function decomposition
void printMenu();
void addEntry(vector<string> &titles, vector<string> &isbNums, vector<int> &coStatus, string book, string num);
void addEntry(vector<string> &titles, vector<string> &isbNums, vector<int> &coStatus, string book, string num, int status);
int calcIsbn(string isbn);
bool checkInput(string name);
void getFileData(string line, string &book, string &isbn, int &ckout);
void displayLibrary(vector<string> &titles, vector<string> &isbNums, vector<int> &coStatus);
void displayLibraryEntry(string book, string isbn, int status);
void listInvalid(vector<string> titles, vector<string> isbNums, vector<int> coStatus);
void outputLibraryFile(string filename, vector<string> titles, vector<string> isbNums, vector<int> coStatus);
void printCheckout(vector<int> coStatus);
void removeEntry(int index, vector<string> &titles, vector<string> &isbNums, vector<int> &coStatus);
int searchName(vector<string> titles);
int searchIsbn(vector<string> isbNums);
void searchResults(int index, vector<string> titles, vector<string> isbNums, vector<int> coStatus);



int main()
{
    string command, name, isbNumber, line;
    int bookNum, count = 0;
    ifstream file;
    vector<string> titles;
    vector<string> isbn;
    vector<int> ckoutStatus;

    cout << "Welcome to the Library Management System" << endl;
    cout << "----------------------------------------" << endl;
    cout << endl;

    do 
    {
        // Output the menu and acquire a user selection
        printMenu();
        cout << endl << "Enter a command (case does not matter): ";

        // We use getline for all user input to avoid needing to handle
        // input buffer issues relating to using both >> and getline
        getline(cin, command);
        cout << endl;

        // add to library
        if (command == "a" || command == "A") {
            cout << "What is the book title? " << endl;
            getline(cin, name);

            if (checkInput(name)) {
                cout << "What is the 13-digit ISBN (with hyphens)? " << endl;
                getline(cin, isbNumber);

                addEntry(titles, isbn, ckoutStatus, name, isbNumber);
                cout << "The Following Entry Was Added" << endl
                     << "-----------------------------" << endl;
    
                displayLibraryEntry(name, isbNumber, 0);
            }
        }

        // clear library
        else if (command == "c" || command == "C") {
            if (titles.size() > 0) { 
                titles.clear();
                isbn.clear();
                ckoutStatus.clear();
                cout << "Your library is now empty." << endl;
            }
            else {
                cout << "The library has no books." << endl;
            }
        }

        // display full library
        else if (command == "d" || command == "D") {
            cout << "Your Current Library" << endl
                 << "--------------------" <<  endl;
    
            if (titles.size() > 0) {
                displayLibrary(titles, isbn, ckoutStatus);
            }
            else {
                cout << "The library has no books." << endl;
            }
        }
        
        // display invalid entries in library
        else if (command == "i" || command == "I") {
            listInvalid(titles, isbn, ckoutStatus);
        }

        // add entries to library from file
        else if (command == "l" || command == "L") {
            count = 0;
            cout << "What database to read from? " << endl;
            getline(cin, name);
            file.open(name);

            if (!file.is_open()) {
                cout << "Could not find the database file." << endl;
            }
            else {
                while (!file.eof()) {
                    getline(file, line);
                    getFileData(line, name, isbNumber, bookNum);
                    addEntry(titles, isbn, ckoutStatus, name, isbNumber, bookNum);
                    count++;
                }
                file.close();
                
                cout << "Read in " << count << " lines from the file." << endl;
            }
        }

        // save current library to a new file
        else if (command == "o" || command == "O") {
            cout << "File Name: ";
            getline(cin, name);

            outputLibraryFile(name, titles, isbn, ckoutStatus);
        }

        // prints out the checkout summary of the library
        else if (command == "p" || command == "P") {
            printCheckout(ckoutStatus);
        }
        
        // removes a single entry with either title or isbn input
        else if (command == "r" || command == "R") {
            cout << "Would you like remove by (1) name or (2) ISBN." << endl
                 << "Enter the numeric choice: " << endl;
            getline(cin, line);

            if (line == "1") {
                bookNum = searchName(titles);
                removeEntry(bookNum, titles, isbn, ckoutStatus);
            }
            else if (line == "2") {
                bookNum = searchIsbn(isbn);
                removeEntry(bookNum, titles, isbn, ckoutStatus);
            }
            else {
                cout << "Invalid remove by choice option." << endl;
            }
        }

        // searches for an entry with either title or isbn
        else if (command == "s" || command == "S") {
            cout << "Would you like to search by (1) name or (2) ISBN." << endl
                 << "Enter the numeric choice: " << endl;
            getline(cin, name);

            if (name == "1") {
                bookNum = searchName(titles);
                searchResults(bookNum, titles, isbn, ckoutStatus);
            }
            else if (name == "2") {
                bookNum = searchIsbn(isbn);
                searchResults(bookNum, titles, isbn, ckoutStatus);
            }
            else {
                cout << "Invalid search by choice option." << endl;
            }
        }

        else if (command != "x" && command != "X") {
            cout << "Unknown command." << endl;
        }

        cout << endl;
    } while (!(command == "x" || command == "X"));

    return 0;
}

// Function definitions for all the glorious function decomposition
// Tip: alphabetical order makes it easy to find things if you have
//      a lot of function decomposition.

/// @brief print out the main menu of the management system
void printMenu() {
    cout << "Library Management Menu" << endl;
    cout << "-----------------------" << endl;
    cout << "A - Add Item To Library" << endl;
    cout << "C - Clear The Library Of All Entries" << endl;
    cout << "D - Display Library Entries" << endl;    
    cout << "I - List Invalid Library Entries" << endl;
    cout << "L - Load Library From File" << endl;    
    cout << "O - Output Library To File" << endl;
    cout << "P - Print Out Checkout Stats" << endl;
    cout << "R - Remove A Library Entry" << endl;      
    cout << "S - Search For A Library Entry" << endl;    
    cout << "X - Exit Program" << endl;
}

//  add book and isbn to library if name is valid and there is no checkout status
//  PARAMETERS: string vector of titles in library, string vector of isbns, 
//              integer vector of checkout statuses, string input of book 
//              title from user, string input isbn from user
void addEntry(vector<string> &titles, vector<string> &isbNums, vector<int> &coStatus, string book, string number) {
    titles.push_back(book);
    isbNums.push_back(number);
    coStatus.push_back(0);
}

//  add book, isbn, and checkout status to library if name is valid
//  PARAMETERS: string vector of titles in library, string vector of isbns, 
//              integer vector of checkout statuses, string input of book title 
//              from user, string input isbn from user, integer to indicate 
//              checkout status
void addEntry(vector<string> &titles, vector<string> &isbNums, vector<int> &coStatus, string book, string number, int status) {
    titles.push_back(book);
    isbNums.push_back(number);
    coStatus.push_back(status);
}

//  calculates the value of the isbn meant to compare to the last digit
//  PARAMETERS: string that holds the full isbn
//  RETURNS: integer of the number to compare to the last digit of the isbn
int calcIsbn(string isbn) {
    int compare = 0, numCount = 0;

    for (int i = 0; i < isbn.length() - 1; i++) {
        if (isdigit(isbn.at(i))) {
            if (numCount % 2 == 0) {    // determines if num is multipied by 3
                compare += int(isbn.at(i) - '0');
            }
            else {
                compare += 3 * int(isbn.at(i) - '0');
            }
            numCount++;
        }
    }

    compare = 10 - (compare % 10);

    return compare;
}

//  checks the validity of title and isbn inputs from user
//  PARAMETERS: string of book name
//  RETURNS: true if there are no commas in the book title, false otherwise
bool checkInput(string name) {
    if (name.find(",") != string::npos) {   // checks title validity
        cout << "The book title cannot contain commas." << endl;
        return false;
    }

    return true;
}

//  displays the current contents in library
//  PARAMETERS: string vector of titles, string vector of isbns, 
//              integer vector of checkout status
void displayLibrary(vector<string> &titles, vector<string> &isbNums, vector<int> &coStatus) {
    for (int i = 0; i < titles.size(); i++) {
        cout << titles.at(i) << " --- "
             << isbNums.at(i) << " --- ";

        if (coStatus.at(i) == 0) {
            cout << "In Library";
        }
        else if (coStatus.at(i) == 1) {
            cout << "Checked Out";
        }
        else if (coStatus.at(i) == 2) {
            cout << "On Loan";
        }
        else if (coStatus.at(i) == 3) {
            cout << "Unknown State";
        }
        else {
            cout << "Invalid State";
        }
        cout << endl; 
    }
}

//  displays one library entry
//  PARAMETERS: string of book title, string of isbn, int value of book status
void displayLibraryEntry(string book, string isbn, int status) {
    cout << book << " --- "
         << isbn << " --- ";

    if (status == 0) {
        cout << "In Library";
    }
    else if (status == 1) {
        cout << "Checked Out";
    }
    else if (status == 2) {
        cout << "On Loan";
    }
    else if (status == 3) {
        cout << "Unknown State";
    }
    else {
        cout << "Invalid State";
    }
    cout << endl; 
}

//  reads the line from a file and splits the book, isbn, and checkout status
//  PARAMETERS: string of one file line, string passed by reference to store book title, 
//              string passed by reference to store isbn, 
//              integer passed by reference to store checkout status
void getFileData(string line, string &book, string &isbn, int &ckout) {
    int i = line.find(",");
    vector<int> index;

    while (i != string::npos) {
        index.push_back(i);
        i = line.find(",", i + 1);
    }


    if (index.size() == 2) {
        book = line.substr(0, index.at(0));
        isbn = line.substr(index.at(0) + 2, index.at(1) - (index.at(0) + 2));
        ckout = stoi(line.substr(index.at(1) + 2));
    }
    else if (index.size() == 1) {
        book = line.substr(0, index.at(0));
        isbn = line.substr(index.at(0) + 2);
        ckout = 0;
    }
    else {
        book = line;
        isbn = "000-0-00-000000-0";
        ckout = 0;
    }
}

//  prints out the books with invalid isbns and/or checkout statuses
//  PARAMETERS: string vector of book titles, string vector of isbns, 
//              integer vector of checkout statuses
void listInvalid(vector<string> titles, vector<string> isbNums, vector<int> coStatus) {
    int compare;
    bool invalid, overall = false;

    cout << "The Following Library Entries Have Invalid Data" << endl
         << "-----------------------------------------------" << endl;


    for (int i = 0; i < titles.size(); i++) {
        invalid = false;
        if (coStatus.at(i) < 0 || coStatus.at(i) > 3) {
            invalid = true;
            overall = true;
        }

        if (isbNums.at(i).length() != 17) {    // checks for 17 characters -- 13 numbers + 4 hyphens
            invalid = true;
            overall = true;
        }

        compare = calcIsbn(isbNums.at(i));
        if (compare == 10 && int(isbNums.at(i).at(16) - '0') != 0) {
            invalid = true;
            overall = true;
        }
        else if (compare != int(isbNums.at(i).at(16) - '0')) {
            invalid = true;
            overall = true;
        }

        if (invalid == true) {
            displayLibraryEntry(titles.at(i), isbNums.at(i), coStatus.at(i));
        }
    }

    if (titles.size() == 0) {
        cout << "The library has no books." << endl;
    }
    else if (!overall) {
        cout << "The library has no invalid entries." << endl;
    }
}

//  creates a new file with the data of the current library
//  PARAMETERS: string of the filename provided by user, 
//              string vector of current book titles in library, 
//              string vector of isbns in library, 
//              integer vector of book checkout statuses
void outputLibraryFile(string filename, vector<string> titles, vector<string> isbNums, vector<int> coStatus) {
    ofstream newFile;

    newFile.open(filename);
    for (int i = 0; i < titles.size(); i++) {
        newFile << titles.at(i) << ", "
                <<isbNums.at(i) << ", "
                << coStatus.at(i) << endl;
    }

    newFile.close();
}

//  outputs the checkout stats of the books in the library
//  PARAMETERS: integer vector of the checkout statuses of all books in library
void printCheckout(vector<int> coStatus) {
    int inLibrary = 0, checkedOut = 0, onLoan = 0, unknown = 0, other = 0;

    for (int i = 0; i < coStatus.size(); i++) {
        if (coStatus.at(i) == 0) {
            inLibrary++;
        }
        else if (coStatus.at(i) == 1) {
            checkedOut++;
        }
        else if (coStatus.at(i) == 2) {
            onLoan++;
        }
        else if (coStatus.at(i) == 3) {
            unknown++;
        }
        else {
            other++;
        }
    }
    
    cout << "Your Current Library's Stats" << endl
         << "----------------------------" << endl
         << "Total Books: " << coStatus.size() << endl
         << "   In Library: " << inLibrary << endl
         << "   Checked Out: " << checkedOut << endl
         << "   On Loan: " << onLoan << endl
         << "   Unknown: " << unknown << endl
         << "   Other: " << other << endl;
}

//  removes book data at index from library
//  PARAMETERS: integer of where book is stored in library, string vector of books 
//              to delete from, string vector of isbns to delete from, 
//              int vector of book statuses to delete from
void removeEntry(int index, vector<string> &titles, vector<string> &isbNums, vector<int> &coStatus) {
    cout << "The Following Entry Was Removed From The Library" << endl
         << "------------------------------------------------" << endl;
    if (index != string::npos) {
        displayLibraryEntry(titles.at(index), isbNums.at(index), coStatus.at(index));
        titles.erase(titles.begin() + index);
        isbNums.erase(isbNums.begin() + index);
        coStatus.erase(coStatus.begin() + index);
    }
    else {
        cout << "No matching entry found in the library." << endl;
    }
}

//  searches for the index of a matching title in library
//  PARAMETERS: string vector of books in library
//  RETURNS: integer of where the match is or -1 if match is not found
int searchName(vector<string> titles) {
    string search;
    int pos = string::npos;

    cout << "Enter the book name: " << endl;
    getline(cin, search);

    for (int i = 0; i < titles.size(); i++) {
        if (search == titles.at(i)) {
            pos =  i;
        }
    }

    return pos;
}

//  searches for the index of a matching isbn in library
//  PARAMETERS: string vector of isbns
//  RETURNS: integer of where the match is or -1 if match is not found
int searchIsbn(vector<string> isbNums) {
    string search;
    int pos = string::npos;

    cout << "Enter the book 13-digit ISBN (with dashes): " << endl;
    getline(cin, search);

    for (int i = 0; i < isbNums.size(); i++) {
        if (search == isbNums.at(i)) {
            pos = i;
        }
    }

    return pos;
}

//  prints out the book at index found from search
//  PARAMETERS: integer of position of matching book, string vector of book titles,
//              string vector of isbns, intger vector of checkout statuses
void searchResults(int index, vector<string> titles, vector<string> isbNums, vector<int> coStatus) {
    cout << "The Following Are Your Search Results" << endl
         << "-------------------------------------" << endl;
    if (index != string::npos) {
        displayLibraryEntry(titles.at(index), isbNums.at(index), coStatus.at(index));
    }
    else {
        cout << "No matching entry found in the library." << endl; 
    }
}