#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./ezdib/ezdib.h"

#define MAX_LENGTH 32
#define TRUE 1
#define FALSE 0

void draw_hasse(char (*)[], char *);
void save_name(char (*)[], char (*)[]);
void find_relation(int (*)[], char (*)[], char (*)[]);
void delete_transitive(int (*)[], int, int, int);
int find_minimal(int (*)[], int, int[]);
void plus_height(int (*)[], int[]);
void edit_height(int (*)[]);
int find_width(int (*)[]);
int find_height(int (*)[]);
int find_order(int[], int, int);

int input_num = 0;   // number of relation for loop
int element_num = 0; // number of element for loop
int error = 0;
int minimal_num = 0;

int main(int argc, char *argv[])
{
    char *file = argv[1];
    FILE *fp;
    char buffer[MAX_LENGTH]; // to save input in array

    char input[MAX_LENGTH][MAX_LENGTH];

    fp = fopen(file, "r");
    while (fgets(buffer, MAX_LENGTH, fp))
    {
        strcpy(input[input_num], buffer);
        input_num++;
    }
    draw_hasse(input, argv[1]);

    return 0;
}

void draw_hasse(char (*input)[MAX_LENGTH], char *file)
{
    char ver[MAX_LENGTH][MAX_LENGTH];
    int relation[MAX_LENGTH][MAX_LENGTH] = {0};
    save_name(input, ver);
    find_relation(relation, input, ver);
    for (int i = 0; i < element_num; i++)
    {
        error = 0;
        delete_transitive(relation, i, i, 0);
    }

    edit_height(relation);
    int height = 0;
    int width = 0;

    height = find_height(relation);
    int width_for_height[MAX_LENGTH] = {0};
    for (int i = 0; i < element_num; i++)
        for (int j = 0; j < element_num; j++)
            if (relation[i][j] > 0)
            {
                width_for_height[relation[i][j]]++;
                break;
            }

    int element_height[MAX_LENGTH] = {0};
    for (int i = 0; i < element_num; i++)
        for (int j = 0; j < element_num; j++)
            if (relation[i][j] > 0)
            {
                element_height[i] = relation[i][j];
                break;
            }

    for (int i = 0; i < element_num; i++)
    {
        for (int j = 0; j < element_num; j++)
        {
            printf("%d ", relation[i][j]);
        }
        printf("\n");
    }

    char ver_order[MAX_LENGTH][MAX_LENGTH];
    int ver_order_num = 0;
    for (int i = 1; i <= height; i++)
    {
        for (int j = 0; j < element_num; j++)
        {
            if (i == element_height[j])
            {
                strcpy(ver_order[ver_order_num], ver[j]);
                ver_order_num++;
            }
        }
    }
    int x;
    HEZDIMAGE hDib;
    HEZDFONT hFont;

    hDib = ezd_create(760, 760, 32, 0);
    ezd_fill(hDib, 0x404040);
    hFont = ezd_load_font(EZD_FONT_TYPE_MEDIUM, 0, 0);
    int name = 0;
    for (int i = 1; i <= height; i++)
    {
        for (int j = 1; j <= width_for_height[i]; j++)
        {
            for (x = 0; x < 10; x++)
                if(height > 1)
                    ezd_circle(hDib, 760 / (width_for_height[i] + 1) * j, 60 + 640 * (i - 1) / (height - 1), x, x * 5);
                else
                    ezd_circle(hDib, 760 / (width_for_height[i] + 1) * j, 60 + 640 * (i - 1) / (height), x, x * 5);
            if (hFont)
            {
                if(height > 1)
                    ezd_text(hDib, hFont, ver_order[name], -1, 30 + 760 / (width_for_height[i] + 1) * j, 60 + 640 * (i - 1) / (height - 1), 0xffffff);
                else
                    ezd_text(hDib, hFont, ver_order[name], -1, 30 + 760 / (width_for_height[i] + 1) * j, 60 + 640 * (i - 1) / (height), 0xffffff);
                name++;
            }
        }
    }
    int start_order, end_order;
    for (int i = 0; i < element_num; i++)
    {
        start_order = find_order(element_height, i, element_height[i]);
        for (int j = 0; j < element_num; j++)
        {
            if (i == j)
                continue;
            if (relation[i][j] > 0)
            {
                end_order = find_order(element_height, j, element_height[j]);
                ezd_line(hDib,
                         760 / (width_for_height[element_height[i]] + 1) * start_order, 60 + 640 * (element_height[i] - 1) / (height - 1),
                         760 / (width_for_height[element_height[j]] + 1) * end_order, 60 + 640 * (element_height[j] - 1) / (height - 1),
                         0xffffff);
                continue;
            }
        }
    }
    strcat(file, ".bmp");
    ezd_save(hDib, file);

    /// Releases the specified font
    if (hFont)
        ezd_destroy_font(hFont);

    // Free resources
    if (hDib)
        ezd_destroy(hDib);
}

void save_name(char (*input)[MAX_LENGTH], char (*ver)[MAX_LENGTH])
{
    for (int i = 0; i < input_num; i++)
    {
        char lhs[MAX_LENGTH];
        strcpy(lhs, input[i]);
        char *rhs = strstr(lhs, "_") + 1;
        strtok(lhs, "_");
        rhs = strtok(rhs, "\n");
        for (int j = 0; j <= element_num; j++)
        {
            if (strcmp(lhs, ver[j]) == 0)
            {
                break;
            }
            if (j == element_num)
            {
                strcpy(ver[j], lhs);
                element_num++;
                break;
            }
        }

        for (int k = 0; k <= element_num; k++)
        {
            if (strcmp(rhs, ver[k]) == 0)
            {
                break;
            }
            if (k == element_num)
            {
                strcpy(ver[k], rhs);
                element_num++;
                break;
            }
        }
    }
}

void find_relation(int (*relation)[MAX_LENGTH], char (*input)[MAX_LENGTH], char (*ver)[MAX_LENGTH])
{
    for (int i = 0; i < input_num; i++)
    {
        char *lhs = input[i];
        char *rhs = strstr(lhs, "_") + 1;
        lhs = strtok(lhs, "_");
        rhs = strtok(rhs, "\n");
        int lhs_index = 0;
        int rhs_index = 0;
        for (int j = 0; j < element_num; j++)
        {
            if (strcmp(lhs, ver[j]) == 0)
                break;
            lhs_index++;
        }
        for (int k = 0; k < element_num; k++)
        {
            if (strcmp(rhs, ver[k]) == 0)
                break;
            rhs_index++;
        }

        if (relation[rhs_index][lhs_index] == 1)
        {
            printf("Invalid input. This is not a partial order relation.");
            exit(0);
        }
        else
            relation[lhs_index][rhs_index] = 1;
    }

    for (int i = 0; i < element_num; i++)
        relation[i][i] = 1; // reflexive closure
}

void delete_transitive(int (*relation)[MAX_LENGTH], int start, int new, int check)
{
    error++;
    if (error > element_num * (element_num + 1) / 2)
    {
        printf("Invalid input. This is not a partial order relation.");
        exit(0);
    }

    for (int i = 0; i < element_num; i++)
    {
        if (i == new)
            continue;
        if (relation[new][i] == 1)
        {
            if (check == 1)
                relation[start][i] = 0;
            delete_transitive(relation, start, i, 1);
        }
    }
}

void edit_height(int (*relation)[MAX_LENGTH])
{
    int minimal[MAX_LENGTH];
    int temp[MAX_LENGTH];
    int temp_num = 0;
    while (element_num != minimal_num)
    {
        for (int i = 0; i < element_num; i++)
        {
            if (find_minimal(relation, i, minimal))
            {
                temp[temp_num] = i;
                temp_num++;
            }
        }
        for (int j = minimal_num; j < temp_num; j++)
        {
            minimal[minimal_num] = temp[j];
            minimal_num++;
        }

        plus_height(relation, minimal);
    }
}

int find_minimal(int (*relation)[MAX_LENGTH], int target, int minimal[MAX_LENGTH])
{
    int i = -1;
LOOP:
    while (i != element_num)
    {
        i++;
        if (i == target)
            continue;
        for (int j = 0; j < minimal_num; j++)
            if (i == minimal[j])
                goto LOOP;

        if (relation[i][target] > 0)
            return FALSE;
    }
    for (int m = 0; m < minimal_num; m++)
        if (target == minimal[m])
            return FALSE;
    return TRUE;
}

void plus_height(int (*relation)[MAX_LENGTH], int minimal[MAX_LENGTH])
{
    int i = -1;
LOOP2:
    while (i != element_num)
    {
        i++;
        for (int j = 0; j < minimal_num; j++)
            if (i == minimal[j])
                goto LOOP2;
        for (int k = 0; k < element_num; k++)
            if (relation[i][k] > 0)
                relation[i][k]++;
    }
}

int find_height(int (*relation)[MAX_LENGTH])
{
    int max = relation[0][0];
    for (int i = 0; i < element_num; i++)
        for (int j = 0; j < element_num; j++)
            if (max < relation[i][j])
                max = relation[i][j];
    return max;
}

int find_width(int (*relation)[MAX_LENGTH])
{
    int width_for_height[MAX_LENGTH] = {0};
    for (int i = 0; i < element_num; i++)
        for (int j = 0; j < element_num; j++)
            if (relation[i][j] > 0)
            {
                width_for_height[relation[i][j]]++;
                break;
            }
    int max = width_for_height[0];
    for (int i = 0; i < element_num; i++)
        if (max < width_for_height[i])
            max = width_for_height[i];
    return max;
}

int find_order(int element_height[MAX_LENGTH], int index, int height)
{
    int order = 1;
    for (int i = 0; i < index; i++)
    {
        if (height == element_height[i])
            order++;
    }
    return order;
}