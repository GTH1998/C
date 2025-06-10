#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct node
{
    int data;
    struct node *next;
}Node_t;


void insert_head_func(Node_t **head, int data)
{
    Node_t *new_node = (Node_t *)malloc(sizeof(Node_t));
    if(NULL == new_node)
    {
        return;
    }
    new_node->data = data;

    new_node->next = *head;
    *head = new_node; 
}
void insert_tail_func(Node_t **head, int data)
{
    Node_t *new_node = (Node_t *)malloc(sizeof(Node_t));
    Node_t *temp = *head;
    if(NULL == new_node)
    {
        return ;
    }
    new_node->data = data;
    if(NULL == temp)
    {
        new_node->next = temp;
        temp = new_node;
        return;
    }
    while(NULL != temp->next)
    {
        temp = temp->next;
    }
    new_node->next = NULL;
    temp->next = new_node;
}

void insert_pos_func(Node_t **head, int data, int pos)
{
    Node_t *new_node = (Node_t *)malloc(sizeof(Node_t));
    Node_t *prev = NULL;
    Node_t *temp = *head;
    new_node->data = data;
    if(1 == pos)
    {
        new_node->next = temp;
        temp = new_node;
        return;
    }
    for(int i = 0; i < pos - 1; i++)
    {
        prev = temp;
        temp = temp->next; 
    }
    prev->next = new_node;
    new_node->next = temp;
}

void delete_pos_func(Node_t **head, int pos)
{
    
    Node_t *temp1 = *head;
    Node_t *temp2 = NULL;
    if(pos == 1)
    {
        temp2 = temp1;
        *head = temp1->next;
        free(temp1);
        return;
    }
    for(int i = 0; i < pos - 2; i++)
    {
        temp1 = temp1->next;
    }
    temp2 = temp1->next;
    *head = temp2->next;
    free(temp2);
}

void reversal_link_list(Node_t **head)
{
    Node_t *current, *prev , *next, *temp;
    temp = NULL;
    current = *head;
    prev = NULL;
    next = current->next;
    osal_printk("AAAAAA\n");
    while(1)
    {
        current->next = prev; 
        if(next == NULL)
        {
            break;
        }
        prev = current;
        current = next;
        next = next->next;
    }
    
    *head = current;

    // current = prve;

    osal_printk("BBBBBB\n");

}



// void delete_the_point(Node_t **head, Node_t pos)
// {
//     Node_t *sthead = (Node_t *)malloc(sizeof(Node_t));
// }
void link_list_printf(Node_t *head)
{
    Node_t *temp = head;
    printf("list all number:");
    while(temp != NULL)
    {
        printf("%d ",temp->data);
        temp = temp->next;
    }
    printf("\n");
}


void link_list_process(void)
{
    Node_t *head = NULL;
    printf("link list process\n");
    insert_head_func(&head, 4);
    insert_head_func(&head, 7);
    insert_head_func(&head, 2);
    insert_head_func(&head, 1);
    link_list_printf(head);
    insert_tail_func(&head, 16);
    insert_tail_func(&head, 20);
    insert_tail_func(&head, 150);
    insert_tail_func(&head, 114);


    link_list_printf(head);
    insert_pos_func(&head,1000, 3);
    insert_pos_func(&head,2000, 5);
    link_list_printf(head);
    delete_pos_func(&head,1);
    delete_pos_func(&head,3);
    link_list_printf(head);
    printf("reversal link list\n");
    reversal_link_list(&head);
    link_list_printf(head);

    printf("reversal link list\n");
    reversal_link_list(&head);
    link_list_printf(head);


}
