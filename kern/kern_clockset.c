/*
* 
* KERNEL < - > USERLAND SceRtc BRIDGE 
* 
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <vitasdkkern.h>
#include <taihen.h>
#include "rtc.h"

int kbriRtcSetCurrentTick(unsigned int timestamp1,unsigned int timestamp2)
{
    unsigned int timestamp[2];
    
    timestamp[0] = timestamp1;
    timestamp[1] = timestamp2;
    
    return ksceRtcSetCurrentTick(timestamp);
}

int kbriRtcSetCurrentNetworkTick(unsigned int timestamp1,unsigned int timestamp2)
{
    unsigned int timestamp[2];
    
    timestamp[0] = timestamp1;
    timestamp[1] = timestamp2;
    
    return ksceRtcSetCurrentNetworkTick(timestamp);
}

int kbriRtcSetCurrentSecureTick(unsigned int timestamp1,unsigned int timestamp2)
{
    unsigned int timestamp[2];
    
    timestamp[0] = timestamp1;
    timestamp[1] = timestamp2;
    
    return ksceRtcSetCurrentSecureTick(timestamp);
}

int kbriSblPostSsMgrSetCpRtc(unsigned int timestamp)
{
	return ksceSblPostSsMgrSetCpRtc(timestamp);
}


void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args)
{
	/*
	*	1/1/2015 check BTFO'd 
	*	
	*	Vita checks elsewhere though :'(
	*/
	
	tai_module_info_t info;
	info.size = sizeof(tai_module_info_t);
	if (taiGetModuleInfoForKernel(KERNEL_PID, "SceRtc", &info) < 0)
	{
		return SCE_KERNEL_START_SUCCESS;
	}

	uint32_t patched_instruction = 0xBF00BF00;	//NOP
    taiInjectDataForKernel(KERNEL_PID, info.modid, 0, 0x93C, &patched_instruction, 0x4); //bcs.w LAB_81000a64 -> NOP (SetCurrentSecureTick)
	taiInjectDataForKernel(KERNEL_PID, info.modid, 0, 0xA94, &patched_instruction, 0x4); //bcs.w LAB_81000bbc -> NOP (SetCurrentNetworkTick)
	
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args)
{
    return SCE_KERNEL_STOP_SUCCESS;
}


