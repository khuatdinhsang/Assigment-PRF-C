
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#define PRODUCT_FILE "E:\\products.txt"
#define ORDER_FILE "E:\\orders.txt"
#define SPLIT_CHARACTER "|"

/*-------------------------------------------*/
struct Product
{
    char id[7];
    char name[101];
    long long price;
};

struct OrderDetail
{
    char id[7];
    char name[101];
    int price;
    int quantity;
};
struct Order
{
    char id[15];
    char customerName[51];
    char customerPhone[12];
    char shippingAddress[201];
    int state;
    struct OrderDetail products[20];
    int pSize; // size of products
    int discount;
    int totalPrice;
};
struct StatisticProduct
{
    char productId[7];
    int quantity;
};
/*-------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void displayMenu()
{
    printf("=========================MENU==========================================");
    printf("\n1. Add new product.");
    printf("\n2. Update product.");
    printf("\n3. Delete product.");
    printf("\n4. Search product by name.");
    printf("\n5. List all products in alphabetical order from 'A' to 'Z'.");
    printf("\n6. Sort and display all products in ascending order of price.");
    printf("\n7. Order.");
    printf("\n8. List all orders in a given month.");
    printf("\n9. Show the detail of an order given its Id.");
    printf("\n10. Calculate number of products sold in month, group by each product.");
    printf("\n11. Find top 5 best-selling products in a specific period.");
    printf("\n12. Exit.");
    printf("\n=======================================================================\n");
}

void displayProduct(struct Product product)
{
    printf("|%10s|%20s|%10lld|\n", product.id, product.name, product.price);
}
void displayProducts(struct Product products[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        displayProduct(products[i]);
    }
}

void displayOrder(struct Order order)
{
    printf("OrderId: %s\n", order.id);
    printf("Customer Name: %s\n", order.customerName);
    printf("Shipping address: %s\n", order.shippingAddress);
    printf("Order state: %d\n", order.state);
    printf("List Products: \n");
    int i;
    printf("|%10s|%20s|%10s|%10s|\n", "id", "pr name", "price", "quantity");
    for (i = 0; i < order.pSize; i++)
    {
        printf("|%10s|%20s|%10lld|%10d|\n", order.products[i].id, order.products[i].name, order.products[i].price, order.products[i].quantity);
    }
    printf("Discount: %d\n", order.discount);
    printf("Total Price: %lld\n", order.totalPrice);
}
void displayOrders(struct Order orders[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        displayOrder(orders[i]);
        printf("\n----------------------------------------------------------------\n");
    }
}

void displayStatisticProduct(struct StatisticProduct statisticProduct)
{
    printf("|%10s|%10d|\n", statisticProduct.productId, statisticProduct.quantity);
}
void displayStatisticProducts(struct StatisticProduct statisticProducts[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        displayStatisticProduct(statisticProducts[i]);
    }
}

// Ham kiem tra xem s1 co chua s2 hay khong
int contains(char s1[], char s2[])
{
    char *sub;
    sub = strstr(s1, s2);
    if (sub != NULL)
    {
        return 1;
    }
    return 0;
}

// Ham swap 2 product
void swapProduct(struct Product *p1, struct Product *p2)
{
    struct Product temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

// Ham swap 2 Statistic Product
void swapStatisticProduct(struct StatisticProduct *p1, struct StatisticProduct *p2)
{
    struct StatisticProduct temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

// Ham xu ly doc ghi file
// Product
/*---------------------------------------------------------------------*/
int readListProducts(struct Product products[])
{ // tra ve size cua product
    FILE *fp = fopen(PRODUCT_FILE, "r");
    if (fp == NULL)
    {
        printf("File not found!");
        exit(1);
    }

    char line[200];

    int i = 0;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *token;
        token = strtok(line, SPLIT_CHARACTER);
        if (strlen(token) == 1)
            continue; // Bo qua dong thua
        strcpy(products[i].id, token);
        token = strtok(NULL, SPLIT_CHARACTER);
        strcpy(products[i].name, token);
        token = strtok(NULL, SPLIT_CHARACTER);
        products[i].price = atoi(token);
        i++;
    }
    fclose(fp);
    return i;
}
void addProduct(struct Product product)
{
    FILE *fp = fopen(PRODUCT_FILE, "a");
    fprintf(fp, "%s|%s|%lld\n", product.id, product.name, product.price);
    fclose(fp);
}
void writeListProducts(struct Product products[], int n)
{
    int i;
    FILE *fp = fopen(PRODUCT_FILE, "w");
    for (i = 0; i < n; i++)
    {
        fprintf(fp, "%s|%s|%lld\n", products[i].id, products[i].name, products[i].price);
    }
    fclose(fp);
}
/*---------------------------------------------------------------------*/

// Ham Xu ly doc ghi file
// Order
/*---------------------------------------------------------------------*/
int readListOrders(struct Order orders[])
{ // tra ve size cua orders
    FILE *fp = fopen(ORDER_FILE, "r");
    if (fp == NULL)
    {
        printf("File not found!");
        exit(1);
    }

    char line[200];

    int i = 0;
    int j = 0;
    int isNewOrder = 1;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *token;

        if (line[0] == ';')
        {
            i++;
            j = 0;
            isNewOrder = 1;
            continue;
        }
        if (isNewOrder)
        {
            token = strtok(line, SPLIT_CHARACTER);
            strcpy(orders[i].id, token);
            token = strtok(NULL, SPLIT_CHARACTER);
            strcpy(orders[i].customerName, token);
            token = strtok(NULL, SPLIT_CHARACTER);
            strcpy(orders[i].customerPhone, token);
            token = strtok(NULL, SPLIT_CHARACTER);
            strcpy(orders[i].shippingAddress, token);
            token = strtok(NULL, SPLIT_CHARACTER);
            orders[i].state = atoi(token);
            token = strtok(NULL, SPLIT_CHARACTER);
            orders[i].discount = atoi(token);
            token = strtok(NULL, SPLIT_CHARACTER);
            orders[i].totalPrice = atoi(token);
            isNewOrder = 0;
        }
        else
        {
            token = strtok(line, SPLIT_CHARACTER);
            if (strlen(token) == 1)
                continue; // Bo qua dong thua
            strcpy(orders[i].products[j].id, token);
            token = strtok(NULL, SPLIT_CHARACTER);
            strcpy(orders[i].products[j].name, token);
            token = strtok(NULL, SPLIT_CHARACTER);
            orders[i].products[j].price = atoi(token);
            token = strtok(NULL, SPLIT_CHARACTER);
            orders[i].products[j].quantity = atoi(token);
            j++;
            orders[i].pSize++;
        }
    }
    fclose(fp);
    return i;
}

void addOrder(struct Order order)
{
    FILE *fp = fopen(ORDER_FILE, "a");
    fprintf(fp, "%s|%s|%s|%s|%d|%d|%lld\n", order.id, order.customerName, order.customerPhone, order.shippingAddress, order.state, order.discount, order.totalPrice);
    int i;
    for (i = 0; i < order.pSize; i++)
    {
        fprintf(fp, "%s|%s|%lld|%d\n", order.products[i].id, order.products[i].name, order.products[i].price, order.products[i].quantity);
    }
    fprintf(fp, ";\n");
    fclose(fp);
}

// Ham xu ly du lieu dau vao
// Product
/*---------------------------------------------------------------------*/

void getPid(char prid[7])
{
    int checkid = 0;
    do
    {
        fflush(stdin);
        checkid = 0;
        gets(prid);
        if (strlen(prid) != 6)
        {
            checkid = 1;
        }
        int i = 0;
        for (i = 0; i < 6; i++)
        {
            if ((prid[i] < 'A' || prid[i] > 'Z') && i < 3)
            {
                checkid = 1;
                break;
            }
            if ((prid[i] < '0' || prid[i] > '9') && i > 2)
            {
                checkid = 1;
                break;
            }
        }
        if (checkid == 1)
        {
            printf("\n\n Wrong. Please try again !\n");
            getch();
            printf("Enter id again: ");
        }
    } while (checkid);
}

void getPname(char prname[100])
{
    int checkname;
    do
    {
        fflush(stdin);
        checkname = 0;
        gets(prname);
        if (strlen(prname) > 100)
            checkname = 1;
        if (prname[0] >= 'a' && prname[0] <= 'z')
            ;
        else if (prname[0] >= 'A' && prname[0] <= 'Z')
            ;
        else
        {
            checkname = 1;
        }
        int i;
        for (i = 1; i < strlen(prname); i++)
        {
            if (prname[i] >= 'a' && prname[i] <= 'z')
                ;
            else if (prname[i] >= 'A' && prname[i] <= 'Z')
                ;
            else if (prname[i] >= '0' && prname[i] <= '9')
                ;
            else if (prname[i] == ' ')
                ;
            else
            {
                checkname = 1;
            }
        }
        if (checkname == 1)
        {
            printf("\nWrong ! Please try again !\n");
            getch();
            printf("Enter name again: ");
        }
    } while (checkname);
}

void getPprice(long long *prprice)
{
    int checkprice;
    do
    {
        fflush(stdin);
        checkprice = 0;
        char c;
        scanf("%ld%c", prprice, &c);
        if (c != '\n')
        {
            printf("Wrong it not interger ! Please try again:\n");
            while (getchar() != '\n')
                ;
            checkprice = 1;
        }
        else if (*prprice < 0)
        {
            printf("Wrong it is negative");
            checkprice = 1;
        }
        if (checkprice == 1)
        {
            printf("\n\nWrong. Please try again !\n");
            getch();
            printf("Enter price again: ");
        }
    } while (checkprice);
}

int checkid(char id[7])
{
    struct Product products[100];
    int pSize = readListProducts(products);
    int i;
    for (i = 0; i < pSize; i++)
    {
        if (strcmp(id, products[i].id) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*/

// case 1:
/*---------------------------------------------------------------------*/
void addNewProduct()
{
    system("cls");
    printf("Add product function.\n");
    struct Product product;
    do
    {
        printf("Enter id: ");
        getPid(product.id);
        if (checkid(product.id))
        {
            printf("\nId existed . Please try again!\n");
        }
    } while (checkid(product.id));

    printf("Enter name: ");
    getPname(product.name);
    printf("Enter price: ");
    getPprice(&product.price);
    addProduct(product);
    printf("Add product successful!!!\n");
    printf("Enter to back menu !");
    getch();
}
/*---------------------------------------------------------------------*/

// case 2:
/*---------------------------------------------------------------------*/
void updateProduct()
{
    system("cls");
    printf("Update product function.\n");
    char id[7];
    printf("Enter id of product:");
    getPid(id);
    struct Product products[100];
    int pSize = readListProducts(products);

    int isExisted = 0;
    int i;
    for (i = 0; i < pSize; i++)
    {
        if (strcmp(products[i].id, id) == 0)
        {
            printf("Enter new id: ");
            getPid(products[i].id);
            printf("Enter new name: ");
            getPname(products[i].name);
            printf("Enter new price: ");
            getPprice(&products[i].price);
            isExisted = 1;
            break;
        }
    }

    if (!isExisted)
    {
        printf("Id not found!!!\n");
        getch();
    }
    else
    {
        writeListProducts(products, pSize);
        printf("Update product successful!!!\n");
        getch();
    }
}

/*---------------------------------------------------------------------*/

// case 3:
/*---------------------------------------------------------------------*/
void deleteProduct()
{
    printf("Delete product function.\n");
    struct Product products[100];
    int pSize = readListProducts(products);
    char id[7];
    printf("Enter id of product:");
    getPid(id);

    int isExisted = 0;
    int i, j;
    for (i = 0; i < pSize; i++)
    {
        if (strcmp(products[i].id, id) == 0)
        {
            for (j = i; j < pSize - 1; j++)
            {
                strcpy(products[j].id, products[j + 1].id);
                strcpy(products[j].name, products[j + 1].name);
                products[j].price = products[j + 1].price;
            }
            pSize--;
            isExisted = 1;
            break;
        }
    }

    if (!isExisted)
    {
        printf("Id not found!!!\n");
        getch();
    }
    else
    {
        writeListProducts(products, pSize);
        printf("Delete product successful!!!\n");
        printf("Enter a key to back menu");
        getch();
    }
}
/*---------------------------------------------------------------------*/

// case 4:
/*---------------------------------------------------------------------*/
void searchProductByName()
{
    system("cls");
    printf("Search product by name function.\n");
    struct Product products[100];
    int pSize = readListProducts(products);
    char keyword[100];
    printf("Enter keyword: ");
    fflush(stdin);
    gets(keyword);

    int i;
    printf("List Result:\n");
    int check = 0;
    for (i = 0; i < pSize; i++)
    {
        if (contains(products[i].name, keyword))
        { // neu chua keyword
            displayProduct(products[i]);
            check = 1;
        }
    }
    if (check == 0)
    {
        printf("Not found !\n");
        printf("Enter a key to back menu");
        getch();
    }
    printf("Enter a key to back menu");
    getch();
}
/*---------------------------------------------------------------------*/

// case 5:
/*---------------------------------------------------------------------*/
void sortByNameASC()
{
    system("cls");
    printf("Sort product by name ascending function.\n");
    struct Product products[100];
    int pSize = readListProducts(products);

    int i, j;
    for (i = 0; i < pSize - 1; i++)
    {
        for (j = i + 1; j < pSize; j++)
        {
            if (strcmp(products[i].name, products[j].name) > 0)
            {
                swapProduct(&products[i], &products[j]);
            }
        }
    }

    printf("List product sort order by name ascending: \n");
    printf("|%10s|%20s|%10s|\n", "ID", "PRODUCT NAME", "PRICE");
    printf("|------------------------------------------|\n");
    displayProducts(products, pSize);
    printf("Enter a key to back menu");
    getch();
}
/*---------------------------------------------------------------------*/

// case 6:
/*---------------------------------------------------------------------*/
void sortByPriceASC()
{
    printf("Sort product by price ascending function.\n");
    struct Product products[100];
    int pSize = readListProducts(products);

    int i, j;
    for (i = 0; i < pSize - 1; i++)
    {
        for (j = i + 1; j < pSize; j++)
        {
            if (products[i].price > products[j].price)
            {
                swapProduct(&products[i], &products[j]);
            }
        }
    }

    printf("List product sort order by price ascending: \n");
    printf("|%10s|%20s|%10s|\n", "ID", "PRODUCT NAME", "PRICE");
    printf("|------------------------------------------|\n");
    displayProducts(products, pSize);
    printf("Enter a key to back menu");
    getch();
}
/*---------------------------------------------------------------------*/

// case 7:
/*---------------------------------------------------------------------*/
void generateOrderId(char id[15])
{
    id[0] = '\0';
    time_t s, val = 1;
    struct tm *current_time;

    s = time(NULL);

    current_time = localtime(&s);
    int day = current_time->tm_mday;

    char str[5];
    if (day < 10)
    {
        strcat(id, "0");
    }
    itoa(day, str, 10);
    strcat(id, str);
    int month = current_time->tm_mon + 1;
    if (month < 10)
    {
        strcat(id, "0");
    }
    itoa(month, str, 10);
    strcat(id, str);

    int year = (current_time->tm_year + 1900);
    if (year < 10)
    {
        strcat(id, "0");
    }
    itoa(year, str, 10);
    strcat(id, str);

    int hours = current_time->tm_hour;
    if (hours < 10)
    {
        strcat(id, "0");
    }
    itoa(hours, str, 10);
    strcat(id, str);

    int minute = current_time->tm_min;
    if (minute < 10)
    {
        strcat(id, "0");
    }
    itoa(minute, str, 10);
    strcat(id, str);

    int second = current_time->tm_sec;
    if (second < 10)
    {
        strcat(id, "0");
    }
    itoa(second, str, 10);
    strcat(id, str);
}

void getOname(char Oname[51])
{
    int check_name;
    do
    {
        fflush(stdin);
        check_name = 0;
        gets(Oname);
        if (strlen(Oname) > 50)
            check_name = 1;
        int i;
        for (i = 0; i < strlen(Oname); i++)
        {
            if (Oname[i] >= 'a' && Oname[i] <= 'z')
            {
            }

            else if (Oname[i] >= 'A' && Oname[i] <= 'Z')
            {
            }
            else if (Oname[i] == ' ')
                ;
            else
            {
                check_name = 1;
            }
        }
        if (check_name == 1)
        {
            printf("Wrong ! Please try again.\n");
            printf("Enter one character to countinue !");
            getch();
            printf("Enter customer name: ");
        }
    } while (check_name);
}

void getOphone(char Ophone[12])
{
    int check_number;
    do
    {
        fflush(stdin);
        check_number = 0;
        gets(Ophone);
        if (strlen(Ophone) > 11 || strlen(Ophone) < 10)
            check_number = 1;
        int i;
        for (i = 0; i < strlen(Ophone); i++)
        {
            if (Ophone[i] >= '0' && Ophone[i] <= '9')
                ;
            else
                check_number = 1;
        }
        if (check_number == 1)
        {
            printf("Wrong ! Please try again.\n");
            printf("Enter one character to countinue !");
            getch();
            printf("Enter customer phone: ");
        }
    } while (check_number);
}

void getOaddr(char Oaddress[201])
{
    int check_address;
    do
    {
        fflush(stdin);
        check_address = 0;
        gets(Oaddress);
        if (strlen(Oaddress) > 200)
            check_address = 1;
        int i;
        for (i = 0; i < strlen(Oaddress); i++)
        {
            if (Oaddress[i] >= 'a' && Oaddress[i] <= 'z')
            {
            }

            else if (Oaddress[i] >= 'A' && Oaddress[i] <= 'Z')
            {
            }
            else if (Oaddress[i] == ' ')
                ;
            else if (Oaddress[i] >= '0' && Oaddress[i] <= '9')
                ;
            else
            {
                check_address = 1;
            }
        }
        if (check_address == 1)
        {
            printf("Wrong ! Please try again.\n");
            printf("Enter one character to countinue !");
            getch();
            printf("Enter shipping address: ");
        }
    } while (check_address);
}

void order()
{
    system("cls");
    struct Order order;
    struct Product products[100];
    int pSize = readListProducts(products);

    printf("List all product: \n");
    printf("|%10s|%20s|%10s|\n", "ID", "PRODUCT NAME", "PRICE");
    printf("|------------------------------------------|\n");
    displayProducts(products, pSize);

    int i, j;
    order.pSize = 0;
    while (1)
    {
        char productId[7];
        do
        {
            printf("Enter product id what you want: ");
            getPid(productId);
            if (checkid(productId) == 0)
            {
                printf("\n Id not existed please try again!\n");
            }
        } while (checkid(productId) == 0);
        int isExisted = 0;

        for (i = 0; i < pSize; i++)
        {
            if (strcmp(products[i].id, productId) == 0)
            {
                int index = -1;
                for (j = 0; j < order.pSize; j++)
                {
                    if (strcmp(order.products[j].id, productId) == 0)
                    {
                        index = j;
                        break;
                    }
                }
                if (index == -1)
                {
                    strcpy(order.products[order.pSize].id, products[i].id);
                    strcpy(order.products[order.pSize].name, products[i].name);
                    order.products[order.pSize].price = products[i].price;
                    order.products[order.pSize].quantity = 1;
                    order.pSize++;
                }
                else
                {
                    strcpy(order.products[index].id, products[i].id);
                    strcpy(order.products[index].name, products[i].name);
                    order.products[index].price = products[i].price;
                    order.products[index].quantity++;
                }
                isExisted = 1;
            }
        }

        printf("\nOrder Success !!\n");
        printf("Do you want to stop shopping(y/n(any key)):");
        char yn;
        fflush(stdin);
        scanf("%c", &yn);
        if (yn == 'y')
        {
            break;
        }
    }
    if (order.pSize > 0)
    {
        generateOrderId(order.id);
        printf("Enter customer name: ");
        getOname(order.customerName);
        printf("Enter customer phone: ");
        getOphone(order.customerPhone);
        printf("Enter shipping address: ");
        getOaddr(order.shippingAddress);
        order.state = 0;
        order.discount = 0;

        // calc total Price
        order.totalPrice = 0;
        for (i = 0; i < order.pSize; i++)
        {
            order.totalPrice += order.products[i].price * order.products[i].quantity - order.discount;
        }
        printf("\n---------------------------------------------------\n");
        displayOrder(order);
        addOrder(order);
        printf("Enter to continue!");
        getch();
    }
}
/*---------------------------------------------------------------------*/

// case 8:
/*---------------------------------------------------------------------*/
int getMonthInOrderId(char orderId[15])
{
    char monthStr[3];
    monthStr[0] = orderId[2];
    monthStr[1] = orderId[3];
    monthStr[2] = '\0';
    return atoi(monthStr);
}

void getOrderbyMonth()
{
    printf("List all orders in a given month function.\n");
    struct Order orders[100];
    int oSize = readListOrders(orders);

    int month;
    printf("Enter month:");
    scanf("%d%*c", &month);
    int i;
    printf("List Order in month %d: \n", month);
    printf("---------------------------------------------------\n");
    int check;
    for (i = 0; i < oSize; i++)
    {
        if (getMonthInOrderId(orders[i].id) == month)
        {
            check = 1;
            displayOrder(orders[i]);
            printf("\n---------------------------------------------------\n");
        }
    }
    if (check == 0)
    {
        printf("No product was bought.");
    }
    printf("Enter to continue!");
    getch();
    printf("\n");
}
/*---------------------------------------------------------------------*/

// case 9:
/*---------------------------------------------------------------------*/
void getOrderDetail()
{
    printf("Show the detail of an order given its Id function.\n");
    struct Order orders[100];
    int oSize = readListOrders(orders);
    char orderId[15];
    printf("Enter order id:");
    fflush(stdin);
    gets(orderId);
    int i;
    int isExisted = 0;
    for (i = 0; i < oSize; i++)
    {
        if (strcmp(orders[i].id, orderId) == 0)
        {
            displayOrder(orders[i]);
            isExisted = 1;
            break;
        }
    }

    if (!isExisted)
    {
        printf("Id not found!!!\n");
    }
    printf("Enter a key to back menu");
    getch();
}
/*---------------------------------------------------------------------*/

// case 10:
/*---------------------------------------------------------------------*/
int containsProductId(struct StatisticProduct statisticProducts[], char productId[7], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (strcmp(statisticProducts[i].productId, productId) == 0)
        {
            return i;
        }
    }
    return -1;
}

void statisticProducts()
{
    printf("Calculate number of products sold in month, group by each product function.\n");
    struct Order orders[100];
    int oSize = readListOrders(orders);
    struct StatisticProduct statisticProducts[100];
    int staSize = 0;
    int i, j;
    for (i = 0; i < oSize; i++)
    {
        for (j = 0; j < orders[i].pSize; j++)
        {
            int index;
            index = containsProductId(statisticProducts, orders[i].products[j].id, staSize);
            if (index == -1)
            {
                strcpy(statisticProducts[staSize].productId, orders[i].products[j].id);
                statisticProducts[staSize].quantity = orders[i].products[j].quantity;
                staSize++;
            }
            else
            {
                statisticProducts[index].quantity += orders[i].products[j].quantity;
            }
        }
    }

    printf("|%10s|%10s|\n", "PRODUCTID", "QUANTITY");
    printf("|---------------------|\n");
    displayStatisticProducts(statisticProducts, staSize);
    printf("Enter a key to back menu");
    getch();
}
/*---------------------------------------------------------------------*/

// case 11:
/*---------------------------------------------------------------------*/
void getTop5BestSeller()
{
    printf("Calculate number of products sold in month, group by each product function.\n");
    struct Order orders[100];
    int oSize = readListOrders(orders);
    struct StatisticProduct statisticProducts[100];
    int staSize = 0;
    int i, j;
    for (i = 0; i < oSize; i++)
    {
        for (j = 0; j < orders[i].pSize; j++)
        {
            int index;
            index = containsProductId(statisticProducts, orders[i].products[j].id, staSize);
            if (index == -1)
            {
                strcpy(statisticProducts[staSize].productId, orders[i].products[j].id);
                statisticProducts[staSize].quantity = orders[i].products[j].quantity;
                staSize++;
            }
            else
            {
                statisticProducts[index].quantity += orders[i].products[j].quantity;
            }
        }
    }

    // sort desc
    for (i = 0; i < staSize - 1; i++)
    {
        for (j = i + 1; j < staSize; j++)
        {
            if (statisticProducts[i].quantity < statisticProducts[j].quantity)
            {
                swapStatisticProduct(&statisticProducts[i], &statisticProducts[j]);
            }
        }
    }

    printf("|%10s|%10s|\n", "PRODUCTID", "QUANTITY");
    printf("|---------------------|\n");
    if (staSize > 5)
    {
        staSize = 5;
    }
    displayStatisticProducts(statisticProducts, staSize);
    printf("Enter a key to back menu");
    getch();
}
/*---------------------------------------------------------------------*/
int main()
{

    while (1)
    {
        system("cls");
        displayMenu();
        int option;
        printf("Enter your option: ");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            addNewProduct();
            break;
        case 2:
            updateProduct();
            break;
        case 3:
            deleteProduct();
            break;
        case 4:
            searchProductByName();
            break;
        case 5:
            sortByNameASC();
            break;
        case 6:
            sortByPriceASC();
            break;
        case 7:
            order();
            break;
        case 8:
            getOrderbyMonth();
            break;
        case 9:
            getOrderDetail();
            break;
        case 10:
            statisticProducts();
            break;
        case 11:
            getTop5BestSeller();
            break;
        case 12:
            exit(0);
        default:
            printf("Wrong Input !! PLease Re-enter The Correct Option");
            getch();
            fflush(stdin);
        }
    }
    return 0;
}
1
