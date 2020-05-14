int ksceRtcSetCurrentTick(unsigned int* timestamp);
int ksceRtcSetCurrentNetworkTick(unsigned int* timestamp);
int ksceRtcSetCurrentSecureTick(unsigned int* timestamp);

int ksceSblPostSsMgrSetCpRtc(unsigned int timestamp);

int ksceSblNvsReadData(int offset,void* buffer,int size);
int ksceSblNvsWriteData(int offset,void* buffer,int size);