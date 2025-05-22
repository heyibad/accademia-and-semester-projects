#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>

using namespace std;

// Function prototypes
bool login();
void displayWelcome();
void displayMenu();
void addPatientRecord();
void addDiagnosisInfo();
void displayPatientHistory();
void displayHospitalInfo();
void displayThankYou();

int main() {
    displayWelcome();
    
    if (!login()) {
        cout << "\n\nLogin failed after multiple attempts. Exiting program.\n";
        return 1;
    }
    
    int choice = 0;
    while (choice != 5) {
        displayMenu();
        
        cout << "\t\t\t\t\t\tEnter your choice: ";
        cin >> choice;
        
        // Handle invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\n\t\t\t\t\t\tInvalid input. Please enter a number.\n";
            continue;
        }
        
        if (choice < 1 || choice > 5) {
            cout << "\n\n\t\t\t\t\t\tInvalid Choice\n";
            cout << "\t\t\t\t\t\tTry again...........\n\n";
            continue;
        }
        
        cout << "\033[2J\033[1;1H"; // Clear screen (works on most terminals)
        
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
    
    return 0;
}

bool login() {
    const int MAX_ATTEMPTS = 3;
    const string CORRECT_PASSWORD = "pass";
    string password;
    int attempts = 0;
    
    while (attempts < MAX_ATTEMPTS) {
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t  HOSPITAL MANAGEMENT SYSTEM \n\n";
        cout << "\t\t\t\t\t\t\t\t------------------------------";
        cout << "\n\t\t\t\t\t\t\t\t\t     LOGIN \n";
        cout << "\t\t\t\t\t\t\t\t------------------------------\n\n";
        cout << "\t\t\t\t\t\t\t\tEnter Password: ";
        
        // Read password (without showing characters)
        char ch;
        password = "";
        while ((ch = cin.get()) != '\n') {
            if (ch == 127 || ch == 8) { // Backspace
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";
                }
            } else {
                password.push_back(ch);
                cout << '*';
            }
        }
        
        if (password == CORRECT_PASSWORD) {
            cout << "\n\n\t\t\t\t\t\t\t\tAccess Granted! \n";
            cout << "\n\t\t\t\t\t\t\t\tPress Enter to continue...";
            cin.get();
            cout << "\033[2J\033[1;1H"; // Clear screen
            return true;
        } else {
            attempts++;
            cout << "\n\n\t\t\t\t\t\t\t\tAccess Denied...\n";
            cout << "\t\t\t\t\t\t\t\tAttempt " << attempts << " of " << MAX_ATTEMPTS << "\n";
            cout << "\n\t\t\t\t\t\t\t\tPress Enter to try again...";
            cin.get();
            cout << "\033[2J\033[1;1H"; // Clear screen
        }
    }
    
    return false;
}

void displayWelcome() {
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
    cout << "\t\t\t\t\t@@ _______________________________________________________________________________________ @@\n";
    cout << "\t\t\t\t\t@@|                                           		                                  |@@\n";
    cout << "\t\t\t\t\t@@|                                           		                                  |@@\n";
    cout << "\t\t\t\t\t@@|                                           		                                  |@@\n";
    cout << "\t\t\t\t\t@@|                                           		                                  |@@\n";
    cout << "\t\t\t\t\t@@|                                           		                                  |@@\n";
    cout << "\t\t\t\t\t@@|                                  WELCOME TO                                           |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|                           HOSPITAL MANAGEMENT SYSTEM                                  |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|_______________________________________________________________________________________|@@\n";
    cout << "\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n\n\n\t\t\t\t\t";
    cout << "Press Enter to continue...";
    cin.ignore();
    cout << "\033[2J\033[1;1H"; // Clear screen
}

void displayMenu() {
    cout << "\033[2J\033[1;1H"; // Clear screen
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t  HOSPITAL MANAGEMENT SYSTEM \n\n";
    cout << "\n\n\t\t\t\t\t\tPlease,  Choose from the following Options: \n\n";
    cout << "\t\t\t\t\t\t _________________________________________________________________ \n";
    cout << "\t\t\t\t\t\t|                                           	                  |\n";
    cout << "\t\t\t\t\t\t|             1  >> Add New Patient Record                        |\n";
    cout << "\t\t\t\t\t\t|             2  >> Add Diagnosis Information                     |\n";
    cout << "\t\t\t\t\t\t|             3  >> Full History of the Patient                   |\n";
    cout << "\t\t\t\t\t\t|             4  >> Information About the Hospital                |\n";
    cout << "\t\t\t\t\t\t|             5  >> Exit the Program                              |\n";
    cout << "\t\t\t\t\t\t|_________________________________________________________________|\n\n";
}

void displayHospitalInfo() {
    ifstream file("hos.txt");
    if (!file) {
        cout << "\nError while opening the hospital information file\n";
        cout << "\nMake sure 'hos.txt' exists in the program directory\n";
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }
    
    cout << "\n\n\n\n\n\n\n\t\t\t\t\t   ...........................Information about the Hospital.............................\n\n";
    
    string line;
    while (getline(file, line)) {
        cout << line << "\n\t\t";
    }
    
    file.close();
    cout << "\n\n\t\tPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void addPatientRecord() {
    // Get current time
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    cout << "\n\n\t\t\t\t\t\t\t\t" << asctime(timeinfo);
    
    string filename;
    cout << "\n\n\n\nEnter the patient's file name: ";
    cin.ignore();
    getline(cin, filename);
    
    ofstream pat_file(filename);
    if (!pat_file) {
        cout << "\nError while creating the file\n";
        cout << "\nPress Enter to continue...";
        cin.get();
        return;
    }
    
    // Patient information structure
    struct patient_info {
        string name;
        string address;
        string contact;
        string age;
        string sex;
        string blood_gp;
        string disease_past;
        string id;
    } patient;
    
    cout << "\n********************************************************************\n";
    pat_file << "\n********************************************************************\n\n";
    
    cout << "\nName: ";
    getline(cin, patient.name);
    pat_file << "Name: " << patient.name << "\n";
    
    cout << "\nAddress: ";
    getline(cin, patient.address);
    pat_file << "Address: " << patient.address << "\n";
    
    cout << "\nContact Number: ";
    getline(cin, patient.contact);
    pat_file << "Contact Number: " << patient.contact << "\n";
    
    cout << "\nAge: ";
    getline(cin, patient.age);
    pat_file << "Age: " << patient.age << "\n";
    
    cout << "\nSex: ";
    getline(cin, patient.sex);
    pat_file << "Sex: " << patient.sex << "\n";
    
    cout << "\nBlood Group: ";
    getline(cin, patient.blood_gp);
    pat_file << "Blood Group: " << patient.blood_gp << "\n";
    
    cout << "\nAny Major disease suffered earlier: ";
    getline(cin, patient.disease_past);
    pat_file << "Any Major disease suffered earlier: " << patient.disease_past << "\n";
    
    cout << "\nPatient ID: ";
    getline(cin, patient.id);
    pat_file << "Patient ID: " << patient.id << "\n";
    
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
    cout << "\033[2J\033[1;1H"; // Clear screen
    cout << "\n\n\n\n\t\t\t\t........................................ Information about " << filename << " ........................................\n\n\n\n";
    
    string line;
    while (getline(inFile, line)) {
        cout << line << "\n";
    }
    
    inFile.close();
    
    // Now open file for appending
    ofstream outFile(filename, ios::app);
    
    // Get current time
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    cout << "\nAdding more information in patient's file................on: " << asctime(timeinfo);
    outFile << "Description of " << asctime(timeinfo) << "\n";
    
    // Diagnosis information structure
    struct diagnosis_info {
        string symptom;
        string diagnosis;
        string medicine;
        string admission;
        string ward;
    } info;
    
    cout << "\nSymptoms: ";
    getline(cin, info.symptom);
    outFile << "Symptoms: " << info.symptom << "\n";
    
    cout << "\nDiagnosis: ";
    getline(cin, info.diagnosis);
    outFile << "Diagnosis: " << info.diagnosis << "\n";
    
    cout << "\nMedicines: ";
    getline(cin, info.medicine);
    outFile << "Medicines: " << info.medicine << "\n";
    
    cout << "\nAdmission Required? (Yes/No): ";
    getline(cin, info.admission);
    outFile << "Admission Required?: " << info.admission << "\n";
    
    cout << "\nType of ward: ";
    getline(cin, info.ward);
    outFile << "Type of ward: " << info.ward << "\n";
    
    outFile << "\n*************************************************************************\n";
    outFile.close();
    
    cout << "\n\n" << info.ward << " ward is alloted Successfully\n";
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
    
    cout << "\033[2J\033[1;1H"; // Clear screen
    cout << "\n\n\n\n\t\t\t\t........................................ Full Medical History of " << filename << " ........................................\n\n\n\n";
    
    string line;
    while (getline(pat_file, line)) {
        cout << line << "\n";
    }
    
    pat_file.close();
    cout << "\nPress Enter to continue...";
    cin.get();
}

void displayThankYou() {
    cout << "\033[2J\033[1;1H"; // Clear screen
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
    cout << "\t\t\t\t\t@@ _______________________________________________________________________________________ @@\n";
    cout << "\t\t\t\t\t@@|                                           		                                  |@@\n";
    cout << "\t\t\t\t\t@@|                                           		                                  |@@\n";
    cout << "\t\t\t\t\t@@|                                           		                                  |@@\n";
    cout << "\t\t\t\t\t@@|                                           		                                  |@@\n";
    cout << "\t\t\t\t\t@@|                                           		                                  |@@\n";
    cout << "\t\t\t\t\t@@|                               THANK YOU FOR USING                                     |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|                            HOSPITAL MANAGEMENT SYSTEM                                 |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|                                                                                       |@@\n";
    cout << "\t\t\t\t\t@@|_______________________________________________________________________________________|@@\n";
    cout << "\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n\n\n\t\t\t\t\t";
}