#include <stdio.h>
#include "soc_osal.h"
#include "osal_debug.h"
#include "platform_types.h"
#include "gpio.h"
#include "hal_gpio.h"
#include "pinctrl.h"
#include "tcxo.h"
#include "hal_pwm.h"
#include "hal_pwm_v151.h"
#include "osal_timer.h"
// #include "../byt_peripheral/byt_peripheral.h"
#include "cmsis_os2.h"
#include "byt_peripheral_process.h"
#include "../main_config.h"

typedef uint8_t tree_queue_t;

typedef struct t_node
{
    int data;
    struct t_node *left;
    struct t_node *right;
}tree_node_t;


tree_node_t *create_tree_node(int data)
{
    tree_node_t *new = (tree_node_t *)malloc(sizeof(tree_node_t));
    if(NULL == new)
    {
        return NULL;
    }
    new->data = data;
    new->left = new->right = NULL;
    return (tree_node_t *)new;
}

tree_node_t *insert_tree_node(tree_node_t *root, int data)
{
    tree_node_t *stroot = root;
    if(NULL == stroot)
    {
        tree_node_t *new_node = create_tree_node(data);
        stroot = new_node;
    }
    else if(stroot->data <= data)
    {
        stroot->left = insert_tree_node(stroot->left, data);
    }
    else if(stroot->right > data)
    {
        stroot->right = insert_tree_node(stroot->right, data);
    }

    return stroot;
}

bool search_tree_node(tree_node_t *root,int data)
{
    tree_node_t *stroot = root;
    if(NULL == stroot)
    {
        return false;
    } 
    if(data == stroot->data)    return true;
    if(data <= stroot->data)    return search_tree_node(stroot->left, data);
    else if(data > stroot->data)    return search_tree_node(stroot->right, data);
}

int find_height(struct tree_node_t *root)
{
    if(root == NULL)
        return -1;
    return ;
}

typedef struct q_node
{
    tree_queue_t *data;
    uint16_t data_len;
    struct q_node *next;
}queue_node_t;

typedef struct  queue_list
{
    queue_node_t *front;
    queue_node_t *rear;
}queue_list_t;



queue_node_t *queue_new_node(uint8_t *data, uint16_t data_len)
{
    uint8_t *new_data = (uint8_t *)malloc(data_len);
    if(new_data == NULL)
    {
        return NULL;
    }
    queue_node_t *new_node = (queue_node_t *)malloc(sizeof(queue_node_t));
    if(new_node == NULL)
    {
        return;
    }
    new_node->data = new_data;
    new_node->data_len = data_len;
    new_node->next = NULL;
}

queue_list_t *new_queue_list(void)
{
    queue_list_t *new_node = (queue_list_t *)malloc(sizeof(queue_list_t)); 
    queue_node_t *tem_data = (queue_node_t *)malloc(sizeof(queue_node_t));
    new_node->front = new_node->rear = tem_data;
    new_node->rear->next = NULL;
    return (queue_list_t *)new_node;   
}

// int queue_list_enqueue()

// void binary_search_tree_process(void)
// {
//     tree_node_t *root = NULL;
//     printf("insert the data\n");
//     root = insert_tree_node(root, 100);
//     root = insert_tree_node(root, 50);
//     root = insert_tree_node(root, 40);
//     root = insert_tree_node(root, 200);
//     root = insert_tree_node(root, 150);
//     root = insert_tree_node(root, 220);
//     root = insert_tree_node(root, 60);
//     printf("search data rinning\n");

//     printf("search data 50: %d",search_tree_node(root, 50));
//     printf("search data 70: %d",search_tree_node(root, 70));
//     printf("search data 160: %d",search_tree_node(root, 160));
//     printf("search data 200: %d",search_tree_node(root, 200));

// }



