#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/FreeRTOSConfig.h>

#define LED1      2
#define LED2      4
#define DELAY_ms 500

void v1ApplicationIdleHook( void );

//creando un tipo de estructura que contiene un mensaje y pin a configurar
typedef struct TAG_OPT{
  const char * message;
  const uint8_t pin;  
  const uint32_t delayMs; 
}Parameters; 

//creamos las instancias de las estructura. Estas se pasaran como parametro al xTaskCreate
//Parametros: mensaje, led pin, delay ms, 
Parameters _Parameters1 = {"Task 1 \n", 2, 100}; 
Parameters _Parameters2 = {"Task 2 \n", 4, 500}; 
//creamos los punteros a las instancias anteriormente creadas
Parameters * _pvParameters1 = &_Parameters1;
Parameters * _pvParameters2 = &_Parameters2;

//creamos una funcion con parametro de entrada un puntero a una estructura 
void LED1ControllerTask(void *xStruct){

  //pointer to structure 
  Parameters * data = (Parameters *) xStruct;  
  String msg = (String) data->message; 
  uint8_t _pin = data->pin; 
  uint32_t _delayMs = data->delayMs; 
  TickType_t _delayTicks = pdTICKS_TO_MS(_delayMs);

  pinMode(_pin, OUTPUT);

  while(1){
    digitalWrite(_pin, digitalRead(_pin)^1);
    Serial.print(msg);
    vTaskDelay(_delayTicks);
  }
}

//creamos una funcion con parametro de entrada un puntero a una estructura 
void LED2ControllerTask(void *xStruct){

  //pointer to structure 
  Parameters * data = (Parameters *) xStruct;  
  String msg = (String) data->message; 
  uint8_t _pin = data->pin; 
  uint32_t _delayMs = data->delayMs; 
  TickType_t _delayTicks = pdTICKS_TO_MS(_delayMs);
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  pinMode(_pin, OUTPUT);

  while(1){
    digitalWrite(_pin, digitalRead(_pin)^1);
    Serial.print(msg);
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS( _delayTicks ));
  }
}

static uint32_t ulIdleCycleCount = 0UL;

// Increments the idleCount variable
void v1ApplicationIdleHook(void)
{
  ulIdleCycleCount++;
}

void idle_function(void *ignore){
  while(1){
    Serial.println("Idle Task: "+ ulIdleCycleCount);
    vTaskDelay( pdTICKS_TO_MS(50) );
  }
}

void setup() {
  Serial.begin(9600); 

  xTaskCreate(LED1ControllerTask, "Led 1", 1000, (void *) &_Parameters1, 2, NULL); 
  xTaskCreate(LED2ControllerTask, "Led 2", 1000, (void *) &_Parameters2, 2, NULL);
  xTaskCreate(idle_function, "Idle Task", 1000, NULL, 2, NULL ); 
}

void loop() {
  // put your main code here, to run repeatedly:
}