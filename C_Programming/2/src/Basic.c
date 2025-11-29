/* Date: 9/11/2025 */
#include <stdio.h>
#include <limits.h>
int main()
{
    // 1 print the size of datatype
    printf("char size  : %zu bytes\n", sizeof(char));
    printf("short size : %zu bytes\n", sizeof(short));
    printf("int size   : %zu bytes\n", sizeof(int));
    printf("long size   : %zu bytes\n", sizeof(long));
    printf("float size : %zu bytes\n", sizeof(float));
    printf("double size : %zu bytes\n", sizeof(double));
    printf("long double size : %zu bytes\n", sizeof(long double));

    // 2 print the limits of data type
    printf("char mini  : %d \n", CHAR_MIN);
    printf("char max  : %d \n", CHAR_MAX);
    printf("-------------------\n");
    printf("short mini  : %d bytes\n", SHRT_MIN);
    printf("short max  : %d bytes\n", SHRT_MAX);
    printf("-------------------\n");
    printf("int mini  : %d bytes\n", INT_MIN);
    printf("int max  : %d bytes\n", INT_MAX);
    printf("-------------------\n");

    // 3 make arthimitic operation on two nummber
    int x = 10;
    float y = 2.5;
    printf("add = %f\n", x + y);
    printf("sub = %f\n", x - y);
    printf("mult = %f\n", x * y);
    printf("div = %f\n", x / y);

    //  3 check if the number positive or negative
    int z = 10;
    if (z > 0)
        printf("Positive Number\n");
    else if (z == 0)
        printf("number is Zero\n");
    else
        printf("negative number\n");

    // print the size of pointer variable
    printf("pointer size : %zu bytes\n", sizeof(void *));
    if (sizeof(void *) == 8)
        printf("64-bit system\n");
    else if (sizeof(void *) == 4)
        printf("32-bit system\n");
    else
        printf("Unknown archtecture\n");

    return 0;
}