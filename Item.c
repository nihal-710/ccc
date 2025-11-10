#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Item {
    char lhs;
    char rhs[20];
    int dot;
} Item;

typedef struct ItemSet {
    Item items[40];
    int count;
} ItemSet;

typedef struct Transition {
    int from;
    char symbol;
    int to;
} Transition;

int n;
char grammarLHS[100];
char grammarRHS[100][10];
ItemSet states[100];
int nStates = 0;

Transition transitions[200];
int nTransitions = 0;

void inputGrammar();
void compute();
void closure(ItemSet *I);
ItemSet GOTO(ItemSet I, char c);
int isSameState(ItemSet *A, ItemSet *B);
void print();

 int isSameState(ItemSet *A, ItemSet *B)
{
    if (A->count != B->count)
        return 0;

    for (int i = 0; i < A->count; i++)
    {
        Item a = A->items[i];
        int found = 0;
        for (int j = 0; j < B->count; j++)
        {
            Item b = B->items[j];
            if (a.lhs == b.lhs && strcmp(a.rhs, b.rhs) == 0 && a.dot == b.dot)
            {
                found = 1;
                break;
            }
        }
        if (!found)
            return 0;
    }

    return 1;
}

ItemSet GOTO(ItemSet I, char c)
{
    ItemSet newstate;
    newstate.count = 0;

    for (int i = 0; i < I.count; i++)
    {
        Item item = I.items[i];
        Item newitem;
        int dot = item.dot;
        if (dot < (int)strlen(item.rhs))
        {
            char next = item.rhs[dot];
            if (next == c)
            {
                newitem = item;
                newitem.dot++;
                newstate.items[newstate.count++] = newitem;
            }
        }
    }

    if (newstate.count > 0)
        closure(&newstate);

    return newstate;
}


void closure(ItemSet *I)
{
    int change = 1;
    while (change)
    {
        change = 0;
        for (int i = 0; i < I->count; i++)
        {
            Item item = I->items[i];
            int dotIndex = item.dot;
            if (dotIndex < (int)strlen(item.rhs))
            {
                char next = item.rhs[dotIndex];
                if (isupper(next))
                {
                    for (int j = 0; j <= n; j++)
                    {
                        if (grammarLHS[j] == next)
                        {
                            Item newitem;
                            newitem.lhs = next;
                            strcpy(newitem.rhs, grammarRHS[j]);
                            newitem.dot = 0;

                            int exist = 0;
                            for (int l = 0; l < I->count; l++)
                            {
                                if (I->items[l].lhs == newitem.lhs && strcmp(newitem.rhs, I->items[l].rhs) == 0 && newitem.dot == I->items[l].dot)
                                {
                                    exist = 1;
                                    break;
                                }
                            }

                            if (!exist)
                            {
                                I->items[I->count++] = newitem;
                                change = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void compute()
{
    // Augment grammar
    grammarLHS[0] = 'Z';
    grammarRHS[0][0] = grammarLHS[1];
    grammarRHS[0][1] = '\0';
    n++;

    // First ItemSet I0
    ItemSet I0;
    I0.items[0].lhs = grammarLHS[0];
    strcpy(I0.items[0].rhs, grammarRHS[0]);
    I0.items[0].dot = 0;
    I0.count = 1;

    closure(&I0);

    states[0] = I0;
    nStates = 1;

    int change = 1;
    while (change)
    {
        change = 0;
        for (int i = 0; i < nStates; i++)
        {
            for (int j = 0; j < states[i].count; j++)
            {
                Item current = states[i].items[j];
                int dot = current.dot;
                if (dot < (int)strlen(current.rhs))
                {
                    char symbol = current.rhs[dot];
                    ItemSet newState = GOTO(states[i], symbol);

                    if (newState.count == 0)
                        continue;

                    int exist = -1;
                    for (int k = 0; k < nStates; k++)
                    {
                        if (isSameState(&states[k], &newState))
                        {
                            exist = k;
                            break;
                        }
                    }

                    int target;
                    if (exist == -1)
                    {
                        target = nStates;
                        states[nStates++] = newState;
                        change = 1;
                    }
                    else
                    {
                        target = exist;
                    }

                    // Record transition
                    transitions[nTransitions].from = i;
                    transitions[nTransitions].symbol = symbol;
                    transitions[nTransitions].to = target;
                    nTransitions++;
                }
            }
        }
    }
}

void print()
{
    printf("----------------------------------------\n");
    for (int i = 0; i < nStates; i++)
    {
        printf("I%d :\n", i);
        for (int j = 0; j < states[i].count; j++)
        {
            Item current = states[i].items[j];
            printf("%c -> ", current.lhs);
            for (int k = 0; k < (int)strlen(current.rhs); k++)
            {
                if (k == current.dot)
                    printf(".");
                printf("%c", current.rhs[k]);
            }
            if (current.dot == (int)strlen(current.rhs))
                printf(".");
            printf("\n");
        }

        // Print all transitions from this state
        for (int t = 0; t < nTransitions; t++)
        {
            if (transitions[t].from == i)
                printf("GOTO ( I%d , %c ) = I%d\n", i,
                       transitions[t].symbol,
                       transitions[t].to);
        }
        printf("\n");
    }
}

int main()
{
    printf("LR(0) Parsing\n");
    inputGrammar();
    compute();
    print();
    return 0;
}
void inputGrammar()
{
    printf("Enter number of productions : ");
    scanf("%d", &n);

    for (int i = 1; i <= n; i++)
    {
        char p[20];
        scanf("%s", p);

        grammarLHS[i] = p[0];
        int index = 0;
        for (int j = 3; j < (int)strlen(p); j++)
        {
            grammarRHS[i][index++] = p[j];
        }
        grammarRHS[i][index++] = '\0';
    }
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

// typedef struct Item
// {
//     char lhs;
//     char rhs[20];
//     int dot;
// } Item;

// typedef struct ItemSet
// {
//     Item items[40];
//     int count;
// } ItemSet;

// int n;
// char grammarLHS[100];
// char grammarRHS[100][10];

// ItemSet states[100];
// int nStates = 0;

// void inputGrammar();
// void compute();
// void closure(ItemSet *I);
// ItemSet GOTO(ItemSet I, char c);
// int isSameState(ItemSet *A, ItemSet *B);
// void print();

// int isSameState(ItemSet *A, ItemSet *B)
// {
//     if (A->count != B->count)
//         return 0;

//     for (int i = 0; i < A->count; i++)
//     {
//         Item a = A->items[i];
//         int found = 0;
//         for (int j = 0; j < B->count; j++)
//         {
//             Item b = B->items[j];
//             if (a.lhs == b.lhs && strcmp(a.rhs, b.rhs) == 0 && a.dot == b.dot)
//             {
//                 found = 1;
//                 break;
//             }
//         }
//         if (!found)
//             return 0;
//     }

//     return 1;
// }

// ItemSet GOTO(ItemSet I, char c)
// {
//     ItemSet newstate;
//     newstate.count = 0;

//     for (int i = 0; i < I.count; i++)
//     {
//         Item item = I.items[i];
//         Item newitem;
//         int dot = item.dot;
//         if (dot < (int)strlen(item.rhs))
//         {
//             char next = item.rhs[dot];
//             if (next == c)
//             {
//                 newitem = item;
//                 newitem.dot++;
//                 newstate.items[newstate.count++] = newitem;
//             }
//         }
//     }

//     if (newstate.count > 0)
//     {
//         closure(&newstate);
//     }

//     return newstate;
// }

// void closure(ItemSet *I)
// {
//     int change = 1;
//     while (change)
//     {
//         change = 0;
//         for (int i = 0; i < I->count; i++)
//         {
//             Item item = I->items[i];
//             int dotIndex = item.dot;
//             if (dotIndex < (int)strlen(item.rhs))
//             {
//                 char next = item.rhs[dotIndex];
//                 if (isupper(next))
//                 {
//                     for (int j = 0; j <= n; j++)
//                     {
//                         if (grammarLHS[j] == next)
//                         {
//                             Item newitem;
//                             newitem.lhs = next;
//                             strcpy(newitem.rhs, grammarRHS[j]);
//                             newitem.dot = 0;

//                             int exist = 0;
//                             for (int l = 0; l < I->count; l++)
//                             {
//                                 if (I->items[l].lhs == newitem.lhs && strcmp(newitem.rhs, I->items[l].rhs) == 0 && newitem.dot == I->items[l].dot)
//                                 {
//                                     exist = 1;
//                                     break;
//                                 }
//                             }

//                             if (!exist)
//                             {
//                                 I->items[I->count++] = newitem;
//                                 change = 1;
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

// void inputGrammar()
// {
//     printf("Enter number of productions : ");
//     scanf("%d", &n);

//     for (int i = 1; i <= n; i++)
//     {
//         char p[20];
//         scanf("%s", p);

//         grammarLHS[i] = p[0];
//         int index = 0;
//         for (int j = 3; j < (int)strlen(p); j++)
//         {
//             grammarRHS[i][index++] = p[j];
//         }
//         grammarRHS[i][index++] = '\0';
//     }
// }

// void compute()
// {
//     // Add new production
//     grammarLHS[0] = 'Z';
//     grammarRHS[0][0] = grammarLHS[1];
//     grammarRHS[0][1] = '\0';
//     n++;

//     // First Item Set
//     ItemSet I0;
//     I0.items[0].lhs = grammarLHS[0];
//     I0.items[0].rhs[0] = grammarRHS[0][0];
//     I0.items[0].rhs[1] = '\0';
//     I0.items[0].dot = 0;
//     I0.count = 1;

//     closure(&I0);

//     nStates++;
//     states[0] = I0;

//     int change = 1;
//     while (change)
//     {
//         change = 0;
//         for (int i = 0; i < nStates; i++)
//         {
//             for (int j = 0; j < states[i].count; j++)
//             {
//                 Item current = states[i].items[j];
//                 int dot = current.dot;
//                 if (dot < (int)strlen(current.rhs))
//                 {
//                     ItemSet newState = GOTO(states[i], current.rhs[dot]);

//                     if (newState.count == 0)
//                         continue;

//                     int exist = 0;
//                     for (int k = 0; k < nStates; k++)
//                     {
//                         if (isSameState(&states[k], &newState))
//                         {
//                             exist = 1;
//                             break;
//                         }
//                     }

//                     if (!exist)
//                     {
//                         states[nStates++] = newState;
//                         change = 1;
//                     }
//                 }
//             }
//         }
//     }
// }

// void print()
// {
//     for (int i = 0; i < nStates; i++)
//     {
//         ItemSet state = states[i];
//         printf("I%d : \n", i);
//         for (int j = 0; j < state.count; j++)
//         {
//             Item current = state.items[j];
//             printf("%c -> ", current.lhs);
//             for (int k = 0; k < (int)strlen(current.rhs); k++)
//             {
//                 if (k == current.dot)
//                 {
//                     printf(".");
//                 }
//                 printf("%c", current.rhs[k]);
//             }
//             if (current.dot == (int)strlen(current.rhs))
//                 printf(".");
//             printf("\n");
//         }
//         printf("\n");
//     }
// }

// int main()
// {
//     printf("LR(0) Parsing\n");

//     inputGrammar();
//     compute();
//     print();

//     return 0;
// }
