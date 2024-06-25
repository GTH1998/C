#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_IN(array,n)\
    for(int i = 0; i < (n); i++)\
        (array)[i] = i;

#define ARRAY_COUT(array,n) \
    for(int i = 0;i < (n);i++) \
        printf("array[%d] = %d",i,array[i]);
//        cout<<"array ["<< i <<"] = "<<(array)[i]<<endl

int main()
{
    int A =10;
    int B =20;
    int C = 30;
    int *p0 = NULL;
    int *p1 = (int*)malloc(sizeof(int)*10);
    int *p2 = (int*)malloc(sizeof(int)*10);
    int *p3 = (int*)malloc(sizeof(int)*10);

    ARRAY_IN(p1,10);
    ARRAY_IN(p2,10);
    ARRAY_IN(p3,10);
    printf("&A = %p\n",&A);
    printf("&B = %p\n",&B);
    printf("&C = %p\n",&C);
    printf("p1 = %p\n",p1);
    printf("p2 = %p\n",p2);
    printf("p3 = %p\n",p3);

    ARRAY_COUT(p1,10);

    ARRAY_COUT(p2,10);
    ARRAY_COUT(p3,10);

    free(p1);
    free(p2);
    free(p3);
    ARRAY_COUT(p1,10);   
    ARRAY_COUT(p2,10);
    ARRAY_COUT(p3,10);
    for(int i=0;i <1024;i++)
    {
        p0 = (int *)malloc(sizeof(int)*1024*1024*1024);

    }
    printf("END \n");
    return 1;
}