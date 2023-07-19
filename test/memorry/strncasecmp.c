/*
strcasecmp.c
*/
#include<strings.h>
#include<stdio.h>

int main()
{
    char *s1 = "aBcDeD";
    char *s2 = "AbCdEd";
    char *s3 = "abcdefg";
    char *s4 = "bacdefg";

    int len = strcasecmp(s1, s2);
    if (!len)
    {
        printf("%s = %s\n", s1, s2);
    }
    else
    {
        printf("%s != %s\n", s1, s2);
    }

    len = strcasecmp(s1, s3);
    if (!len)
    {
        printf("%s = %s\n", s1, s3);
    }
    else
    {
        printf("%s != %s, len = %d\n", s1, s3, len);
    }

    len = strcasecmp(s1, s4);
    if (!len)
    {
        printf("%s = %s\n", s1, s4);
    }
    else
    {
        printf("%s != %s, len = %d\n", s1, s4, len);
    }

    return 0;
}