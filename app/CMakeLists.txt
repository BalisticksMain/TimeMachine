cmake_minimum_required(VERSION 2.8)

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_C_COMPILER "arm-vita-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-vita-eabi-g++")

project(TimeMachine)
set(TITLE_ID "TIMEMACHI")

set(CMAKE_C_FLAGS "-Wl,-q -O3 -std=c99")

add_executable(${PROJECT_NAME}.elf
	main.c
)

target_link_libraries(${PROJECT_NAME}.elf
	vita2d
	SceDisplay_stub
	SceGxm_stub
	ScePgf_stub
	SceCtrl_stub
	SceVshBridge_stub
	SceAppMgr_stub
	SceIofilemgr_stub
	SceSysmodule_stub
	SceNpManager_stub
	SceNpCommon_stub_weak
	SceCommonDialog_stub
	ScePower_stub
	SceRegistryMgr_stub
	SceSblPostSsMgr_stub
	m
	c
	SceLibKernel_stub
	taihen_stub
	user_clockset_stub_weak
)

add_custom_target(${PROJECT_NAME}.vpk ALL
	COMMAND vita-elf-create ${PROJECT_NAME}.elf ${PROJECT_NAME}.velf
	COMMAND vita-make-fself ${PROJECT_NAME}.velf eboot.bin
	COMMAND vita-mksfoex -s TITLE_ID=${TITLE_ID} "Time Machine" param.sfo
	COMMAND vita-pack-vpk -s param.sfo -b eboot.bin -a kern_clockset.skprx=kern_clockset.skprx -a user_clockset.suprx=user_clockset.suprx -a sce_sys/icon0.png=sce_sys/icon0.png -a sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png -a sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml TimeMachine.vpk
)
add_dependencies(${PROJECT_NAME}.vpk ${PROJECT_NAME}.elf)
