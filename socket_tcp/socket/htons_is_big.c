#include <stdio.h>
#include <arpa/inet.h>

int main(void)
{
    short tmp = 0x1234;
    // printf("%x\n", (char)tmp);
    // printf("%d\n", (char *)tmp == 0x12);
    // printf("%d\n", (char *)tmp == 0x34);
    
    /* 
     * 使用强制数据类型转换，short 转至 char 后留下的是低位。
     *
     * 查看存储的是高位还是低位
     *
     * 高位说明是小端，低位说明是大端
     *
     * */
    
    // 一般主机采用小端， 网络采用大端
    if ((char)tmp == 0x34) {
        printf("It's little\n");  // 低存低
    } else if ((char)tmp == 0x12) {
        printf("It's big\n");  // 低存高
    }

    tmp = htons(tmp);

    if ((*(char *)&tmp) == 0x34) {
        printf("It's little\n");   // 低存低
    } else if ((*(char *)&tmp) == 0x12) {
        printf("It's big\n");  // 低存高
    }

    return 0;
}
