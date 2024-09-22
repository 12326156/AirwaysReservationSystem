#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// Struct to hold flight information
struct Flight {
    int id;
    string flight_number;
    string departure;
    string arrival;
    int seats_available;
};

// Struct to hold customer information
struct Customer {
    string name;
    string email;
    string phone;
    int flight_id;  // Stores flight ID that the customer booked
};

// Function to load flights from the file
vector<Flight> loadFlights() {
    vector<Flight> flights;
    ifstream file("flights.txt");

    if (!file) {
        cout << "Error opening flight data file." << endl;
        return flights;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Flight flight;
        string temp;

        getline(ss, temp, ';');
        flight.id = stoi(temp);

        getline(ss, flight.flight_number, ';');
        getline(ss, flight.departure, ';');
        getline(ss, flight.arrival, ';');
        
        getline(ss, temp, ';');
        flight.seats_available = stoi(temp);

        flights.push_back(flight);
    }
    file.close();
    return flights;
}

// Function to save updated flights to the file
void saveFlights(const vector<Flight>& flights) {
    ofstream file("flights.txt");
    if (file.is_open()) {
        for (const auto& flight : flights) {
            file << flight.id << ";" << flight.flight_number << ";" 
                 << flight.departure << ";" << flight.arrival << ";" 
                 << flight.seats_available << endl;
        }
        file.close();
    } else {
        cout << "Error saving flight data." << endl;
    }
}

// Function to display all flights
void displayFlights(const vector<Flight>& flights) {
    cout << "\nAvailable Flights:" << endl;
    cout << "ID\tFlight Number\tDeparture\tArrival\tSeats Available" << endl;
    for (const auto& flight : flights) {
        cout << flight.id << "\t" << flight.flight_number << "\t\t" 
             << flight.departure << "\t" << flight.arrival << "\t" 
             << flight.seats_available << endl;
    }
}

// Function to book a flight
void bookFlight(vector<Flight>& flights, vector<Customer>& customers) {
    int flight_id;
    cout << "\nEnter the flight ID you want to book: ";
    cin >> flight_id;

    // Find flight by ID
    bool flight_found = false;
    for (auto& flight : flights) {
        if (flight.id == flight_id) {
            flight_found = true;
            if (flight.seats_available > 0) {
                Customer customer;
                cout << "Enter your name: ";
                cin.ignore();  // Ignore newline from the previous input
                getline(cin, customer.name);
                cout << "Enter your email: ";
                getline(cin, customer.email);
                cout << "Enter your phone: ";
                getline(cin, customer.phone);

                // Decrease available seats and assign flight ID to the customer
                flight.seats_available--;
                customer.flight_id = flight_id;

                customers.push_back(customer);

                cout << "\nBooking successful! Your flight details:" << endl;
                cout << "Flight Number: " << flight.flight_number << endl;
                cout << "Departure: " << flight.departure << endl;
                cout << "Arrival: " << flight.arrival << endl;
                cout << "Seats Remaining: " << flight.seats_available << endl;

                saveFlights(flights);
            } else {
                cout << "Sorry, no seats available on this flight." << endl;
            }
            break;
        }
    }

    if (!flight_found) {
        cout << "Flight ID not found." << endl;
    }
}

// Function to cancel a booking
void cancelBooking(vector<Flight>& flights, vector<Customer>& customers) {
    string email;
    cout << "\nEnter your email to cancel booking: ";
    cin >> email;

    bool customer_found = false;
    for (size_t i = 0; i < customers.size(); ++i) {
        if (customers[i].email == email) {
            customer_found = true;
            int flight_id = customers[i].flight_id;

            // Find the flight and increase the available seats
            for (auto& flight : flights) {
                if (flight.id == flight_id) {
                    flight.seats_available++;
                    cout << "\nBooking cancelled for Flight Number: " 
                         << flight.flight_number << endl;
                    saveFlights(flights);
                    break;
                }
            }

            // Remove the customer from the list
            customers.erase(customers.begin() + i);
            break;
        }
    }

    if (!customer_found) {
        cout << "No booking found for this email." << endl;
    }
}

// Function to search flights by departure and arrival
void searchFlights(const vector<Flight>& flights) {
    string departure, arrival;
    cout << "\nEnter departure city: ";
    cin >> departure;
    cout << "Enter arrival city: ";
    cin >> arrival;

    cout << "\nFlights from " << departure << " to " << arrival << ":" << endl;
    bool flight_found = false;
    for (const auto& flight : flights) {
        if (flight.departure == departure && flight.arrival == arrival) {
            flight_found = true;
            cout << "ID: " << flight.id << ", Flight Number: " 
                 << flight.flight_number << ", Seats Available: " 
                 << flight.seats_available << endl;
        }
    }

    if (!flight_found) {
        cout << "No flights found for this route." << endl;
    }
}

// Function to display customer details
void displayCustomers(const vector<Customer>& customers) {
    cout << "\nCustomer Details:" << endl;
    for (const auto& customer : customers) {
        cout << "Name: " << customer.name << ", Email: " 
             << customer.email << ", Phone: " << customer.phone 
             << ", Flight ID: " << customer.flight_id << endl;
    }
}

// Function to display the menu and interact with the system
void displayMenu() {
    vector<Flight> flights = loadFlights();
    vector<Customer> customers;
    int choice;
    
    do {
        cout << "\n--- Airways Reservation System ---" << endl;
        cout << "1. View Available Flights" << endl;
        cout << "2. Book a Flight" << endl;
        cout << "3. Cancel a Booking" << endl;
        cout << "4. Search Flights" << endl;
        cout << "5. View Customer Details" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayFlights(flights);
                break;
            case 2:
                bookFlight(flights, customers);
                break;
            case 3:
                cancelBooking(flights, customers);
                break;
            case 4:
                searchFlights(flights);
                break;
            case 5:
                displayCustomers(customers);
                break;
            case 6:
                cout << "Thank you for using the Airways Reservation System!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 6);

    cout << "Press Enter to exit...";
    cin.ignore();  // Ignore newline from the previous input
    cin.get();     // Wait for Enter key before closing
}

int main() {
    displayMenu();
    return 0;
}
