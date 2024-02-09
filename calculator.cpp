#include <iostream>

using namespace std;

void convertSecondsToHours() {
    int seconds;
    cout << "Enter the time in seconds: ";
    cin >> seconds;
    if (seconds < 0) {
        cout << "Invalid input. Time must be a non-negative integer.\n";
        return;
    }
    int hours = seconds / 3600;
    cout << seconds << " seconds is equal to " << hours << " hours.\n";
}

void convertSecondsToMinutes() {
    int seconds;
    cout << "Enter the time in seconds: ";
    cin >> seconds;
    if (seconds < 0) {
        cout << "Invalid input. Time must be a non-negative integer.\n";
        return;
    }
    int minutes = seconds / 60;
    cout << seconds << " seconds is equal to " << minutes << " minutes.\n";
}

void convertMinutesToHours() {
    int minutes;
    cout << "Enter the time in minutes: ";
    cin >> minutes;
    if (minutes < 0) {
        cout << "Invalid input. Time must be a non-negative integer.\n";
        return;
    }
    int hours = minutes / 60;
    cout << minutes << " minutes is equal to " << hours << " hours.\n";
}

void convertHoursToSeconds() {
    int hours;
    cout << "Enter the time in hours: ";
    cin >> hours;
    if (hours < 0) {
        cout << "Invalid input. Time must be a non-negative integer.\n";
        return;
    }
    int seconds = hours * 3600;
    cout << hours << " hours is equal to " << seconds << " seconds.\n";
}

int main() {
    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Convert seconds to hours\n";
        cout << "2. Convert seconds to minutes\n";
        cout << "3. Convert minutes to hours\n";
        cout << "4. Convert hours to seconds\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                convertSecondsToHours();
                break;
            case 2:
                convertSecondsToMinutes();
                break;
            case 3:
                convertMinutesToHours();
                break;
            case 4:
                convertHoursToSeconds();
                break;
            case 5:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number from 1 to 5.\n";
                break;
        }
    } while (choice != 5);

    return 0;
}
