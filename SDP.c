#include <stdio.h>
#include <string.h>

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
    int bazar_choice;
    do {
        printf("\nEnter your choice (1-%d): ", MAX_BAZARS);
        scanf("%d", &bazar_choice);
        if (bazar_choice < 1 || bazar_choice > MAX_BAZARS) {
            printf("Invalid selection! Please choose a valid bazar.\n");
        }
    } while (bazar_choice < 1 || bazar_choice > MAX_BAZARS);
    return bazar_choice;
}

int get_product_choice() {
    int product_choice;
    do {
        printf("\nSelect a product (1-%d): ", MAX_PRODUCTS);
        scanf("%d", &product_choice);
        if (product_choice < 1 || product_choice > MAX_PRODUCTS) {
            printf("Invalid selection! Please choose a valid product.\n");
        }
    } while (product_choice < 1 || product_choice > MAX_PRODUCTS);
    return product_choice;
}

int main() {
    char bazar_names[MAX_BAZARS][50] = {
        "Dhaka Bazar",
        "Chittagong Bazar",
        "Khulna Bazar"
    };

    char product_names[MAX_PRODUCTS][50] = {
        "Rice", "Sugar", "Oil", "Flour", "Lentils"
    };

    float today_prices[MAX_BAZARS][MAX_PRODUCTS] = {
        {55.0, 90.0, 150.0, 50.0, 130.0},
        {54.0, 92.0, 152.0, 48.0, 128.0},
        {53.0, 89.0, 148.0, 47.0, 126.0}
    };

    float old_prices[MAX_BAZARS][MAX_PRODUCTS] = {
        {50.0, 85.0, 140.0, 45.0, 120.0},
        {49.0, 87.0, 145.0, 44.0, 118.0},
        {48.0, 84.0, 138.0, 42.0, 116.0}
    };

    print_border();
    printf("Welcome to Smart Market Price Monitoring System\n");
    print_border();

    display_bazars(bazar_names);
    int bazar_choice = get_bazar_choice();

    display_products(product_names);
    int product_choice = get_product_choice();

    print_border();
    printf("\nProduct: %s\n", product_names[product_choice - 1]);
    printf("Today's Price: %.2f BDT\n", today_prices[bazar_choice - 1][product_choice - 1]);
    printf("Old Price: %.2f BDT\n", old_prices[bazar_choice - 1][product_choice - 1]);
    print_border();

    return 0;
}
