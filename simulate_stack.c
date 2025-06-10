#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STACK_NUMBER  1024
int top_point = -1;
int stack_array[STACK_NUMBER];

typedef struct nnode
{
    int data;
    struct nnode *next;
}S_Node_t;

void push_array_stack(int x)
{
    if(++top_point == 1024)
    {
        top_point--;
        printf("stack overflow\n");
        return;
    }
    stack_array[top_point] = x;
}
void pop_array_stack(void)
{
    if(top_point == -1)
    {
        printf("array stack is empty\n");
        return;
    }
    top_point--;
}

void array_stack_printf(void)
{
    int temp = top_point;
    printf("array stacl printf:");
    while(temp >= 0)
    {
        printf("%d ",stack_array[temp]);
        temp--;
    }
    printf("\n");
}
void reversal_the_array_simulate_stack(void)
{
    int temp_point = top_point;
    int temp = 0;
    for(int i = 0; i < temp_point/2; i++)
    {
        temp = stack_array[temp - i];
        stack_array[temp - i] = stack_array[i];
        stack_array[i] = temp;
    }
}


void stack_printf(S_Node_t *head)
{
    S_Node_t *temp = head;
    printf("stack print:");
    while(temp->next)
    {
        printf("%d ",temp->data);
        temp = temp->next;
    }
    printf("\n");
}



void array_simulate_stack_process_func(void)
{
    push_array_stack(3);
    array_stack_printf();

    push_array_stack(5);
    array_stack_printf();

    push_array_stack(6);
    array_stack_printf();

    push_array_stack(17);
    array_stack_printf();

    push_array_stack(90);
    array_stack_printf();

    // pop_array_stack();
    // array_stack_printf();
    // pop_array_stack();
    // array_stack_printf();
    // pop_array_stack();
    // array_stack_printf();
    printf("reversal the array stack\n");
    reversal_the_array_simulate_stack();
    array_stack_printf();

}



S_Node_t *create_node(int data)
{
    S_Node_t *new_node = (S_Node_t *)malloc(sizeof(S_Node_t));
    new_node->data = data;
    new_node->next = NULL;
    return (S_Node_t *)new_node;    
}

void link_list_simulate_stack_push(S_Node_t **head, int data)
{
    S_Node_t *new_node = create_node(data);
    S_Node_t *temp = *head;
    new_node->next = temp;
    *head = new_node;

}

void link_list_simulate_stack_pop(S_Node_t **head)
{
    S_Node_t *temp = *head;
    // S_Node_t *temp2 = NULL;
    if(NULL == temp)
    {
        printf("stack is empty\n");
        return;
    }
    (*head) = temp->next;
    free(temp);
}

void reversal_link_list_simulate_stack(S_Node_t **head)
{
    // S_Node_t *sthead = *head;
    S_Node_t *prev, *current, *next;
    current = *head;
    prev = NULL;
    next = current->next;
    int timer = 0;
    while(current)
    {
        printf("%d ",timer++);
        // temp = sthead;
        current->next = prev;
        // if(next == NULL)
        // {
        //     break;

        // }
        prev = current;
        current = next;
        if(current == NULL)
        {
            printf("current FFF\n");
            break;
        }
        
        next = next->next;
    }
    *head = prev;

}
void link_list_simulate_printf(S_Node_t *head)
{
    S_Node_t *sthead = head;
    printf("link list simulate printf:");
    while(sthead)
    {
        printf("%d ", sthead->data);
        sthead = sthead->next;
    }
    printf("\n");
}
void link_list_simulate_process_func(void)
{
    S_Node_t *head = NULL;
    link_list_simulate_stack_push(&head, 30);
    link_list_simulate_printf(head);

    link_list_simulate_stack_push(&head, 32);
    link_list_simulate_printf(head);
    link_list_simulate_stack_push(&head, 40);
    link_list_simulate_printf(head);
    link_list_simulate_stack_push(&head, 100);
    link_list_simulate_printf(head);
    link_list_simulate_stack_push(&head, 500);
    link_list_simulate_printf(head);    

    printf("reversal the stack\n");
    reversal_link_list_simulate_stack(&head);
    link_list_simulate_printf(head);    

    // link_list_simulate_stack_pop(&head);
    // link_list_simulate_printf(head);    

    // link_list_simulate_stack_pop(&head);
    // link_list_simulate_printf(head);    

    // link_list_simulate_stack_pop(&head);
    // link_list_simulate_printf(head);    
}

void no_name_func(void)
{
    printf("( : %d\n",'(');
    printf(") : %d\n",')');

    printf("[ : %d\n",'[');
    printf("] : %d\n",']');

    printf("{ : %d\n",'{');
    printf("} : %d\n",'}');

}

void bracket_process_func(void)
{
    
}

void stack_process_func(void)
{
    printf("stack_process_func\n");
    // array_simulate_stack_process_func();
    // link_list_simulate_process_func();
    no_name_func();
}