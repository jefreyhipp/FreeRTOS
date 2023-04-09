#include <Arduino.h>
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include <freertos/FreeRTOSConfig.h>
#include <freertos/queue.h>
#include <FreeRTOS/semphr.h>


#define LED1  2
#define LED2  4

/*
Select which type of semaphore you will want to test
1. Binary Semaphore
2. Semaphore counting
*/
#define BinarySemaphore   0
#define SemaphoreCounting 1

#if SemaphoreCounting

#endif

//semaphore handler
#if SemaphoreCounting
  SemaphoreHandle_t xCountingSemaphore; 
#endif
#if BinarySemaphore
  SemaphoreHandle_t xBinarySemaphore;
#endif

void Task1 (void * pvParameters){
  pinMode(LED1, OUTPUT);

  #if SemaphoreCounting
    xSemaphoreGive(xCountingSemaphore);
  #endif

  while(1){

    #if BinarySemaphore 
      xSemaphoreTake(xBinarySemaphore, portMAX_DELAY); 
    #endif
    #if SemaphoreCounting
      xSemaphoreTake(xCountingSemaphore, portMAX_DELAY); 
    #endif

    digitalWrite( LED1, digitalRead(LED1)^1 );
    Serial.println("Task 1 is running");

    #if BinarySemaphore 
      xSemaphoreGive(xBinarySemaphore); 
    #endif
    #if SemaphoreCounting
      xSemaphoreGive(xCountingSemaphore); 
    #endif

    vTaskDelay(1);
  }
}

void Task2 (void * pvParameters){
  pinMode(LED2, OUTPUT); 
  while(1){

    #if BinarySemaphore 
      xSemaphoreTake(xBinarySemaphore, portMAX_DELAY); 
    #endif
    #if SemaphoreCounting
      xSemaphoreTake(xCountingSemaphore, portMAX_DELAY); 
    #endif 

    digitalWrite(LED2, digitalRead(LED2)^1);
    Serial.println("Task 2 is running");

    #if BinarySemaphore 
      xSemaphoreGive(xBinarySemaphore); 
    #endif
    #if SemaphoreCounting
      xSemaphoreGive(xCountingSemaphore); 
    #endif

    vTaskDelay(1);
  }
}


void setup() {
  Serial.begin(115200);

  #if BinarySemaphore
    xBinarySemaphore = xSemaphoreCreateBinary();
  #endif  
  #if SemaphoreCounting
    xCountingSemaphore = xSemaphoreCreateCounting(1, 0);
  #endif

  xTaskCreate( Task1, "Task 1", 1000, NULL, 1, NULL);
  xTaskCreate( Task2, "Task 2", 1000, NULL, 1, NULL);

  #if SemaphoreCounting
    xSemaphoreGive(xCountingSemaphore);
  #endif

}

void loop() {
  // put your main code here, to run repeatedly:
}