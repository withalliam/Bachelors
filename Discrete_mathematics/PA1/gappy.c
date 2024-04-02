#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *fp = fopen("formula", "w");

    int i, j, b;

    // i: 1~9   j: 1~9    b: 0,1 (white, black)
    for (i = 1; i <= 9; i++)
        for (j = 1; j <= 9; j++)
            for (b = 0; b <= 1; b++)
                fprintf(fp, "(declare-const p%d%d%d Bool)\n", i, j, b);
    // example p000 p001
    //           T    T  = contradiction
    //           T    F  = White
    //           F    T  = Black
    //           F    F  = contradiciton

    // Each cell must be either a black or white. (and (or w b)(not (and w b)))
    fprintf(fp, "; Q0\n");
    fprintf(fp, "(assert (and ");
    for (i = 1; i <= 9; i++)
    {
        fprintf(fp, "(and ");
        for (j = 1; j <= 9; j++)
        {
            fprintf(fp, "(and (or p%d%d%d p%d%d%d) (not (and p%d%d%d p%d%d%d)))", i, j, 0, i, j, 1, i, j, 0, i, j, 1);
        }
        fprintf(fp, ")");
    }
    fprintf(fp, "))\n");

    // There must be no two black cells are adjacent to each other
    // horizontally
    fprintf(fp, "; Q1-row\n");
    fprintf(fp, "(assert (and ");
    for (i = 1; i <= 9; i++)
    {
        fprintf(fp, "(and ");
        for (j = 1; j <= 8; j++)
        {
            fprintf(fp, "(not (and p%d%d%d p%d%d%d))", i, j, 1, i, j + 1, 1);
        }
        fprintf(fp, ")");
    }
    fprintf(fp, "))\n");

    // There must be no two black cells are adjacent to each other
    // vertically
    fprintf(fp, "; Q1-column\n");
    fprintf(fp, "(assert (and ");
    for (i = 1; i <= 8; i++)
    {
        fprintf(fp, "(and ");
        for (j = 1; j <= 9; j++)
        {
            fprintf(fp, "(not (and p%d%d%d p%d%d%d))", i, j, 1, i + 1, j, 1);
        }
        fprintf(fp, ")");
    }
    fprintf(fp, "))\n");

    // There must be no two black cells are adjacent to each other
    // diagonally (right)
    fprintf(fp, "; Q1-diag-right\n");
    fprintf(fp, "(assert (and ");
    for (i = 1; i <= 8; i++)
    {
        fprintf(fp, "(and ");
        for (j = 1; j <= 8; j++)
        {
            fprintf(fp, "(not (and p%d%d%d p%d%d%d))", i, j, 1, i + 1, j + 1, 1);
        }
        fprintf(fp, ")");
    }
    fprintf(fp, "))\n");

    // There must be no two black cells are adjacent to each other
    // diagonally (right)
    fprintf(fp, "; Q1-diag-right\n");
    fprintf(fp, "(assert (and ");
    for (i = 1; i <= 8; i++)
    {
        fprintf(fp, "(and ");
        for (j = 2; j <= 9; j++)
        {
            fprintf(fp, "(not (and p%d%d%d p%d%d%d))", i, j, 1, i + 1, j - 1, 1);
        }
        fprintf(fp, ")");
    }
    fprintf(fp, "))\n");

    // There must be no more than three black cells in each row
    fprintf(fp, "; Q2-row\n");
    fprintf(fp, "(assert (and ");
    for (i = 1; i <= 9; i++)
    {
        fprintf(fp, "(and ");
        for (j = 1; j <= 7; j++)
        {
            fprintf(fp, "(and ");
            for (int jj = j + 1; jj <= 8; jj++)
            {
                fprintf(fp, "(and ");
                for (int jjj = jj + 1; jjj <= 9; jjj++)
                {
                    fprintf(fp, "(not (and p%d%d%d p%d%d%d p%d%d%d))", i, j, 1, i, jj, 1, i, jjj, 1);
                }
                fprintf(fp, ")");
            }
            fprintf(fp, ")");
        }
        fprintf(fp, ")");
    }
    fprintf(fp, "))\n");

    // There must be no more than three black cells in each column
    fprintf(fp, "; Q2-column\n");
    fprintf(fp, "(assert (and ");
    for (j = 1; j <= 9; j++)
    {
        fprintf(fp, "(and ");
        for (i = 1; i <= 7; i++)
        {
            fprintf(fp, "(and ");
            for (int ii = i + 1; ii <= 8; ii++)
            {
                fprintf(fp, "(and ");
                for (int iii = ii + 1; iii <= 9; iii++)
                {
                    fprintf(fp, "(not (and p%d%d%d p%d%d%d p%d%d%d))", i, j, 1, ii, j, 1, iii, j, 1);
                }
                fprintf(fp, ")");
            }
            fprintf(fp, ")");
        }
        fprintf(fp, ")");
    }
    fprintf(fp, "))\n");

    char command;
    char fileName[128];

    printf("Do you want to type the puzzle? (y/n) > ");
    scanf("%c", &command);

    // puzzle input , pre-assigned
    if (command == 'y')
    {
        char label, buffer;
        int space;
        printf("Type the gappy puzzle!\n");
        scanf("%c", &buffer);
        //puzzle input (row)
        fprintf(fp, "; pre-assigned-row\n");
        fprintf(fp, "(assert (and ");
        for (int i = 1; i <= 9; i++)
        {
            scanf("%c", &label);
            scanf("%c", &buffer);
            space = (int)label - '0';
            if (space > 7 || space < 0)
            {
                printf("This is not valid input.\n");
                return 0;
            }
            fprintf(fp, "(or ");
            for (int j = 1; j <= 8 - space; j++)
            { // j + space + 1 <= 9 -> j <= 8 - space
                fprintf(fp, "(and p%d%d%d p%d%d%d)", i, j, 1, i, j + space + 1, 1);
            }
            fprintf(fp, ")");
        }
        fprintf(fp, "))\n");

        //puzzle input (column)
        fprintf(fp, "; pre-assigned-column\n");
        fprintf(fp, "(assert (and ");
        for (int j = 1; j <= 9; j++)
        {
            scanf("%c", &label);
            scanf("%c", &buffer);
            space = (int)label - '0';
            if (space > 7 || space < 0)
            {
                printf("This is not a valid input.\n");
                return 0;
            }
            fprintf(fp, "(or ");
            for (int i = 1; i <= 8 - space; i++)
            { // i + space + 1 <= 9 -> i <= 8 - space
                fprintf(fp, "(and p%d%d%d p%d%d%d)", i, j, 1, i + space + 1, j, 1);
            }
            fprintf(fp, ")");
        }
        fprintf(fp, "))\n");
    }
    else if (command == 'n')
    {
        char label, buffer;
        int space;
        printf("Type the file name of gappy puzzle!\n");
        scanf("%s", fileName);

        FILE *gappy = fopen(fileName, "r");

        fprintf(fp, "; pre-assigned-row\n");
        fprintf(fp, "(assert (and ");
        for (int i = 1; i <= 9; i++)
        {
            fscanf(gappy, "%c", &label);
            fscanf(gappy, "%c", &buffer);
            space = (int)label - '0';
            if (space > 7 || space < 0)
            {
                printf("This is not a valid input.\n");
                return 0;
            }
            fprintf(fp, "(or ");
            for (int j = 1; j <= 8 - space; j++)
            {
                fprintf(fp, "(and p%d%d%d p%d%d%d)", i, j, 1, i, j + space + 1, 1);
            }
            fprintf(fp, ")");
        }
        fprintf(fp, "))\n");

        fprintf(fp, "; pre-assigned-column\n");
        fprintf(fp, "(assert (and ");
        for (int j = 1; j <= 9; j++)
        {
            fscanf(gappy, "%c", &label);
            fscanf(gappy, "%c", &buffer);
            space = (int)label - '0';
            if (space > 7 || space < 0)
            {
                printf("This is not valid input.\n");
                return 0;
            }
            fprintf(fp, "(or ");
            for (int i = 1; i <= 8 - space; i++)
            {
                fprintf(fp, "(and p%d%d%d p%d%d%d)", i, j, 1, i + space + 1, j, 1);
            }
            fprintf(fp, ")");
        }
        fprintf(fp, "))\n");
    }
    else
    {
        return 0;
    }

    fprintf(fp, "(check-sat)\n(get-model)\n");

    fclose(fp);

    // formula

    FILE *fin = popen("z3 formula", "r");
    char buf[128];
    char buf1[128];
    char buf2[128];
    fscanf(fin, "%s %s", buf, buf1);

    if (strcmp(buf, "unsat") == 0)
    {
        printf("This puzzle has no solution.\n");
        return 0;
    }

    int gappy[9][9];

    while (!feof(fin))
    {
        fscanf(fin, "%s", buf);  //printf("%s ", buf) ;
        fscanf(fin, "%s", buf1); //printf("%s ", buf1) ;
        fscanf(fin, "%s", buf);  //printf("%s ", buf) ;
        fscanf(fin, "%s", buf);  //printf("%s ", buf) ;
        fscanf(fin, "%s", buf2); //printf("%s\n", buf2) ;
        if (strcmp(buf2, "true)") == 0)
        {
            int i = buf1[1] - '0';
            int j = buf1[2] - '0';
            int b = buf1[3] - '0';
            gappy[i][j] = b;
        }
    }

    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            switch (gappy[i][j])
            {
            case 0:
                printf("W ");
                break;
            case 1:
                printf("B ");
                break;

            default:
                printf("This puzzle has no solution.\n");
                break;
            }
        }
        printf("\n");
    }

    fclose(fin);

    return 0;
}
