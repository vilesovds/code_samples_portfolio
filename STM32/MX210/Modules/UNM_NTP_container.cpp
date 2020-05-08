#include "defaults.h"
#include "container.h"
#include "UNM_NTP.c"
#include "UNM_NTP_cfg.c"
#include "RTOS.h"
#include "BSP.h"
#include "FS.h"
#include "UNM_NTP_container.h"
#include "stdlib.h"

#include "IP_Int.h"


#if IP_SUPPORT_PACKET_TIMESTAMP == 0
  #error "Packet timestamp IP_SUPPORT_PACKET_TIMESTAMP needs to be activated"
#endif

static unsigned long ID4402_count;

#pragma location="CONT_CONF_SEGMENT"
__root const CONTAINERS_CONF_POINTER  ID4402_conf={ID4402,UNM_NTP_cfg,sizeof(UNM_NTP_cfg)};

static ID4402_CLASS *CreateClass(CONTAINER_FIELD_ABSTRACT* pCont);

#pragma location="CONT_INIT_SEGMENT"
__root const CONTAINER_PRG  ID4402_decl={ID4402_LO,ID4402_HI,(ABSTR_CONT* (*)(CONTAINER_FIELD_ABSTRACT* pCont))&CreateClass,NULL};

static ID4402_CLASS *CreateClass(CONTAINER_FIELD_ABSTRACT* pCont)
{
    if (ID4402_count>=MAX_ID4402_COPY) {
        Print_DBGU((unsigned char*)"ERROR! ID4402 can not have more copys!!!\r\n");
        return NULL;
    }
    ID4402_CLASS *pclass=new ID4402_CLASS(pCont);

    return pclass;
}

void Container_Task (void * p_arg);

ID4402_CLASS::ID4402_CLASS(CONTAINER_FIELD_ABSTRACT* pCont):ABSTR_CONT(pCont,&ID4402_iospace, &ID4402_conf, sizeof(*this))
{
    OS_EnterRegion();
    Id=ID4402;
	
	Task = &Task_s;
	OS_CreateTaskEx(Task, "ID4402 Task", pCont->setups.priority,
			Container_Task, &OSTaskStk, sizeof(OSTaskStk),
#if !defined(OS_LIBMODE_XR)
			MAX_ID4402_COPY,
#endif
			this);
    instance_num=ID4402_count;
    ID4402_count++;
    OS_LeaveRegion();
}

ID4402_CLASS::~ID4402_CLASS()
{
    OS_EnterRegion();
    if (ID4402_count) ID4402_count--;
    OS_LeaveRegion();
}

void Container_Task (void * p_arg) {
    ID4402_CLASS *pThis=(ID4402_CLASS *)p_arg;
    pThis->Container_TaskExt();
    OS_TerminateTask(pThis->Task);
}


extern int _IFaceId;

void ID4402_CLASS::UpdateParams(void)
{
    IP_LOG((IP_MTYPE_APPLICATION, "Update params"));
    IP_NTP_CLIENT_Halt(1);
    SyncInputs(pcontainer);
    if(VAL_ENABLE){
        IP_NTP_CLIENT_Start();
        IP_LOG((IP_MTYPE_APPLICATION, "NTP enabled"));
        // chk if empty
        if (VAL_POOL_SERVERS!='\0'){
            IP_LOG((IP_MTYPE_APPLICATION,"added pool %s",(char *)&VAL_POOL_SERVERS));
            IP_NTP_CLIENT_AddServerPool(_IFaceId, (char *)&VAL_POOL_SERVERS);
            //IP_NTP_CLIENT_AddServerPool(_IFaceId, (char *)&mpCInputs->STT_DESC_ROOT.vVAL_POOL_SERVERS);
        }
        if(0 != VAL_NTP_IP1){
            IP_LOG((IP_MTYPE_APPLICATION,"added server 0x%08",VAL_NTP_IP1));
            IP_NTP_CLIENT_AddServerClock(_IFaceId, VAL_NTP_IP1);
        }
        if(0 != VAL_NTP_IP2){
            IP_LOG((IP_MTYPE_APPLICATION,"added server 0x%08x",VAL_NTP_IP2));
            IP_NTP_CLIENT_AddServerClock(_IFaceId, VAL_NTP_IP2);
        }
        VAL_STATUS = STATUS_ON;
        mTime = IP_OS_GetTime32();
    }else{
        VAL_STATUS = STATUS_OFF;
    }
    SyncOutputs(pcontainer);
}

void ID4402_CLASS::Container_TaskExt (void) {
    int Status;
    IP_NTP_TIMESTAMP Timestamp;
    TDateTime temptime;
    uint16_t min_node, max_node;
    uint16_t *pNodeId;
    //first time always outputs
    SyncOutputs(pcontainer);
    
    OS_Q_Create(&mQchange, &mQchangeBuffer, sizeof(mQchangeBuffer));
    do{
        // Editor of params
        pIEDITOR = (ABSTARACT_IEDITOR_CLASS * )GetAbstractByType(ABSTRACT_IF_TYPE_IEDITOR);
        // Subscriber to params changes 
        pChange = (ABSTARACT_IPARAM_CHANGE_CLASS * )GetAbstractByType(ABSTRACT_IF_TYPE_IPARAM_CHANGE);
        OS_Delay(10);
    }while((NULL==pChange)||(NULL==pIEDITOR));
    // get min and max for subscribe
    pParam =pIEDITOR->GetByChOffset(this,OFF_VAL_ENABLE);
    min_node = pParam->NodeNum;
    pParam =pIEDITOR->GetByChOffset(this,OFF_VAL_TIME_SYNC);
    max_node = pParam->NodeNum;
    pChange->Subscribe(&mQchange,min_node,max_node);

    UpdateParams();
    while (1) {
        if (0 == VAL_ENABLE){
            // TODO set status
            // wait for params changed
            if( OS_QUEUE_GetPtrBlocked(&mQchange, (void**)&pNodeId)>0){
                IP_LOG((IP_MTYPE_APPLICATION, "Exit from blocked"));
                // update params
                UpdateParams();
                OS_QUEUE_Purge(&mQchange);
            }
        }else{
            if(OS_QUEUE_GetPtrTimed(&mQchange, (void**)&pNodeId, MAX(pcontainer->setups.period, ID4402_MIN_PERIOD))>0){
                IP_LOG((IP_MTYPE_APPLICATION, "Got while timed"));
                // update params
                UpdateParams();
                OS_QUEUE_Purge(&mQchange);
            }else{
                // nothing changed and enabled
                IP_NTP_CLIENT_Run();
                if (0 != IP_IsExpired(mTime)) {
                    IP_LOG((IP_MTYPE_APPLICATION, "Chk for sync"));
                    Status = IP_NTP_GetTimestamp(&Timestamp);
                    if(0 == Status ){
                        IP_LOG((IP_MTYPE_APPLICATION, "Synced"));
                        temptime.to_time(Timestamp.Seconds);
                        RTClock::Set(&temptime);
                        mTime += MS_TO_TICKS(VAL_TIME_SYNC*1000U);
                        VAL_STATUS = STATUS_SYNC;
                    }else{
                        VAL_STATUS = STATUS_ON;
                    }
                    SyncOutputs(pcontainer);
                }
              
            }
        }
    }
}
