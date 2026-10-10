#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct BookRequestNode {
    int studentID;
    string studentName;
    int bookID;
    string bookTitle;
    int priority; // 1 = exam, 2 = assignment/project, 3 = normal reading
    BookRequestNode* next;
};

class PriorityQueue {
private:
    BookRequestNode* front;
    BookRequestNode* rear;
    int count;

public:
    PriorityQueue() {
        front = NULL;
        rear = NULL;
        count = 0;
    }

    ~PriorityQueue() {
        while (front != NULL) {
            BookRequestNode* temp = front;
            front = front->next;
            delete temp;
        }
        rear = NULL;
        count = 0;
    }

    void Add(BookRequestNode* request) {
        request->next = NULL;

        if (front == NULL) {
            front = request;
            rear = request;
        } else if (request->priority < front->priority) {
            request->next = front;
            front = request;
        } else {
            BookRequestNode* current = front;
            // <= keeps earlier requests first when priorities are equal.
            while (current->next != NULL &&
                   current->next->priority <= request->priority) {
                current = current->next;
            }

            request->next = current->next;
            current->next = request;

            if (request->next == NULL) {
                rear = request;
            }
        }
        count++;
    }

    BookRequestNode* Remove() {
        if (front == NULL) {
            return NULL;
        }

        BookRequestNode* request = front;
        front = front->next;

        if (front == NULL) {
            rear = NULL;
        }

        request->next = NULL;
        count--;
        return request;
    }

    bool IsEmpty() {
        return front == NULL;
    }

    bool IsNotEmpty() {
        return front != NULL;
    }

    void PrintQueue() {
        if (IsEmpty()) {
            cout << "No pending book requests.\n";
            return;
        }

        BookRequestNode* current = front;
        cout << "\n--- Pending Requests (highest priority first) ---\n";
        while (current != NULL) {
            cout << "Student ID: " << current->studentID << "\n";
            cout << "Student Name: " << current->studentName << "\n";
            cout << "Book ID: " << current->bookID << "\n";
            cout << "Book Title: " << current->bookTitle << "\n";
            cout << "Priority: " << current->priority;
            if (current->priority == 1) cout << " (Exam / Urgent)";
            else if (current->priority == 2) cout << " (Assignment / Project)";
            else cout << " (Normal Reading)";
            cout << "\n-----------------------------\n";
            current = current->next;
        }
        cout << "Total pending requests: " << count << "\n";
    }
};

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInput();
            return value;
        }
        cout << "Invalid input. Please enter a number.\n";
        clearInput();
    }
}

BookRequestNode* CreateRequest() {
    BookRequestNode* request = new BookRequestNode;

    request->studentID = readInt("Enter Student ID: ");

    cout << "Enter Student Name: ";
    getline(cin, request->studentName);

    request->bookID = readInt("Enter Book ID: ");

    cout << "Enter Book Title: ";
    getline(cin, request->bookTitle);

    while (true) {
        request->priority = readInt(
            "Enter Priority (1=Exam/Urgent, 2=Assignment/Project, 3=Normal Reading): ");
        if (request->priority >= 1 && request->priority <= 3) break;
        cout << "Priority must be 1, 2, or 3. Try again.\n";
    }

    request->next = NULL;
    return request;
}

int main() {
    PriorityQueue requests;
    int choice;

    do {
        cout << "\n===== Library Book Request System =====\n";
        cout << "1. Add Book Request\n";
        cout << "2. Process Highest Priority Request\n";
        cout << "3. Display All Requests\n";
        cout << "4. Check Queue Status\n";
        cout << "5. Exit\n";

        choice = readInt("Enter your choice: ");

        switch (choice) {
        case 1: {
            BookRequestNode* request = CreateRequest();
            requests.Add(request);
            cout << "Book request added successfully.\n";
            break;
        }
        case 2: {
            BookRequestNode* processed = requests.Remove();
            if (processed == NULL) {
                cout << "Queue is empty. There is no request to process.\n";
            } else {
                cout << "\n--- Request Processed ---\n";
                cout << "Student: " << processed->studentName << "\n";
                cout << "Book: " << processed->bookTitle << "\n";
                cout << "Priority: " << processed->priority << "\n";
                delete processed;
            }
            break;
        }
        case 3:
            requests.PrintQueue();
            break;
        case 4:
            if (requests.IsEmpty()) cout << "Queue status: EMPTY\n";
            else cout << "Queue status: NOT EMPTY\n";
            break;
        case 5:
            cout << "Exiting Library Book Request System.\n";
            break;
        default:
            cout << "Invalid choice. Please select 1 to 5.\n";
        }
    } while (choice != 5);

    return 0;
}
