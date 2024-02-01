#include <stdio.h>

int main() {
    printf("%d\n", sizeof(unsigned char)); // one byte
    printf("%d\n", sizeof(char)); // one byte
    printf("%d\n", sizeof(short)); // two bytes
    printf("%d\n", sizeof(int)); // four bytes
    printf("%d\n", sizeof(long)); // four bytes
    printf("%d\n", sizeof(long long)); // eight bytes
    printf("%d\n", sizeof(float)); // four bytes
    printf("%d\n", sizeof(double)); // eight bytes
    printf("%d\n", sizeof(short int)); // two bytes
    return 0;
}
