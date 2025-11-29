/* Date: 11/11/2025 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add(int x, int y)
{
    printf(" the sum is : %d \n", x + y);
}
void sub(int x, int y)
{
    printf(" the sub is : %d \n", x - y);
}
void mult(int x, int y)
{
    printf(" the sum is : %d \n", x * y);
}
void divei(int x, int y)
{
    printf(" the sum is : %d \n", x / y);
}

char *dublicate_allocation(char arr[])
{
    int size = strlen(arr);
    char *ptr = (char *)malloc(size);
    int i = 0;
    for (i = 0; i < size; i++)
    {
        ptr[i] = arr[i];
    }
    ptr[i] = '\0';
    return ptr;
}

int main()
{

    // 1 access number by pointer
    int x = 10, *ptr1 = &x;
    printf(" x before edit = %d \n", x);

    *ptr1 = 20;
    printf(" x after edit = %d \n", x);

    // 2 access array using pointer
    int arr[5] = {1, 2, 3, 4, 5};
    int *ptr2 = arr;
    for (int i = 0; i < 5; i++)
    {
        printf(" %d  before element is %d \n", i + 1, *(ptr2 + i));
    }
    for (int i = 10; i < 15; i++)
    {
        *(ptr2 + i - 10) = i;
    }
    for (int i = 0; i < 5; i++)
    {
        printf(" %d  after element is %d \n", i + 1, *(ptr2 + i));
    }

    // 3 using array of pointer to function
    int num1, num2;
    char op;
    typedef void (*ptrf)(int, int);
    ptrf operation[4] = {add, sub, mult, divei};

    printf("Enter two number to make operation: ");
    scanf("%d %d", &num1, &num2);

    printf("Enter operation you want / - + * :");
    scanf(" %c", &op);

    switch (op)
    {
    case '+':
    {
        operation[0](num1, num2);
    }
    break;
    case '-':
    {
        operation[1](num1, num2);
    }
    break;
    case '*':
    {
        operation[2](num1, num2);
    }
    break;
    case '/':
    {
        operation[3](num1, num2);
    }
    break;
    }

    // 4 use memory allocation (Heap)
    int size, sum = 0, average = 0, *ptr3 = NULL;
    printf(("Enter the size you want to allocat: "));
    scanf("%d", &size);

    ptr3 = (int *)malloc(size * sizeof(int));

    if (ptr3 == NULL)
    {
        printf("No free location to allocat");
    }
    else
    {
        printf("Enter %d number: ", size);
        for (int i = 0; i < size; i++)
        {
            scanf("%d", &ptr3[i]);
            sum += ptr3[i];
        }
        average = sum / size;

        printf(" the sum equal: %d\n", sum);
        printf(" the average equal: %d\n", average);
        free(ptr3);
        ptr3 = NULL;
    }

    char *ptr4 = dublicate_allocation("ramadan");
    printf("%s", ptr4);
    free(ptr4);
    return 0;
}
