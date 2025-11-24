#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cctype>
#include <limits>   // ← add this line

using namespace std;

const int MAX_REVIEWS = 100;

// Arrays to store data
string trends[MAX_REVIEWS];
int ratings[MAX_REVIEWS];
string reviewers[MAX_REVIEWS];
int reviewCount = 0;

// Helper: convert a string to lowercase (for case-insensitive search)
string toLower(const string &s) {
    string result = s;
    for (char &c : result) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

// Load reviews from file into arrays
void loadReviews(const string &filename) {
    ifstream inFile(filename);

    if (!inFile) {
        // File does not exist yet – that's okay, just start empty
        return;
    }

    string line;
    while (getline(inFile, line) && reviewCount < MAX_REVIEWS) {
        if (line.empty()) continue;

        stringstream ss(line);
        string word;
        string parts[20];
        int partCount = 0;

        // Split line into words
        while (ss >> word && partCount < 20) {
            parts[partCount++] = word;
        }

        if (partCount < 3) {
            // Not enough parts for trend, rating, reviewer, skip line
            continue;
        }

        // Last word = reviewer, second last = rating, rest = trend name
        reviewers[reviewCount] = parts[partCount - 1];

        int rating = stoi(parts[partCount - 2]);
        ratings[reviewCount] = rating;

        string trendName;
        for (int i = 0; i < partCount - 2; ++i) {
            trendName += parts[i];
            if (i != partCount - 3) {
                trendName += " ";
            }
        }
        trends[reviewCount] = trendName;

        reviewCount++;
    }

    inFile.close();
}

// Append a single new review to the file
void appendReviewToFile(const string &filename,
                        const string &trend,
                        int rating,
                        const string &reviewer) {
    ofstream outFile(filename, ios::app); // append mode
    if (!outFile) {
        cout << "Error opening file for writing.\n";
        return;
    }
    outFile << trend << " " << rating << " " << reviewer << "\n";
    outFile.close();
}

// Display all reviews in a formatted table
void viewAllReviews() {
    if (reviewCount == 0) {
        cout << "No reviews found.\n";
        return;
    }

    cout << left << setw(25) << "TikTok Trend"
         << setw(10) << "Rating"
         << setw(15) << "Reviewer" << "\n";
    cout << string(50, '-') << "\n";

    for (int i = 0; i < reviewCount; ++i) {
        cout << left << setw(25) << trends[i]
             << setw(10) << ratings[i]
             << setw(15) << reviewers[i] << "\n";
    }
}

// Search for a trend/influencer by name (case-insensitive)
void searchReview() {
    if (reviewCount == 0) {
        cout << "No reviews to search.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover newline
    string searchName;
    cout << "Enter trend/influencer name: ";
    getline(cin, searchName);

    string searchLower = toLower(searchName);
    bool found = false;

    for (int i = 0; i < reviewCount; ++i) {
        if (toLower(trends[i]) == searchLower) {
            cout << "Trend: " << trends[i]
                 << ", Rating: " << ratings[i]
                 << ", Reviewed by: " << reviewers[i] << "\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No review found for \"" << searchName << "\".\n";
    }
}

// Add a new review (update array and file)
void addNewReview(const string &filename) {
    if (reviewCount >= MAX_REVIEWS) {
        cout << "Maximum number of reviews reached.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover newline

    string trend;
    int rating;
    string reviewer;

    cout << "Enter trend/influencer name: ";
    getline(cin, trend);

    cout << "Enter rating (1-10): ";
    cin >> rating;

    while (rating < 1 || rating > 10) {
        cout << "Invalid rating. Enter a number between 1 and 10: ";
        cin >> rating;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear newline

    cout << "Enter reviewer name: ";
    getline(cin, reviewer);

    // Store in arrays
    trends[reviewCount] = trend;
    ratings[reviewCount] = rating;
    reviewers[reviewCount] = reviewer;
    reviewCount++;

    // Append to file
    appendReviewToFile(filename, trend, rating, reviewer);

    cout << "Review added successfully!\n";
}

int main() {
    const string filename = "tiktok_reviews.txt";

    // Step 1: Load existing reviews from file
    loadReviews(filename);

    int choice = 0;
    do {
        cout << "\nWelcome to the TikTok Review Database\n";
        cout << "1. View All Reviews\n";
        cout << "2. Search for a Trend/Influencer\n";
        cout << "3. Add a New Review\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewAllReviews();
                break;
            case 2:
                searchReview();
                break;
            case 3:
                addNewReview(filename);
                break;
            case 4:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 4);

    return 0;
}