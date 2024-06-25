#include <iostream>

using namespace std;
#define ARRAY_COUT(array,n) \
    for(int i = 0;i < (n);i++) \
        cout<<"array ["<< i <<"] = "<<(array)[i]<<endl

#define ARRAY_IN(array,n)\
    for(int i = 0; i < (n); i++)\
        (array)[i] = i;
int main()
{
    int test;
    int *p = NULL;
    p = (int*)malloc(10);

    ARRAY_IN(p,10);
    p++;
    ARRAY_COUT(p,10);

    cout<<"p size = "<<sizeof(p)<<endl;
    




    return 0;
}