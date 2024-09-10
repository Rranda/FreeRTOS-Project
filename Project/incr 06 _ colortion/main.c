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
TaskHandle_t clr;

int temp = 0;

char msg[15];
void vApplicationIdleHook(void);
void RoutineDInterruption(void*);
void finTravail(void*);
void afficher(void*);
void coloration(void*);

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


//=============================== coloration ===============================


void coloration(void *pvParameters) {
 (void) pvParameters;
 
 printf("\nTask Coloration \n");
 
 
 
 for(int i=0;i<12;i++){
 	switch(i){	case 0:{ //Déparaffinage
 	                       for(int k=0 ; k<2; k++){
 	                         printf("\n Xylène \n");
                              
                                xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
                               // vTaskDelay(100);//pendant 15 min 
                                                   }          
                                

				 break;}
 			
			case 1:{//Réhydratation 
                                 printf("\nEthanol\n");
                             
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
                               //vTaskDelay(100);//pendant 5 min 
				break;}
				
			case 2:{
			           temp ++;
                                  if(temp = 70){
                                  printf("\n alcool à %d °C \n",temp);
                          
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );}
                              // vTaskDelay(100);//pendant 5 min 
				break;}
			case 3:{
			
                                  printf("\nColoration avec l’hématoxyline \n");
                             
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
                               //vTaskDelay(100);//pendant 25 minutes
			        break;}
			case 4:{

                                  printf("\nRinçage dans un l’eau\n");
                            
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
		              //vTaskDelay(100);//pendant 5 minutes

								break;}
			case 5:{

                                  printf("\n Coloration à l’éosine\n");
                           
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
		              //vTaskDelay(100);//pendant 15 minutes.

								break;}
			case 6:{

                                  printf("\n  Lavage à l’eau pour éliminer l’excès de colorant.\n");
                           
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
		            
								break;}
		       case 7:{

                                  printf("\n  Déshydratation dans l’alcool à 70°C \n");
                             
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
                               //vTaskDelay(100);//pendant 10 minutes
                               break;}
                               
                      case 8:{
                               temp ++;
                                if(temp = 70){
                                  printf("\n  Déshydratation dans l’alcool à %d °C \n",temp);
                         
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );}
                               //vTaskDelay(100);//pendant 10 minutes
		            
								break;}
							
			case 9:{
                                
                                  printf("\n  Déshydratation dans l’alcool absolu \n");
                              
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
                               //vTaskDelay(100);//pendant 3 minutes.
		            
								break;}
		      case 10:{

                                  printf("\n Séchages des lame \n");
                            
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );

		            
								break;}
								
		      case 11:{

                                  printf("\n Clarification dans le xylène \n");
                               
                               xQueueSendToBack( mailbox, &msg, ( TickType_t ) 10 );
                               //vTaskDelay(100);//pendant 15 minutes.
		            
								break;}
								
							default: break;
						}

}
 printf("\nTask Coloration terminé \n");
 
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
xTaskCreate( coloration, "clr", 128, NULL, 3, &clr );
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

