#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/FreeRTOSConfig.h>
#include <freertos/queue.h>

/*
Define which method to use
  - sendUint8_t : Read the valueToSend in the sender func, and send to queue
  - sendStruct_t : Sends the structure to the queue, and the receiver func is responsible to decode it
*/
#define sendUint8_t   0
#define sendStruct_t  1

#define LED1      2
#define LED2      4
#define msDelaySender  100
#define msDelayReceiver 10

QueueHandle_t queue_data; 

typedef enum{
  eSender1,
  eSender2, 
} dataSource_t; 

typedef struct TAG_OPT{
  uint8_t valueToSend; 
  dataSource_t eDataSource; 
} Data_t; 

static const Data_t xStructToSend[2] =  { {100, eSender1}, {200, eSender2} }; 
static const Data_t * pStructToSend[2] = {&xStructToSend[0], &xStructToSend[1]};

//creamos una funcion con parametro de entrada un puntero a una estructura 
void Sender(void *xStruct){

  //to monitor de if the data was sent sucessfully or not
  BaseType_t qStatus;
  //maximum allowed time to send data
  TickType_t xTicksToWait( pdMS_TO_TICKS(100) );
  TickType_t _delayTicks = pdTICKS_TO_MS(msDelaySender); 

  #if sendUint8_t
    //pointer to structure 
    Data_t * data = (Data_t *) xStruct;  
    uint8_t valueToSend = data->valueToSend;
    uint8_t Sender = data->eDataSource; 
  #endif

  pinMode(LED1, OUTPUT);

  while(1){

    #if sendUint8_t
      qStatus = xQueueSend(queue_data, &valueToSend, xTicksToWait);
    #endif
    #if sendStruct_t 
      qStatus = xQueueSend(queue_data, xStruct, xTicksToWait);
    #endif

    if (qStatus == pdPASS){
      digitalWrite(LED1, digitalRead(LED1)^1);
    }else{
      Serial.println("Fail to send data!");
    }

    vTaskDelay(_delayTicks);
  }
}

//creamos una funcion con parametro de entrada un puntero a una estructura 
void Receiver(void *xStruct){

  //to monitor if data was received sucessfully
  BaseType_t qStatus;
  //maximum allowed time to get some data
  const TickType_t xTicksToWait = pdMS_TO_TICKS(200); 
  //delay to receive a value
  TickType_t _delayTicks( pdMS_TO_TICKS(msDelayReceiver) ); 

  #if sendUint8_t
    //receive variable
    uint32_t receivedValue;
  #endif
  #if sendStruct_t
    //receive variable
    Data_t receivedStruct;
    Data_t * pReceivedStruct = &receivedStruct; 
  #endif

  pinMode(LED2, OUTPUT);

  while(1){

    #if sendUint8_t
      qStatus = xQueueReceive(queue_data, &receivedValue, xTicksToWait);
    #endif
    #if sendStruct_t
      qStatus = xQueueReceive(queue_data, pReceivedStruct, xTicksToWait); 
    #endif

    if(qStatus == pdPASS){

      digitalWrite(LED2, digitalRead(LED2)^1);

      #if sendUint8_t
        Serial.print("Received Value: ");
        Serial.println(receivedValue);
      #endif
      #if sendStruct_t
        uint8_t ReceivedValue = pReceivedStruct->valueToSend; 
        uint8_t Sender = pReceivedStruct->eDataSource; 
        Serial.print("Sender ");
        Serial.print(Sender);
        Serial.print(" -> Receive Value: ");
        Serial.println(ReceivedValue); 
      #endif

    }else{
      Serial.println("Fail to Receive data!");
    }
    vTaskDelay(_delayTicks); 
  }
}

  void setup() {
  Serial.begin(115200); 

  #if sendUint8_t
    queue_data = xQueueCreate(3, sizeof(uint8_t));
  #endif
  #if sendStruct_t 
    queue_data = xQueueCreate(3, sizeof(Data_t));
  #endif

  xTaskCreate(Sender, "Sender 1", 1000, (void *) pStructToSend[0], 1, NULL);
  xTaskCreate(Sender, "Sender 2", 1000, (void *) pStructToSend[1], 1, NULL);

  xTaskCreate(Receiver, "Receiver", 1000, NULL, 1, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}