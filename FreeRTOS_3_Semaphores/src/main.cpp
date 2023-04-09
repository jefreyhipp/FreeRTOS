#include <Arduino.h>
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include <freertos/FreeRTOSConfig.h>
#include <freertos/queue.h>
#include <FreeRTOS/semphr.h>

#define LED1  2
#define LED2  4

/*
Counting semaphores are used for resource allocation where multiple tasks can access 
a shared resource up to a certain limit, while binary semaphores are used for mutual 
exclusion or simple signaling between tasks. Counting semaphores can have a variable 
count, whereas binary semaphores can only have two states.

Both binary semaphores and mutexes can provide mutual exclusion between tasks, 
mutexes are typically used for exclusive access to resources, support priority inheritance, 
and can only be released by the task that has taken them. Binary semaphores, 
on the other hand, can be used for signaling between tasks and can be released by any task.
*/

/*
Select which type of semaphore you will want to test
1. Binary Semaphore
2. Semaphore counting
3. Mutex
*/
#define BinarySemaphore   0
#define SemaphoreCounting 0
#define Mutex             1

//semaphore handler
#if SemaphoreCounting
  SemaphoreHandle_t xCountingSemaphore; 
#endif
#if BinarySemaphore
  SemaphoreHandle_t xBinarySemaphore;
#endif
#if Mutex
  SemaphoreHandle_t xMutex; 
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
    #if Mutex
      xSemaphoreTake(xMutex, portMAX_DELAY);
    #endif

    digitalWrite( LED1, digitalRead(LED1)^1 );
    Serial.println("Task 1 is running");

    #if BinarySemaphore 
      xSemaphoreGive(xBinarySemaphore); 
    #endif
    #if SemaphoreCounting
      xSemaphoreGive(xCountingSemaphore); 
    #endif
    #if Mutex
      xSemaphoreGive(xMutex);
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
    #if Mutex
      xSemaphoreTake(xMutex, portMAX_DELAY);
    #endif

    digitalWrite(LED2, digitalRead(LED2)^1);
    Serial.println("Task 2 is running");

    #if BinarySemaphore 
      xSemaphoreGive(xBinarySemaphore); 
    #endif
    #if SemaphoreCounting
      xSemaphoreGive(xCountingSemaphore); 
    #endif
    #if Mutex
      xSemaphoreGive(xMutex);
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
  #if Mutex
    xMutex = xSemaphoreCreateMutex();
  #endif

  xTaskCreate( Task1, "Task 1", 1000, NULL, 1, NULL);
  xTaskCreate( Task2, "Task 2", 1000, NULL, 2, NULL);

  #if SemaphoreCounting
    xSemaphoreGive(xCountingSemaphore);
  #endif

}

void loop() {
  // put your main code here, to run repeatedly:
}