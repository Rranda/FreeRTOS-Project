#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"            
#include "timers.h"
#include <semphr.h>
#include <queue.h>
//#include <stdint.h>
//============================== variables ======================================
SemaphoreHandle_t SerialSemaphore;
QueueHandle_t mailbox;
TaskHandle_t ROUTIN;
TaskHandle_t lcd;
TaskHandle_t fin;
TaskHandle_t pfn;

int i = 0;
int temp = 0;

char msg[15];
void vApplicationIdleHook(void);
void RoutineDInterruption(void*);
void finTravail(void*);
void afficher(void*);
void paraffine(void*);


//=========================== Routine D'Interruption ===========================
void RoutineDInterruption(void *pvParameters) {
 (void) pvParameters;
while(1){

printf("\ntask traitment de l'intruption \n");
  msg[0] = 'o';
  msg[1] = 'n';
  msg[2] = ' ';
  msg[3] = ' ';
  msg[4] = '\0';
  xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );

xSemaphoreGiveFromISR(SerialSemaphore,NULL);

vTaskSuspend(ROUTIN);
 vTaskResume(fin);

}
}
//=============================== afficher ======================================
void afficher(void *pvParameters) {
 (void) pvParameters;
while(1){

xQueueReceive(mailbox, &msg,( TickType_t ) 10 );
printf("\ntask afficher ************* machine:   %s \n",msg);

}    
}


//================================= Paraffine =============================================
void paraffine(void *pvParameters){
 (void) pvParameters;

temp ++;
 if(temp = 5){ //la temperature ici doit etre 56°c
  msg[0] = 'P';
  msg[1] = 'a';
  msg[2] = 'r';
  msg[3] = 'r';
  msg[4] = 'a';
  msg[5] = 'f';
  msg[6] = 'i';
  msg[7] = 'n';
  msg[8] = 'e';
  msg[9] = ' ';
  msg[10] = 'o';
  msg[11] = 'n';
  msg[12] = '\0';
  
      printf("Task temperature de paraffine est = %d °C \n",temp);
 xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
  
       printf("Task Infusion de paraffine terminé \n \n",temp);
       vTaskDelay(1000);//pendant 4h
  }
  
 temp --;
 if(temp = -2){ //la temperature ici doit etre 56°c 

    printf("Task temperature = %d °C \n",temp);
   }
   printf("\nTask formation de paraffinee \n");
  
  msg[0] = 'o';
  msg[1] = 'n';
  msg[2] = '\0';
  msg[3] = '\0';
 xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
 printf("\nTask formation de paraffine terminé\n");

 vTaskResume(fin);
  //vTaskResume(ROUTIN)

}

//===============================finTravail===============================================
void finTravail(void *pvParameters) {
 (void) pvParameters;
  vTaskSuspend(fin);
 printf( "\ntask fin travail \n\n");

  msg[0] = 'o';
  msg[1] = 'f';
  msg[2] = 'f';
  msg[3] = ' ';
  msg[4] = '\0';
  
  xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );

 

    vSemaphoreDelete(SerialSemaphore);
    vQueueDelete( mailbox);
    vTaskDelete(lcd);
    vTaskDelete(ROUTIN);
    
    vTaskEndScheduler();
}

//====================== =========== ==== main ====================================
int main ( void )
{

vSemaphoreCreateBinary( SerialSemaphore );
xSemaphoreTake( SerialSemaphore,( TickType_t ) 10);
mailbox= xQueueCreate( 1, sizeof( msg ) );

printf("cliquer sur entree pour lancer la simulation " );
getchar();

msg[0]='o';
msg[1]='f';
msg[2]='f';
msg[3] = ' ';
msg[4] = '\0';
xQueueSendToBack( mailbox,&msg,( TickType_t ) 10 );

xTaskCreate( afficher, "lcd", 128, NULL, 6, &lcd );
xTaskCreate( RoutineDInterruption, "gestion E/S", 128, NULL, 4, &ROUTIN );
xTaskCreate( finTravail, "suppression", 128, NULL, 5, &fin );
xTaskCreate( paraffine, "pfn", 128, NULL, 3, &pfn );
vTaskStartScheduler();
    return 0;
}
//================================ end Main ============================

//================================== system =============================
void vAssertCalled( unsigned long ulLine, const char * const pcFileName )// gestion exception
{
     taskENTER_CRITICAL();
    {
        printf("[ end ] %s:%lu\n", pcFileName, ulLine);
        fflush(stdout);
    }
    taskEXIT_CRITICAL();
    exit(-1);
}

void vApplicationIdleHook(void){}

