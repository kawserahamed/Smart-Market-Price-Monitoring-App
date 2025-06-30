#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BAZARS 100
#define MAX_PRODUCTS 100
#define ADMIN_USER "admin"
#define ADMIN_PASS "1234"

char bazar_names[MAX_BAZARS][50];
char product_names[MAX_PRODUCTS][50];
float today_prices[MAX_PRODUCTS][MAX_BAZARS];
float old_prices[MAX_PRODUCTS][MAX_BAZARS];
int total_bazars = 0;
int total_products = 0;

void print_border() {
    printf("\n-------------------------------------------------\n");
}

void load_market_data() {
    FILE *file = fopen("market_data.txt", "r");
    if (!file) {
        printf("Error opening market_data.txt\n");
        exit(1);
    }

    char line[500];
    fgets(line, sizeof(line), file);
    char *token = strtok(line, " \n");
    while (token != NULL) {
        strcpy(bazar_names[total_bazars++], token);
        token = strtok(NULL, " \n");
    }

    while (fscanf(file, "%s", product_names[total_products]) != EOF) {
        for (int i = 0; i < total_bazars; i++) {
            fscanf(file, "%f", &today_prices[total_products][i]);
        }
        total_products++;
    }
    fclose(file);
}

void load_old_market_data() {
    FILE *file = fopen("old_market_data.txt", "r");
    if (!file) return;
    char line[500];
    fgets(line, sizeof(line), file);
    int index = 0;
    while (fscanf(file, "%s", line) != EOF && index < MAX_PRODUCTS) {
        for (int j = 0; j < total_bazars; j++) {
            fscanf(file, "%f", &old_prices[index][j]);
        }
        index++;
    }
    fclose(file);
}

void save_market_data() {
    FILE *file = fopen("market_data.txt", "w");
    for (int i = 0; i < total_bazars; i++) {
        fprintf(file, "%s ", bazar_names[i]);
    }
    fprintf(file, "\n");
    for (int i = 0; i < total_products; i++) {
        fprintf(file, "%s", product_names[i]);
        for (int j = 0; j < total_bazars; j++) {
            fprintf(file, " %.2f", today_prices[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void display_bazars() {
    for (int i = 0; i < total_bazars; i++) {
        printf("%d. %s\n", i + 1, bazar_names[i]);
    }
}

void display_products() {
    for (int i = 0; i < total_products; i++) {
        printf("%d. %s\n", i + 1, product_names[i]);
    }
}

int get_choice(const char *prompt, int max) {
    int choice;
    do {
        printf("%s (1-%d): ", prompt, max);
        scanf("%d", &choice);
    } while (choice < 1 || choice > max);
    return choice - 1;
}

void submit_complaint(char *bazar, char *product) {
    char seller[50], phone[20], complaint[300];
    getchar();
    printf("Seller Name: ");
    fgets(seller, sizeof(seller), stdin);
    seller[strcspn(seller, "\n")] = '\0';
    printf("Phone: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = '\0';
    printf("Your Complaint: ");
    fgets(complaint, sizeof(complaint), stdin);
    complaint[strcspn(complaint, "\n")] = '\0';

    char date_str[50];
    time_t t = time(NULL);
    strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", localtime(&t));

    FILE *f = fopen("complaints.txt", "a");
    fprintf(f, "Date: %s\nBazar: %s\nProduct: %s\nSeller: %s\nPhone: %s\nComplaint: %s\n\n",
            date_str, bazar, product, seller, phone, complaint);
    fclose(f);
    printf("Complaint recorded. Thank you.\n");
}

void normal_user_flow() {
    print_border();
    printf("Available Bazars:\n");
    display_bazars();
    int b_choice = get_choice("Select Bazar", total_bazars);

    print_border();
    printf("Available Products:\n");
    display_products();
    int p_choice = get_choice("Select Product", total_products);

    float current = today_prices[p_choice][b_choice];
    float old = old_prices[p_choice][b_choice];

    print_border();
    printf("Product: %s\n", product_names[p_choice]);
    printf("Current Price at %s: %.2f BDT\n", bazar_names[b_choice], current);
    printf("Previous Price: %.2f BDT\n", old);

    if (old > 0) {
        float change = current - old;
        float percent = (change / old) * 100;
        if (change > 0)
            printf("Price increased by %.2f%% (%.2f BDT)\n", percent, change);
        else if (change < 0)
            printf("Price decreased by %.2f%% (%.2f BDT)\n", -percent, -change);
        else
            printf("No change in price.\n");
    } else {
        printf("No old data available.\n");
    }
    print_border();

    char c;
    printf("File a complaint? (y/n): ");
    scanf(" %c", &c);
    if (c == 'y' || c == 'Y') {
        submit_complaint(bazar_names[b_choice], product_names[p_choice]);
    }
}

int admin_login() {
    char user[20], pass[20];
    printf("Admin Username: "); scanf("%s", user);
    printf("Admin Password: "); scanf("%s", pass);
    return strcmp(user, ADMIN_USER) == 0 && strcmp(pass, ADMIN_PASS) == 0;
}

void add_bazar() {
    getchar();
    printf("Enter new bazar name: ");
    fgets(bazar_names[total_bazars], 50, stdin);
    bazar_names[total_bazars][strcspn(bazar_names[total_bazars], "\n")] = '\0';
    for (int i = 0; i < total_products; i++) {
        printf("Price of %s at %s: ", product_names[i], bazar_names[total_bazars]);
        scanf("%f", &today_prices[i][total_bazars]);
    }
    total_bazars++;
    save_market_data();
    printf("New bazar added.\n");
}

void add_product() {
    getchar();
    printf("Enter new product name: ");
    fgets(product_names[total_products], 50, stdin);
    product_names[total_products][strcspn(product_names[total_products], "\n")] = '\0';
    for (int i = 0; i < total_bazars; i++) {
        printf("Price at %s: ", bazar_names[i]);
        scanf("%f", &today_prices[total_products][i]);
    }
    total_products++;
    save_market_data();
    printf("New product added.\n");
}

void remove_bazar() {
    display_bazars();
    int index = get_choice("Select Bazar to remove", total_bazars);
    for (int i = index; i < total_bazars - 1; i++) {
        strcpy(bazar_names[i], bazar_names[i + 1]);
        for (int j = 0; j < total_products; j++) {
            today_prices[j][i] = today_prices[j][i + 1];
        }
    }
    total_bazars--;
    save_market_data();
    printf("Bazar removed successfully.\n");
}

void remove_product() {
    display_products();
    int index = get_choice("Select Product to remove", total_products);
    for (int i = index; i < total_products - 1; i++) {
        strcpy(product_names[i], product_names[i + 1]);
        for (int j = 0; j < total_bazars; j++) {
            today_prices[i][j] = today_prices[i + 1][j];
        }
    }
    total_products--;
    save_market_data();
    printf("Product removed successfully.\n");
}

void admin_panel() {
    int choice;
    do {
        print_border();
        printf("Admin Panel:\n1. Add Product\n2. Add Bazar\n3. View All\n4. Remove Product\n5. Remove Bazar\n6. Exit\nChoice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: add_product(); break;
            case 2: add_bazar(); break;
            case 3:
                print_border();
                for (int i = 0; i < total_products; i++) {
                    printf("%s: ", product_names[i]);
                    for (int j = 0; j < total_bazars; j++) {
                        printf("%.2f ", today_prices[i][j]);
                    }
                    printf("\n");
                }
                break;
            case 4: remove_product(); break;
            case 5: remove_bazar(); break;
            case 6: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 6);
}

int main() {
    load_market_data();
    load_old_market_data();

    char date_str[50];
    time_t t = time(NULL);
    strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", localtime(&t));

    print_border();
    printf("Smart Market Price Monitoring System\nDate & Time: %s\n", date_str);
    print_border();

    int user_type;
    printf("1. Normal User\n2. Admin\nChoose user type: ");
    scanf("%d", &user_type);

    if (user_type == 1) normal_user_flow();
    else if (user_type == 2) {
        if (admin_login()) admin_panel();
        else printf("Login failed.\n");
    } else {
        printf("Invalid choice.\n");
    }
    return 0;
}
