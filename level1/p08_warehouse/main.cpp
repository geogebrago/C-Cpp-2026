#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <limits.h>
#include "../../safe_input.h"

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

static int find_item(const char *id);

static void get_path(char *path, const char *name)
{
    char source_path[MAX_PATH_LENGTH];
    DWORD length = GetFullPathNameA(__FILE__, MAX_PATH_LENGTH,
                                    source_path, NULL);

    if (length == 0 || length >= MAX_PATH_LENGTH)
    {
        path[0] = '\0';
        return;
    }

    char *last_slash = strrchr(source_path, '\\');
    char *last_forward_slash = strrchr(source_path, '/');

    if (last_slash == NULL ||
        (last_forward_slash != NULL && last_forward_slash > last_slash))
        last_slash = last_forward_slash;

    if (last_slash == NULL)
    {
        path[0] = '\0';
        return;
    }

    *(last_slash + 1) = '\0';

    int written = snprintf(path, MAX_PATH_LENGTH, "%s%s",
                           source_path, name);

    if (written < 0 || written >= MAX_PATH_LENGTH)
        path[0] = '\0';
}

static void load_items()
{
    char file_path[MAX_PATH_LENGTH];
    get_path(file_path, "goods.txt");

    if (file_path[0] == '\0')
    {
        printf("文件路径错误！\n");
        return;
    }

    FILE *file = fopen(file_path, "r");

    if (file == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }

    while (item_count < MAX_ITEM_TYPES - 1 &&
           fscanf(file, "%104s %lf %d",
                  input_buffer, &price_temp, &menu_choice) == 3)
    {
        if (price_temp <= 0 || menu_choice < 0)
            continue;

        if (find_item(input_buffer) != -1)
        {
            printf("发现重复商品型号 %s，已跳过重复记录！\n",
                   input_buffer);
            continue;
        }

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

    printf("请输入商品型号：");

    std::string input;
    if (!safe_input(input))
    {
        return;
    }
    strncpy(input_buffer, input.c_str(), MAX_ID_LENGTH - 1);
    input_buffer[MAX_ID_LENGTH - 1] = '\0';

    int position = find_item(input_buffer);
    long long temp_quantity;

    if (position == -1)
    {
        if (item_count >= MAX_ITEM_TYPES - 1)
        {
            printf("商品种类已达到上限！\n");
            return;
        }

        while (1)
        {
            printf("请输入商品数量：\n");

            if (!safe_input(temp_quantity))
            {
                return;
            }

            if (temp_quantity > 0 && temp_quantity <= INT_MAX)
                break;

            printf("商品数量必须大于0且不超过%d！\n", INT_MAX);
        }

        int new_quantity = (int)temp_quantity;
        double new_price;

        while (1)
        {
            printf("请输入商品价格：\n");

            if (!safe_input(new_price))
            {
                return;
            }

            if (new_price > 0)
                break;

            printf("商品价格必须大于0！\n");
        }

        ++item_count;
        strcpy(items[item_count].id, input_buffer);
        items[item_count].quantity = new_quantity;
        items[item_count].price = new_price;
    }
    else
    {
        printf("当前库存：%d\n", items[position].quantity);

        while (1)
        {
            printf("请输入商品入库数量：\n");

            if (!safe_input(temp_quantity))
            {
                return;
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

        std::string input;
        if (!safe_input(input))
        {
            return;
        }
        strncpy(input_buffer, input.c_str(), MAX_ID_LENGTH - 1);
        input_buffer[MAX_ID_LENGTH - 1] = '\0';

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

        if (!safe_input(temp_quantity))
        {
            return;
        }

        if (temp_quantity > 0 &&
            temp_quantity <= items[position].quantity)
            break;

        printf("出库数量必须大于0且不能超过当前库存！\n");
    }

    items[position].quantity -= (int)temp_quantity;

    if (items[position].quantity == 0)
    {
        for (int i = position; i < item_count; ++i)
            items[i] = items[i + 1];

        --item_count;
        printf("商品库存为0，已删除该商品！\n");
    }
    else
    {
        printf("已出库\n");
    }
}

static void save_items()
{
    char file_path[MAX_PATH_LENGTH];
    get_path(file_path, "goods.txt");

    if (file_path[0] == '\0')
    {
        printf("文件路径错误，数据保存失败！\n");
        return;
    }

    FILE *file = fopen(file_path, "w");

    if (file == NULL)
    {
        printf("文件打开失败，数据保存失败！\n");
        return;
    }

    for (int i = 1; i <= item_count; ++i)
    {
        int repeated = 0;

        for (int j = 1; j < i; ++j)
            if (strcmp(items[i].id, items[j].id) == 0)
            {
                repeated = 1;
                break;
            }

        if (repeated)
        {
            printf("发现重复商品型号 %s，已跳过重复存档！\n",
                   items[i].id);
            continue;
        }

        if (fprintf(file, "%s %.2f %d\n",
                    items[i].id,
                    items[i].price,
                    items[i].quantity) < 0)
        {
            printf("数据写入失败！\n");
            fclose(file);
            return;
        }
    }

    if (fclose(file) != 0)
    {
        printf("文件保存失败！\n");
        return;
    }

    printf("数据保存成功！\n");
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

        if (!safe_input(menu_choice))
        {
            save_items();
            return 0;
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