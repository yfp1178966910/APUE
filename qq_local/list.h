/*#########################################################################
# File Name: list.h
# Author: yanfeipeng
# address: seclab
# Created Time: Fri 13 Oct 2017 11:02:38 PM EST
#########################################################################*/
// 登录列表以链表形式存在
#ifndef __LIST_H__
#define __LIST_H__

struct list{
    char name[4];
    struct list *next;
};

// 创建列表
struct list *create_list();

// 遍历
void traverse(struct list *l);

// 插入
void insert_list(struct list *l, int n, char *str);

// 查询节点
int Get_node(struct list *l, char *str);

// 删除节点
void delete_node(struct list *l, char *str);

// 判空
int empty_list(struct list *l);

#endif

struct list *create_list(){
    struct list *l;
    l = (struct list *)malloc(sizeof(struct list *));
    if (l != NULL ){
        l -> next = NULL;
    }
    return l;
}

int Get_node(struct list *l, char *str){

    struct list *p = l->next;
    int pos = 0;

    while( p != NULL &&  (strcmp(p->name, str)) != 0){
        
        p = p->next;
        pos++;
    }

    if ( p != NULL )
        return pos+1;

    return -1;
}

void traverse(struct list *l){

    struct list *p = l->next;
    while(p != NULL){
        printf("%s ", p->name);
        p = p -> next;
    }
    printf("\n");
}

void delete_node(struct list *l, char *str){

    int temp = Get_node(l, str);
    if ( temp == -1 )
        return;

    int found = 0;
    struct list *p = l, *q;
    
    while( p != NULL && found < temp-1 ){
        
        p = p->next;
        found++;
    }

    if ( p == NULL )
        return;

    p->next = p->next->next;

    
}

void insert_list(struct list *l, int n, char *str){
    
    struct list *p, *q;
    p = l;

    int found = 0;

    while(found < n-1 && p != NULL){
        p = p -> next;
        found++;
    }

    if ( p == NULL  || n < 0){
        return -1;
    }

    if ( found > n )
        return -1;


    q = (struct list *)malloc(sizeof(struct list *));
    if (q != NULL ){

        strcpy(q->name, str);
        q->next = p->next;
        p->next = q;
    }
    else{
        
        printf("error\n");
        exit(-1);
    }
}


int empty_list(struct list *l){
    
    if (  l -> next == NULL)
        return 0;
    return 1;
}
