#include<stdio.h>
#include<stdlib.h>

//构造链表结构体
struct linknode
{
	int data;
	struct linknode *next;
};

typedef struct linknode LinkNode;
LinkNode *creatlinknode()//创建链表
{
	int i;
	LinkNode *head,*ptr,*p;
	head=(LinkNode*)malloc(sizeof(LinkNode));
	if(!head)
	{
		printf("分配内存失败\n");
		exit(1);
	}
	printf("输入第一个数据");
	scanf("%d",&head->data);
	head->next=NULL;
	ptr=head;
	for(i=1;i<6;i++)
	{
		p=(LinkNode*)malloc(sizeof(LinkNode));
		if(!p)
		{
			printf("内存分配失败\n");
			exit(1);
		}
		printf("输入第%d个数据",i+1);
		scanf("%d",&p->data);
		p->next=NULL;
		ptr->next=p;
		ptr=ptr->next;
	}
	return head;
}

//遍历链表
LinkNode *findnode(LinkNode *head,int num)
{
	LinkNode *ptr;
	ptr=head;
	while(ptr!=NULL)
	{
		if(ptr->data==num) return ptr;
		ptr=ptr->next;
	}
	return ptr;
}

//插入链表节点
LinkNode *insertnode(LinkNode *head,LinkNode *ptr,int value)
{
	LinkNode *newnode=(LinkNode*)malloc(sizeof(LinkNode));
	if(!newnode) return NULL;
	newnode->data =value;
	newnode->next = NULL;
	if(ptr==NULL)
	{
		newnode->next=head;
		return newnode;
	}
	else
	{
		if(ptr->next==NULL) ptr->next=newnode;
		else
		{
			newnode->next=ptr->next;
			ptr->next=newnode;
		}
	}
	return head;
}

//删除链表节点
LinkNode *deletenode(LinkNode *head,LinkNode *ptr)
{
	LinkNode *pre;
	if(ptr==head)
		return head->next;
	else
	{
	pre=head;
	while(pre->next!=ptr)
		pre=pre->next;
	if(ptr->next==NULL)
		pre->next=NULL;
	else
		pre->next=ptr->next;
	}
	free(ptr);//释放节点内存
	return head;
}

//输出链表
void printnode(LinkNode *ptr)
{
	while(ptr!=NULL)
	{
	printf("%d\t",ptr->data);
	ptr=ptr->next;
	}
	printf("\n");
}

// 释放链表的内存
void freelinknode(LinkNode *head)
{
	LinkNode *ptr;
	while(head!=NULL)
	{
		ptr=head;
		head=head->next;
		free(ptr);
	}
}

int main()
{
	int num,value;
	LinkNode *head,*ptr;
	head=creatlinknode();
	printnode(head);
	printf("输入要查找的数据\n");
	scanf("%d",&num);
	ptr=findnode(head,num);
	if(!ptr)
		printf("没有找到\n");
	else
	{
		printf("成功找到数据\n输入你要插入的数据\n");
		scanf("%d",&value);
		head=insertnode(head,ptr,value);
		printnode(head);
	}
	printf("输入要查找并删除的数据\n");
	scanf("%d",&num);
	ptr=findnode(head,num);
	if(!ptr){
		printf("没有找到数据\n");
	} else {
		printf("找到了\n");
		head=deletenode(head,ptr);
		printnode(head);
	}
	freelinknode(head);
	return 0;
}