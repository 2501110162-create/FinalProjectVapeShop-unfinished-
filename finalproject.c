#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Exact filename from your project 
#define DATA_FILE "FinalProjecctData.txt"

// Function Prototypes
void viewProducts();
void searchProduct();
void buyProduct();
void addProduct();
void trim(char *str);

int main() {
    char username[20], password[20];
    int choice;

    // --- LOGIN PHASE ---
    printf("--- Admin Login ---\n");
    printf("Username: ");
    scanf(" %[^\n]s", username); 
    printf("Password: ");
    scanf("%s", password);

    // Credentials based on your requirement
    if (strcmp(username, "admin 123") != 0 || strcmp(password, "password") != 0) {
        printf("\nInvalid credentials! Access Denied.\n");
        return 1;
    }

    // --- MAIN MENU LOOP (Flowchart Logic) ---
    do {
        printf("\n==================================\n");
        printf(" Vape Inventory Management System \n");
        printf("==================================\n");
        printf("1. View Products\n");
        printf("2. Search Product (ID or Name)\n");
        printf("3. Buy Product (ID or Name)\n");
        printf("4. Add Product\n");
        printf("5. Exit\n");
        printf("----------------------------------\n");
        printf("Enter Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            continue;
        }

        switch (choice) {
            case 1: viewProducts(); break;
            case 2: searchProduct(); break;
            case 3: buyProduct(); break;
            case 4: addProduct(); break;
            case 5: printf("Saving Data and Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 5);

    return 0;
}

// --- HELPER: FIXES ALIGNMENT BY REMOVING EXTRA SPACES ---
void trim(char *str) {
    char *p = str;
    while (isspace(*p)) p++;
    if (*p == 0) return;
    char *end = p + strlen(p) - 1;
    while (end > p && isspace(*end)) end--;
    *(end + 1) = 0;
    memmove(str, p, strlen(p) + 1);
}

// --- CHOICE 1: VIEW PRODUCTS ---
void viewProducts() {
    FILE *file = fopen(DATA_FILE, "r");
    if (!file) { printf("\nError: File not found.\n"); return; }

    char id[10], name[100]; float price; int stock;
    printf("\n%-5s | %-40s | %-8s | %-5s\n", "ID", "PRODUCT NAME", "PRICE", "STOCK");
    printf("----------------------------------------------------------------------\n");
    
    // Reads ID, Name, Price, and Stock [cite: 1, 2, 3]
    while (fscanf(file, " %[^,], %[^,], %f, %d\n", id, name, &price, &stock) != EOF) {
        trim(id); trim(name);
        printf("%-5s | %-40s | P%-7.2f | %-5d\n", id, name, price, stock);
    }
    fclose(file);
}

// --- CHOICE 2: SEARCH PRODUCT (ID OR NAME) ---
void searchProduct() {
    FILE *file = fopen(DATA_FILE, "r");
    if (!file) return;

    int type;
    char search[100], id[10], name[100]; float price; int stock; int found = 0;

    printf("\nSearch by: \n1. ID\n2. Name\nEnter choice: ");
    scanf("%d", &type);
    printf("Enter Search Term: ");
    scanf(" %[^\n]s", search);

    while (fscanf(file, " %[^,], %[^,], %f, %d\n", id, name, &price, &stock) != EOF) {
        trim(id); trim(name);
        // Toggle logic for ID or Name search
        if ((type == 1 && strcmp(id, search) == 0) || 
            (type == 2 && strcasecmp(name, search) == 0)) {
            printf("\n[FOUND] ID: %s | %s | Price: P%.2f | Stock: %d\n", id, name, price, stock);
            found = 1;
        }
    }
    if (!found) printf("Product not found.\n");
    fclose(file);
}

// --- CHOICE 3: BUY PRODUCT (ID OR NAME) ---
void buyProduct() {
    FILE *file = fopen(DATA_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!file || !temp) return;

    int type, found = 0, qty;
    char search[100], id[10], name[100]; float price; int stock;

    printf("\nBuy Product by: \n1. ID\n2. Name\nEnter choice: ");
    scanf("%d", &type);
    printf("Enter Search Term: ");
    scanf(" %[^\n]s", search);

    while (fscanf(file, " %[^,], %[^,], %f, %d\n", id, name, &price, &stock) != EOF) {
        trim(id); trim(name);
        if ((type == 1 && strcmp(id, search) == 0) || 
            (type == 2 && strcasecmp(name, search) == 0)) {
            found = 1;
            printf("Found: %s. Current Stock: %d. \nEnter Quantity: ", name, stock);
            scanf("%d", &qty);
            if (qty <= stock) {
                stock -= qty;
                printf("Total Price: P%.2f. Purchase Successful!\n", price * (float)qty);
            } else {
                printf("Insufficient Stock!\n");
            }
        }
        fprintf(temp, "%s, %s, %.2f, %d\n", id, name, price, stock);
    }
    fclose(file); fclose(temp);
    remove(DATA_FILE); rename("temp.txt", DATA_FILE);
    if (!found) printf("Product not found.\n");
}

// --- CHOICE 4: ADD PRODUCT ---
void addProduct() {
    FILE *file = fopen(DATA_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    char newName[100], newId[10], id[10], name[100]; 
    float newPrice, price; int newStock, stock, alreadyExists = 0;

    printf("Enter Product Name: ");
    scanf(" %[^\n]s", newName);
    printf("Price: "); scanf("%f", &newPrice);
    printf("Quantity: "); scanf("%d", &newStock);

    if (file) {
        while (fscanf(file, " %[^,], %[^,], %f, %d\n", id, name, &price, &stock) != EOF) {
            trim(id); trim(name);
            if (strcasecmp(name, newName) == 0) {
                stock += newStock; // Update existing
                price = newPrice;  
                alreadyExists = 1;
            }
            fprintf(temp, "%s, %s, %.2f, %d\n", id, name, price, stock);
        }
        fclose(file);
    }

    if (!alreadyExists) {
        printf("Enter New ID (e.g., 036): ");
        scanf("%s", newId);
        fprintf(temp, "%s, %s, %.2f, %d\n", newId, newName, newPrice, newStock);
        printf("New product added successfully!\n");
    } else {
        printf("Product updated successfully!\n");
    }
    
    fclose(temp);
    remove(DATA_FILE); rename("temp.txt", DATA_FILE);
}