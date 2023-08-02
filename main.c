#include <project.h>
#include "ble.h"

/***************************************************************
 * Function to handle the BLE stack
 **************************************************************/
void BleCallBack(uint32 event)
{
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;

    switch(event)
    {
        /* if there is a disconnect or the stack just turned on from a reset then start the advertising and turn on the LED blinking */
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            /* Start advertising after disconnection */
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_SLOW);
            break;
        
        /* when a connection is made, update the LED and Capsense states in the GATT database and stop blinking the LED */    
        case CYBLE_EVT_GATT_CONNECT_IND:
            /* Update the GATT database with your service and characteristics here */
            /* For example, you can call a function to configure and register the GATT service */
            /* configureAndRegisterGattService(); */
            break;
        
        default:
            break;
    }
} 

/***************************************************************
 * Function to configure and register the GATT service
 **************************************************************/
void configureAndRegisterGattService()
{
    /* Define your custom GATT service and characteristics here */
    CYBLE_GATTS_HANDLE_VALUE_NTF_T charValue;
    CYBLE_GATT_HANDLE_VALUE_PAIR_T myHandle;

    /* Initialize and register the GATT service */
    /* Assuming you have a custom service UUID "0x2800" */
    CYBLE_GATT_HANDLE_VALUE_PAIR_T myServiceHandle;
    myServiceHandle.attrHandle = 0x0001; /* Specify a handle for your service */
    myServiceHandle.value.len = 6; /* Length of the service UUID */
    myServiceHandle.value.val = (uint8 *)"\x00\x28\x00\x00\x00\x00"; /* Replace with your service UUID */
    CyBle_GattsWriteAttributeValue(&myServiceHandle, 0, &cyBle_connHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
    CyBle_GattsWriteAttributeValue(&myServiceHandle, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);

    /* Initialize and register the characteristic */
    /* Assuming you have a custom characteristic UUID "0x2803" */
    myHandle.attrHandle = 0x0002; /* Specify a handle for your characteristic */
    myHandle.value.val = (uint8 *)"\x00\x00"; /* Initial value of your characteristic */
    myHandle.value.len = 2; /* Length of the characteristic value */
    CyBle_GattsWriteAttributeValue(&myHandle, 0, &cyBle_connHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
    CyBle_GattsWriteAttributeValue(&myHandle, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);

    /* Set the permissions for the characteristic (e.g., read, write, notify) */
    //CYBLE_GATT_DB_ATTR_SET_GEN_VALUE(myHandle.attrHandle, 0x0F, 0x04, 0, 0, 0);
    
    /* Register the GATT service with the BLE stack */
    CyBle_GattsEnableAttribute(myHandle.attrHandle);
}

/***************************************************************
 * Main
 **************************************************************/
int main()
{
    CyGlobalIntEnable; 
        
    /* Start BLE stack and register the callback function */
    CyBle_Start(BleCallBack);
    
    for(;;)
    {        
        /* Call CyBle_ProcessEvents() to process BLE events */
        CyBle_ProcessEvents();
        /* Enter low power mode */
        CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);    
    }
}