#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct node
{
    int data;
    struct node *next;
    struct node *prev;
}Node_t;

void link_list_print_func(Node_t *head)
{
    Node_t *st_head = head;
    printf("link list print\n");
    while(st_head)
    {
        printf("%d ",st_head->data);
        st_head = st_head->next;
    }
    printf("\n");
}

Node_t *create_node(int data)
{
    Node_t *new_node = (Node_t *)malloc(sizeof(Node_t));
    if(NULL == new_node)
    {
        return NULL;
    }
    new_node->data = data;
    return (Node_t *)new_node;
}

static void insert_double_link_list(Node_t **head, int data)
{
    Node_t *new_node = create_node(data);
    Node_t *temp = *head; 
    if(temp == NULL)
    {
       *head = new_node;
        return;
    }
    temp->prev = new_node;
    new_node->next = temp;
    *head = new_node; 
}

static void tail_insert_double_link_list(Node_t **head,int data)
{
    Node_t *new_node = create_node(data);
    Node_t *temp = *head; 
    if(temp == NULL)
    {
        *head = new_node;
        return;
    }
    while(temp->next)
    {
        temp = temp->next;
    }
    temp->next = new_node;
    new_node->prev = temp;
}

void reversal_printf(Node_t *head)
{
    Node_t *temp = head;
    printf("reversal printf double link list:");
    while(temp->next)
    {
        temp = temp->next;
    }
    while(temp->prev)
    {
        printf("%d ",temp->data);
        temp = temp->prev;
    }
    printf("\n");

}

void double_link_list_process(void)
{
    Node_t *head = NULL;
    insert_double_link_list(&head, 5);
    insert_double_link_list(&head, 3);
    insert_double_link_list(&head, 7);
    insert_double_link_list(&head, 9);
    insert_double_link_list(&head, 100);

    link_list_print_func(head);
    tail_insert_double_link_list(&head, 200);
    tail_insert_double_link_list(&head, 300);
    tail_insert_double_link_list(&head, 400);
    tail_insert_double_link_list(&head, 500);
    link_list_print_func(head);
    reversal_printf(head);

}
