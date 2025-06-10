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

typedef uint8_t ring_buf_t;
typedef void *ring_buffer_head_t;
#define BUFF_MAX_LEN 16

// ring buffer node define
typedef struct r_node
{
    ring_buf_t *data;
    uint16_t data_len;
    struct r_node *next;
    
}r_node_t;


//ring buffer point defination ;
typedef struct rw_point
{
    r_node_t *p_read;
    r_node_t *p_write;
    r_node_t *p_head;
    r_node_t *p_tail;
}rw_point_t;


/**********************************************************************************************************************
 * @brief  init the ring buffer
 * @param  ring_buf_len: ring buffer data size.
 * @output
 * @return ring buffer point.
 *********************************************************************************************************************/
rw_point_t *ring_buffer_init(uint8_t ring_buf_len)
{
    r_node_t *head = (r_node_t *)malloc(sizeof(r_node_t) * ring_buf_len);
    r_node_t *temp = head;
    r_node_t *temp1 = head;
    printf("head: 0x%x;head + 1: 0x%x;r_node_t size: %d\n", head, (head + 1), sizeof(r_node_t));
    if(head == NULL)
    {
        return NULL;
    }
    printf("A\n");
    rw_point_t *rw_head = (rw_point_t *)malloc(sizeof(rw_point_t));
    if(NULL == rw_head)
    {
        return NULL;
    }
    printf("B\n");

    rw_head->p_head = rw_head->p_read = rw_head->p_write = (r_node_t*)head;
    rw_head->p_tail = (head + ring_buf_len);
    for(uint8_t i = 0; i < ring_buf_len; i ++)
    {
        printf("C %d\n", i);

        temp1->next = (temp1 + 1);
        
        if(i == (ring_buf_len - 1))
        {
            temp1->next = temp; 
        } 
        else
        {
            temp1= temp1->next;
        }
    }
    printf("D\n");
    return rw_head;
}
/****************************************************************************************************
 *@brief ring buffer insert data 
 *@param data: insert buffer data
 *@param data_len: insert buffer data length
 *@return ring buffer insert node
****************************************************************************************************/


r_node_t *ring_buf_insert_data(ring_buf_t *data, uint16_t data_len)
{
    if(NULL == data || data_len == 0)
    {
        printf("data is 0\n");
        return NULL;
    }
    ring_buf_t *new_node = (ring_buf_t *)malloc(sizeof(ring_buf_t) * data_len);
    if(NULL == new_node)
    {
        printf("malloc is NULL\n");
        return NULL;
    }
}
/***********************************************************************************************
 * @brief ring buffer put data in the buffer
 * @param head: ring buffer handle
 * @param data: point to data buffer
 * @param data_len: put data in the ring buffer data length
 * @output
 * @return : None
 * **********************************************************************************************/
void ring_buffer_enqueue(ring_buffer_head_t head, ring_buf_t *data, uint16_t data_len)
{
    rw_point_t *p_head = (rw_point_t *)head;
    if(NULL == p_head)
    {
        printf("enqueue head is NULL\n");
        return ;
    }
    ring_buf_t *tem_data = (ring_buf_t *)malloc(sizeof(ring_buf_t) * data_len);
    if(NULL == tem_data)
    {
        printf("enqueue malloc error\n");
        return ;
    }

    printf("write add: 0x%x,read add: 0x%x\n",p_head->p_write,p_head->p_read);
    if(p_head->p_write == p_head->p_read)
    {
        
        if(p_head->p_write->data != NULL)
        {
            printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF: 0x%x\n",p_head->p_write->data);
            free(p_head->p_read->data);
            p_head->p_read = p_head->p_read->next; 
            
        }
        
    }
    
    printf("C\n");
    p_head->p_write->data = tem_data;
    p_head->p_write->data_len = data_len;
    memcpy(p_head->p_write->data, data, data_len);
    p_head->p_write = p_head->p_write->next;
 
}
/******************************************************************************************************
 * @brief ring buffer get data and remove data from ring buffer
 * @param head: ring buffer handle
 * @param data: get data from ring buffer
 * @param data_len: get data length
 * @output
 * @return None
 ******************************************************************************************************/
void ring_buffer_dequeue(ring_buffer_head_t head, ring_buf_t *data, uint16_t data_len)
{
    rw_point_t *p_head = (rw_point_t *)head;
    if(NULL == head || data == NULL || data_len == 0)
    {
        printf("dequene is error\n");
        return ;
    }
    if(p_head->p_read == p_head->p_write)
    {
        if(p_head->p_read->data == NULL)
        {
            printf("ring buffer is NULl\n");
            return;
        }
        else
        {
            if(data_len > p_head->p_read->data_len)
            {
                data_len = p_head->p_read->data_len;
            }
            memcpy(data, p_head->p_read->data,data_len);
            free(p_head->p_read->data);
            p_head->p_read = p_head->p_read->next;
            return;
        }
    }

    if(data_len > p_head->p_read->data_len)
        data_len = p_head->p_read->data_len;

    memcpy(data, p_head->p_read->data, data_len);
    free(p_head->p_read->data);
    p_head->p_read = p_head->p_read->next;

}
/********************************************************************************************************
 *@brief printf ring buffer all node data  
 *@param head：ring buffer Handle
 *@output
 *@return None
 *********************************************************************************************************/
void ring_buffer_printf(ring_buffer_head_t head)
{
    if(head == NULL)
        return;
    rw_point_t *phead = (rw_point_t *)head;
    r_node_t *temp = phead->p_read;
    uint8_t buffer_len = 0;
    uint8_t index = 0;
    if(phead == NULL)
    {
        printf("ring buffer is NULL\n");
        return ;
    }
    if(phead->p_read == phead->p_write)
    {
        if(phead->p_read->data == NULL)
        {
            return;
        }
        else
        {
            for(uint8_t i = 0;i < BUFF_MAX_LEN; i++)
            {
                printf("buffer index:%d : addr: 0x%x;",index, temp);
                index ++;
                for(uint16_t i = 0; i < temp->data_len; i++)
                {
                    printf("%d ",temp->data[i]);
                }
                printf("\n");
                temp = temp->next;
            }

            return;
        }
    }
    while(temp != phead->p_write)
    {
        printf("buffer index:%d : addr: 0x%x;",index, temp);
        index ++;
        for(uint16_t i = 0; i < temp->data_len; i++)
        {
            printf("%d ",temp->data[i]);
        }
        printf("\n");
        temp = temp->next;
    }
}
/**********************************************************************************************
 *@brief printf 
 * 
 * 
 * 
 ***********************************************************************************************/

void printf_ring_buffer(ring_buffer_head_t *head)
{
    printf("printf ring buffe:\n");
    rw_point_t *phead = (rw_point_t *)head;
    r_node_t *temp_read = phead->p_read; 
    uint8_t idx = 0;
    if(phead == NULL)
    {
        return;
    }
    while(temp_read)
    {
        printf("read addr: 0x%x ,idx %d;",temp_read, idx);
        for(uint8_t i = 0; i < temp_read->data_len; i++)
        {
            printf("%d ",temp_read->data[i]);
        }
        printf("\n");
        idx++;
        if(temp_read == phead->p_write)
        {
            return;
        }
        temp_read = temp_read->next;
    }

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
/****************************************************************************************************************
 *@brief slow and fast point check the link list is ring 
 *@param head: link list Headle
 *@output
 *@return false : link list is not ring , ture : link list is ring
 *****************************************************************************************************************/
bool check_link_list_is_ring(ring_buffer_head_t head)
{
    rw_point_t *phead = (rw_point_t *)head;
    r_node_t *k_point = phead;
    r_node_t *m_point = phead;
    while(1)
    {
        k_point = (k_point->next->next);
        if(k_point->next == NULL || k_point == NULL)
        {
            printf("ring false\n");
            return false;
        }
        m_point = (m_point->next);
        if(m_point == k_point)
        {
            printf("ring true\n");
            return true;
        }
    } 
}

/**********************************************************************************************************
 *@brief check ring buffer addr 
 *@param head: ring buffer Handle
 *@output
 *@return None
 ***********************************************************************************************************/

void check_ring_buffer(ring_buffer_head_t head)
{
    printf("check ring buffer\n");
    rw_point_t *phead = (rw_point_t *)head;
    r_node_t *temp = phead->p_write; 
    printf("head addr:0x%x, tail addr:0x%x\n",phead->p_head,phead->p_tail);
    for(uint8_t i = 0; i < 32; i++)
    {
        printf("index: %d;addr: 0x%x ,data NULL?: %d\n",i, temp->data,(temp->data == NULL ?  1 : 0));
        temp = temp->next;
    }

}
/*********************************************************************************************************
 * @brief get ring buffer read point data size
 * @param head: ring buffer Handle
 * @output
 * @return read node data size
/*******************************************************************************************************/

uint16_t get_ring_buffer_read_data_size(ring_buffer_head_t head)
{
    rw_point_t *phead = (rw_point_t *)head;
    if(NULL == phead)
    {
        printf("ring buffer is NULL\n");
        return 0;
    }
    if(phead->p_read <= 0)
    {
        return 0;
    }
    return phead->p_read->data_len;
}
/*************************************************************************************************
 *@brief clear ring buffer all data 
 *@param head: ring buffer Handle
 *output
 *@return false : buffer is NULL, true : ring buffer clear success
 **************************************************************************************************/
bool clear_ring_buffer(ring_buffer_head_t head)
{
    rw_point_t *phead = (rw_point_t *)head;
    uint8_t index = 0;
    if(NULL == phead)
    {
        printf("ring buffer is NULL\n");
        return false;
    }
    r_node_t *temp = phead->p_read;
    while(1)
    {
        printf("index : %d\n",index);
        index ++;
        temp->data_len = 0;
        free(temp->data);
        temp->data = NULL;
        printf("free node addr : 0x%x, data NULL ? :%d\n",temp->data, (temp->data == NULL ? 1 : 0));
        temp = temp->next;
        if(temp == phead->p_read)
        {
            phead->p_read = phead->p_write = phead->p_head;
            return true;
        }    
    }
}
/*********************************************************************************************/
bool destory_ring_buffer(ring_buffer_head_t *head)
{
    rw_point_t *phead = (rw_point_t *)*head;
    if(NULL == phead)
    {
        printf("ring buffer is NULL\n");
        return false;
    }
    r_node_t *temp = phead->p_read;

    while(1)
    {
        free(temp->data);
        temp = temp->next;
        if(temp == phead->p_read)
        {
            break;
        }
    }
    free(phead->p_head);
    free(phead);
    *head = NULL;
    return true;
}
void ring_buffer_process(void)
{
    rw_point_t *phead = ring_buffer_init(16);
    check_ring_buffer(phead);
    printf("check link list is ring :%d\n", check_link_list_is_ring(phead));
    uint8_t  data1[10] = {0};
    uint8_t data2[11] = {0};
    uint8_t data3[12] = {0};
    uint8_t data4[13] = {0};
    uint8_t get_data[20] = {0};
    memset(data1, 4, sizeof(data1));
    memset(data2, 5, sizeof(data2));
    memset(data3, 6, sizeof(data3));
    memset(data4, 7, sizeof(data4));
    printf("ring_buffer_process\n");

    ring_buffer_enqueue(phead, data1, sizeof(data1));
    ring_buffer_printf(phead);
    printf("********************************1\n");
    ring_buffer_enqueue(phead, data2, sizeof(data2));
    ring_buffer_printf(phead);
    printf("********************************2\n");
    ring_buffer_enqueue(phead, data3, sizeof(data3));
    ring_buffer_printf(phead);
    printf("********************************3\n");
    ring_buffer_enqueue(phead, data4, sizeof(data4));
    ring_buffer_printf(phead);
    printf("get ring buffer read size: %d\n",get_ring_buffer_read_data_size(phead));
    printf("********************************4\n");

    ring_buffer_enqueue(phead, data1, sizeof(data1));
    ring_buffer_printf(phead);
    printf("********************************5\n");
    ring_buffer_enqueue(phead, data2, sizeof(data2));
    ring_buffer_printf(phead);
    printf("********************************6\n");
    ring_buffer_enqueue(phead, data3, sizeof(data3));
    ring_buffer_printf(phead);
    printf("get ring buffer read size: %d\n",get_ring_buffer_read_data_size(phead));

    printf("********************************7\n");
    ring_buffer_enqueue(phead, data4, sizeof(data4));
    ring_buffer_printf(phead);

    printf("clear ring buffer\n");
    clear_ring_buffer(phead);
    check_ring_buffer(phead);
    ring_buffer_printf(phead);

    printf("********************************8\n");


     ring_buffer_enqueue(phead, data1, sizeof(data1));
    ring_buffer_printf(phead);
    printf("********************************9\n");
    ring_buffer_enqueue(phead, data2, sizeof(data2));
    ring_buffer_printf(phead);
    printf("********************************10\n");
    ring_buffer_enqueue(phead, data3, sizeof(data3));
    ring_buffer_printf(phead);
    printf("********************************10\n");
    ring_buffer_enqueue(phead, data4, sizeof(data4));
    ring_buffer_printf(phead);
    printf("********************************11\n");

     ring_buffer_enqueue(phead, data1, sizeof(data1));
    ring_buffer_printf(phead);
    printf("********************************12\n");

    ring_buffer_enqueue(phead, data2, sizeof(data2));
    ring_buffer_printf(phead);
    printf("********************************13\n");
    ring_buffer_enqueue(phead, data3, sizeof(data3));
    ring_buffer_printf(phead);
    // printf_ring_buffer(phead);
    printf("********************************14\n");
    ring_buffer_enqueue(phead, data4, sizeof(data4));
    ring_buffer_printf(phead);
    // printf_ring_buffer(phead);

      printf("********************************\n");
    ring_buffer_dequeue(phead, get_data, sizeof(get_data));
    ring_buffer_printf(phead);
    get_data_printf(get_data, sizeof(get_data) );
    printf("********************************\n");
    ring_buffer_dequeue(phead, get_data, sizeof(get_data));
    ring_buffer_printf(phead);
    get_data_printf(get_data, sizeof(get_data) );
    printf("********************************\n");


    printf("destory ring buffer\n");
    printf("head addr: 0x%x, write add: 0x%x, read addr: 0x%x\n",phead, phead->p_write, phead->p_read);
    destory_ring_buffer(&phead);
    printf("head addr: 0x%x, write add: 0x%x, read addr: 0x%x\n",phead, phead->p_write, phead->p_read);

    printf("********************************15\n");

     ring_buffer_enqueue(phead, data1, sizeof(data1));
    ring_buffer_printf(phead);

    // printf_ring_buffer(phead);

    printf("********************************16\n");
    ring_buffer_enqueue(phead, data2, sizeof(data2));
    ring_buffer_printf(phead);
    printf("********************************17\n");
    ring_buffer_enqueue(phead, data3, sizeof(data3));
    ring_buffer_printf(phead);
    printf("********************************18\n");
    ring_buffer_enqueue(phead, data4, sizeof(data4));
    ring_buffer_printf(phead);


    printf("*************dequeue*******************\n");
    ring_buffer_dequeue(phead, get_data, sizeof(get_data));
    ring_buffer_printf(phead);
    get_data_printf(get_data, sizeof(get_data) );

     printf("********************************\n");
    ring_buffer_dequeue(phead, get_data, sizeof(get_data));
    ring_buffer_printf(phead);
    get_data_printf(get_data, sizeof(get_data) );

     printf("********************************\n");
    ring_buffer_dequeue(phead, get_data, sizeof(get_data));
    ring_buffer_printf(phead);
    get_data_printf(get_data, sizeof(get_data) );


}