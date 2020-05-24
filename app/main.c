#include <stdio.h>
#include <string.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <psp2/kernel/threadmgr.h>
#include <vitasdk.h>
#include <taihen.h>
#include <sys/syslimits.h>
#include <appmgr_user.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vita2d.h>

#include "rtcUserBridge.h"

int ret;
SceCtrlData pad;
SceUID fd;	
int sel = 0;

int set_secure_tick(uint64_t timestamp)
{
	
	unsigned int split1 = timestamp >> 32;
	unsigned int split2 = timestamp & 0xffffffff;
	
	return briRtcSetCurrentSecureTick(split2,split1);
}

int set_current_tick(uint64_t timestamp)
{
	
	unsigned int split1 = timestamp >> 32;
	unsigned int split2 = timestamp & 0xffffffff;
	
	return briRtcSetCurrentTick(split2,split1);
}

int set_network_tick(uint64_t timestamp)
{
	
	unsigned int split1 = timestamp >> 32;
	unsigned int split2 = timestamp & 0xffffffff;
	
	return briRtcSetCurrentNetworkTick(split2,split1);
}

int set_cp_rtc(uint64_t timestamp)
{
	int epoch;
	epoch = ((timestamp / 1000) - 62135596800000) / 1000;
	return briSblPostSsMgrSetCpRtc(epoch);
}


void main() {

	char kplugin_path[0x200];
	memset(kplugin_path,0x00,0x200);
	
	char uplugin_path[0x200];
	memset(uplugin_path,0x00,0x200);
	
	char titleid[12];
	sceAppMgrUmount("app0:");
	
	
	sceAppMgrAppParamGetString(0, 12, titleid , 256);
	sprintf(kplugin_path, "ux0:app/%s/kern_clockset.skprx", titleid);
	int kernel_modid = 0;
	kernel_modid = taiLoadStartKernelModule(kplugin_path, 0, NULL, 0);
	if(kernel_modid <= 0 && kernel_modid != 0x8002d013 && kernel_modid != 0x8002d004)
	{
		sceKernelExitProcess(0);
	}
	
	
	sceAppMgrAppParamGetString(0, 12, titleid , 256);
	sprintf(uplugin_path, "ux0:app/%s/user_clockset.suprx", titleid);
	int user_modid = 0;
	user_modid = sceKernelLoadStartModule(uplugin_path, 0, NULL, 0, NULL, NULL);
		
	if(user_modid <= 0)
	{
		sceKernelExitProcess(0);
	}
	

	
	vita2d_pgf *pgf;

	vita2d_init();
	vita2d_set_clear_color(RGBA8(0, 0, 0, 255));

	pgf = vita2d_load_default_pgf();

	
	goto draw_main_screen;
draw_main_screen:
	sel = 0;
	while(1){
		memset(&pad, 0, sizeof(pad));
		sceCtrlPeekBufferPositive(0, &pad, 1);
		if(pad.buttons == SCE_CTRL_UP)
		{
			if(sel != 0)
				sel -= 1;
			else
				sel = 3;
			sceKernelDelayThread(100000);
		}
		else if(pad.buttons == SCE_CTRL_DOWN)
		{
			if(sel != 3)
				sel += 1;
			else
				sel = 0;
			sceKernelDelayThread(100000);
		}
		else if(pad.buttons == SCE_CTRL_CROSS)
		{
			sceKernelDelayThread(100000);
			goto draw_time_set_screen;
		}
		
		vita2d_start_drawing();
		vita2d_clear_screen();
	
		vita2d_pgf_draw_text(pgf, 1, 20, RGBA8(255, 255, 255, 255), 1.0f, "Time Machine");
                switch(sel){
	        case 0:
		vita2d_pgf_draw_text(pgf, 1, 40, RGBA8(0, 255, 0, 255), 1.0f, "CurrentTick");
                vita2d_pgf_draw_text(pgf, 1, 60, RGBA8(255, 255, 255, 255), 1.0f, "NetworkTick");
                vita2d_pgf_draw_text(pgf, 1, 80, RGBA8(255, 255, 255, 255), 1.0f, "SecureTick");
                vita2d_pgf_draw_text(pgf, 1, 100, RGBA8(255, 255, 255, 255), 1.0f, "CpRtc (DevKit Only)");
		break;
                case 1:
                vita2d_pgf_draw_text(pgf, 1, 40, RGBA8(255, 255, 255, 255), 1.0f, "CurrentTick");
		vita2d_pgf_draw_text(pgf, 1, 60, RGBA8(0, 255, 0, 255), 1.0f, "NetworkTick");
                vita2d_pgf_draw_text(pgf, 1, 80, RGBA8(255, 255, 255, 255), 1.0f, "SecureTick");
                vita2d_pgf_draw_text(pgf, 1, 100, RGBA8(255, 255, 255, 255), 1.0f, "CpRtc (DevKit Only)");
                break;
                case 2:
	        vita2d_pgf_draw_text(pgf, 1, 40, RGBA8(255, 255, 255, 255), 1.0f, "CurrentTick");
                vita2d_pgf_draw_text(pgf, 1, 60, RGBA8(255, 255, 255, 255), 1.0f, "NetworkTick");
		vita2d_pgf_draw_text(pgf, 1, 80, RGBA8(0, 255, 0, 255), 1.0f, "SecureTick");
                vita2d_pgf_draw_text(pgf, 1, 100, RGBA8(255, 255, 255, 255), 1.0f, "CpRtc (DevKit Only)");
	        break;
       	        case 3:
                vita2d_pgf_draw_text(pgf, 1, 40, RGBA8(255, 255, 255, 255), 1.0f, "CurrentTick");
                vita2d_pgf_draw_text(pgf, 1, 60, RGBA8(255, 255, 255, 255), 1.0f, "NetworkTick");
                vita2d_pgf_draw_text(pgf, 1, 80, RGBA8(255, 255, 255, 255), 1.0f, "SecureTick");
 		vita2d_pgf_draw_text(pgf, 1, 100, RGBA8(0, 255, 0, 255), 1.0f, "CpRtc (DevKit Only)");
                break;
		}

          
		
		vita2d_end_drawing();
		vita2d_swap_buffers();
		

	}
	
draw_time_set_screen:
	vita2d_set_clear_color(RGBA8(0, 0, 0, 255));

	sceKernelDelayThread(150000);
	int selectedPartOfTime = 0;
	
	SceDateTime dateTime;
	memset(&dateTime,0x00,sizeof(SceDateTime)); 
	
	SceRtcTick curTime = {0};
	
	if(sel == 1)
		sceRtcGetCurrentNetworkTick(&curTime);
	else
		sceRtcGetCurrentTick(&curTime);
	
	sceRtcConvertUtcToLocalTime(&curTime,&curTime);
	
	sceRtcSetTick(&dateTime,&curTime);

	char day[0x10];
	char mounth[0x10];
	char year[0x10];

	char second[0x10];
	char minute[0x10];
	char hour[0x10];
	
	memset(day,0x00,0x10);
	memset(mounth,0x00,0x10);
	memset(year,0x00,0x10);
	
	memset(second,0x00,0x10);
	memset(minute,0x00,0x10);
	memset(hour,0x00,0x10);

		
	char buf[0x100];
	memset(buf,0x00,0x100);
	
	char timestamp_string[0x500];
	memset(timestamp_string,0x00,0x500);
	
	int time_sel = 0;
	
	while(1){
		memset(&pad, 0, sizeof(pad));
		sceCtrlPeekBufferPositive(0, &pad, 1);
		if(pad.buttons == SCE_CTRL_LEFT)
		{
			if(time_sel != 0)
				time_sel -= 1;
			else
				time_sel = 5;
			sceKernelDelayThread(100000);
		}
		else if(pad.buttons == SCE_CTRL_RIGHT)
		{
			if(time_sel != 5)
				time_sel += 1;
			else
				time_sel = 0;
			sceKernelDelayThread(100000);
		}
		else if(pad.buttons == SCE_CTRL_UP)
		{
			switch(time_sel){
				case 0: 
					dateTime.day ++;
										
					ret = sceRtcGetTick(&dateTime, &curTime);
					if(ret < 0)
						dateTime.day --;
					
					break;
				case 1:
					dateTime.month ++;
															
					ret = sceRtcGetTick(&dateTime, &curTime);
					if(ret < 0)
						dateTime.month --;
					
					break;
				case 2:
					dateTime.year ++;
					
					ret = sceRtcGetTick(&dateTime, &curTime);
					if(ret < 0)
						dateTime.year --;
					
					break;
				case 3:
					dateTime.hour ++;
					
					ret = sceRtcGetTick(&dateTime, &curTime);
					if(ret < 0)
						dateTime.hour --;
					
					break;
				case 4:
					dateTime.minute ++;
					
					ret = sceRtcGetTick(&dateTime, &curTime);
					if(ret < 0)
						dateTime.minute --;
					
					break;
				case 5:
					dateTime.second ++;

					ret = sceRtcGetTick(&dateTime, &curTime);
					if(ret < 0)
						dateTime.second --;
					
					break;
			}
			sceKernelDelayThread(100000);
		}
		else if(pad.buttons == SCE_CTRL_DOWN)
		{
			switch(time_sel){
				case 0: 
					dateTime.day --;
										
					ret = sceRtcGetTick(&dateTime, &curTime);
					if(ret < 0)
						dateTime.day ++;
					
					break;
				case 1:
					dateTime.month --;
															
					ret = sceRtcGetTick(&dateTime, &curTime);
					if(ret < 0)
						dateTime.month ++;
					
					break;
				case 2:
					dateTime.year --;
					
					ret = sceRtcGetTick(&dateTime, &curTime);
					if(ret < 0)
						dateTime.year ++;
					
					break;
				case 3:
					dateTime.hour --;
					
					ret = sceRtcGetTick(&dateTime, &curTime);
					if(ret < 0)
						dateTime.hour ++;
					
					break;
				case 4:
					dateTime.minute --;
					
					ret = sceRtcGetTick(&dateTime, &curTime);
					if(ret < 0)
						dateTime.minute ++;
					
					break;
				case 5:
					dateTime.second --;

					ret = sceRtcGetTick(&dateTime, &curTime);
					if(ret < 0)
						dateTime.second ++;
					
					break;
			}
			sceKernelDelayThread(100000);
		}
		else if(pad.buttons == SCE_CTRL_CIRCLE)
		{
			goto draw_main_screen;
			break;				
		}
		else if(pad.buttons == SCE_CTRL_CROSS)
		{
		  sceRtcConvertLocalTimeToUtc(&curTime, &curTime);
			switch(sel)
			{
				case 0:
					ret = set_current_tick(curTime.tick);

					sprintf(buf,"ksceRtcSetCurrentTick has ret 0x%x",ret);
					
					vita2d_start_drawing();
					vita2d_clear_screen();

					vita2d_pgf_draw_text(pgf, 1, 20, RGBA8(255, 255, 255, 255), 1.0f, buf);
					

					vita2d_end_drawing();
					vita2d_swap_buffers();
					sceKernelDelayThread(2000000);
					
					goto draw_main_screen;
					break;
				case 1:
					ret = set_network_tick(curTime.tick);
					
					sprintf(buf,"ksceRtcSetCurrentNetworkTick has ret 0x%x",ret);
					
					vita2d_start_drawing();
					vita2d_clear_screen();

					vita2d_pgf_draw_text(pgf, 1, 20, RGBA8(255, 255, 255, 255), 1.0f, buf);
					

					vita2d_end_drawing();
					vita2d_swap_buffers();
					sceKernelDelayThread(2000000);
					
					goto draw_main_screen;
					break;
				case 2:
					ret = set_secure_tick(curTime.tick);
					
					sprintf(buf,"ksceRtcSetCurrentSecureTick has ret 0x%x",ret);
					
					vita2d_start_drawing();
					vita2d_clear_screen();

					vita2d_pgf_draw_text(pgf, 1, 20, RGBA8(255, 255, 255, 255), 1.0f, buf);
					

					vita2d_end_drawing();
					vita2d_swap_buffers();
					sceKernelDelayThread(2000000);
					
					goto draw_main_screen;
					break;
				case 3:
					ret = set_cp_rtc(curTime.tick); 
					
					sprintf(buf,"ksceSblPostSsMgrSetCpRtc has ret 0x%x",ret);
					
					vita2d_start_drawing();
					vita2d_clear_screen();

					vita2d_pgf_draw_text(pgf, 1, 20, RGBA8(255, 255, 255, 255), 1.0f, buf);
					

					vita2d_end_drawing();
					vita2d_swap_buffers();
					sceKernelDelayThread(2000000);
					
					
					goto draw_main_screen;
					break;				
			}
		}
		vita2d_start_drawing();
		vita2d_clear_screen();
	
		vita2d_pgf_draw_text(pgf, 1, 20, RGBA8(255, 255, 255, 255), 1.0f, "Time Machine - Select Time (DD MM YYYY H M S)");
		
		snprintf(day, 0x10, "%02d",dateTime.day);
		snprintf(mounth, 0x10, "%02d",dateTime.month);
		snprintf(year, 0x10, "%04d",dateTime.year);
		
		snprintf(second, 0x10, "%02d",dateTime.second);
		snprintf(minute, 0x10, "%02d",dateTime.minute);
		snprintf(hour, 0x10, "%02d",dateTime.hour);
		
		snprintf(timestamp_string,0x500,"Timestamp: 0x%llx",curTime.tick);
		
		if(time_sel == 0)
			vita2d_pgf_draw_text(pgf, 0, 40, RGBA8(0, 255, 0, 255), 1.0f, day);
		else
			vita2d_pgf_draw_text(pgf, 0, 40, RGBA8(255, 255, 255, 255), 1.0f, day);
		if(time_sel == 1)
			vita2d_pgf_draw_text(pgf, 40, 40, RGBA8(0, 255, 0, 255), 1.0f, mounth);
		else
			vita2d_pgf_draw_text(pgf, 40, 40, RGBA8(255, 255, 255, 255), 1.0f, mounth);
		if(time_sel == 2)
			vita2d_pgf_draw_text(pgf, 80, 40, RGBA8(0, 255, 0, 255), 1.0f, year);
		else
			vita2d_pgf_draw_text(pgf, 80, 40, RGBA8(255, 255, 255, 255), 1.0f, year);

		if(time_sel == 3)
			vita2d_pgf_draw_text(pgf, 150, 40, RGBA8(0, 255, 0, 255), 1.0f, hour);
		else
			vita2d_pgf_draw_text(pgf, 150, 40, RGBA8(255, 255, 255, 255), 1.0f, hour);
		if(time_sel == 4)
			vita2d_pgf_draw_text(pgf, 190, 40, RGBA8(0, 255, 0, 255), 1.0f, minute);
		else
			vita2d_pgf_draw_text(pgf, 190, 40, RGBA8(255, 255, 255, 255), 1.0f, minute);
		if(time_sel == 5)
			vita2d_pgf_draw_text(pgf, 230, 40, RGBA8(0, 255, 0, 255), 1.0f, second);
		else
			vita2d_pgf_draw_text(pgf, 230, 40, RGBA8(255, 255, 255, 255), 1.0f, second);
		

		vita2d_pgf_draw_text(pgf, 0, 80, RGBA8(255, 255, 255, 255), 1.0f, timestamp_string);
					
		vita2d_end_drawing();
		vita2d_swap_buffers();
	}
	

	sceKernelExitProcess(0);	
}
