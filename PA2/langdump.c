#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 65

typedef struct resultList // A linked list is used to avoid overlapping result values.
{
    struct resultList *next;
    char member[MAX_LENGTH];
} resultList;
resultList *resultlist;

void find_member(char (*)[], int, int); 
// A function of storing the members belonging to the language in a linked list.
void replace(char (*)[], int, char *, char *, int, resultList *);
// A recursion function
char *find_symbol(char *);
// A function that returns a pointer of the symbol.
void erase_newline(char (*)[], int);
void addList(resultList *, char *);
void printList(resultList *);
int check_length(char *);

int main(int argc, char *argv[])
{

    char *file = argv[1];
    int length = atoi(argv[2]); // CLI =  ./langdump ??.txt length
    FILE *fp;
    char buffer[MAX_LENGTH]; // to save grammar in array
    char grammar[MAX_LENGTH][MAX_LENGTH];
    int grammar_size = 0; // for the loop in recursive function

    fp = fopen(file, "r");

    while (fgets(buffer, MAX_LENGTH, fp))
    {
        strcpy(grammar[grammar_size], buffer);
        grammar_size++;
    }
    find_member(grammar, grammar_size, length);

    printf("=================\n");
    printList(resultlist);
    free(resultlist);

    return 0;
}

void find_member(char (*grammar)[MAX_LENGTH], int grammar_size, int length)
{
    resultlist = malloc(sizeof(resultList));
    resultlist->next = NULL;

    char symbol[MAX_LENGTH];
    erase_newline(grammar, grammar_size);
    for (int i = 0; i < grammar_size; i++)
    {
        if (grammar[i][0] == 'S' && grammar[i][1] == ' ')
            strcpy(symbol, strstr(grammar[i], "::= ") + 4);
    }

    if (symbol == NULL)
    {
        printf("given input is invalid.\n");
        return;
    }
    char result[sizeof(symbol)];
    strcpy(result, symbol);

    replace(grammar, grammar_size, result, symbol, length, resultlist);
}

void replace(char (*grammar)[MAX_LENGTH], int grammar_size, char *result, char *symbol, int length, resultList *resultlist)
{
    if (check_length(result) > length) // check length
        return;
    for (int i = 0; i < grammar_size; i++)
    {
        if (strncmp(grammar[i], symbol, strlen(symbol)) == 0)
        {
            char *rhs;
            rhs = strstr(grammar[i], "::= ") + 4;
            char new_result[MAX_LENGTH];
            int new_length = 0;
            char *symbolPtr = find_symbol(result);

            if (symbolPtr == NULL)
            {
                strcpy(new_result, rhs);
            }
            else
            {
                for (int i = 0; i < strlen(result) - strlen(symbolPtr); i++)
                {
                    new_result[new_length] = result[i];
                    new_length++;
                }
                new_result[new_length] = '\0';
                strcat(new_result, rhs);
                symbolPtr += strlen(symbol) + 1;
                strcat(new_result, " ");
                strcat(new_result, symbolPtr);
            }

            char temp_result[MAX_LENGTH];
            if (find_symbol(new_result) == NULL)
            {
                int temp_length = 0;
                int isConcrete = 0;
                for (int p = 0; p <= strlen(new_result); p++)
                {
                    if (new_result[p] == '\"')
                    {
                        isConcrete++;
                        continue;
                    }
                    else if (new_result[p] == ' ' && isConcrete % 2 == 0)
                    {
                        continue;
                    }
                    else if (new_result[p] == ' ' && isConcrete % 2 != 0)
                    {
                        temp_result[temp_length] = new_result[p];
                        temp_length++;
                        continue;
                    }
                    else if (new_result[p] == '\0')
                    {
                        temp_result[temp_length] = '\0';
                        break;
                    }
                    else
                    {
                        temp_result[temp_length] = new_result[p];
                        temp_length++;
                    }
                }

                if (strlen(temp_result) <= length)
                {
                    addList(resultlist, temp_result);
                }
                else
                    break;
            }
            else
            {
                symbolPtr = find_symbol(new_result);
                char newSymbol[16];
                int newSymbol_length = 0;
                while (symbolPtr[0] != ' ')
                {
                    newSymbol[newSymbol_length] = symbolPtr[0];
                    symbolPtr++;
                    newSymbol_length++;
                }
                newSymbol[newSymbol_length] = '\0';
                replace(grammar, grammar_size, new_result, newSymbol, length, resultlist);
            }
        }
    }
}

char *find_symbol(char *result)
{
    char *returnPtr = result;
    int isConcrete = 0;
    while (returnPtr[0] != '\n' && returnPtr[0] != '\0')
    {
        if (returnPtr[0] == '\"')
        {
            isConcrete++;
            returnPtr++;
            continue;
        }
        else if (returnPtr[0] == ' ' && isConcrete % 2 == 0)
        {
            returnPtr++;
            continue;
        }
        else if (isConcrete % 2 == 0)
        {
            return returnPtr;
        }
        else
            returnPtr++;
    }

    returnPtr = NULL;

    return returnPtr;
}

void erase_newline(char (*grammar)[MAX_LENGTH], int grammar_size)
{
    for (int i = 0; i < grammar_size; i++)
    {
        int j = 0;
        while (grammar[i][j])
        {
            j++;
        }
        if (grammar[i][j - 1] == '\n')
            grammar[i][j - 1] = '\0';
    }
}

void addList(resultList *resultlist, char *newMember)
{
    if (resultlist->next == NULL)
    {
        resultList *newList = malloc(sizeof(resultList));
        strcpy(newList->member, newMember);
        newList->next = NULL;

        resultlist->next = newList;
    }
    else
    {
        resultList *curr = resultlist;
        while (curr->next != NULL)
        {
            if (strcmp(curr->member, newMember) == 0)
                return;
            curr = curr->next;
        }

        if (strcmp(curr->member, newMember) == 0)
            return;
        resultList *newList = malloc(sizeof(resultList));
        strcpy(newList->member, newMember);
        newList->next = NULL;

        curr->next = newList;
    }

    return;
}

void printList(resultList *resultlist)
{
    int num = 0;
    resultlist = resultlist->next;
    while (resultlist != NULL)
    {
        num++;
        printf("%s\n", resultlist->member);
        resultlist = resultlist->next;
    }
    printf("=================\n");
    printf("%d\n", num);
}

int check_length(char *string)
{
    int isConcrete = 0;
    int string_length = 0;
    for (int i = 0; i <= strlen(string); i++)
    {
        if (string[i] == '\"')
        {
            isConcrete++;
            continue;
        }
        else if (string[i] == ' ' && isConcrete % 2 == 0)
        {
            continue;
        }
        else if (string[i] == ' ' && isConcrete % 2 != 0)
        {
            string_length++;
            continue;
        }
        else if (string[i] == '\0')
        {
            break;
        }
        else
        {
            string_length++;
        }
    }

    return string_length;
}