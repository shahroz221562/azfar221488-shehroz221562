#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
#include <string>
#include <cctype>
#include <iomanip>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// Data Structures
struct LibraryBook {
    string title;
    string author;
    long long isbn;
    int quantity;
};

class BookManager {
private:
    list<LibraryBook> libraryBooks;
    stack<LibraryBook> removedBooks;  // Using stack to keep track of removed books
    queue<LibraryBook> addedBooks;    // Using queue to keep track of added books
    ofstream outputFile;

public:
    // Constructor
    BookManager() {
        createNewFile();  // Open a new file for each run
    }

    // Destructor to close the file
    ~BookManager() {
        closeFile();
    }

    // Function to create a new file for each run
    void createNewFile() {
        outputFile.close();  // Close the existing file
        string fileName = "book_input_" + to_string(time(0)) + ".txt";  // Create a unique file name using the timestamp
        outputFile.open(fileName, ios::app);
        cout << "Output will be added to file: " << fileName << endl;
    }

    // Function to close the file
    void closeFile() {
        outputFile.close();
    }

    // Function to add a book to the system
    void addBook() {
        LibraryBook newLibraryBook;
        cout << "Enter Book Title for Adding: " << endl;
        cin.ignore();
        getline(cin, newLibraryBook.title);

        // Validate Title
        while (true) {
            bool validTitle = all_of(newLibraryBook.title.begin(), newLibraryBook.title.end(), [](char c) {
                return (isalpha(c) || isspace(c) || c == ',');
            });

            if (validTitle) {
                // Valid title
                break;
            } else {
                cout << "Invalid input, Please try again. Enter the Valid Title (Alphabets only)" << endl;
                cout << "Enter Book Title for Adding: " << endl;
                getline(cin, newLibraryBook.title);
            }
        }

        // Validate Author Name
        while (true) {
            cout << "Enter Author for adding: " << endl;
            getline(cin, newLibraryBook.author);

            if (all_of(newLibraryBook.author.begin(), newLibraryBook.author.end(), [](char c) {
                return (isalpha(c) || isspace(c));
            })) {
                // Valid author name
                break;
            } else {
                cout << "Invalid input, Please try again.Enter the Valid Author Name" << endl;
            }
        }

        while (true) {
            cout << "Enter ISBN for Adding: " << endl;
            cin >> newLibraryBook.isbn;

            string isbnStr = to_string(newLibraryBook.isbn);

            if (cin.fail() || (isbnStr.length() != 10 && isbnStr.length() != 13)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please Enter a Valid ISBN." << endl;
            } else {
                break;
            }
        }

        cout << "Enter quantity of the book: " << endl;
        while (true) {
            string quantityInput;
            cin >> quantityInput;

            if (all_of(quantityInput.begin(), quantityInput.end(), ::isdigit)) {
                newLibraryBook.quantity = stoi(quantityInput);
                if (newLibraryBook.quantity >= 0) {
                    break;
                }
            }

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid quantity (non-negative digits only)." << endl;
            cout << "Enter quantity of the book: " << endl;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Check if the book with the same title, author, and ISBN already exists
        auto it = find_if(libraryBooks.begin(), libraryBooks.end(), [newLibraryBook](const LibraryBook& book) {
            return (book.title == newLibraryBook.title && book.author == newLibraryBook.author && book.isbn == newLibraryBook.isbn);
        });

        if (it != libraryBooks.end()) {
            // Book already exists, update the quantity
            it->quantity += newLibraryBook.quantity;
            cout << "Quantity of existing book updated successfully." << endl;
        } else {
            // Book doesn't exist, add a new entry
            libraryBooks.push_back(newLibraryBook);
            addedBooks.push(newLibraryBook);  // Add to the queue of added books
            cout << "Book Added Successfully." << endl;

            // Write the input to the file
            outputFile << "Title: " << newLibraryBook.title << ", Author: " << newLibraryBook.author
                       << ", ISBN: " << newLibraryBook.isbn << ", Quantity: " << newLibraryBook.quantity << endl;
        }

        cout << endl;
    }

    // Function to display information about all books
    void displayBooks() {
        cout << "Book Store:" << endl;
        for (const auto& book : libraryBooks) {
            cout << "Title: " << setw(20) << book.title << " Author: " << setw(20) << book.author
                 << " ISBN: " << setw(15) << book.isbn << " Quantity: " << book.quantity << endl << endl;
        }
    }

    // Function to search for a book
    void searchBook() {
        long long bookIsbn;
        while (true) {
            cout << "Enter ISBN of the book to search (10 or 13 digits): ";
            cin >> bookIsbn;

            string isbnStr = to_string(bookIsbn);

            if (cin.fail() || (isbnStr.length() != 10 && isbnStr.length() != 13)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a valid ISBN." << endl;
            } else {
                break;
            }
        }

        auto it = find_if(libraryBooks.begin(), libraryBooks.end(), [bookIsbn](const LibraryBook& book) {
            return book.isbn == bookIsbn;
        });

        if (it != libraryBooks.end()) {
            const LibraryBook& book = *it;
            cout << "Book found:" << endl;
            cout << "Title: " << book.title << endl << " Author: " << book.author << endl
                 << " ISBN: " << book.isbn << endl << " Quantity: " << book.quantity << endl;
        } else {
            cout << "Book not found." << endl;
        }
    }

    // Function to remove a book by name and ISBN
    void removeBook() {
        string bookName;
        long long bookIsbn;

        cout << "Enter the name of the book to remove: ";
        cin.ignore();
        getline(cin, bookName);

        // Convert bookName to lowercase for case-insensitive comparison
        transform(bookName.begin(), bookName.end(), bookName.begin(), ::tolower);

        while (true) {
            cout << "Enter ISBN of the book to remove: ";
            cin >> bookIsbn;

            if (cin.fail() || (to_string(bookIsbn).length() != 10 && to_string(bookIsbn).length() != 13)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a valid ISBN." << endl;
            } else {
                break;
            }
        }

        auto it = find_if(libraryBooks.begin(), libraryBooks.end(), [bookName, bookIsbn](const LibraryBook& book) {
            string lowerCaseTitle = book.title;
            transform(lowerCaseTitle.begin(), lowerCaseTitle.end(), lowerCaseTitle.begin(), ::tolower);
            return (lowerCaseTitle.find(bookName) != string::npos) && (book.isbn == bookIsbn);
        });

        if (it != libraryBooks.end()) {
            int quantityToRemove;
            cout << "Enter the quantity of books to remove: ";
            while (true) {
                string quantityInput;
                cin >> quantityInput;

                if (all_of(quantityInput.begin(), quantityInput.end(), ::isdigit)) {
                    quantityToRemove = stoi(quantityInput);
                    if (quantityToRemove > 0 && quantityToRemove <= it->quantity) {
                        break;
                    }
                }

                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a valid quantity (positive digits, less than or equal to available quantity)." << endl;
                cout << "Enter the quantity of books to remove: ";
            }

            it->quantity -= quantityToRemove;

            if (it->quantity == 0) {
                removedBooks.push(*it);  // Add to the stack of removed books
                libraryBooks.erase(it);
                cout << "Book removed successfully." << endl;
            } else {
                cout << "Quantity of book updated successfully." << endl;
            }
        } else {
            cout << "Book not found and Unable to remove." << endl;
        }
    }
};

int main() {
    BookManager bookManager;

    int userChoice;
    do {
        cout << " [+] --------------------------------------------------------------------------------------- [+]  " << endl;
        cout << endl;
        cout << "                *******  Welcome to the Online Book Management  *********                          " << endl;
        cout << endl;
        cout << "______________________________ Our Menu For Online Book Managment _________________________________" << endl;
        cout << endl;
        cout << "[+]======> Enter 1 For Adding the Book:" << endl;
        cout << "[+]======> Enter 2 For Display the Book:" << endl;
        cout << "[+]======> Enter 3 For Search the Book:" << endl;
        cout << "[+]======> Enter 4 For Remove the Book:" << endl;
        cout << "[+]======> Enter 5 For Exit the Book:" << endl;
        cout << endl;
        cout << "Enter your choice: ";
        cin >> userChoice;

        switch (userChoice) {
            case 1:
                bookManager.addBook();
                break;
            case 2:
                bookManager.displayBooks();
                break;
            case 3:
                bookManager.searchBook();
                break;
            case 4:
                bookManager.removeBook();
                break;
            case 5:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again and Enter the Valid Choice" << endl;
        }

    } while (userChoice != 5);

    return 0;
}
