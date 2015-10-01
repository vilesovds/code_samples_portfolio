#include "events_log.h"
#include "config.h"


#include "ff.h"
FIL flog;

TaskHandle_t xEventsLogTaskHandler = NULL;
QueueHandle_t xEventsLogItems = NULL;

/*Prototypes*/
void vEventsLogTask(void *pvParameters);
/**
*   @brief
*/
void EventsLogInit(void)
{
  if(xEventsLogTaskHandler == NULL){
    xTaskCreate(vEventsLogTask, (const char * )"EventsLog",EVENTSLOG_TASK_STACK,
            NULL,PRIO_EVENTSLOG_TASK, &xEventsLogTaskHandler);
  }
  configASSERT(xEventsLogTaskHandler!=NULL);
}

/**
*   @brief
*/
void EventsLogAddItem(events_log_item_t * item, TickType_t wait)
{
  if(xEventsLogItems){
    xQueueSend(xEventsLogItems,(void *)item,wait);
  }
}

/**
*   @brief Add log event by codenum and param value. No need time and date values
*/
void EventsLogAddByCode(events_log_code_t code, uint32_t param, TickType_t wait)
{
  RTC_TimeTypeDef timestruct;
  RTC_DateTypeDef datestruct;
  events_log_item_t item;
  
  RTC_GetTime(RTC_Format_BIN, &timestruct);
  RTC_GetDate(RTC_Format_BIN, &datestruct);
  item.code = code;
  item.param = param;
  item.date = datestruct;
  item.time = timestruct;
  EventsLogAddItem(&item, wait);
}

/**
*   @brief
*/
void EventsLogFlush(void)
{
  events_log_item_t logitem;
  UINT bw;
  FRESULT fres;
  if(xEventsLogItems == NULL) return;
  while(uxQueueMessagesWaiting(xEventsLogItems)){
    xQueueReceive(xEventsLogItems, &logitem, portMAX_DELAY);
    fres = f_write(&flog,(void *)&logitem,sizeof(logitem),&bw);
    if((FR_OK != fres)||(sizeof(logitem) != bw)){
        // error
        //EventsLogExit();
      break;
    }
  }
  f_sync(&flog);
}

/**
*   @brief
*/
void vEventsLogTask(void *pvParameters)
{
  FRESULT fres;
  events_log_item_t logitem;
  if(xEventsLogItems == NULL){
    xEventsLogItems = xQueueCreate(10, sizeof(events_log_item_t));
  }
  assert_param(xEventsLogItems != NULL);
  
  fres = f_open(&flog,"events.log",FA_OPEN_ALWAYS | FA_WRITE);
  assert_param(fres == FR_OK);
  
  /* Seek to end of the file to append data */
   f_lseek(&flog, f_size(&flog));
  while(1){
    if(xQueueReceive(xEventsLogItems, &logitem, pdMS_TO_TICKS(3000))){
      UINT bw;
      fres = f_write(&flog,(void *)&logitem,sizeof(logitem),&bw);
      if((FR_OK != fres)||(sizeof(logitem) != bw)){
        // error
        //EventsLogExit();
      }
    }else{
      EventsLogFlush();
    }
  }
}

/**
*   @brief Close file and kill task
*/
void EventsLogExit(void)
{
  if(xEventsLogTaskHandler!=NULL){
    f_close(&flog);
    taskENTER_CRITICAL();
    TaskHandle_t h = xEventsLogTaskHandler;
    xEventsLogTaskHandler = NULL;
    taskEXIT_CRITICAL();
    vTaskDelete(h);
  }
}

