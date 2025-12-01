/* Date: 10/11/2025 */
#include <stdio.h>
#include "user_function.h"

int main(int argc, char *argv[])
{
    // 1 deal with main parameter
    for (int i = 0; i < argc; i++)
    {
        if (i == 0)
            printf("usage : %s\n", argv[i]);
        else
        {
            printf(" user %d : %s \n", i, argv[i]);
        }
    }

    // 2 create function in another file and include it
    int sum = sum_TwoNumber(2, 3);
    print(sum);
    sum = sum_muti_pra(1, 2, 3, 4, 5);
    print(sum);

    // 3 use struct
    struct book
    {
        char titel[10];
        char author[10];
        int price;
        int pages;
    };
    struct book programming = {"C", "Aya", 100, 20};
    printf("book title is : %s \n", programming.titel);
    printf("book author is :%s \n", programming.author);
    printf("book price is : %d \n", programming.price);
    printf("book pages is : %d \n", programming.pages);

    // 4 use Enum
    typedef enum
    {
        SAT,
        SUN,
        MON,
        TUS,
        WEN,
        THUR,
        FRI
    } day;
    day one = SAT;
    printf("frist day is : %d \n", one);

    // 4  use union
    union type
    {
        int x;
        char y;
    };
    union type check;
    check.x = 320;
    check.y = 2;
    printf("x = %d \n", check.x);
    printf("y = %d \n", check.y);

    // 5 use struct with bit field inside union
    union cotrol_reserved
    {
        struct
        {
            unsigned int enable : 1;
            unsigned int mode : 2;
            unsigned int : 4;
            unsigned int status : 1;
        } bits;
        unsigned char raw_byte;
    };
    union cotrol_reserved reg;
    reg.bits.enable = 0;
    reg.bits.status = 1;
    reg.bits.mode = 3;
    printf(" enable bit: %d\n", reg.bits.enable);
    printf(" status bit: %d\n", reg.bits.status);
    printf(" enable bit: %d\n", reg.bits.mode);
    printf("byte value is: %d\n", reg.raw_byte);

    // use control flow
    int x = 10;
    switch (x)
    {

    case 10:
    {
        int y = 6;
        printf("%d", y);
    }
    break;
    }

    return 0;
}