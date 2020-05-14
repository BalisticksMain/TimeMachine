// userland <-> userland bridge

#include <psp2/kernel/modulemgr.h>
#include <taihen.h>
#include <vitasdk.h>
#include "rtcKernelBridge.h"

int briRtcSetCurrentTick(unsigned int timestamp1, unsigned int timestamp2)
{
    return kbriRtcSetCurrentTick(timestamp1,timestamp2);
}

int briRtcSetCurrentNetworkTick(unsigned int timestamp1, unsigned int timestamp2)
{
    return kbriRtcSetCurrentNetworkTick(timestamp1,timestamp2);
}

int briRtcSetCurrentSecureTick(unsigned int timestamp1, unsigned int timestamp2)
{
    return kbriRtcSetCurrentSecureTick(timestamp1,timestamp2);
}

int briSblPostSsMgrSetCpRtc(unsigned int timestamp)
{
    return kbriSblPostSsMgrSetCpRtc(timestamp);
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args) {
  return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
  return SCE_KERNEL_STOP_SUCCESS;
}
