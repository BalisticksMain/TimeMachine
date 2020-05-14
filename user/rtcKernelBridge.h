int kbriRtcSetCurrentTick(unsigned int timestamp1, unsigned int timestamp2);
int kbriRtcSetCurrentNetworkTick(unsigned int timestamp1, unsigned int timestamp2);
int kbriRtcSetCurrentSecureTick(unsigned int timestamp1, unsigned int timestamp2);
int kbriSblPostSsMgrSetCpRtc(unsigned int timestamp);
int kbriDumpNvsAct(unsigned int fileno);
int kbriRestoreNvsAct(unsigned int fileno);