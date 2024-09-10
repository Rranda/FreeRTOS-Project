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
TaskHandle_t dsy;

int i = 0;

char msg[10];
void vApplicationIdleHook(void);
void RoutineDInterruption(void*);
void finTravail(void*);
void afficher(void*);
void deshydra(void*);



//============================ Routine D'Interruption ===========================
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

//=============================== Déshydratation ======================================

void deshydra(void *pvParameters) {
 (void) pvParameters;
 
 printf("\nTask déshydratation \n");
 
 
 
 for(i=0;i<5;i++){
 	switch(i){	case 0:{
 	                         printf("\nFormol 10% \n");
                                for(int i=0;i<2;i++){
                               msg[0] = '1';
                               msg[1] = '0';
                               msg[2] = '%';
                               msg[3] = '\0';
                                                             
                                xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10);
                                vTaskDelay(100);//pendant 1h 
                                }

				 break;}
 			
			case 1:{
                                 printf("\nEthanol 70% \n");
                               msg[0] = '7';
                               msg[1] = '0';
                               msg[2] = '%';
                               msg[3] = '\0';

                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
                               vTaskDelay(1000);//pendant 11 à 12h 
				break;}
			case 2:{
                                  printf("\nEthanol 80% \n");
                               msg[0] = '8';
                               msg[1] = '0';
                               msg[2] = '%';
                               msg[3] = ' ';
                               msg[4] = '\0';
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
                               vTaskDelay(1000);//pendant 11 à 12h 
				break;}
			case 3:{
			
                                  printf("\nEthanol 95% \n");
                               msg[0] = '9';
                               msg[1] = '5';
                               msg[2] = '%';
                               msg[3] = ' ';
                               msg[4] = '\0';
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
                               vTaskDelay(1000);//pendant 11 à 12h 
			        break;}
			case 4:{
                                  
                                  printf("\nEthanol 100%\n");
                                  for(int i=0;i<3;i++){
                               msg[0] = '1';
                               msg[1] = '0';
                               msg[2] = '0';
                               msg[3] = '%';
                               msg[4] = '\0';
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 1000 );}
                               vTaskDelay(100);//pendant 1h 

								break;}
		        
							default: break;
						}

}
  printf("\nTask déshydratation terminé \n");
vTaskResume(fin);
  //vTaskResume(ROUTIN); 
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

xTaskCreate( afficher, "lcd", 128, NULL, 5, &lcd );
xTaskCreate( RoutineDInterruption, "gestion E/S", 128, NULL, 3, &ROUTIN );
xTaskCreate( finTravail, "suppression", 128, NULL, 4, &fin );
xTaskCreate( deshydra, "dsy", 128, NULL, 3, &dsy );
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

