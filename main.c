#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_BOLD          "\x1b[1m"
#define ANSI_BLINK         "\x1b[5m"
#define ANSI_RESET         "\x1b[0m"

void setColor(int ForgC)
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}
COORD coord = {0, 0};
void gotoxy(int x, int y)
{
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
typedef struct product product;
typedef struct category category;
static int lastProductId = 1;
struct product
{
    int id;
    char name_product[50];
    float quantity;
    float price;
};
struct category
{
    char name_category[50];
    int num_of_products;
    product* product;
};

float getFloatInput()
{
    float value;
    while (1)
    {
        if (scanf("%f", &value) == 1)
        {
            if (value > 0)
            {
                break;
            }
            else
            {
                printf("Invalid input. Please enter a positive number: ");
            }
        }
        else
        {
            printf("Invalid input. Please enter a valid number: ");
            while (getchar() != '\n');
        }
    }
    return value;
}

category add_category(category *cat, int num_of_category)
{
    category new_cat;
    printf("Enter category name: ");
    scanf("%s", new_cat.name_category);

    for (int i = 0; i < num_of_category; i++)
    {
        if (strcmp(cat[i].name_category, new_cat.name_category) == 0)
        {
            printf("Category already exists. Enter a different category name: ");
            scanf("%s", new_cat.name_category);
            i = -1;
        }
    }
    printf("Do you want to add products to this category? (y/n): ");
    char addPro;
    scanf(" %c", &addPro);

    if (addPro == 'y' || addPro == 'Y')
    {
          do
            {
            printf("Enter the number of products for this category: ");
                new_cat.num_of_products = getFloatInput();

                if (  new_cat.num_of_products <= 0)
                {
                    printf("Invalid input. Price must be greater than 0.\n");
                }
            }
            while (  new_cat.num_of_products <= 0);

        new_cat.product = malloc(new_cat.num_of_products * sizeof(product));

        if (new_cat.product == NULL)
        {
            fprintf(stderr, "Memory allocation for products failed.\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < new_cat.num_of_products; i++)
        {
            new_cat.product[i].id = lastProductId++;
            printf("ID for product: %d\n", new_cat.product[i].id);

            printf("Enter name for product: ");
            scanf("%s", new_cat.product[i].name_product);

            do
            {
                printf("Enter price of product: ");
                new_cat.product[i].price = getFloatInput();

                if (new_cat.product[i].price <= 0)
                {
                    printf("Invalid input. Price must be greater than 0.\n");
                }
            }
            while (new_cat.product[i].price <= 0);

            do
            {
                printf("Enter quantity of product: ");
                new_cat.product[i].quantity = getFloatInput();

                if (new_cat.product[i].quantity < 0)
                {
                    printf("Invalid input. Quantity must be non-negative.\n");
                }
            }
            while (new_cat.product[i].quantity < 0);

            printf("Product added successfully.\n");
        }
    }
    else
    {
        new_cat.num_of_products = 0;
        new_cat.product = NULL;
    }

    return new_cat;
}
void add_product(category* cat, int num_of_category)
{
    char name_cat[50];
    int cat_index = -1;
    printf("Enter category name: ");
    scanf("%s", name_cat);

    for (int i = 0; i < num_of_category; i++)
    {
        if (strcmp(cat[i].name_category, name_cat) == 0)
        {
            cat_index = i;
            break;
        }
    }

    if (cat_index >= 0)
    {
        char product_name[50];
        printf("Enter name for product: ");
        scanf("%s", product_name);
        int pro_index = -1;
        for (int i = 0; i < cat[cat_index].num_of_products; i++)
        {
            if (strcmp(cat[cat_index].product[i].name_product, product_name) == 0)
            {
                pro_index = i;
                break;
            }
        }

        if (pro_index >= 0)
        {
            printf("Product with the same name already exists.\n");
            printf("Enter additional quantity: ");
            float additional_quantity = getFloatInput();
            if (additional_quantity > 0)
            {
                cat[cat_index].product[pro_index].quantity += additional_quantity;
                printf("Quantity extended successfully.\n");
            }
            else
            {
                printf("Invalid input. Additional quantity must be greater than 0.\n");
            }
        }
        else
        {
            cat[cat_index].num_of_products++;
            cat[cat_index].product = realloc(cat[cat_index].product, cat[cat_index].num_of_products * sizeof(product));
            pro_index = cat[cat_index].num_of_products - 1;

            cat[cat_index].product[pro_index].id = lastProductId++;
            printf(" ID for product: %d\n", cat[cat_index].product[pro_index].id);

            strcpy(cat[cat_index].product[pro_index].name_product, product_name);

            do
            {
                printf("Enter price of product: ");
                cat[cat_index].product[pro_index].price = getFloatInput();

                if (cat[cat_index].product[pro_index].price <= 0)
                {
                    printf("Invalid input. Price must be greater than 0.\n");
                }
            }
            while (cat[cat_index].product[pro_index].price <= 0);

            do
            {
                printf("Enter quantity of product: ");
                cat[cat_index].product[pro_index].quantity = getFloatInput();

                if (cat[cat_index].product[pro_index].quantity < 0)
                {
                    printf("Invalid input. Quantity must be non-negative.\n");
                }
            }
            while (cat[cat_index].product[pro_index].quantity < 0);

            printf("Product added successfully.\n");
        }
    }
    else
    {
        printf("Please enter a valid category before adding or extending a product.\n");
        system("pause");
    }
}

void move_product(category* cat, int num_of_category)
{
    char source_cat_name[50], dest_cat_name[50];
    int source_cat_index = -1, dest_cat_index = -1;
    char product_name[50];
    int product_index = -1;
    printf("Enter source category name: ");
    scanf("%s", source_cat_name);
    for (int i = 0; i < num_of_category; i++)
    {
        if (strcmp(cat[i].name_category, source_cat_name) == 0)
        {
            source_cat_index = i;
            break;
        }
    }
    if (source_cat_index == -1)
    {
        printf("Source category not found.\n");
        return;
    }
    printf("Enter product name to move: ");
    scanf("%s", product_name);
    for (int i = 0; i < cat[source_cat_index].num_of_products; i++)
    {
        if (strcmp(cat[source_cat_index].product[i].name_product, product_name) == 0)
        {
            product_index = i;
            break;
        }
    }
    if (product_index == -1)
    {
        printf("Product not found in the source category.\n");
        return;
    }
    printf("Enter destination category name: ");
    scanf("%s", dest_cat_name);
    for (int i = 0; i < num_of_category; i++)
    {
        if (strcmp(cat[i].name_category, dest_cat_name) == 0)
        {
            dest_cat_index = i;
            break;
        }
    }

    if (dest_cat_index == -1)
    {
        printf("Destination category not found.\n");
        return;
    }
    cat[dest_cat_index].num_of_products++;
    cat[dest_cat_index].product = realloc(cat[dest_cat_index].product,
                                          cat[dest_cat_index].num_of_products * sizeof(product));
    int last_dest_index = cat[dest_cat_index].num_of_products - 1;
    cat[dest_cat_index].product[last_dest_index] = cat[source_cat_index].product[product_index];

    cat[source_cat_index].num_of_products--;
    cat[source_cat_index].product = realloc(cat[source_cat_index].product,
                                            cat[source_cat_index].num_of_products * sizeof(product));

    printf("Product moved successfully.\n");
    system("pause");
}
void display_categories(category *cat, int num_of_category)
{
    for (int i = 0; i < num_of_category; i++)
    {
        printf("Category: %s                              Number of Products: %d\n", cat[i].name_category,cat[i].num_of_products);
        for (int j = 0; j < cat[i].num_of_products; j++)
        {
            printf("ID: %d           ||Name: %s      ||Price: %f        ||Quantity: %f",cat[i].product[j].id,cat[i].product[j].name_product,cat[i].product[j].price, cat[i].product[j].quantity);
            printf("\n");
        }
        printf("-----------------------------------------------------------------------------------\n");
    }
}
void print_receipt(float total_price)
{
    printf("\n===== Receipt =====\n");
    printf("Total Price: $%.2f\n", total_price);
    printf("===================\n\n");
}
void sell_product(category* cat, int num_of_category)
{
    char category_name[50], product_name[50], quantity_str[50];
    int category_index = -1, product_index = -1;
    float quantity_to_sell, total_price = 0.0;

    display_categories((category*)cat, num_of_category);
    do
    {
        category_index = -1;
        product_index = -1;

        printf("Enter category name: ");
        scanf("%s", category_name);

        for (int i = 0; i < num_of_category; i++)
        {
            if (strcmp(cat[i].name_category, category_name) == 0)
            {
                category_index = i;
                break;
            }
        }

        if (category_index == -1)
        {
            printf("Category not found\n");
            continue;
        }

        printf("Enter product name: ");
        scanf("%s", product_name);

        for (int i = 0; i < cat[category_index].num_of_products; i++)
        {
            if (strcmp(cat[category_index].product[i].name_product, product_name) == 0)
            {
                product_index = i;
                break;
            }
        }

        if (product_index == -1)
        {
            printf("Product not found in the category.\n");
            continue;
        }

        do
        {
            printf("Enter quantity to sell: ");
            scanf("%s", quantity_str);

            // Convert the input string to a float using sscanf
            if (sscanf(quantity_str, "%f", &quantity_to_sell) != 1 || quantity_to_sell <= 0)
            {
                printf("Invalid quantity input. Please enter a positive number.\n");
            }
            else
            {
                break;
            }
        }
        while (1);
        float price = cat[category_index].product[product_index].price;
        total_price += quantity_to_sell * price;
        if (quantity_to_sell > cat[category_index].product[product_index].quantity)
        {
            printf("Not enough quantity available for sale.\n");
        }
        else
        {
            cat[category_index].product[product_index].quantity -= quantity_to_sell;
            printf("Sale completed successfully.\n");
        }

        printf("Do you want to sell more products? (y/n): ");
        char choice;
        scanf(" %c", &choice);

        if (choice != 'y' && choice != 'Y')
        {
            print_receipt(total_price);
            system("pause");

            break;

        }

    }
    while (1);
}
void delete_product(category* cat, int num_of_category)
{
    char category_name[50], product_name[50];
    int category_index = -1, product_index = -1;

    printf("Enter category name: ");
    scanf("%s", category_name);

    for (int i = 0; i < num_of_category; i++)
    {
        if (strcmp(cat[i].name_category, category_name) == 0)
        {
            category_index = i;
            break;
        }
    }

    if (category_index == -1)
    {
        printf("Category not found.\n");
        return;
    }

    printf("Enter product name to delete: ");
    scanf("%s", product_name);

    for (int i = 0; i < cat[category_index].num_of_products; i++)
    {
        if (strcmp(cat[category_index].product[i].name_product, product_name) == 0)
        {
            product_index = i;
            break;
        }
    }

    if (product_index == -1)
    {
        printf("Product not found in the category.\n");
        return;
    }

    for (int i = product_index; i < cat[category_index].num_of_products - 1; i++)
    {
        cat[category_index].product[i] = cat[category_index].product[i + 1];
    }

    cat[category_index].num_of_products--;

    cat[category_index].product = realloc(cat[category_index].product,
                                          cat[category_index].num_of_products * sizeof(product));

    printf("Product deleted successfully.\n");
}


int main()
{


    category* cat = NULL;
    product *product=NULL;
    char menu[7][15] = {"Add Category","Add Product","delete_product","Display All","move","sell product","exit"};
    int index = 0;
    int num_of_category = 0;
            system("color 3A");

    while (true)
    {

        system("cls");

        for (int i = 0; i < 7; i++)
        {
            gotoxy(90, i + 2);
            if (i == index)
            {
                setColor(12);


            }
            else
            {
                setColor(15);

            }
            printf(" %i )  %s \n", i + 1, menu[i]);
        }

        char current_key = getch();
        if(current_key==-32)
        {
            current_key = getch();

            if (current_key == 80)//down
            {
                index = (index + 1) % 7;
            }
            else if (current_key == 72) //up
            {
                index = (index - 1 + 7) % 7;
            }
            else if (current_key == 71)//home
            {
                index = 0;

            }
            else if (current_key == 79)//end
            {
                index = 2;
            }
        }
        else if (current_key == 27)//esc
        {
            exit(0);
        }
        else if (current_key == 13) //enter
        {

            switch (index)
            {
            case 0:
            {
                cat = realloc(cat, (num_of_category + 1) * sizeof(category));
                cat[num_of_category] = add_category(cat, num_of_category);
                num_of_category++;
                break;
            }

            case 1:
            {
                add_product(cat, num_of_category);
                break;
            }
            case 2:
            {
                delete_product(cat, num_of_category);
                break;
                system("pause");

            }

            case 3:
            {
                printf("\n Displaying all categories and products:\n");
                display_categories((category*)cat, num_of_category);
                system("pause");
                break;
            }
            case 4:
            {
                move_product(cat, num_of_category);
                break;
            }
            case 5:
            {
                sell_product(cat, num_of_category);
                break;
            }
            case 6:
            {
                printf("Press any key to exit\n");
                exit(0);
            }


            system("pause");
            }
        }
    }
    return 0;
}
