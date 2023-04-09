#include <Arduino.h>
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include <freertos/FreeRTOSConfig.h>
#include <freertos/queue.h>
#include <FreeRTOS/semphr.h>
#include <FreeRTOS/timers.h>

#define   LED1  2
#define   LED2  4

//Timers handlers, xTimer1 -> One shot timer, xTimer2 and xTimer3 -> Auto Reload timer
TimerHandle_t xTimer1, xTimer2, xTimer3; 

//To check the status of the timer
BaseType_t xTimer1Started, xTimer2Started, xTimer3Started; 

//Defining Timer periods
#define OneShotTimerPeriod        (pdMS_TO_TICKS(500))
#define AutoReloadTimerPeriod_1   (pdMS_TO_TICKS(500))
#define AutoReloadTimerPeriod_2   (pdMS_TO_TICKS(1000))

uint32_t Timer1Counter; 

void OneShotTimerCallbackFunc ( TimerHandle_t xTimer){
  static TickType_t xTimerNow; 
  xTimerNow = xTaskGetTickCount();
  Serial.print("One shot timer -> xTimerNow: ");
  Serial.println(xTimerNow); 
  digitalWrite(LED1, digitalRead(LED1)^1);

}

void AutoReloadTimerCallbackFunc ( TimerHandle_t xTimer ){
  static TickType_t xTimerNow;  
  xTimerNow = xTaskGetTickCount();

  if(xTimer == xTimer2){
    Timer1Counter++; 
    Serial.print("Auto Reload Timer 1 -> xTimerNow: ");
    Serial.println(xTimerNow);
    Serial.print("Counter: ");
    Serial.println(Timer1Counter);
    digitalWrite(LED2, digitalRead(LED2)^1);

    //Stopping the timer at the 7nth cicle only as example
    if(Timer1Counter == 7){
      xTimerStop(xTimer2, 0);
    }
  }else{
    Serial.print("Auto Reload Timer 2 -> xTimerNow: ");
    Serial.println(xTimerNow);
    digitalWrite(LED1, digitalRead(LED1)^1);
  }

}

void setup() {
  
  Serial.begin(115200); 
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);


  //Creating new timers (one shot timer and auto reload timer)
  xTimer1 = xTimerCreate("One shot Timer 1", OneShotTimerPeriod, pdFALSE, 0, OneShotTimerCallbackFunc);
  xTimer2 = xTimerCreate("Auto Reload Timer 1", AutoReloadTimerPeriod_1, pdTRUE, 0, AutoReloadTimerCallbackFunc);
  xTimer3 = xTimerCreate("Auto Reload Timer 2", AutoReloadTimerPeriod_2, pdTRUE, 0, AutoReloadTimerCallbackFunc);

  //Be sure that all timer began correctly, and start it
  if (xTimer1 != NULL && xTimer2 != NULL && xTimer3 != NULL){
    xTimer1Started = xTimerStart(xTimer1, 0);
    xTimer2Started = xTimerStart(xTimer2, 0);
    xTimer3Started = xTimerStart(xTimer3, 0); 
  }

  //check if they start correctly
  while (xTimer1Started != pdPASS && xTimer2Started != pdPASS && xTimer3Started != pdPASS ) {}

}

void loop() {
  // put your main code here, to run repeatedly:
}