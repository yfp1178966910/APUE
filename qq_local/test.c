/*#########################################################################
# File Name: test.c
# Author: yanfeipeng
# address: seclab
# Created Time: 2018年03月27日 星期二 19时26分25秒
#########################################################################*/

#include <stdio.h>
#include <string.h>
#include <time.h>

int main()
{
    double n,m;
    int i;
    // scanf("%d %d", &n, &m);
    n = 30;
    m = 2000000000;
    double v[30];
    for(i=0; i<n; i++)
    {
        // scanf("%d",&v[i]);
        v[i] = 1000000000;
    }
    int j, l, k;
    int sum = 0;
    int num = 0;
    
   
    for (i=0; i<n; i++)
    {
        if (v[i] <= m)
            num++;
    }

    double start, finish;
    start = clock();
    for(i=1; i<n; i++) // 控制物品组合件数
        for(j=0; j<n-i; j++) // 从第一件开始组合
        {
            for(k=j+1; k<n-i+1; k++) 
            {
                sum = v[j];
                for(l=0; l<i; l++) // 控制件数组合质量之和
                {
                    sum += v[k+l];
                    if (sum > m)
                        break;
                }
                if (sum <= m)
                    num++;
            }
            
        }
    finish = clock();
    printf("%d\n",++num);
    printf("%.4f seconds\n", (finish-start)/CLOCKS_PER_SEC);
    return 0;
}
