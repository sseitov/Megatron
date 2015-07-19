
/* File generated by gen_cfile.py. Should not be modified. */

#ifndef 2088C_H
#define 2088C_H

#include "data.h"

/* Prototypes of function provided by object dictionnary */
UNS32 _valueRangeTest (UNS8 typeValue, void * value);
const indextable * _scanIndexOD (UNS16 wIndex, UNS32 * errorCode, ODCallback_t **callbacks);

/* Master node data struct */
extern CO_Data _Data;
extern UNS8 Type_Code_of_CAN_2000C_Module_Type_Code_of_PO_ch0;		/* Mapped at index 0x2004, subindex 0x01 */
extern UNS8 Type_Code_of_CAN_2000C_Module_Type_Code_of_PO_ch1;		/* Mapped at index 0x2004, subindex 0x02 */
extern UNS8 Type_Code_of_CAN_2000C_Module_Type_Code_of_PO_ch2;		/* Mapped at index 0x2004, subindex 0x03 */
extern UNS8 Type_Code_of_CAN_2000C_Module_Type_Code_of_PO_ch3;		/* Mapped at index 0x2004, subindex 0x04 */
extern UNS8 Type_Code_of_CAN_2000C_Module_Type_Code_of_PO_ch4;		/* Mapped at index 0x2004, subindex 0x05 */
extern UNS8 Type_Code_of_CAN_2000C_Module_Type_Code_of_PO_ch5;		/* Mapped at index 0x2004, subindex 0x06 */
extern UNS8 Type_Code_of_CAN_2000C_Module_Type_Code_of_PO_ch6;		/* Mapped at index 0x2004, subindex 0x07 */
extern UNS8 Type_Code_of_CAN_2000C_Module_Type_Code_of_PO_ch7;		/* Mapped at index 0x2004, subindex 0x08 */
extern UNS32 Read_High_Speed_Counter_High_Speed_Counter_ch0;		/* Mapped at index 0x3000, subindex 0x01 */
extern UNS32 Read_High_Speed_Counter_High_Speed_Counter_ch1;		/* Mapped at index 0x3000, subindex 0x02 */
extern UNS32 Read_High_Speed_Counter_High_Speed_Counter_ch2;		/* Mapped at index 0x3000, subindex 0x03 */
extern UNS32 Read_High_Speed_Counter_High_Speed_Counter_ch3;		/* Mapped at index 0x3000, subindex 0x04 */
extern UNS32 Read_High_Speed_Counter_High_Speed_Counter_ch4;		/* Mapped at index 0x3000, subindex 0x05 */
extern UNS32 Read_High_Speed_Counter_High_Speed_Counter_ch5;		/* Mapped at index 0x3000, subindex 0x06 */
extern UNS32 Read_High_Speed_Counter_High_Speed_Counter_ch6;		/* Mapped at index 0x3000, subindex 0x07 */
extern UNS32 Read_High_Speed_Counter_High_Speed_Counter_ch7;		/* Mapped at index 0x3000, subindex 0x08 */
extern UNS8 Clear_Counter_Clear_Counter_ch0;		/* Mapped at index 0x3002, subindex 0x01 */
extern UNS8 Clear_Counter_Clear_Counter_ch1;		/* Mapped at index 0x3002, subindex 0x02 */
extern UNS8 Clear_Counter_Clear_Counter_ch2;		/* Mapped at index 0x3002, subindex 0x03 */
extern UNS8 Clear_Counter_Clear_Counter_ch3;		/* Mapped at index 0x3002, subindex 0x04 */
extern UNS8 Clear_Counter_Clear_Counter_ch4;		/* Mapped at index 0x3002, subindex 0x05 */
extern UNS8 Clear_Counter_Clear_Counter_ch5;		/* Mapped at index 0x3002, subindex 0x06 */
extern UNS8 Clear_Counter_Clear_Counter_ch6;		/* Mapped at index 0x3002, subindex 0x07 */
extern UNS8 Clear_Counter_Clear_Counter_ch7;		/* Mapped at index 0x3002, subindex 0x08 */
extern UNS8 Pulse_Output_Start_Pulse_Outputs_ch0;		/* Mapped at index 0x3100, subindex 0x01 */
extern UNS8 Pulse_Output_Start_Pulse_Outputs_ch1;		/* Mapped at index 0x3100, subindex 0x02 */
extern UNS8 Pulse_Output_Start_Pulse_Outputs_ch2;		/* Mapped at index 0x3100, subindex 0x03 */
extern UNS8 Pulse_Output_Start_Pulse_Outputs_ch3;		/* Mapped at index 0x3100, subindex 0x04 */
extern UNS8 Pulse_Output_Start_Pulse_Outputs_ch4;		/* Mapped at index 0x3100, subindex 0x05 */
extern UNS8 Pulse_Output_Start_Pulse_Outputs_ch5;		/* Mapped at index 0x3100, subindex 0x06 */
extern UNS8 Pulse_Output_Start_Pulse_Outputs_ch6;		/* Mapped at index 0x3100, subindex 0x07 */
extern UNS8 Pulse_Output_Start_Pulse_Outputs_ch7;		/* Mapped at index 0x3100, subindex 0x08 */
extern UNS16 Set_Burst_Count_16_Bit_Burst_Count_ch0;		/* Mapped at index 0x3101, subindex 0x01 */
extern UNS16 Set_Burst_Count_16_Bit_Burst_Count_ch1;		/* Mapped at index 0x3101, subindex 0x02 */
extern UNS16 Set_Burst_Count_16_Bit_Burst_Count_ch2;		/* Mapped at index 0x3101, subindex 0x03 */
extern UNS16 Set_Burst_Count_16_Bit_Burst_Count_ch3;		/* Mapped at index 0x3101, subindex 0x04 */
extern UNS16 Set_Burst_Count_16_Bit_Burst_Count_ch4;		/* Mapped at index 0x3101, subindex 0x05 */
extern UNS16 Set_Burst_Count_16_Bit_Burst_Count_ch5;		/* Mapped at index 0x3101, subindex 0x06 */
extern UNS16 Set_Burst_Count_16_Bit_Burst_Count_ch6;		/* Mapped at index 0x3101, subindex 0x07 */
extern UNS16 Set_Burst_Count_16_Bit_Burst_Count_ch7;		/* Mapped at index 0x3101, subindex 0x08 */
extern UNS32 Set_Pulse_Frequency_32_Bit_Pulse_Frequency_ch0;		/* Mapped at index 0x3102, subindex 0x01 */
extern UNS32 Set_Pulse_Frequency_32_Bit_Pulse_Frequency_ch1;		/* Mapped at index 0x3102, subindex 0x02 */
extern UNS32 Set_Pulse_Frequency_32_Bit_Pulse_Frequency_ch2;		/* Mapped at index 0x3102, subindex 0x03 */
extern UNS32 Set_Pulse_Frequency_32_Bit_Pulse_Frequency_ch3;		/* Mapped at index 0x3102, subindex 0x04 */
extern UNS32 Set_Pulse_Frequency_32_Bit_Pulse_Frequency_ch4;		/* Mapped at index 0x3102, subindex 0x05 */
extern UNS32 Set_Pulse_Frequency_32_Bit_Pulse_Frequency_ch5;		/* Mapped at index 0x3102, subindex 0x06 */
extern UNS32 Set_Pulse_Frequency_32_Bit_Pulse_Frequency_ch6;		/* Mapped at index 0x3102, subindex 0x07 */
extern UNS32 Set_Pulse_Frequency_32_Bit_Pulse_Frequency_ch7;		/* Mapped at index 0x3102, subindex 0x08 */
extern UNS16 Set_Pulse_Duty_16_Bit_Pulse_Duty_ch0;		/* Mapped at index 0x3103, subindex 0x01 */
extern UNS16 Set_Pulse_Duty_16_Bit_Pulse_Duty_ch1;		/* Mapped at index 0x3103, subindex 0x02 */
extern UNS16 Set_Pulse_Duty_16_Bit_Pulse_Duty_ch2;		/* Mapped at index 0x3103, subindex 0x03 */
extern UNS16 Set_Pulse_Duty_16_Bit_Pulse_Duty_ch3;		/* Mapped at index 0x3103, subindex 0x04 */
extern UNS16 Set_Pulse_Duty_16_Bit_Pulse_Duty_ch4;		/* Mapped at index 0x3103, subindex 0x05 */
extern UNS16 Set_Pulse_Duty_16_Bit_Pulse_Duty_ch5;		/* Mapped at index 0x3103, subindex 0x06 */
extern UNS16 Set_Pulse_Duty_16_Bit_Pulse_Duty_ch6;		/* Mapped at index 0x3103, subindex 0x07 */
extern UNS16 Set_Pulse_Duty_16_Bit_Pulse_Duty_ch7;		/* Mapped at index 0x3103, subindex 0x08 */
extern UNS8 Set_Hardware_Trig_8_Bit_Hardware_Trig_ch0;		/* Mapped at index 0x3104, subindex 0x01 */
extern UNS8 Set_Hardware_Trig_8_Bit_Hardware_Trig_ch1;		/* Mapped at index 0x3104, subindex 0x02 */
extern UNS8 Set_Hardware_Trig_8_Bit_Hardware_Trig_ch2;		/* Mapped at index 0x3104, subindex 0x03 */
extern UNS8 Set_Hardware_Trig_8_Bit_Hardware_Trig_ch3;		/* Mapped at index 0x3104, subindex 0x04 */
extern UNS8 Set_Hardware_Trig_8_Bit_Hardware_Trig_ch4;		/* Mapped at index 0x3104, subindex 0x05 */
extern UNS8 Set_Hardware_Trig_8_Bit_Hardware_Trig_ch5;		/* Mapped at index 0x3104, subindex 0x06 */
extern UNS8 Set_Hardware_Trig_8_Bit_Hardware_Trig_ch6;		/* Mapped at index 0x3104, subindex 0x07 */
extern UNS8 Set_Hardware_Trig_8_Bit_Hardware_Trig_ch7;		/* Mapped at index 0x3104, subindex 0x08 */
extern UNS8 Set_Sync_Channel_8_Bit_Sync_Channel_ch0;		/* Mapped at index 0x3105, subindex 0x01 */
extern UNS8 Set_Sync_Channel_8_Bit_Sync_Channel_ch1;		/* Mapped at index 0x3105, subindex 0x02 */
extern UNS8 Set_Sync_Channel_8_Bit_Sync_Channel_ch2;		/* Mapped at index 0x3105, subindex 0x03 */
extern UNS8 Set_Sync_Channel_8_Bit_Sync_Channel_ch3;		/* Mapped at index 0x3105, subindex 0x04 */
extern UNS8 Set_Sync_Channel_8_Bit_Sync_Channel_ch4;		/* Mapped at index 0x3105, subindex 0x05 */
extern UNS8 Set_Sync_Channel_8_Bit_Sync_Channel_ch5;		/* Mapped at index 0x3105, subindex 0x06 */
extern UNS8 Set_Sync_Channel_8_Bit_Sync_Channel_ch6;		/* Mapped at index 0x3105, subindex 0x07 */
extern UNS8 Set_Sync_Channel_8_Bit_Sync_Channel_ch7;		/* Mapped at index 0x3105, subindex 0x08 */
extern UNS8 Set_Sync_Start_8_Bit_Sync_Start;		/* Mapped at index 0x3106, subindex 0x01 */
extern UNS16 Set_Deceleration_16_Bit_Deceleration_ch0;		/* Mapped at index 0x3110, subindex 0x01 */
extern UNS16 Set_Deceleration_16_Bit_Deceleration_ch1;		/* Mapped at index 0x3110, subindex 0x02 */
extern UNS16 Set_Deceleration_16_Bit_Deceleration_ch2;		/* Mapped at index 0x3110, subindex 0x03 */
extern UNS16 Set_Deceleration_16_Bit_Deceleration_ch3;		/* Mapped at index 0x3110, subindex 0x04 */
extern UNS16 Set_Deceleration_16_Bit_Deceleration_ch4;		/* Mapped at index 0x3110, subindex 0x05 */
extern UNS16 Set_Deceleration_16_Bit_Deceleration_ch5;		/* Mapped at index 0x3110, subindex 0x06 */
extern UNS16 Set_Deceleration_16_Bit_Deceleration_ch6;		/* Mapped at index 0x3110, subindex 0x07 */
extern UNS16 Set_Deceleration_16_Bit_Deceleration_ch7;		/* Mapped at index 0x3110, subindex 0x08 */
extern UNS16 Set_Acceleration_16_Bit_Acceleration_ch0;		/* Mapped at index 0x3111, subindex 0x01 */
extern UNS16 Set_Acceleration_16_Bit_Acceleration_ch1;		/* Mapped at index 0x3111, subindex 0x02 */
extern UNS16 Set_Acceleration_16_Bit_Acceleration_ch2;		/* Mapped at index 0x3111, subindex 0x03 */
extern UNS16 Set_Acceleration_16_Bit_Acceleration_ch3;		/* Mapped at index 0x3111, subindex 0x04 */
extern UNS16 Set_Acceleration_16_Bit_Acceleration_ch4;		/* Mapped at index 0x3111, subindex 0x05 */
extern UNS16 Set_Acceleration_16_Bit_Acceleration_ch5;		/* Mapped at index 0x3111, subindex 0x06 */
extern UNS16 Set_Acceleration_16_Bit_Acceleration_ch6;		/* Mapped at index 0x3111, subindex 0x07 */
extern UNS16 Set_Acceleration_16_Bit_Acceleration_ch7;		/* Mapped at index 0x3111, subindex 0x08 */
extern UNS8 Set_Channel_Group_8_Bit_Channel_Group_ch0;		/* Mapped at index 0x3112, subindex 0x01 */
extern UNS8 Set_Channel_Group_8_Bit_Channel_Group_ch1;		/* Mapped at index 0x3112, subindex 0x02 */
extern UNS8 Set_Channel_Group_8_Bit_Channel_Group_ch2;		/* Mapped at index 0x3112, subindex 0x03 */
extern UNS8 Set_Channel_Group_8_Bit_Channel_Group_ch3;		/* Mapped at index 0x3112, subindex 0x04 */
extern UNS8 Set_Channel_Group_8_Bit_Channel_Group_ch4;		/* Mapped at index 0x3112, subindex 0x05 */
extern UNS8 Set_Channel_Group_8_Bit_Channel_Group_ch5;		/* Mapped at index 0x3112, subindex 0x06 */
extern UNS8 Set_Channel_Group_8_Bit_Channel_Group_ch6;		/* Mapped at index 0x3112, subindex 0x07 */
extern UNS8 Set_Channel_Group_8_Bit_Channel_Group_ch7;		/* Mapped at index 0x3112, subindex 0x08 */
extern UNS8 Read_Inputs_8_Bit[1];		/* Mapped at index 0x6000, subindex 0x01 - 0x01 */

#endif // 2088C_H