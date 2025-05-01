#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BAZARS 3
#define MAX_PRODUCTS 5

void print_border() {
    printf("\n-------------------------------------------------\n");
}

void display_bazars(char bazar_names[MAX_BAZARS][50]) {
    printf("Select a Bazar:\n");
    for (int i = 0; i < MAX_BAZARS; i++) {
        printf("%d. %s\n", i + 1, bazar_names[i]);
    }
}

void display_products(char product_names[MAX_PRODUCTS][50]) {
    printf("\nAvailable products:\n");
    for (int i = 0; i < MAX_PRODUCTS; i++) {
        printf("%d. %s\n", i + 1, product_names[i]);
    }
}

int get_bazar_choice() {
    int choice;
    do {
        printf("\nEnter your choice (1-%d): ", MAX_BAZARS);
        scanf("%d", &choice);
        if (choice < 1 || choice > MAX_BAZARS) {
            printf("Invalid selection! Please choose a valid bazar.\n");
        }
    } while (choice < 1 || choice > MAX_BAZARS);
    return choice;
}

int get_product_choice() {
    int choice;
    do {
        printf("\nSelect a product (1-%d): ", MAX_PRODUCTS);
        scanf("%d", &choice);
        if (choice < 1 || choice > MAX_PRODUCTS) {
            printf("Invalid selection! Please choose a valid product.\n");
        }
    } while (choice < 1 || choice > MAX_PRODUCTS);
    return choice;
}

void load_names(char filename[], char names[][50], int count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        exit(1);
    }
    for (int i = 0; i < count; i++) {
        fgets(names[i], 50, file);
        names[i][strcspn(names[i], "\n")] = '\0';  // remove newline
    }
    fclose(file);
}

void load_prices_from_single_file(char filename[], float old_prices[MAX_BAZARS][MAX_PRODUCTS], float today_prices[MAX_BAZARS][MAX_PRODUCTS]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < MAX_BAZARS; i++) {
        for (int j = 0; j < MAX_PRODUCTS; j++) {
            fscanf(file, "%f", &old_prices[i][j]); // Line 1: old prices
        }
        for (int j = 0; j < MAX_PRODUCTS; j++) {
            fscanf(file, "%f", &today_prices[i][j]); // Line 2: today's prices
        }
    }

    fclose(file);
}

int main() {
    char bazar_names[MAX_BAZARS][50];
    char product_names[MAX_PRODUCTS][50];
    float today_prices[MAX_BAZARS][MAX_PRODUCTS];
    float old_prices[MAX_BAZARS][MAX_PRODUCTS];

    // Load data
    load_names("bazar_names.txt", bazar_names, MAX_BAZARS);
    load_names("product_names.txt", product_names, MAX_PRODUCTS);
    load_prices_from_single_file("prices.txt", old_prices, today_prices);

    // Get today's date
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char date_str[20];
    strftime(date_str, sizeof(date_str), "%Y-%m-%d", tm_info);

    // Header
    print_border();
    printf("Welcome to Smart Market Price Monitoring System\n");
    printf("Today's Date: %s\n", date_str);
    print_border();

    // User Input
    display_bazars(bazar_names);
    int bazar_choice = get_bazar_choice();

    display_products(product_names);
    int product_choice = get_product_choice();

    // Show Prices
    print_border();
    printf("Today's Date: %s\n", date_str);
    printf("\nProduct: %s\n", product_names[product_choice - 1]);
    printf("Today's Price: %.2f BDT\n", today_prices[bazar_choice - 1][product_choice - 1]);
    printf("Old Price: %.2f BDT\n", old_prices[bazar_choice - 1][product_choice - 1]);
    print_border();

    return 0;
}
