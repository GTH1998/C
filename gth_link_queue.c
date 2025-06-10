#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

typedef void* stLINK_QUEUE_T;

typedef struct g_node
{
    uint8_t  *data;
    uint16_t datalen;
    struct g_node_t *next;
}g_node_t;

typedef struct g_link_list
{
    g_node_t *front;
    g_node_t *rear;
}LINK_QUEUE_T;

LINK_QUEUE_T *create_link_queue(void)
{
    LINK_QUEUE_T *new_queue = (LINK_QUEUE_T *)malloc(sizeof(LINK_QUEUE_T));
    if(NULL == new_queue)
    {
        return NULL;
    }
    g_node_t *new_node = (g_node_t *)malloc(sizeof(g_node_t)); 
    new_queue->front = new_queue->rear = new_node;
    new_queue->rear->next = NULL;
    return (LINK_QUEUE_T *)new_queue;
}

int link_queue_enqueue(stLINK_QUEUE_T head,uint8_t *data,uint16_t data_len )
{
    LINK_QUEUE_T *sthead = (LINK_QUEUE_T *)head;
    g_node_t *new_node = (g_node_t *)malloc(sizeof(g_node_t));
    if(NULL == new_node)
    {
        return 1;
    }
    new_node->next = NULL;
    new_node->datalen = data_len;
    new_node->data = (uint8_t *)malloc(data_len);
    if(NULL == new_node->data)
    {
        return 1;
    }
    memcpy(new_node->data, data, data_len);
    sthead->rear->next = new_node;
    sthead->rear = new_node;
    return 0; 
}

int link_queue_dequeue(stLINK_QUEUE_T head, uint8_t *data, uint16_t data_len)
{
    LINK_QUEUE_T *sthead = (LINK_QUEUE_T *)head;
    g_node_t *temp = sthead->front->next;
    if(NULL == sthead->front->next )
    {
        printf("queue is empty\n");
        return 1;
    }
    if(data_len > temp->datalen)
    {
        data_len = temp->datalen;
    }
    memcpy(data, temp->data, data_len);
    sthead->front->next = temp->next;
    free(temp->data);
    free(temp);
}

uint16_t link_queue_data_peek(stLINK_QUEUE_T head,uint8_t **data)
{
    LINK_QUEUE_T *sthead = (LINK_QUEUE_T *)head;
    g_node_t *temp = NULL;
    if(sthead == NULL || data == NULL)
    {
        return 0;
    }
    temp = sthead->front->next;
    if(temp == NULL)
    {
        return 0;
    }
    *data = temp->data;
    return temp->datalen;
}

int link_queue_clear(stLINK_QUEUE_T head)
{
    LINK_QUEUE_T *sthead = (LINK_QUEUE_T *)head;
    g_node_t *temp = NULL;
    if(NULL == sthead)
    {
        return;
    }

    temp = sthead->front->next;

    while(temp)
    {
        sthead->front->next = temp->next;
        free(temp->data);
        free(temp);
        temp = sthead->front->next; 
    }
    sthead->front = sthead->rear;
    sthead->front->next = NULL;
    return 0;
}
int link_queue_desitory(stLINK_QUEUE_T head)
{
    printf("destroy the link queue\n");
    LINK_QUEUE_T *sthead = (LINK_QUEUE_T *)head;
    g_node_t *temp = NULL;
    if(NULL == sthead)
    {
        return 1;
    } 
    temp = sthead->front->next;
    while(temp)
    {
        sthead->front->next = temp->next;
        free(temp->data);
        free(temp);
        temp = sthead->front->next;
    }
    printf("FFFFF1\n");
    // free(sthead->front);
    printf("FFFFF2\n");

    free(sthead);
    printf("FFFFF3\n");

    return 0;
}

bool queue_is_empty(stLINK_QUEUE_T head)
{
    LINK_QUEUE_T *sthead = (LINK_QUEUE_T *)head;
    if(NULL == sthead)
    {
        return true;
    }
    if(NULL == sthead->front->next)
    {
        return true;
    }
    return false;
}

uint16_t get_front_data_size(stLINK_QUEUE_T head)
{
    LINK_QUEUE_T *sthead = (LINK_QUEUE_T *)head;
    g_node_t *temp = sthead->front->next;
    uint16_t data_len = 0;
    if(sthead == NULL || temp == NULL)
    {
        return 0;
    }
    data_len = temp->datalen;
    return data_len;
}
int gth_link_queue_printf(stLINK_QUEUE_T head)
{
    LINK_QUEUE_T *sthead = head;
    g_node_t *temp = sthead->front->next;
    uint8_t queue_node_number = 0;
    if(sthead == NULL || temp == NULL)
    {
        return 1;
    }
    while(temp)
    {
        printf("queue printf %d:",++queue_node_number);
        for(uint16_t i = 0; i < temp->datalen; i++)
        {
            printf("%d ",temp->data[i]);
        }
        printf("\n");
        temp = temp->next;
    }
    return 0;
}


void sample_printf(uint8_t *data, uint16_t data_len)
{
    for(uint16_t i = 0; i < data_len; i++)
    {
        printf("%d ",data[i]);
    }
    printf("\n");
}
void gth_link_queue_process_func(void)
{
    uint8_t array[10] = {0};
    uint8_t array1[30] = {9,8,7,6,5,4,3,2,1};
    memset(&array1[8], 10, 10);
    uint8_t array2[] = {4,5,6,7,8,9,10,25,0};
    uint8_t array3[] = {10,8,7,6,5,4,3,2,1};
    uint8_t array4[] = {30,8,7,6,5,4,3,2,1};

    LINK_QUEUE_T *head = create_link_queue();
    printf("queue is empty:%d \n",queue_is_empty(head));
    printf("gth link queue process func: enqueue 1\n");
    link_queue_enqueue(head, array1, sizeof(array1));
    gth_link_queue_printf(head);
    printf("gth link queue process func: enqueue 2\n");
    printf("queue length is %d\n",get_front_data_size(head));


    link_queue_enqueue(head, array2, sizeof(array2));
    gth_link_queue_printf(head);

    printf("gth link queue process func: enqueue 3\n");

    link_queue_enqueue(head, array3, sizeof(array3));
    gth_link_queue_printf(head);

    printf("queue length is %d\n",get_front_data_size(head));
    printf("gth link queue process func: enqueue 4\n");


    link_queue_enqueue(head, array4, sizeof(array4));
    gth_link_queue_printf(head);
    printf("queue is empty:%d \n",queue_is_empty(head));

    
    printf("queue length is %d\n",get_front_data_size(head));

    link_queue_clear(head);
    printf("queue length is %d\n",get_front_data_size(head));


    printf("gth link queue process func: dequeue 1\n");
    link_queue_dequeue(head, array, sizeof(array));
    sample_printf(array, sizeof(array));

     printf("gth link queue process func: dequeue 2\n");
    link_queue_dequeue(head, array, sizeof(array));
    sample_printf(array, sizeof(array));

     printf("gth link queue process func: dequeue 3\n");
    link_queue_dequeue(head, array, sizeof(array));
    sample_printf(array, sizeof(array));

    printf("gth link queue process func: dequeue 4\n");
    link_queue_dequeue(head, array, sizeof(array));
    sample_printf(array, sizeof(array));

    printf("gth link queue process func: dequeue 5\n");
    link_queue_dequeue(head, array, sizeof(array));
    sample_printf(array, sizeof(array));

    printf("queue is empty:%d \n",queue_is_empty(head));

     printf("gth link queue process func: enqueue 1\n");
    link_queue_enqueue(head, array1, sizeof(array1));
    gth_link_queue_printf(head);

     printf("gth link queue process func: enqueue 1\n");
    link_queue_enqueue(head, array2, sizeof(array2));
    gth_link_queue_printf(head);

     printf("gth link queue process func: enqueue 1\n");
    link_queue_enqueue(head, array3, sizeof(array3));
    gth_link_queue_printf(head);

    link_queue_desitory(head);

    gth_link_queue_printf(head);

}
