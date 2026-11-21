
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product
{
    int id;
    char name[50];
    float price;
    int quantity;
};

void addProduct();
void viewProducts();
void searchProduct();
void updateProduct();
void deleteProduct();
void sellProduct();
void lowStockAlert();
void totalInventoryValue();

int readInt(char message[])
{
    char input[100];
    int value;
    char extra;

    while(1)
    {
        printf("%s", message);
        fgets(input, sizeof(input), stdin);

        if(sscanf(input, "%d %c", &value, &extra) == 1)
            return value;

        printf("Invalid input! Enter a valid number.\n");
    }
}

float readFloat(char message[])
{
    char input[100];
    float value;
    char extra;

    while(1)
    {
        printf("%s", message);
        fgets(input, sizeof(input), stdin);

        if(sscanf(input, "%f %c", &value, &extra) == 1)
            return value;

        printf("Invalid input! Enter a valid amount.\n");
    }
}

void readName(char name[])
{
    printf("Enter product name: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0';
}

int productExists(int id)
{
    FILE *fp;
    struct Product p;

    fp = fopen("products.dat", "rb");

    if(fp == NULL)
        return 0;

    while(fread(&p, sizeof(p), 1, fp) == 1)
    {
        if(p.id == id)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

int main()
{
    int choice;

    do
    {
        printf("\n====================================\n");
        printf("             STOCKMASTER\n");
        printf("       INVENTORY MANAGEMENT\n");
        printf("====================================\n");

        printf("1. Add Product\n");
        printf("2. View All Products\n");
        printf("3. Search Product\n");
        printf("4. Update Product\n");
        printf("5. Delete Product\n");
        printf("6. Sell Product\n");
        printf("7. Low Stock Alert\n");
        printf("8. Total Inventory Value\n");
        printf("0. Exit\n");

        printf("====================================\n");

        choice = readInt("Enter your choice: ");

        switch(choice)
        {
            case 1:
                addProduct();
                break;

            case 2:
                viewProducts();
                break;

            case 3:
                searchProduct();
                break;

            case 4:
                updateProduct();
                break;

            case 5:
                deleteProduct();
                break;

            case 6:
                sellProduct();
                break;

            case 7:
                lowStockAlert();
                break;

            case 8:
                totalInventoryValue();
                break;

            case 0:
                printf("\nThank you for using StockMaster!\n");
                break;

            default:
                printf("\nInvalid choice! Try again.\n");
        }

    } while(choice != 0);

    return 0;
}

// 1. Add Product
void addProduct()
{
    FILE *fp;
    struct Product p;

    printf("\n--- ADD NEW PRODUCT ---\n");

    p.id = readInt("Enter product ID: ");

    if(p.id <= 0)
    {
        printf("Product ID must be positive.\n");
        return;
    }

    if(productExists(p.id))
    {
        printf("Product ID already exists!\n");
        return;
    }

    readName(p.name);

    if(strlen(p.name) == 0)
    {
        printf("Product name cannot be empty.\n");
        return;
    }

    p.price = readFloat("Enter product price: ");
    p.quantity = readInt("Enter product quantity: ");

    if(p.price < 0 || p.quantity < 0)
    {
        printf("Price and quantity cannot be negative.\n");
        return;
    }

    fp = fopen("products.dat", "ab");

    if(fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    fwrite(&p, sizeof(p), 1, fp);
    fclose(fp);

    printf("\nProduct added successfully!\n");
}

// 2. View All Products
void viewProducts()
{
    FILE *fp;
    struct Product p;
    int count = 0;

    fp = fopen("products.dat", "rb");

    if(fp == NULL)
    {
        printf("\nNo products available.\n");
        return;
    }

    printf("\n---------------- PRODUCT LIST ----------------\n");
    printf("%-10s %-20s %-12s %-10s\n",
           "ID", "Name", "Price", "Quantity");

    while(fread(&p, sizeof(p), 1, fp) == 1)
    {
        printf("%-10d %-20s Rs.%-9.2f %-10d\n",
               p.id, p.name, p.price, p.quantity);
        count++;
    }

    fclose(fp);

    if(count == 0)
        printf("No products found.\n");

    printf("----------------------------------------------\n");
}

// 3. Search Product
void searchProduct()
{
    FILE *fp;
    struct Product p;
    int id, found = 0;

    printf("\n--- SEARCH PRODUCT ---\n");

    id = readInt("Enter product ID: ");

    fp = fopen("products.dat", "rb");

    if(fp == NULL)
    {
        printf("No products available.\n");
        return;
    }

    while(fread(&p, sizeof(p), 1, fp) == 1)
    {
        if(p.id == id)
        {
            printf("\nProduct Found!\n");
            printf("ID       : %d\n", p.id);
            printf("Name     : %s\n", p.name);
            printf("Price    : Rs. %.2f\n", p.price);
            printf("Quantity : %d\n", p.quantity);

            found = 1;
            break;
        }
    }

    fclose(fp);

    if(!found)
        printf("Product not found!\n");
}

// 4. Update Product
void updateProduct()
{
    FILE *fp;
    struct Product p;
    int id, found = 0;
    long position;

    printf("\n--- UPDATE PRODUCT ---\n");

    id = readInt("Enter product ID: ");

    fp = fopen("products.dat", "rb+");

    if(fp == NULL)
    {
        printf("No products available.\n");
        return;
    }

    while(1)
    {
        position = ftell(fp);

        if(fread(&p, sizeof(p), 1, fp) != 1)
            break;

        if(p.id == id)
        {
            printf("Product: %s\n", p.name);

            p.price = readFloat("Enter new price: ");
            p.quantity = readInt("Enter new quantity: ");

            if(p.price < 0 || p.quantity < 0)
            {
                printf("Invalid price or quantity!\n");
                fclose(fp);
                return;
            }

            fseek(fp, position, SEEK_SET);
            fwrite(&p, sizeof(p), 1, fp);

            printf("Product updated successfully!\n");

            found = 1;
            break;
        }
    }

    fclose(fp);

    if(!found)
        printf("Product not found!\n");
}

// 5. Delete Product
void deleteProduct()
{
    FILE *fp, *temp;
    struct Product p;
    int id, found = 0;

    printf("\n--- DELETE PRODUCT ---\n");

    id = readInt("Enter product ID: ");

    fp = fopen("products.dat", "rb");

    if(fp == NULL)
    {
        printf("No products available.\n");
        return;
    }

    temp = fopen("temp.dat", "wb");

    if(temp == NULL)
    {
        printf("Unable to create temporary file!\n");
        fclose(fp);
        return;
    }

    while(fread(&p, sizeof(p), 1, fp) == 1)
    {
        if(p.id == id)
        {
            found = 1;
        }
        else
        {
            fwrite(&p, sizeof(p), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if(found)
    {
        remove("products.dat");

        if(rename("temp.dat", "products.dat") == 0)
            printf("Product deleted successfully!\n");
        else
            printf("Error updating product file!\n");
    }
    else
    {
        remove("temp.dat");
        printf("Product not found!\n");
    }
}

// 6. Sell Product
void sellProduct()
{
    FILE *fp;
    struct Product p;
    int id, quantity, found = 0;
    long position;
    float bill;

    printf("\n--- SELL PRODUCT ---\n");

    id = readInt("Enter product ID: ");

    fp = fopen("products.dat", "rb+");

    if(fp == NULL)
    {
        printf("No products available.\n");
        return;
    }

    while(1)
    {
        position = ftell(fp);

        if(fread(&p, sizeof(p), 1, fp) != 1)
            break;

        if(p.id == id)
        {
            found = 1;

            printf("Product Name: %s\n", p.name);
            printf("Available Quantity: %d\n", p.quantity);

            quantity = readInt("Enter quantity to sell: ");

            if(quantity <= 0)
            {
                printf("Enter a valid quantity!\n");
            }
            else if(quantity > p.quantity)
            {
                printf("Insufficient stock!\n");
            }
            else
            {
                bill = quantity * p.price;
                p.quantity = p.quantity - quantity;

                fseek(fp, position, SEEK_SET);
                fwrite(&p, sizeof(p), 1, fp);

                printf("\n========== SALES BILL ==========\n");
                printf("Product : %s\n", p.name);
                printf("Price   : Rs. %.2f\n", p.price);
                printf("Quantity: %d\n", quantity);
                printf("--------------------------------\n");
                printf("Total Bill: Rs. %.2f\n", bill);
                printf("================================\n");
                printf("Sale completed successfully!\n");
            }

            break;
        }
    }

    fclose(fp);

    if(!found)
        printf("Product not found!\n");
}

// 7. Low Stock Alert
void lowStockAlert()
{
    FILE *fp;
    struct Product p;
    int found = 0;

    fp = fopen("products.dat", "rb");

    if(fp == NULL)
    {
        printf("No products available.\n");
        return;
    }

    printf("\n--- LOW STOCK PRODUCTS ---\n");

    printf("%-10s %-20s %-10s\n",
           "ID", "Name", "Quantity");

    while(fread(&p, sizeof(p), 1, fp) == 1)
    {
        if(p.quantity <= 5)
        {
            printf("%-10d %-20s %-10d\n",
                   p.id, p.name, p.quantity);
            found = 1;
        }
    }

    if(!found)
        printf("All products have sufficient stock.\n");

    fclose(fp);
}

// 8. Total Inventory Value
void totalInventoryValue()
{
    FILE *fp;
    struct Product p;
    float total = 0;

    fp = fopen("products.dat", "rb");

    if(fp == NULL)
    {
        printf("No products available.\n");
        return;
    }

    while(fread(&p, sizeof(p), 1, fp) == 1)
    {
        total = total + (p.price * p.quantity);
    }

    fclose(fp);

    printf("\n--- INVENTORY VALUE ---\n");
    printf("Total Inventory Value: Rs. %.2f\n", total);
}
