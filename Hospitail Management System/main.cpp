#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>
#include <vector>

using namespace std;

// Forward declarations
class Patient;
class Hospital;
class DiagnosisInfo;

// Patient class to store patient information
class Patient {
private:
    string name;
    string address;
    string contact;
    string age;
    string sex;
    string blood_gp;
    string disease_past;
    string id;

public:
    Patient() {}
    
    void inputDetails() {
        cout << "\nName: ";
        getline(cin, name);
        
        cout << "\nAddress: ";
        getline(cin, address);
        
        cout << "\nContact Number: ";
        getline(cin, contact);
        
        cout << "\nAge: ";
        getline(cin, age);
        
        cout << "\nSex: ";
        getline(cin, sex);
        
        cout << "\nBlood Group: ";
        getline(cin, blood_gp);
        
        cout << "\nAny Major disease suffered earlier: ";
        getline(cin, disease_past);
        
        cout << "\nPatient ID: ";
        getline(cin, id);
    }
    
    void saveToFile(ofstream& file) const {
        file << "Name: " << name << "\n";
        file << "Address: " << address << "\n";
        file << "Contact Number: " << contact << "\n";
        file << "Age: " << age << "\n";
        file << "Sex: " << sex << "\n";
        file << "Blood Group: " << blood_gp << "\n";
        file << "Any Major disease suffered earlier: " << disease_past << "\n";
        file << "Patient ID: " << id << "\n";
    }
};

// DiagnosisInfo class to store diagnosis information
class DiagnosisInfo {
private:
    string symptom;
    string diagnosis;
    string medicine;
    string admission;
    string ward;
    string timestamp;

public:
    DiagnosisInfo() {
        // Get current time
        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        timestamp = asctime(timeinfo);
    }
    
    void inputDetails() {
        cout << "\nSymptoms: ";
        getline(cin, symptom);
        
        cout << "\nDiagnosis: ";
        getline(cin, diagnosis);
        
        cout << "\nMedicines: ";
        getline(cin, medicine);
        
        cout << "\nAdmission Required? (Yes/No): ";
        getline(cin, admission);
        
        cout << "\nType of ward: ";
        getline(cin, ward);
    }
    
    void saveToFile(ofstream& file) const {
        file << "Description of " << timestamp << "\n";
        file << "Symptoms: " << symptom << "\n";
        file << "Diagnosis: " << diagnosis << "\n";
        file << "Medicines: " << medicine << "\n";
        file << "Admission Required?: " << admission << "\n";
        file << "Type of ward: " << ward << "\n";
    }
    
    string getWard() const {
        return ward;
    }
    
    string getTimestamp() const {
        return timestamp;
    }
};

// Hospital class to manage the hospital system
class Hospital {
private:
    const string password = "pass";
    const int maxLoginAttempts = 3;
    
    void clearScreen() const {
        cout << "\033[2J\033[1;1H"; // Clear screen (works on most terminals)
    }
    
public:
    Hospital() {}
    
    void displayWelcome() const {
        clearScreen();
        cout << "\n\n";
        cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
        cout << "@@ ________________________________________________________________________ @@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|                           WELCOME TO                                   |@@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|                     HOSPITAL MANAGEMENT SYSTEM                         |@@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|________________________________________________________________________|@@\n";
        cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        clearScreen();
    }
    
    bool login() const {
        int attempts = 0;
        string inputPassword;
        
        while (attempts < maxLoginAttempts) {
            cout << "\n\n";
            cout << "                    HOSPITAL MANAGEMENT SYSTEM\n\n";
            cout << "                    ------------------------------\n";
            cout << "                             LOGIN\n";
            cout << "                    ------------------------------\n\n";
            cout << "                    Enter Password: ";
            
            // Read password (without showing characters)
            char ch;
            inputPassword = "";
            while ((ch = cin.get()) != '\n') {
                if (ch == 127 || ch == 8) { // Backspace
                    if (!inputPassword.empty()) {
                        inputPassword.pop_back();
                        cout << "\b \b";
                    }
                } else {
                    inputPassword.push_back(ch);
                    cout << '*';
                }
            }
            
            if (inputPassword == password) {
                cout << "\n\nAccess Granted!\n";
                cout << "\nPress Enter to continue...";
                cin.get();
                clearScreen();
                return true;
            } else {
                attempts++;
                cout << "\n\nAccess Denied...\n";
                cout << "Attempt " << attempts << " of " << maxLoginAttempts << "\n";
                cout << "\nPress Enter to try again...";
                cin.get();
                clearScreen();
            }
        }
        
        return false;
    }
    
    void displayMenu() const {
        clearScreen();
        cout << "\n\n";
        cout << "                    HOSPITAL MANAGEMENT SYSTEM\n\n";
        cout << "Please, Choose from the following Options:\n\n";
        cout << " _________________________________________________________________\n";
        cout << "|                                                                 |\n";
        cout << "|             1  >> Add New Patient Record                        |\n";
        cout << "|             2  >> Add Diagnosis Information                     |\n";
        cout << "|             3  >> Full History of the Patient                   |\n";
        cout << "|             4  >> Information About the Hospital                |\n";
        cout << "|             5  >> Exit the Program                              |\n";
        cout << "|_________________________________________________________________|\n\n";
    }
    
    void displayThankYou() const {
        clearScreen();
        cout << "\n\n";
        cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
        cout << "@@ ________________________________________________________________________ @@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|                       THANK YOU FOR USING                              |@@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|                    HOSPITAL MANAGEMENT SYSTEM                          |@@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|                                                                        |@@\n";
        cout << "@@|________________________________________________________________________|@@\n";
        cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n";
    }
    
    void displayHospitalInfo() const {
        ifstream file("hos.txt");
        if (!file) {
            cout << "\nError while opening the hospital information file\n";
            cout << "\nMake sure 'hos.txt' exists in the program directory\n";
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
            return;
        }
        
        cout << "\n\n";
        cout << "...........................Information about the Hospital.............................\n\n";
        
        string line;
        while (getline(file, line)) {
            cout << line << "\n";
        }
        
        file.close();
        cout << "\n\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
    
    void addPatientRecord() {
        // Get current time
        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        
        cout << "\n\n" << asctime(timeinfo);
        
        string filename;
        cout << "\n\nEnter the patient's file name: ";
        cin.ignore();
        getline(cin, filename);
        
        ofstream pat_file(filename);
        if (!pat_file) {
            cout << "\nError while creating the file\n";
            cout << "\nPress Enter to continue...";
            cin.get();
            return;
        }
        
        cout << "\n********************************************************************\n";
        pat_file << "\n********************************************************************\n\n";
        
        Patient patient;
        patient.inputDetails();
        patient.saveToFile(pat_file);
        
        cout << "\n********************************************************************\n";
        pat_file << "\n********************************************************************\n\n";
        
        pat_file.close();
        cout << "\nInformation Saved Successfully\n";
        cout << "\nPress Enter to continue...";
        cin.get();
    }
    
    void addDiagnosisInfo() {
        string filename;
        cout << "\n\nEnter the patient's file name to be opened: ";
        cin.ignore();
        getline(cin, filename);
        
        // First open file for reading
        ifstream inFile(filename);
        if (!inFile) {
            cout << "\nError: File does not exist or cannot be opened\n";
            cout << "\nPress Enter to continue...";
            cin.get();
            return;
        }
        
        // Display existing patient information
        clearScreen();
        cout << "\n\n";
        cout << "........................................ Information about " << filename << " ........................................\n\n";
        
        string line;
        while (getline(inFile, line)) {
            cout << line << "\n";
        }
        
        inFile.close();
        
        // Now open file for appending
        ofstream outFile(filename, ios::app);
        
        cout << "\nAdding more information in patient's file................\n";
        
        DiagnosisInfo info;
        cout << "Adding diagnosis on: " << info.getTimestamp();
        info.inputDetails();
        info.saveToFile(outFile);
        
        outFile << "\n*************************************************************************\n";
        outFile.close();
        
        cout << "\n\n" << info.getWard() << " ward is alloted Successfully\n";
        cout << "\nPress Enter to continue...";
        cin.get();
    }
    
    void displayPatientHistory() {
        string filename;
        cout << "\n\nEnter the patient's file name to be opened: ";
        cin.ignore();
        getline(cin, filename);
        
        ifstream pat_file(filename);
        if (!pat_file) {
            cout << "\nError: File does not exist or cannot be opened\n";
            cout << "\nPress Enter to continue...";
            cin.get();
            return;
        }
        
        clearScreen();
        cout << "\n\n";
        cout << "........................................ Full Medical History of " << filename << " ........................................\n\n";
        
        string line;
        while (getline(pat_file, line)) {
            cout << line << "\n";
        }
        
        pat_file.close();
        cout << "\nPress Enter to continue...";
        cin.get();
    }
    
    void run() {
        displayWelcome();
        
        if (!login()) {
            cout << "\n\nLogin failed after multiple attempts. Exiting program.\n";
            return;
        }
        
        int choice = 0;
        while (choice != 5) {
            displayMenu();
            
            cout << "Enter your choice: ";
            cin >> choice;
            
            // Handle invalid input
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n\nInvalid input. Please enter a number.\n";
                continue;
            }
            
            if (choice < 1 || choice > 5) {
                cout << "\n\nInvalid Choice\n";
                cout << "Try again...........\n\n";
                continue;
            }
            
            clearScreen();
            
            switch (choice) {
                case 1:
                    addPatientRecord();
                    break;
                case 2:
                    addDiagnosisInfo();
                    break;
                case 3:
                    displayPatientHistory();
                    break;
                case 4:
                    displayHospitalInfo();
                    break;
                case 5:
                    displayThankYou();
                    break;
            }
        }
    }
};

// Main function
int main() {
    Hospital hospital;
    hospital.run();
    return 0;
}