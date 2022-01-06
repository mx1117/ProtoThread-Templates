#include "protothread.h"

/*
 * �����ź���
 */
process_sem_t received, sended;

PROCESS(sem_received_process,"sem_received_process test");

PROCESS(sem_send_process,"sem_send_process test");

PROCESS_THREAD (sem_received_process, ev, data)
{
    static struct etimer et;
    PROCESS_BEGIN();
    printf("sem_received_process begin\n");
    while(1){
        PROCESS_SEM_WAIT(&received);
        printf("received:%02x\n",ev);
        etimer_set(&et,PROTOTHREAD_CLOCK_SECONDS);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));//�ȴ�һ�����ٷ����ź���
        printf("etimer ev:%02x\n",ev);
        PROCESS_SEM_SIGNAL(&sem_send_process, &sended);
    }
    PROCESS_END();
}

PROCESS_THREAD (sem_send_process, ev, data)
{
    static struct etimer et;
    PROCESS_BEGIN();
    printf("sem_send_process begin\n");
    while(1){
        PROCESS_SEM_SIGNAL(&sem_received_process, &received);//���ͷ��ȷ����ź���
        PROCESS_SEM_WAIT(&sended);
        printf("sended\n");
        etimer_set(&et,PROTOTHREAD_CLOCK_SECONDS);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        printf("etimer ev:%02x\n",ev);
    }
    PROCESS_END();
}

void pt_sem_test(void)
{
    PROCESS_SEM_INIT(&sended, 0);
    PROCESS_SEM_INIT(&received, 0);
    process_start(&sem_received_process, NULL);
    process_start(&sem_send_process, NULL);
    //PROCESS_SEM_SIGNAL(&sended);
}
