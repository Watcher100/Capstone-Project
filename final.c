#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 500
#define MAX_DOCTORS 100

// Structure for Patient
typedef struct {
    int id;
    char name[50];
    char age[10];
    char gender[10];
    char address[100];
    char contact[15];
    char illness[50];
    char room[10];
    int isAdmitted;
    float billAmount;
    int doctorId; // ID of the Assigned doctor
} Patient;

// Structure for Doctor
typedef struct {
    int id;
    char name[50];
    char specialty[50];
} Doctor;

// Global variables
Patient patients[MAX_PATIENTS];
Doctor doctors[MAX_DOCTORS];
int patientCount = 0;
int doctorCount = 0;

// Function prototypes
void registerUser();
void login();
void loadDataFromFile();
void saveDataToFile();
void addPatient();
void viewPatients();
void updatePatient();
void deletePatient();
void deleteDoctor();
void manageBilling();
void assignDoctor();
void viewDoctors();
void addDoctor();
void searchAndDisplayPatientById();
void searchAndDisplayDoctorById();
int searchPatientById(int id);
int searchDoctorById(int id);
void displayMenu();

int main() {
    int choice;

    // Display registration/login menu
    printf("==== Welcome to the Hospital Patient Management System ====\n");
    int loggedIn = 0;
    while (!loggedIn) {
        int userChoice;
        printf("\n1. Register\n2. Login\nEnter your choice: ");
        scanf("%d", &userChoice);

        if (userChoice == 1) {
            registerUser(); // Register a new user
        } else if (userChoice == 2) {
            login();        // Login with existing credentials
            loggedIn = 1;   // Exit loop on successful login
        } else {
            printf("Invalid choice. Please choose 1 (Register) or 2 (Login).\n");
        }
    }

    loadDataFromFile(); // Load data from file at the start

    do {
        displayMenu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPatient();
                break;
            case 2:
                viewPatients();
                break;
            case 3:
                updatePatient();
                break;
            case 4:
                deletePatient();
                break;
            case 5:
                manageBilling();
                break;
            case 6:
                assignDoctor();
                break;
            case 7:
                addDoctor();
                break;
            case 8:
                viewDoctors();
                break;
            case 9:
                deleteDoctor();
                break;
            case 10:
                searchAndDisplayPatientById();
                break;
            case 11:
                searchAndDisplayDoctorById();
                break;
            case 12:
                saveDataToFile();
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 12);

    return 0;
}

// Function to register a new user
void registerUser() {
    char username[50], password[50];

    printf("\n==== Register ====\n");
    printf("Enter a username: ");
    scanf("%s", username);
    printf("Enter a password: ");
    scanf("%s", password);

    // Save to file
    FILE *file = fopen("user_data.txt", "w");
    if (file == NULL) {
        printf("Error: Could not open file for registration.\n");
        return;
    }
    fprintf(file, "Username: %s\nPassword: %s\n", username, password);
    fclose(file);

    printf("Registration successful! You can now log in.\n");
}

// Function to handle user login
void login() {
    char username[50], password[50];
    char storedUsername[50], storedPassword[50];

    // Load user data from file
    FILE *file = fopen("user_data.txt", "r");
    if (file == NULL) {
        printf("No user data found. Please register first.\n");
        exit(1);
    }
    fscanf(file, "Username: %s\nPassword: %s\n", storedUsername, storedPassword);
    fclose(file);

    int attempts = 3; // Number of login attempts
    while (attempts > 0) {
        printf("\n==== Login ====\n");
        printf("Enter username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        // Check credentials
        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            printf("Login successful! Welcome, %s.\n", username);
            return; // Exit the login function and proceed to the main program
        } else {
            attempts--;
            printf("Invalid username or password. You have %d attempt(s) left.\n", attempts);
        }
    }

    // Exit program after failed attempts
    printf("Too many failed login attempts. Exiting program.\n");
    exit(1);
}

// Function to display the menu
void displayMenu() {
    printf("\n==== Hospital Patient Management System ====\n");
    printf("1. Add Patient\n");
    printf("2. View All Patients\n");
    printf("3. Update Patient Information\n");
    printf("4. Delete Patient Record\n");
    printf("5. Manage Billing\n");
    printf("6. Assign Doctor to Patient\n");
    printf("7. Add Doctor\n");
    printf("8. View All Doctors\n");
    printf("9. Delete Doctor Record\n");
    printf("10. Search Patient by ID\n");
    printf("11. Search Doctor by ID\n");
    printf("12. Exit\n");
}

// Function to add a new patient
void addPatient() {
    if (patientCount >= MAX_PATIENTS) {
        printf("Cannot add more patients. Database is full.\n");
        return;
    }

    Patient newPatient;
    printf("Enter patient ID: ");
    scanf("%d", &newPatient.id);

    if (searchPatientById(newPatient.id) != -1) {
        printf("Error: Patient ID %d already exists. Please enter a unique ID.\n", newPatient.id);
        return;
    }

    printf("Enter patient name: ");
    scanf("%s", newPatient.name);
    printf("Enter age: ");
    scanf("%s", newPatient.age);
    printf("Enter gender: ");
    scanf("%s", newPatient.gender);
    printf("Enter address: ");
    scanf("%s", newPatient.address);
    printf("Enter contact number: ");
    scanf("%s", newPatient.contact);
    printf("Enter illness: ");
    scanf("%s", newPatient.illness);
    printf("Enter room number: ");
    scanf("%s", newPatient.room);
    newPatient.isAdmitted = 1; // Mark as admitted
    newPatient.billAmount = 0.0;
    newPatient.doctorId = -1; // No doctor assigned yet

    patients[patientCount++] = newPatient;
    printf("Patient added successfully with ID %d!\n", newPatient.id);

    // Save the updated data after adding the patient
    saveDataToFile();
}

// Function to view all patients
void viewPatients() {
    if (patientCount == 0) {
        printf("No patient records found.\n");
        return;
    }

    printf("\n==== List of Patients ====\n");
    for (int i = 0; i < patientCount; i++) {
        printf("ID No: %d\nName: %s\nAge: %s\nGender: %s\nAddress: %s\nContact Number: %s\nRoom No: %s\nIllness: %s\nBill: %.2f\n",
               patients[i].id, patients[i].name, patients[i].age, patients[i].gender, patients[i].address, patients[i].contact,
               patients[i].room, patients[i].illness, patients[i].billAmount);

               if(patients[i].doctorId == -1){
                    printf("No doctor Assigned yet.\n\n");
               }else{
                    printf("Assigned Doctor ID: %d\n\n",patients[i].doctorId);
               }
    }
}

// Function to update patient information
void updatePatient() {
    int id;
    printf("Enter patient ID to update: ");
    scanf("%d", &id);

    int index = searchPatientById(id);
    if (index == -1) {
        printf("Patient with ID %d not found.\n", id);
        return;
    }

    printf("Updating information for %s (ID %d):\n", patients[index].name, id);
    printf("Enter new contact number: ");
    scanf("%s", patients[index].contact);
    printf("Enter new room number: ");
    scanf("%s", patients[index].room);
    printf("Is the patient admitted? (1-Yes, 0-No): ");
    scanf("%d", &patients[index].isAdmitted);

    printf("Patient information updated successfully!\n");

    // Save the updated data after modifying the patient
    saveDataToFile();
}

// Function to delete a patient by ID
void deletePatient() {
    int id;
    printf("Enter patient ID to delete: ");
    scanf("%d", &id);

    int index = searchPatientById(id);
    if (index == -1) {
        printf("Patient with ID %d not found.\n", id);
        return;
    }

    for (int i = index; i < patientCount - 1; i++) {
        patients[i] = patients[i + 1];
    }
    patientCount--;
    printf("Patient with ID %d deleted successfully.\n", id);

    // Save the updated data after deleting the patient
    saveDataToFile();
}

/// Function to manage billing
void manageBilling() {
    int id;
    printf("Enter patient ID for billing: ");
    scanf("%d", &id);

    int index = searchPatientById(id);
    if (index == -1) {
        printf("Patient with ID %d not found.\n", id);
        return;
    }

    printf("Current bill amount for %s: %.2f\n", patients[index].name, patients[index].billAmount);
    float newCharge;
    printf("Enter additional charges: ");
    scanf("%f", &newCharge);

    patients[index].billAmount += newCharge;
    printf("Updated bill amount: %.2f\n", patients[index].billAmount);
    
    // Save the updated data after modifying the bill amount
    saveDataToFile();
}


// Function to assign doctor to patient
void assignDoctor() {
    int patientId, doctorId;
    printf("Enter patient ID: ");
    scanf("%d", &patientId);

    int patientIndex = searchPatientById(patientId);
    if (patientIndex == -1) {
        printf("Patient with ID %d not found.\n", patientId);
        return;
    }

    printf("Enter doctor ID to assign: ");
    scanf("%d", &doctorId);

    int doctorIndex = searchDoctorById(doctorId);
    if (doctorIndex == -1) {
        printf("Doctor with ID %d not found.\n", doctorId);
        return;
    }

    patients[patientIndex].doctorId = doctorId;
    printf("Doctor %s assigned to patient %s.\n", doctors[doctorIndex].name, patients[patientIndex].name);

    // Save the updated data after assigning a doctor
    saveDataToFile();
}

// Function to view all doctors
void viewDoctors() {
    if (doctorCount == 0) {
        printf("No doctor records found.\n");
        return;
    }

    printf("\n==== List of Doctors ====\n");
    for (int i = 0; i < doctorCount; i++) {
        printf("Doctor ID: %d\nName: %s\nSpecialty: %s\n\n",
               doctors[i].id, doctors[i].name, doctors[i].specialty);
    }
}

// Function to add a new doctor
void addDoctor() {
    if (doctorCount >= MAX_DOCTORS) {
        printf("Cannot add more doctors. Database is full.\n");
        return;
    }

    Doctor newDoctor;
    printf("Enter doctor ID: ");
    scanf("%d", &newDoctor.id);

    if (searchDoctorById(newDoctor.id) != -1) {
        printf("Error: Doctor ID %d already exists. Please enter a unique ID.\n", newDoctor.id);
        return;
    }

    printf("Enter doctor name: ");
    scanf("%s", newDoctor.name);
    printf("Enter specialty: ");
    scanf("%s", newDoctor.specialty);

    doctors[doctorCount++] = newDoctor;
    printf("Doctor added successfully with ID %d!\n", newDoctor.id);

    // Save the updated data after adding the doctor
    saveDataToFile();
}

// Function to delete a doctor by ID
void deleteDoctor() {
    int id;
    printf("Enter doctor ID to delete: ");
    scanf("%d", &id);

    int index = searchDoctorById(id);
    if (index == -1) {
        printf("Doctor with ID %d not found.\n", id);
        return;
    }

    for (int i = index; i < doctorCount - 1; i++) {
        doctors[i] = doctors[i + 1];
    }
    doctorCount--;
    printf("Doctor with ID %d deleted successfully.\n", id);

    // Save the updated data after deleting the doctor
    saveDataToFile();
}

// Function to search for a patient by ID
int searchPatientById(int id) {
    for (int i = 0; i < patientCount; i++) {
        if (patients[i].id == id) {
            return i; // Patient found, return index
        }
    }
    return -1; // Patient not found
}

// Function to search for a doctor by ID
int searchDoctorById(int id) {
    for (int i = 0; i < doctorCount; i++) {
        if (doctors[i].id == id) {
            return i; // Doctor found, return index
        }
    }
    return -1; // Doctor not found
}

// Function to search and display patient by ID
void searchAndDisplayPatientById() {
    int id;
    printf("Enter patient ID to search: ");
    scanf("%d", &id);

    int index = searchPatientById(id);
    if (index == -1) {
        printf("Patient with ID %d not found.\n", id);
        return;
    }

    printf("\nPatient Details:\n");
    printf("ID No: %d\nName: %s\nAge: %s\nGender: %s\nAddress: %s\nContact Number: %s\nRoom No: %s\nIllness: %s\nBill: %.2f\n",
           patients[index].id, patients[index].name, patients[index].age, patients[index].gender, patients[index].address,
           patients[index].contact, patients[index].room, patients[index].illness, patients[index].billAmount);

    if(patients[index].doctorId == -1){
        printf("No doctor assigned.\n");
    }else{
        printf("Assigned Doctor ID: %d\n", patients[index].doctorId);
    }
}

// Function to search and display doctor by ID
void searchAndDisplayDoctorById() {
    int id;
    printf("Enter doctor ID to search: ");
    scanf("%d", &id);

    int index = searchDoctorById(id);
    if (index == -1) {
        printf("Doctor with ID %d not found.\n", id);
        return;
    }

    printf("\nDoctor Details:\n");
    printf("ID: %d\nName: %s\nSpecialty: %s\n", doctors[index].id, doctors[index].name, doctors[index].specialty);
}

// Function to save all data to file
void saveDataToFile() {
    FILE *patientFile = fopen("patients.txt", "w");
    FILE *doctorFile = fopen("doctors.txt", "w");
    if (patientFile == NULL || doctorFile == NULL) {
        printf("Error: Could not open file for saving data.\n");
        return;
    }

    // Save patients to file
    for (int i = 0; i < patientCount; i++) {
        fprintf(patientFile, "ID: %d\nName: %s\nAge: %s\nGender: %s\nAddress: %s\nContact: %s\nIllness: %s\nRoom: %s\nBill: %.2f\nDoctor ID: %d\n",
                patients[i].id, patients[i].name, patients[i].age, patients[i].gender, patients[i].address,
                patients[i].contact, patients[i].illness, patients[i].room, patients[i].billAmount, patients[i].doctorId);
    }

    // Save doctors to file
    for (int i = 0; i < doctorCount; i++) {
        fprintf(doctorFile, "ID: %d\nName: %s\nSpecialty: %s\n", doctors[i].id, doctors[i].name, doctors[i].specialty);
    }

    fclose(patientFile);
    fclose(doctorFile);

    printf("Data saved successfully.\n");
}

// Function to load all data from file
void loadDataFromFile() {
    FILE *patientFile = fopen("patients.txt", "r");
    FILE *doctorFile = fopen("doctors.txt", "r");
    if (patientFile == NULL || doctorFile == NULL) {
        printf("Error: Could not open file for loading data.\n");
        return;
    }

    // Load patients from file
    while (fscanf(patientFile, "ID: %d\nName: %s\nAge: %s\nGender: %s\nAddress: %s\nContact: %s\nIllness: %s\nRoom: %s\nBill: %f\nDoctor ID: %d\n",
                  &patients[patientCount].id, patients[patientCount].name, patients[patientCount].age,
                  patients[patientCount].gender, patients[patientCount].address, patients[patientCount].contact,
                  patients[patientCount].illness, patients[patientCount].room, &patients[patientCount].billAmount,
                  &patients[patientCount].doctorId) != EOF) {
        patientCount++;
    }

    // Load doctors from file
    while (fscanf(doctorFile, "ID: %d\nName: %s\nSpecialty: %s\n", &doctors[doctorCount].id, doctors[doctorCount].name, doctors[doctorCount].specialty) != EOF) {
        doctorCount++;
    }

    fclose(patientFile);
    fclose(doctorFile);
}