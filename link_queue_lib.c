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

typedef uint8_t link_queue_data_t;
typedef void *link_queue_head_t;


//link queue data struct define
typedef struct lq_node
{
    link_queue_data_t *data;
    uint16_t data_len;
    struct lq_node *next;
}lq_node_t;

typedef struct
{
    lq_node_t *front;
    lq_node_t *rear;
}link_queue_t;

lq_node_t *new_link_queue_node(link_queue_data_t *data, uint16_t data_len)
{
    lq_node_t *new_node = (lq_node_t *)malloc(sizeof(lq_node_t));
    if(NULL == new_node)
    {
        printf("malloc node is NULL\n");
        return;
    }

    new_node->data = (link_queue_data_t *)malloc(sizeof(link_queue_data_t) * data_len);
    memcpy(new_node->data , data, data_len);
    new_node->next = NULL;
    new_node->data_len = data_len;
    return (lq_node_t *)new_node;
}

link_queue_t *link_queue_init(void)
{
    link_queue_t *new_node = (link_queue_t *)malloc(sizeof(link_queue_t));
    if(NULL == new_node)
    {
        printf("malloc data is NULL\n");
        return NULL;
    } 
    lq_node_t *temp_node = (lq_node_t *)malloc(sizeof(lq_node_t));
    if(NULL == temp_node)
    {
        printf("malloc temp data is NULL\n");
        return NULL;
    }
    new_node->front = new_node->rear = temp_node;
    new_node->rear->next = NULL;
    return new_node;

}

int link_queue_enqueue(link_queue_head_t head,link_queue_data_t *data, uint16_t data_len)
{
    link_queue_t *phead = (link_queue_t *)head;
    if(head == NULL || data == NULL || data_len == 0)
    {
        return 1;
    }
    lq_node_t *temp = new_link_queue_node( data, data_len);
    printf("A\n");
    if(NULL == temp)
    {
        return 2;
    }
    printf("B\n");
    // if(phead->front->next == NULL)
    // {
    //     phead->front->next = temp;
    // }
    printf("rear : 0x%x; tem: 0x%x;\n",phead->rear, temp);
    phead->rear->next = temp;
    printf("C\n");

    phead->rear = temp;
    printf("d\n");

    
    return 0;
}

int link_queue_dequeue(link_queue_head_t *head, link_queue_data_t *data, uint16_t data_len)
{
    if(head == NULL || data == NULL || data_len == 0)
    {
        printf("dequeue is error\n");
        return 1;
    }
    link_queue_t *phead = (link_queue_t *)head;
    lq_node_t *temp = phead->front->next;
    if(NULL == temp)
    {
        return 2;
    }
    if(data_len > temp->data_len)
        data_len = temp->data_len;

    memcpy(data, temp->data, data_len);
    
    phead->front->next = temp->next;
    free(temp->data);
    free(temp);
    return 0;
}

int link_queue_clear(link_queue_head_t *head)
{
    link_queue_t *phead = (link_queue_t *)head;
    if(NULL == phead)
    {
        printf("link queue is null\n");
        return 1;
    }
    if(phead->rear == phead->front)
    {
        printf("link queue data is null\n");
        return 2;
    }
    lq_node_t *temp_node = phead->front->next;
    while(temp_node)
    {
        phead->front->next = temp_node->next;
        free(temp_node->data);
        free(temp_node);
        temp_node = phead->front->next;
    }
    phead->front = phead->rear;
    return 0;
}

void get_data_printf(uint8_t *buf, uint16_t buf_size)
{
    printf("get data:");
    for(uint16_t i = 0;i < buf_size; i++)
    {
        printf("%d ",buf[i]);
    }
    printf("\n");
}


int link_queue_printf(link_queue_head_t head)
{
   printf("p\n");
    link_queue_t *phead = (link_queue_t *)head;
    lq_node_t *temp = phead->front->next;

    if(NULL == head || temp == NULL)
    {
        printf("link queue is NULL\n");
        return 1;
    }
    uint8_t index = 0;
    while(temp)
    {
        printf("link queue node: %d:",index);
        index ++;
        get_data_printf(temp->data, temp->data_len);
        temp = temp->next;
    }
}
uint16_t get_link_queue_front_length(link_queue_head_t head)
{
    link_queue_t *phead = (link_queue_t *)head;
    if(phead == NULL || phead->front->next == NULL)
    {
        printf("link queue is NULL or data is NULL\n");
        return -1;
    }
    return phead->front->next->data_len;
}

bool link_queue_is_empty(link_queue_head_t head)
{
    link_queue_t *phead = (link_queue_t *)head;
    if(phead == NULL)
    {
        return true;
    }
    if(phead->front->next == NULL)
    {
        return true;
    }
    return false;
}

int link_queue_destroy(link_queue_head_t head)
{
    link_queue_t *phead = (link_queue_t *)head;
    if(NULL == phead)
    {
        printf("link queue is NULL\n");
        return 1;
    }
    lq_node_t *temp = phead->front->next;
    while(temp)
    {
        phead->front->next = temp->next;
        free(temp->data);
        free(temp);
        temp = phead->front->next;
    }
    free(phead->front);
    free(phead); 

}

void gth_link_queue_process(void)
{
    uint8_t array[10] = {0};
    uint8_t array1[11] = {0};
    uint8_t array2[12] = {0};
    uint8_t array3[13] = {0};
    uint8_t array4[14] = {0};
    uint8_t array5[15] = {0};
    uint8_t array6[16] = {0};
    uint8_t array7[17] = {0};
    uint8_t test_buf[30] = {0};
    
    memset(array, 2, sizeof(array));
    memset(array1, 3, sizeof(array1));
    memset(array2, 4, sizeof(array2));
    memset(array3, 5, sizeof(array3));
    memset(array4, 6, sizeof(array4));
    memset(array5, 7, sizeof(array5));
    memset(array6, 8, sizeof(array6));
    memset(array7, 9, sizeof(array7));
    link_queue_t *phead = link_queue_init();
    printf("link queue is empty: %d\n",link_queue_is_empty(phead) ? 1 : 0); 
    printf("link queue enqueue: front:0x%x,rear: 0x%x\n",phead->front, phead->rear);
    link_queue_enqueue(phead, array, sizeof(array));
    link_queue_printf(phead);
    link_queue_enqueue(phead, array1, sizeof(array1));
    link_queue_printf(phead);
    link_queue_enqueue(phead, array2, sizeof(array2));
    link_queue_printf(phead);
    link_queue_enqueue(phead, array3, sizeof(array3));
    link_queue_printf(phead);
    link_queue_enqueue(phead, array4, sizeof(array4));
    link_queue_printf(phead);
    link_queue_enqueue(phead, array5, sizeof(array5));
    link_queue_printf(phead);
    link_queue_enqueue(phead, array6, sizeof(array6));
    link_queue_printf(phead);
    link_queue_enqueue(phead, array7, sizeof(array7));
    link_queue_printf(phead);

    link_queue_enqueue(phead, array, sizeof(array));
    link_queue_printf(phead);
    link_queue_enqueue(phead, array1, sizeof(array1));
    link_queue_printf(phead);
    link_queue_enqueue(phead, array2, sizeof(array2));
    link_queue_printf(phead);
    printf("link queue is empty: %d\n",link_queue_is_empty(phead) ? 1 : 0); 

    printf("link queue get front data length: %d\n",get_link_queue_front_length(phead));

    printf("**********link queue dequeue**********\n");
    link_queue_dequeue(phead, test_buf, sizeof(test_buf));
    link_queue_printf(phead);
    printf("dequeue data :");
    get_data_printf(test_buf, sizeof(test_buf));
    link_queue_dequeue(phead, test_buf, sizeof(test_buf));
    link_queue_printf(phead);

    printf("dequeue data :");
    get_data_printf(test_buf, sizeof(test_buf));
    printf("link queue get front data length: %d\n",get_link_queue_front_length(phead));

    link_queue_dequeue(phead, test_buf, sizeof(test_buf));
    link_queue_printf(phead);
    printf("dequeue data :");
    get_data_printf(test_buf, sizeof(test_buf));
    link_queue_dequeue(phead, test_buf, sizeof(test_buf));
    link_queue_printf(phead);
    printf("dequeue data :");
    get_data_printf(test_buf, sizeof(test_buf));
    link_queue_dequeue(phead, test_buf, sizeof(test_buf));
    link_queue_printf(phead);
    printf("dequeue data :");
    get_data_printf(test_buf, sizeof(test_buf));
    link_queue_dequeue(phead, test_buf, sizeof(test_buf));
    link_queue_printf(phead);
    printf("dequeue data :");
    get_data_printf(test_buf, sizeof(test_buf));

    link_queue_dequeue(phead, test_buf, sizeof(test_buf));
    link_queue_printf(phead);
    printf("dequeue data :");
    get_data_printf(test_buf, sizeof(test_buf));
    link_queue_dequeue(phead, test_buf, sizeof(test_buf));
    link_queue_printf(phead);
    printf("dequeue data :");
    get_data_printf(test_buf, sizeof(test_buf));
    printf("clear link queue\n");
    printf("link queue get front data length: %d\n",get_link_queue_front_length(phead));
    link_queue_destroy(phead);
    link_queue_printf(phead);
    printf("link queue is empty: %d\n",link_queue_is_empty(phead) ? 1 : 0); 


}






