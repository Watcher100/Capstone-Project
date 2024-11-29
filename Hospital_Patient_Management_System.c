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
    FILE *file = fopen("user_data.dat", "w");
    if (file == NULL) {
        printf("Error: Could not open file for registration.\n");
        return;
    }
    fprintf(file, "%s\n%s\n", username, password);
    fclose(file);

    printf("Registration successful! You can now log in.\n");
}

// Function to handle user login
void login() {
    char username[50], password[50];
    char storedUsername[50], storedPassword[50];

    // Load user data from file
    FILE *file = fopen("user_data.dat", "r");
    if (file == NULL) {
        printf("No user data found. Please register first.\n");
        exit(1);
    }
    fscanf(file, "%s\n%s\n", storedUsername, storedPassword);
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
    scanf(" %[^\n]", newPatient.name);
    printf("Enter age: ");
    scanf(" %[^\n]", newPatient.age);
    printf("Enter gender: ");
    scanf(" %[^\n]", newPatient.gender);
    printf("Enter address: ");
    scanf(" %[^\n]", newPatient.address);
    printf("Enter contact number: ");
    scanf(" %[^\n]", newPatient.contact);
    printf("Enter illness: ");
    scanf(" %[^\n]", newPatient.illness);
    printf("Enter room number: ");
    scanf(" %[^\n]", newPatient.room);
    newPatient.isAdmitted = 1; // Mark as admitted
    newPatient.billAmount = 0.0;
    newPatient.doctorId = -1; // No doctor assigned yet

    patients[patientCount++] = newPatient;
    printf("Patient added successfully with ID %d!\n", newPatient.id);
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
    scanf(" %[^\n]", patients[index].contact);
    printf("Enter new room number: ");
    scanf(" %[^\n]", patients[index].room);
    printf("Is the patient admitted? (1-Yes, 0-No): ");
    scanf("%d", &patients[index].isAdmitted);

    printf("Patient information updated successfully!\n");
}

// Function to search and display patient details by their ID
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
    printf("ID: %d\n", patients[index].id);
    printf("Name: %s\n", patients[index].name);
    printf("Age: %s\n", patients[index].age);
    printf("Gender: %s\n", patients[index].gender);
    printf("Address: %s\n", patients[index].address);
    printf("Contact: %s\n", patients[index].contact);
    printf("Illness: %s\n", patients[index].illness);
    printf("Room No: %s\n", patients[index].room);
    printf("Admitted: %s\n", patients[index].isAdmitted ? "Yes" : "No");
    printf("Bill Amount: %.2f\n", patients[index].billAmount);

    if(patients[index].doctorId == -1){
        printf("No doctor assigned yet.\n\n");
    }else{
        printf("Assigned Doctor ID: %d\n\n", patients[index].doctorId);
    }
    
}

// Function to search and display doctor details by their ID
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
    printf("ID: %d\n", doctors[index].id);
    printf("Name: %s\n", doctors[index].name);
    printf("Specialty: %s\n\n", doctors[index].specialty);
}

// Function to delete a patient record
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
    printf("Patient with ID %d deleted successfully!\n", id);
}

// Function to delete a Doctor record
void deleteDoctor() {
    int id;
    printf("Enter doctor ID to delete: ");
    scanf("%d", &id);

    int index = searchDoctorById(id);
    if (index == -1) {
        printf("Doctor with ID %d not found.\n", id);
        return;
    }

    // Update patients assigned to this doctor
    for (int i = 0; i < patientCount; i++) {
        if (patients[i].doctorId == id) {
            patients[i].doctorId = -1; // Unassign doctor
        }
    }

    // Shift doctor records to delete the entry
    for (int i = index; i < doctorCount - 1; i++) {
        doctors[i] = doctors[i + 1];
    }
    doctorCount--;

    printf("Doctor with ID %d deleted successfully!\n", id);
}

// Function to manage billing
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
    scanf(" %[^\n]", newDoctor.name);
    printf("Enter specialty: ");
    scanf(" %[^\n]", newDoctor.specialty);

    doctors[doctorCount++] = newDoctor;
    printf("Doctor added successfully with ID %d!\n", newDoctor.id);
}

// Function to assign a doctor to a patient
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
    printf("Doctor ID %d assigned to patient ID %d successfully!\n", doctorId, patientId);
}

// Function to view all doctors
void viewDoctors() {
    if (doctorCount == 0) {
        printf("No doctor records found.\n");
        return;
    }

    printf("\n==== List of Doctors ====\n");
    for (int i = 0; i < doctorCount; i++) {
        printf("ID No: %d\nName: %s\nSpecialty: %s\n\n", doctors[i].id, doctors[i].name, doctors[i].specialty);
    }
}

// Function to search patient by ID
int searchPatientById(int id) {
    for (int i = 0; i < patientCount; i++) {
        if (patients[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Function to search doctor by ID
int searchDoctorById(int id) {
    for (int i = 0; i < doctorCount; i++) {
        if (doctors[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Function to load data from a file
void loadDataFromFile() {
    FILE *file = fopen("hospital_data.dat", "rb");
    if (file != NULL) {
        fread(&patientCount, sizeof(int), 1, file);
        fread(&patients, sizeof(Patient), patientCount, file);
        fread(&doctorCount, sizeof(int), 1, file);
        fread(&doctors, sizeof(Doctor), doctorCount, file);
        fclose(file);
    }
}

// Function to save data to a file
void saveDataToFile() {
    FILE *file = fopen("hospital_data.dat", "wb");
    if (file != NULL) {
        fwrite(&patientCount, sizeof(int), 1, file);
        fwrite(&patients, sizeof(Patient), patientCount, file);
        fwrite(&doctorCount, sizeof(int), 1, file);
        fwrite(&doctors, sizeof(Doctor), doctorCount, file);
        fclose(file);
    }
}
