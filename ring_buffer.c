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

typedef  uint8_t ring_buff_data ;
#define  RING_BUFFER_SIZE    16
typedef struct r_node
{
    ring_buff_data *data;
    uint16_t ring_buff_data_size;
    
    int read;
    int write;
    int data_len;
}r_node_t;

r_node_t *ring_buffer_init(void)
{
    r_node_t *new_node = (r_node_t *)malloc(sizeof(r_node_t));
    
    if(NULL == new_node)
    {
        printf("new node create is failer\n");
        return NULL;
    }
    new_node->data = (ring_buff_data *)malloc(RING_BUFFER_SIZE*sizeof(ring_buff_data));
    if(NULL == new_node->data)
    {
        printf("new data create is failer\n");
        return NULL;
    }
    new_node->ring_buff_data_size = RING_BUFFER_SIZE;
    new_node->data_len = 0;
    new_node->read = new_node->write = 0;
    return (r_node_t *)new_node;
}

int ring_buffer_write_data(r_node_t *head, ring_buff_data *data, uint16_t data_len)
{
    r_node_t *sthead = head;
    if(sthead == NULL)
    {
        printf("ring buffer is NULL\n");
        return 1;
    }
    if(sthead->data_len == sthead->ring_buff_data_size)
    {
        printf("ring buffer is full\n");
        return 1;
    }else if(0 == data_len)
    {
        printf("write data is empty\n");
        return 1;
    }
    int free_space = 0;
    if(sthead->write >= sthead->read)
    {
        //data space is two
        // free_space = sthead->write - sthead->read -1;
        free_space = (sthead->ring_buff_data_size - sthead->write) + (sthead->read - 1);
    }
    else
    {
        free_space = sthead->read - sthead->write - 1; // data space is one
    }

    int need_write_len = data_len > free_space ? free_space : data_len;
    printf("F1\n");
    if(sthead->write >= sthead->read)
    {

        if(need_write_len >= (sthead->ring_buff_data_size - sthead->write ))
        {
            // write data size over the data buffer,divition write two 
            printf("F2\n");

            memcpy(&sthead->data[sthead->write], data, sthead->ring_buff_data_size - sthead->write);
            printf("F3\n");

            memcpy(&sthead->data, &data[sthead->ring_buff_data_size - sthead->write - 1],
                                    need_write_len - (sthead->ring_buff_data_size - sthead->write ));

            sthead->write = (sthead->write + data_len) % sthead->ring_buff_data_size;

            sthead->data_len =   need_write_len;          
        }
        else
        {
            printf("F4\n");
            printf("write length:%d, read length:%d;\n",sthead->write, sthead->read);
            memcpy(&sthead->data[sthead->write], data, need_write_len);

            sthead->write = ((sthead->write + need_write_len) % sthead->ring_buff_data_size); 
            sthead->data_len =   need_write_len;          

        }
    }
    else
    {
        printf("F5\n");
        memcpy(&sthead->data[sthead->write], data, data_len);
        sthead->write += data_len;
        sthead->data_len = data_len;
    }
            printf("F6\n");
    
    return 0;
}


int ring_buffer_read_data(r_node_t *head,ring_buff_data *data, uint16_t data_len)
{
    r_node_t *sthead = head;
    if(NULL == sthead)
    {
        printf("ring buffer read data head is NULL\n");
        return 1;
    }
    // if(data_len > sthead->ring_buff_data_size )
    // {
    //     printf("data len > ring buffer size\n");
    //     data_len = sthead->ring_buff_data_size;
    // }
    int read_buffer_size = 0;
    int read_data_len = 0;

    if(sthead->write >= sthead->read)
    {
        read_buffer_size = sthead->write - sthead->read ;
        if(data_len < read_buffer_size)
        {
            read_data_len = data_len;
        }
        else
        {
            read_data_len = read_buffer_size;
        }

        memcpy(data, sthead->data[sthead->read], read_data_len);
        sthead->read += read_data_len;
    }
    else
    {
        // read_buffer_size = (sthead->ring_buff_data_size - sthead->read) + (sthead->write - 1); 
        if((sthead->ring_buff_data_size - sthead->read) < data_len)
        {
            memcpy(data, sthead->data[sthead->read], data_len);
            sthead->read += data_len;
        }
        else
        {
            memcpy(data, sthead->data[sthead->ring_buff_data_size - sthead->read], sthead->ring_buff_data_size - sthead->read);
            memcpy(data[sthead->ring_buff_data_size - sthead->read], sthead->data[0],
                         (sthead->ring_buff_data_size + data_len - 1 )%sthead->ring_buff_data_size);
            sthead->read = (sthead->ring_buff_data_size + data_len - 1)%sthead->ring_buff_data_size;

        }
    }
    
}

void ring_buffer_clear_buffer(r_node_t *head)
{
    r_node_t *sthead = head;
    printf("clear ring buffer func\n");
    if(NULL == sthead)
    {
        printf("ring buffer is NULL\n");
        return ;
    }
    if(sthead->data == NULL)
    {
        printf("ring buffer data is NULL\n");
        return ;
    }
    sthead->write = sthead->read = 0;
    memset(sthead->data, 0, sthead->ring_buff_data_size);
}
void ring_buffer_destory(r_node_t *head)
{
    r_node_t *sthead = head;
    if(NULL == sthead)
    {
        printf("ring buffer is NULL\n");
        return;
    }
    free(sthead->data);
    free(sthead);
}

void ring_buffer_printf(r_node_t *head)
{
    r_node_t *sthead = head;
    printf("ring_buffer_printf: read: %d, write%d\n",sthead->read, sthead->write);
    if(sthead->write > sthead->read)
    {
        for(uint8_t i = sthead->read; i < sthead->write; i++)
        {
            printf("%d ",sthead->data[i]);
        }   
    }
    else
    {
        for(uint8_t i = sthead->read; i < (sthead->ring_buff_data_size - (sthead->read) + sthead->write + 1); i++)
        {
            printf("%d ",sthead->data[(sthead->read + i)%sthead->ring_buff_data_size]);
        }
    }
   printf("\n");
}

uint8_t array_buffer_in_data(uint8_t *data, uint16_t data_len, uint8_t off_set)
{
    for(uint8_t i = 0; i < data_len; i++)
    {
        data[i] = i + off_set;
    }
}

void ring_buffer_process_func(void)
{
    printf("ring buffer process func\n");
    r_node_t *head = ring_buffer_init();
    uint8_t array[5] = {0};
    uint8_t read_array[3];
    uint8_t array1[14] = {0};
    array_buffer_in_data(array1, sizeof(array1), 40);
    for(uint8_t i = 0; i < 5; i ++)
    {
        array[i] = i + 10;
    }

    ring_buffer_write_data(head, array, sizeof(array));
    ring_buffer_printf(head);
    ring_buffer_read_data(head, read_array,sizeof(read_array));
    ring_buffer_printf(head);

    ring_buffer_clear_buffer(head);
    ring_buffer_printf(head);

    ring_buffer_write_data(head, array, sizeof(array));
    ring_buffer_printf(head);

    ring_buffer_write_data(head, array1, sizeof(array1));
    ring_buffer_printf(head);

}
