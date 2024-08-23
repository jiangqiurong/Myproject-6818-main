#include "list.h"

//初始化链表
KernelNode_t* list_init()
{
    //1 申请头节点内存空间
    KernelNode_t *head = malloc(sizeof(KernelNode_t));
    if(head == NULL)
    {
        perror("malloc error");
        return NULL;
    }
    
    //2 初始化
    ElemType_t data = {
        .fileName = "Hdata",
        .password = "Hdata",
        .user = "Hdata"
    };
    head->data = data;
    INIT_LIST_HEAD(&(head->list));

    //head->list.prev = &(head->list);
    //head->list.next = &(head->list);

    return head;
}

//从链表尾部插入数据
void list_insert_tail(KernelNode_t*head,ElemType_t data)
{
    //1、新建一个结点并且初始化
    KernelNode_t *newNode = malloc(sizeof(KernelNode_t));
    if(newNode == NULL)
    {
        perror("malloc error");
        return ;
    }    
    newNode->data = data;
    INIT_LIST_HEAD(&(newNode->list));    

    //2、尾插
    list_add_tail(&(newNode->list), &(head->list));
}

//从链表头部插入数据
void list_insert_head(KernelNode_t*head,ElemType_t data)
{
    //1、新建一个结点并且初始化
    KernelNode_t *newNode = malloc(sizeof(KernelNode_t));
    if(newNode == NULL)
    {
        perror("malloc error");
        return ;
    }    
    newNode->data = data;
    INIT_LIST_HEAD(&(newNode->list));    

    //2、头插
    list_add(&(newNode->list), &(head->list));
}

//删除链表节点  返回值：true--删除成功  false--删除失败
bool list_delete(KernelNode_t* head,ElemType_t data)
{
    //如果链表为空，返回false
    if(list_empty(&(head->list)))
        return false;

    //遍历链表，找到目标节点后进行删除相关操作，返回true
    struct list_head* pos;
    list_for_each(pos,&(head->list))
    {
        KernelNode_t* trgNode = list_entry(pos,KernelNode_t,list);
        if(strcmp(trgNode->data.fileName,data.fileName) == 0)
        {
            list_del(pos);
            free(trgNode);
            return true;
        }
    }

    //找不到要删除的节点，返回false
    return false;
}

//修改链表节点数据  返回值：true--修改成功  false--修改失败
bool list_update(KernelNode_t* head,ElemType_t srcData,ElemType_t newData)
{
    //如果链表为空，返回false
    if(list_empty(&(head->list)))
        return false;

    //遍历链表，找到目标节点后修改节点数据，返回true
    struct list_head* pos;
    list_for_each(pos,&(head->list))
    {
        KernelNode_t* trgNode = list_entry(pos,KernelNode_t,list);
        if(strcmp(trgNode->data.fileName,srcData.fileName) == 0)
        {
            trgNode->data = newData;
            return true;
        }
    }

    //找不到目标节点，返回false
    return false;
}

//查找数据  返回值：true--查找成功  false--查找失败
bool list_find(KernelNode_t* head,ElemType_t data)
{
    //如果链表为空，返回false
    if(list_empty(&(head->list)))
        return false;

    //遍历链表，找到目标节点，返回true
    struct list_head* pos;
    struct list_head* n;
    list_for_each_safe(pos,n,&(head->list))
    {
        KernelNode_t* p = list_entry(pos,KernelNode_t,list);
        if(strcmp(p->data.fileName,data.fileName) == 0)
            return true;
    }

    //找不到目标节点，返回false
    return false;
}

//顺序打印链表
void list_show(KernelNode_t*head)
{
    /*
    #define list_for_each(pos, head)    for(pos = (head)->next; pos != (head); pos = pos->next)
    */
    struct list_head *pos;
    //for(pos=head->list.next; pos!=&(head->list); pos=pos->next)
    list_for_each(pos, &(head->list))
    {
        KernelNode_t* p = list_entry(pos,KernelNode_t,list);
        printf("%s ",p->data.user);
        printf("%s ",p->data.password);
        printf("%s ",p->data.fileName);
    }
    printf("\n");
}

//逆序打印链表
void list_show_reverse(KernelNode_t*head)
{
    struct list_head *pos;
    
    list_for_each_prev(pos, &(head->list))
    {
        KernelNode_t* p = list_entry(pos,KernelNode_t,list);
        printf("%s ",p->data.user);
        printf("%s ",p->data.password);
        printf("%s ",p->data.fileName);
    }
    printf("\n");
}

//销毁链表
void list_destroy(KernelNode_t*head)
{
    //链表为空直接释放头节点
    if(list_empty(&head->list))
    {
        free(head);
        return;
    }

    //从链表尾部开始删到链表头
    KernelNode_t *p = list_entry(head->list.prev,KernelNode_t,list);
    KernelNode_t *q = list_entry(p->list.prev,KernelNode_t,list);
    while(1)
    {
        if(q == head)
        {
            free(p);
            free(q);
            break;
        }

        free(p);
        p = q;
        q = list_entry(p->list.prev,KernelNode_t,list);
    }
}