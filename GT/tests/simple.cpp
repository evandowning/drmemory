#include <stdio.h>
#include <string.h>

int main(void)
{
    FILE *fp;
    fp = fopen("file.txt","w");
    fclose(fp);

    return 0;
}
