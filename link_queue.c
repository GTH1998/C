// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>


// typedef struct linkqueue_node
// {
//     int data;
//     struct linkqueue_node *next;
// }linkqueue_node_t;

// typedef struct LINK_QUEUE
// {
//     linkqueue_node_t *front;
//     linkqueue_node_t *rear;
// }link_queue_t;

// link_queue_t *link_queue_create(void)
// {
//     link_queue_t *new_queue = (link_queue_t*)malloc(sizeof(link_queue_t));
//     if(NULL == new_queue)
//     {
//         return;
//     }
//     q_node_t *new_node = (q_node_t*)malloc(sizeof(q_node_t));
//     new_queue->front = new_queue->rear = new_node;
//     new_queue->front->next = NULL;
//     return new_queue; 
// }

// q_node_t *create_new_node(int x)
// {
//     q_node_t *new_node = (q_node_t *)malloc(sizeof(q_node_t));
//     if(new_node == NULL)
//         return;
//     new_node->data = x;
//     new_node->next = NULL;
//     return new_node; 
// }

// int link_queue_put(link_queue_t *head, int data)
// {
//     link_queue_t *sthead = (link_queue_t *)head;
//     q_node_t *new_node = create_new_node(data);
//     if(new_node == NULL)
//         return 1;

//     if(NULL == sthead->front->next)
//             sthead->front->next = new_node; 

//     sthead->rear->next = new_node;
//     sthead->rear = new_node;
//     return 0;
    
// }

// int link_queue_pop(link_queue_t *head,int *data)
// {
//     link_queue_t *sthead = head;
//     q_node_t *new_node = sthead->front->next;

//     if(sthead == NULL)
//         return 1;
//     *data = sthead->front->next->data;
//     sthead->front->next = new_node->next;

    
//     free(new_node);
//     return 0;
// }
// int link_queue_printf(link_queue_t *head)
// {
//     link_queue_t *sthead = (link_queue_t *)head;
//     q_node_t *temp = sthead->front->next;
//     printf("link queue printf:");
//     while(temp)
//     {
//         printf("%d ",temp->data);
//         temp = temp->next;
//     }
//     printf("\n");
// }

// int MXD_queue_is_empty(link_queue_t *head)
// {
//     if(head->rear == head->front)
//     {
//         return 0;
//     }
//     else
//     {
//         return 1;
//     }
// }



// void MXD_link_list(voide)
// {
//     int data;
//     link_queue_t *head = link_queue_create();
//     printf("link dequeue start\n");
//     link_queue_put(head, 30);
//     link_queue_printf(head);
//     link_queue_put(head, 50);
//     link_queue_printf(head);
//     link_queue_put(head, 570);
//     link_queue_printf(head);
//     link_queue_put(head, 1000);
//     link_queue_printf(head);

//     printf("link dequeue start\n");
//     link_queue_pop(head, &data);
//     link_queue_printf(head);
//     printf("pop %d\n",data);

//     link_queue_pop(head, &data);
//     link_queue_printf(head);
//     printf("pop %d\n",data);


//     link_queue_pop(head, &data);
//     link_queue_printf(head);
//     printf("pop %d\n",data);
// }





// void enqueue_func(link_queue_t *head, int data)
// {
//     link_queue_t *sthead = (link_queue_t*)head;
//     q_node_t *new_node = (q_node_t *)malloc(sizeof(q_node_t));
//     new_node->data = data;
//     new_node->next = NULL;
//     if(sthead->front == NULL && sthead->rear == NULL)
//     {
//         sthead->front = sthead->rear = new_node;
//         new_node->next = NULL;
//         return;
//     }

//     sthead->rear->next = new_node;
//     sthead->rear = new_node;
// }

// int dequeue_func(link_queue_t *head, int *data)
// {
//     link_queue_t *sthead = (link_queue_t *)head;
//     q_node_t *temp = sthead->front;
//     if(sthead->front == NULL)
//     {
//         printf("link queue is empty\n");
//         *data = 0;
//         sthead->front = sthead->rear = NULL;
//         return 1;
//     }
//     *data = temp->data;
//     sthead->front = sthead->front->next;
//     free(temp);
//     return 0;
// }

// int indian_link_queue_printf(link_queue_t *head)
// {
//     link_queue_t *sthead = head;
//     q_node_t *temp = sthead->front;
//     printf("link queue printf:");
//     if(temp == NULL)
//     {
//         printf("link queue is empty\n");
//         return;
//     }
//     while(temp)
//     {
//         printf("%d ",temp->data);
//         temp = temp->next;
//     }
//     printf("\n");
// }

// bool indian_link_queue_is_empty(link_queue_t *head)
// {
//     link_queue_t *temp = head;
//     if(NULL == temp->front)
//     {
//         return true;
//     }
//     return false;
// }

// int indian_get_link(link_queue_t *head)
// {
//     link_queue_t *sthead = head;
//     q_node_t *temp = sthead->front; 
//     int link_len = 0;
//     if(temp == NULL)
//     {
//         return 0;
//     }
//     while(temp)
//     {
//         link_len ++;
//         temp = temp->next;
//     }
//     return link_len;
// }


// void indian_link_list(void)
// {
//     int data;
//     printf("A\n");
//     link_queue_t *head = NULL;
//     bool queue_is_queue = false;
//     printf("B\n");
//     head = (link_queue_t *)malloc(sizeof(link_queue_t));
//     head->front = head->rear = NULL;
//     // head->rear->next = NULL;
//     printf("queue length: %d\n",indian_get_link(head));
//     printf("indian_link_list enqueue\n");
//     enqueue_func(head, 39);
//     indian_link_queue_printf(head);

//     enqueue_func(head, 50);
//     indian_link_queue_printf(head);

//     enqueue_func(head, 60);
//     indian_link_queue_printf(head);
//     printf("queue length: %d\n",indian_get_link(head));

//     enqueue_func(head, 90);
//     indian_link_queue_printf(head);

//     printf("indian link list dequeue\n");
//     dequeue_func(head, &data);
//     indian_link_queue_printf(head);

//     printf("pop %d\n",data);
//     printf("queue is empty %d\n",indian_link_queue_is_empty(head));
//     printf("queue length: %d\n",indian_get_link(head));

//     dequeue_func(head, &data);
//     indian_link_queue_printf(head);
//     printf("pop %d\n",data);
//     printf("queue is empty %d\n",indian_link_queue_is_empty(head));

//     dequeue_func(head, &data);
//     indian_link_queue_printf(head);
//     printf("pop %d\n",data);
//     printf("queue is empty %d\n",indian_link_queue_is_empty(head));


//     dequeue_func(head, &data);
//     indian_link_queue_printf(head);
//     printf("pop %d\n",data);
//     printf("queue is empty %d\n",indian_link_queue_is_empty(head));


//      dequeue_func(head, &data);
//     indian_link_queue_printf(head);
//     printf("pop %d\n",data);
//     printf("queue is empty %d\n",indian_link_queue_is_empty(head));


//     printf("indian_link_list enqueue\n");
//     enqueue_func(head, 70);
//     indian_link_queue_printf(head);

//     enqueue_func(head, 90);
//     indian_link_queue_printf(head);

//     enqueue_func(head, 200);
//     indian_link_queue_printf(head);

//     enqueue_func(head, 300);
//     indian_link_queue_printf(head);
//     printf("queue length: %d\n",indian_get_link(head));

//     printf("indian link list process end\n");
// }




// void link_queue_process_func(void)
// {
//     // MXD_link_list();    
//     indian_link_list();
    

// }