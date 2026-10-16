#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <limits.h>
#define MAX_ITEM_TYPES 105
#define MAX_ID_LENGTH 105
#define MAX_PATH_LENGTH 260
static int menu_choice, item_count;
static double price_temp;
static char input_buffer[MAX_ID_LENGTH];
typedef struct
{
    char id[MAX_ID_LENGTH];
    int quantity;
    double price;
} Item;
static Item items[MAX_ITEM_TYPES];
static void get_path(char *path, const char *name)
{
    DWORD length = GetModuleFileNameA(NULL, path, MAX_PATH_LENGTH);
    if (length == 0 || length >= MAX_PATH_LENGTH)
    {
        path[0] = '\0';
        return;
    }
    char *last_slash = strrchr(path, '\\');
    if (last_slash != NULL)
        *(last_slash + 1) = '\0';
    sprintf(path + strlen(path), "%s", name);
}

static void clear_input()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

static void load_items()
{
    char file_path[MAX_PATH_LENGTH];
    get_path(file_path, "goods.txt");
    FILE *file = fopen(file_path, "r");

    if (file == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }

    while (item_count < MAX_ITEM_TYPES - 1 &&
           fscanf(file, "%103s %lf %d", input_buffer, &price_temp, &menu_choice) == 3)
    {
        if (price_temp <= 0 || menu_choice < 0)
            continue;

        ++item_count;
        strcpy(items[item_count].id, input_buffer);
        items[item_count].quantity = menu_choice;
        items[item_count].price = price_temp;
    }

    fclose(file);
}

static int find_item(const char *id)
{
    for (int i = 1; i <= item_count; ++i)
        if (strcmp(id, items[i].id) == 0)
            return i;

    return -1;
}

static void print_all_items()
{
    for (int i = 1; i <= item_count; ++i)
        printf("%-15s %-15.2f %-15d\n",
               items[i].id, items[i].price, items[i].quantity);
}

static void show_inventory()
{
    printf("\n========= 存货列表 ===========\n");

    if (item_count == 0)
    {
        printf("当前没有商品\n");
        return;
    }

    printf("%-15s %-15s %-15s\n", "型号", "单价", "数量");
    printf("---------------------------------------------\n");

    print_all_items();
}

static void add_stock()
{
    printf("\n========== 入库 ==========\n");

    if (item_count >= MAX_ITEM_TYPES - 1)
    {
        printf("商品种类已达到上限！\n");
        return;
    }

    printf("请输入商品型号：");

    if (scanf("%103s", input_buffer) != 1)
    {
        if (feof(stdin))
            return;

        printf("输入错误！\n");
        clear_input();
        return;
    }

    int position = find_item(input_buffer);

    if (position == -1)
    {
        ++item_count;
        strcpy(items[item_count].id, input_buffer);

        long long temp_quantity;
        while (1)
        {
            printf("请输入商品数量：\n");

            if (scanf("%lld", &temp_quantity) != 1)
            {
                if (feof(stdin))
                    return;

                printf("请输入整数！\n");
                clear_input();
                continue;
            }

            if (temp_quantity > 0 && temp_quantity <= INT_MAX)
                break;

            printf("商品数量必须大于0且不超过%d！\n", INT_MAX);
        }
        items[item_count].quantity = (int)temp_quantity;

        while (1)
        {
            printf("请输入商品价格：\n");

            if (scanf("%lf", &items[item_count].price) != 1)
            {
                if (feof(stdin))
                    return;

                printf("请输入数字！\n");
                clear_input();
                continue;
            }

            if (items[item_count].price > 0)
                break;

            printf("商品价格必须大于0！\n");
        }
    }
    else
    {
        printf("当前库存：%d\n", items[position].quantity);

        long long temp_quantity;
        while (1)
        {
            printf("请输入商品入库数量：\n");

            if (scanf("%lld", &temp_quantity) != 1)
            {
                if (feof(stdin))
                    return;

                printf("请输入整数！\n");
                clear_input();
                continue;
            }

            if (temp_quantity > 0 && temp_quantity <= INT_MAX)
                break;

            printf("入库数量必须大于0且不超过%d！\n", INT_MAX);
        }

        if (items[position].quantity > INT_MAX - temp_quantity)
        {
            printf("库存数量过大！\n");
            return;
        }

        items[position].quantity += (int)temp_quantity;
    }

    printf("已入库\n");
}

static void remove_stock()
{
    printf("\n========== 出库 ==========\n");

    int position = -1;

    while (1)
    {
        printf("请输入商品型号：");

        if (scanf("%103s", input_buffer) != 1)
        {
            if (feof(stdin))
                return;

            printf("输入错误！\n");
            clear_input();
            continue;
        }

        position = find_item(input_buffer);

        if (position != -1)
            break;

        printf("不存在该商品！\n");
    }

    printf("当前库存：%d\n", items[position].quantity);

    long long temp_quantity;
    while (1)
    {
        printf("请输入商品出库数量：\n");

        if (scanf("%lld", &temp_quantity) != 1)
        {
            if (feof(stdin))
                return;

            printf("请输入整数！\n");
            clear_input();
            continue;
        }

        if (temp_quantity > 0 && temp_quantity <= items[position].quantity)
            break;

        printf("出库数量必须大于0且不能超过当前库存！\n");
    }

    items[position].quantity -= (int)temp_quantity;

    printf("已出库\n");
}

static void save_items()
{
    char file_path[MAX_PATH_LENGTH];
    get_path(file_path, "goods.txt");
    FILE *file = fopen(file_path, "w");

    if (file == NULL)
    {
        printf("文件打开失败，数据保存失败！\n");
        return;
    }

    for (int i = 1; i <= item_count; ++i)
        fprintf(file, "%s %.2f %d\n",
                items[i].id, items[i].price, items[i].quantity);

    fclose(file);
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    load_items();

    while (1)
    {
        printf("\n====================\n");
        printf("     简单进销存\n");
        printf("====================\n");
        printf("1. 显示存货列表\n");
        printf("2. 入库\n");
        printf("3. 出库\n");
        printf("4. 退出程序\n");
        printf("请选择：\n");

        if (scanf("%d", &menu_choice) != 1)
        {
            if (feof(stdin))
            {
                save_items();
                return 0;
            }

            printf("请输入1~4的数字！\n");
            clear_input();
            continue;
        }

        if (menu_choice == 1)
        {
            show_inventory();
        }
        else if (menu_choice == 2)
        {
            add_stock();
        }
        else if (menu_choice == 3)
        {
            remove_stock();
        }
        else if (menu_choice == 4)
        {
            save_items();
            printf("程序已退出！\n");
            return 0;
        }
        else
        {
            printf("请输入1~4！\n");
        }
    }

    return 0;
}
