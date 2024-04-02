#include <stdio.h>
#include <string.h>

int main()
{
    FILE *fp = fopen("formula", "w");

    int i, j, m, n, r, s;

    for (i = 1; i <= 9; i++)
        for (j = 1; j <= 9; j++)
            for (n = 1; n <= 9; n++)
                fprintf(fp, "(declare-const p%d%d%d Bool)\n", i, j, n);

    // Q1 : Each cell must have exactly one number.
    fprintf(fp, "; Q1\n");
    fprintf(fp, "(assert (and ");
    for (i = 1; i <= 9; i++)
    {
        fprintf(fp, "(and ");
        for (j = 1; j <= 9; j++)
        {
            fprintf(fp, "(and ");
            for (n = 1; n <= 8; n++)
            {
                fprintf(fp, "(and ");
                for (m = n + 1; m <= 9; m++)
                {
                    fprintf(fp, "(not (and p%d%d%d p%d%d%d))", i, j, n, i, j, m);
                }
                fprintf(fp, ")");
            }
            fprintf(fp, ")");
        }
        fprintf(fp, ")");
    }
    fprintf(fp, "))\n");

    // Q2 : Each row must have numbers from 1 to 9.
    fprintf(fp, "; Q2\n");
    fprintf(fp, "(assert (and ");
    for (i = 1; i <= 9; i++)
    {
        fprintf(fp, "(and ");
        for (n = 1; n <= 9; n++)
        {
            fprintf(fp, "(or ");
            for (j = 1; j <= 9; j++)
            {
                fprintf(fp, "p%d%d%d ", i, j, n);
            }
            fprintf(fp, ")");
        }
        fprintf(fp, ")");
    }
    fprintf(fp, "))\n");

    // Q3 : Each column must have numbers from 1 to 9.
    fprintf(fp, "; Q3\n");
    fprintf(fp, "(assert (and ");
    for (j = 1; j <= 9; j++)
    {
        fprintf(fp, "(and ");
        for (n = 1; n <= 9; n++)
        {
            fprintf(fp, "(or ");
            for (i = 1; i <= 9; i++)
            {
                fprintf(fp, "p%d%d%d ", i, j, n);
            }
            fprintf(fp, ")");
        }
        fprintf(fp, ")");
    }
    fprintf(fp, "))\n");

    // Q4 : Each subgrid must have numbers from 1 to 9.
    fprintf(fp, "; Q4\n");
    fprintf(fp, "(assert (and ");
    for (r = 0; r <= 2; r++)
    {
        fprintf(fp, "(and ");
        for (s = 0; s <= 2; s++)
        {
            fprintf(fp, "(and ");
            for (n = 1; n <= 9; n++)
            {
                fprintf(fp, "(or ");
                for (i = 1; i <= 3; i++)
                {
                    fprintf(fp, "(or ");
                    for (j = 1; j <= 3; j++)
                    {
                        fprintf(fp, "p%d%d%d ", 3 * r + i, 3 * s + j, n);
                    }
                    fprintf(fp, ")");
                }
                fprintf(fp, ")");
            }
            fprintf(fp, ")");
        }
        fprintf(fp, ")");
    }
    fprintf(fp, "))\n");

    char ch;
    char fileName[128];
    int sudoku[9][9];

    printf("Would you like to enter the puzzle yourself? (y/n) > ");
    scanf("%c", &ch);

    if (ch == 'y' || 'Y')
    {
        char num, buffer;
        printf("? ? ? ? ? 5 ? 6 ?\n");
        printf("? ? ? 4 2 ? ? ? 1\n");
        printf("? 7 ? ? 6 ? ? 3 ?\n");
        printf("? ? ? ? 3 ? ? ? 2\n");
        printf("? 2 ? ? ? ? ? 9 ?\n");
        printf("6 ? ? ? 7 ? ? ? ?\n");
        printf("? 4 ? ? 1 ? ? 8 ?\n");
        printf("8 ? ? ? 4 7 ? ? ?\n");
        printf("? 1 ? 9 ? ? ? ? ?\n");
        printf("Please enter the Sudoku puzzle in the same format as above!\n");
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                scanf("%c", &num);
                scanf("%c", &buffer);
                if (num == '?')
                    sudoku[i][j] = 0;
                else
                    sudoku[i][j] = (int)num - '0';
            }
        }
    }
    else if (ch == 'n' || 'N')
    {
        printf("Pleanse enter the name of the file!\n");
        scanf("%s", fileName);

        FILE *fp = fopen(fileName, "r");

        char num, buffer;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                fscanf(fp, "%c", &num);
                fscanf(fp, "%c", &buffer);
                if (num == '?')
                    sudoku[i][j] = 0;
                else
                    sudoku[i][j] = (int)num - '0';
            }
        }
    }
    else
    {
        return 0;
    }

    printf("This is the puzzle entered.\n");
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            (sudoku[i][j] != 0)
                ? printf("%d ", sudoku[i][j])
                : printf("? ");
            if (j == 2 || j == 5)
            {
                printf("| ");
            }
        }
        if (i == 2 || i == 5)
        {
            printf("\n----------------------");
        }
        printf("\n");
    }

    // Q5 : pre-assigned condition.
    fprintf(fp, "; Q5\n");
    fprintf(fp, "(assert (and ");

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (sudoku[i][j] != 0)
                fprintf(fp, "p%d%d%d ", i + 1, j + 1, sudoku[i][j]);
        }
    }
    fprintf(fp, "))\n");

    fprintf(fp, "(check-sat)\n(get-model)\n");

    fclose(fp);

    FILE *fin = popen("z3 formula", "r");
    char buf[128];
    char buf1[128];
    char buf2[128];
    fscanf(fin, "%s %s", buf, buf);
    int count = 0;

    int arr[9][9];
    while (!feof(fin))
    {
        fscanf(fin, "%s", buf);  //printf("%s ", buf) ;
        fscanf(fin, "%s", buf1); //printf("%s ", buf1) ;
        fscanf(fin, "%s", buf);  //printf("%s ", buf) ;
        fscanf(fin, "%s", buf);  //printf("%s ", buf) ;
        fscanf(fin, "%s", buf2); //printf("%s\n", buf2) ;
        if (strcmp(buf2, "true)") == 0)
        {
            //printf("%s ", buf1) ;
            int i = buf1[1] - '0' - 1;
            int j = buf1[2] - '0' - 1;
            int n = buf1[3] - '0';
            arr[i][j] = n;
            //printf("%s\n", buf2) ;
            count++;
        }
    }
    printf("%d\n", count);

    if (count < 81)
    {
        printf("It is a Sudoku puzzle with wrong conditions.\n");
    }
    else
    {
        printf("This is the answer to the entered puzzle.\n");
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                printf("%d ", arr[i][j]);
                if (j == 2 || j == 5)
                {
                    printf("| ");
                }
            }
            if (i == 2 || i == 5)
            {
                printf("\n———————————");
            }
            printf("\n");
        }
    }
    pclose(fin);
}