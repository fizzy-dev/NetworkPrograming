#include <stdio.h>
#include <string.h>
int main()
{
    int len;
    const char str[] = "https://QTM.com/lap-trinh-c/index.jsp";
    const char ch = '/';
    char *ret;
    ret = strrchr(str, ch);
    printf("Chuoi xuat hien sau dau |%c| cuoi cung la: \n|%s|\n", ch, ret);
    return (0);
}