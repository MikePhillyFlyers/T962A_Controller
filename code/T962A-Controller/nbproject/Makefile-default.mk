#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/T962A_Controller.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/T962A_Controller.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=source/__main.c source/initialize.c source/spi.c source/timers.c source/uarts.c source/lcd.c source/import.s source/onewire.c source/sched.c source/buzzer.c source/circbuffer.c source/eeprom.c source/i2c.c source/systemfan.c source/setup.c source/reflow.c source/reflow_profiles.c source/rtc.c source/PID_v1.c source/nvstorage.c source/io.c source/sensor.c source/max31855.c source/keypad.c source/main_work.c source/vic.c source/serial.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/source/__main.o ${OBJECTDIR}/source/initialize.o ${OBJECTDIR}/source/spi.o ${OBJECTDIR}/source/timers.o ${OBJECTDIR}/source/uarts.o ${OBJECTDIR}/source/lcd.o ${OBJECTDIR}/source/import.o ${OBJECTDIR}/source/onewire.o ${OBJECTDIR}/source/sched.o ${OBJECTDIR}/source/buzzer.o ${OBJECTDIR}/source/circbuffer.o ${OBJECTDIR}/source/eeprom.o ${OBJECTDIR}/source/i2c.o ${OBJECTDIR}/source/systemfan.o ${OBJECTDIR}/source/setup.o ${OBJECTDIR}/source/reflow.o ${OBJECTDIR}/source/reflow_profiles.o ${OBJECTDIR}/source/rtc.o ${OBJECTDIR}/source/PID_v1.o ${OBJECTDIR}/source/nvstorage.o ${OBJECTDIR}/source/io.o ${OBJECTDIR}/source/sensor.o ${OBJECTDIR}/source/max31855.o ${OBJECTDIR}/source/keypad.o ${OBJECTDIR}/source/main_work.o ${OBJECTDIR}/source/vic.o ${OBJECTDIR}/source/serial.o
POSSIBLE_DEPFILES=${OBJECTDIR}/source/__main.o.d ${OBJECTDIR}/source/initialize.o.d ${OBJECTDIR}/source/spi.o.d ${OBJECTDIR}/source/timers.o.d ${OBJECTDIR}/source/uarts.o.d ${OBJECTDIR}/source/lcd.o.d ${OBJECTDIR}/source/import.o.d ${OBJECTDIR}/source/onewire.o.d ${OBJECTDIR}/source/sched.o.d ${OBJECTDIR}/source/buzzer.o.d ${OBJECTDIR}/source/circbuffer.o.d ${OBJECTDIR}/source/eeprom.o.d ${OBJECTDIR}/source/i2c.o.d ${OBJECTDIR}/source/systemfan.o.d ${OBJECTDIR}/source/setup.o.d ${OBJECTDIR}/source/reflow.o.d ${OBJECTDIR}/source/reflow_profiles.o.d ${OBJECTDIR}/source/rtc.o.d ${OBJECTDIR}/source/PID_v1.o.d ${OBJECTDIR}/source/nvstorage.o.d ${OBJECTDIR}/source/io.o.d ${OBJECTDIR}/source/sensor.o.d ${OBJECTDIR}/source/max31855.o.d ${OBJECTDIR}/source/keypad.o.d ${OBJECTDIR}/source/main_work.o.d ${OBJECTDIR}/source/vic.o.d ${OBJECTDIR}/source/serial.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/source/__main.o ${OBJECTDIR}/source/initialize.o ${OBJECTDIR}/source/spi.o ${OBJECTDIR}/source/timers.o ${OBJECTDIR}/source/uarts.o ${OBJECTDIR}/source/lcd.o ${OBJECTDIR}/source/import.o ${OBJECTDIR}/source/onewire.o ${OBJECTDIR}/source/sched.o ${OBJECTDIR}/source/buzzer.o ${OBJECTDIR}/source/circbuffer.o ${OBJECTDIR}/source/eeprom.o ${OBJECTDIR}/source/i2c.o ${OBJECTDIR}/source/systemfan.o ${OBJECTDIR}/source/setup.o ${OBJECTDIR}/source/reflow.o ${OBJECTDIR}/source/reflow_profiles.o ${OBJECTDIR}/source/rtc.o ${OBJECTDIR}/source/PID_v1.o ${OBJECTDIR}/source/nvstorage.o ${OBJECTDIR}/source/io.o ${OBJECTDIR}/source/sensor.o ${OBJECTDIR}/source/max31855.o ${OBJECTDIR}/source/keypad.o ${OBJECTDIR}/source/main_work.o ${OBJECTDIR}/source/vic.o ${OBJECTDIR}/source/serial.o

# Source Files
SOURCEFILES=source/__main.c source/initialize.c source/spi.c source/timers.c source/uarts.c source/lcd.c source/import.s source/onewire.c source/sched.c source/buzzer.c source/circbuffer.c source/eeprom.c source/i2c.c source/systemfan.c source/setup.c source/reflow.c source/reflow_profiles.c source/rtc.c source/PID_v1.c source/nvstorage.c source/io.c source/sensor.c source/max31855.c source/keypad.c source/main_work.c source/vic.c source/serial.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/T962A_Controller.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MZ2048EFH064
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/source/import.o: source/import.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/import.o 
	@${FIXDEPS} "${OBJECTDIR}/source/import.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -DICD3PlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${OBJECTDIR}/source/import.o source/import.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=ICD3PlatformTool=1,--gdwarf-2,-MD="${OBJECTDIR}/source/import.o.d",-I"source/includes" -I"source/pic32/includes"
else
${OBJECTDIR}/source/import.o: source/import.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/import.o 
	@${FIXDEPS} "${OBJECTDIR}/source/import.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${OBJECTDIR}/source/import.o source/import.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--gdwarf-2,-MD="${OBJECTDIR}/source/import.o.d",-I"source/includes" -I"source/pic32/includes"
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/source/__main.o: source/__main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/__main.o.d 
	@${RM} ${OBJECTDIR}/source/__main.o 
	@${FIXDEPS} "${OBJECTDIR}/source/__main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/__main.o.d" -o ${OBJECTDIR}/source/__main.o source/__main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/initialize.o: source/initialize.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/initialize.o.d 
	@${RM} ${OBJECTDIR}/source/initialize.o 
	@${FIXDEPS} "${OBJECTDIR}/source/initialize.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/initialize.o.d" -o ${OBJECTDIR}/source/initialize.o source/initialize.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/spi.o: source/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/spi.o.d 
	@${RM} ${OBJECTDIR}/source/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/source/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/spi.o.d" -o ${OBJECTDIR}/source/spi.o source/spi.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/timers.o: source/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/timers.o.d 
	@${RM} ${OBJECTDIR}/source/timers.o 
	@${FIXDEPS} "${OBJECTDIR}/source/timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/timers.o.d" -o ${OBJECTDIR}/source/timers.o source/timers.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/uarts.o: source/uarts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/uarts.o.d 
	@${RM} ${OBJECTDIR}/source/uarts.o 
	@${FIXDEPS} "${OBJECTDIR}/source/uarts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/uarts.o.d" -o ${OBJECTDIR}/source/uarts.o source/uarts.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/lcd.o: source/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/lcd.o.d 
	@${RM} ${OBJECTDIR}/source/lcd.o 
	@${FIXDEPS} "${OBJECTDIR}/source/lcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/lcd.o.d" -o ${OBJECTDIR}/source/lcd.o source/lcd.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/onewire.o: source/onewire.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/onewire.o.d 
	@${RM} ${OBJECTDIR}/source/onewire.o 
	@${FIXDEPS} "${OBJECTDIR}/source/onewire.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/onewire.o.d" -o ${OBJECTDIR}/source/onewire.o source/onewire.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/sched.o: source/sched.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/sched.o.d 
	@${RM} ${OBJECTDIR}/source/sched.o 
	@${FIXDEPS} "${OBJECTDIR}/source/sched.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/sched.o.d" -o ${OBJECTDIR}/source/sched.o source/sched.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/buzzer.o: source/buzzer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/buzzer.o.d 
	@${RM} ${OBJECTDIR}/source/buzzer.o 
	@${FIXDEPS} "${OBJECTDIR}/source/buzzer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/buzzer.o.d" -o ${OBJECTDIR}/source/buzzer.o source/buzzer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/circbuffer.o: source/circbuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/circbuffer.o.d 
	@${RM} ${OBJECTDIR}/source/circbuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/source/circbuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/circbuffer.o.d" -o ${OBJECTDIR}/source/circbuffer.o source/circbuffer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/eeprom.o: source/eeprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/eeprom.o.d 
	@${RM} ${OBJECTDIR}/source/eeprom.o 
	@${FIXDEPS} "${OBJECTDIR}/source/eeprom.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/eeprom.o.d" -o ${OBJECTDIR}/source/eeprom.o source/eeprom.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/i2c.o: source/i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/i2c.o.d 
	@${RM} ${OBJECTDIR}/source/i2c.o 
	@${FIXDEPS} "${OBJECTDIR}/source/i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/i2c.o.d" -o ${OBJECTDIR}/source/i2c.o source/i2c.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/systemfan.o: source/systemfan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/systemfan.o.d 
	@${RM} ${OBJECTDIR}/source/systemfan.o 
	@${FIXDEPS} "${OBJECTDIR}/source/systemfan.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/systemfan.o.d" -o ${OBJECTDIR}/source/systemfan.o source/systemfan.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/setup.o: source/setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/setup.o.d 
	@${RM} ${OBJECTDIR}/source/setup.o 
	@${FIXDEPS} "${OBJECTDIR}/source/setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/setup.o.d" -o ${OBJECTDIR}/source/setup.o source/setup.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/reflow.o: source/reflow.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/reflow.o.d 
	@${RM} ${OBJECTDIR}/source/reflow.o 
	@${FIXDEPS} "${OBJECTDIR}/source/reflow.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/reflow.o.d" -o ${OBJECTDIR}/source/reflow.o source/reflow.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/reflow_profiles.o: source/reflow_profiles.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/reflow_profiles.o.d 
	@${RM} ${OBJECTDIR}/source/reflow_profiles.o 
	@${FIXDEPS} "${OBJECTDIR}/source/reflow_profiles.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/reflow_profiles.o.d" -o ${OBJECTDIR}/source/reflow_profiles.o source/reflow_profiles.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/rtc.o: source/rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/rtc.o.d 
	@${RM} ${OBJECTDIR}/source/rtc.o 
	@${FIXDEPS} "${OBJECTDIR}/source/rtc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/rtc.o.d" -o ${OBJECTDIR}/source/rtc.o source/rtc.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/PID_v1.o: source/PID_v1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/PID_v1.o.d 
	@${RM} ${OBJECTDIR}/source/PID_v1.o 
	@${FIXDEPS} "${OBJECTDIR}/source/PID_v1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/PID_v1.o.d" -o ${OBJECTDIR}/source/PID_v1.o source/PID_v1.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/nvstorage.o: source/nvstorage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/nvstorage.o.d 
	@${RM} ${OBJECTDIR}/source/nvstorage.o 
	@${FIXDEPS} "${OBJECTDIR}/source/nvstorage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/nvstorage.o.d" -o ${OBJECTDIR}/source/nvstorage.o source/nvstorage.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/io.o: source/io.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/io.o.d 
	@${RM} ${OBJECTDIR}/source/io.o 
	@${FIXDEPS} "${OBJECTDIR}/source/io.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/io.o.d" -o ${OBJECTDIR}/source/io.o source/io.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/sensor.o: source/sensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/sensor.o.d 
	@${RM} ${OBJECTDIR}/source/sensor.o 
	@${FIXDEPS} "${OBJECTDIR}/source/sensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/sensor.o.d" -o ${OBJECTDIR}/source/sensor.o source/sensor.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/max31855.o: source/max31855.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/max31855.o.d 
	@${RM} ${OBJECTDIR}/source/max31855.o 
	@${FIXDEPS} "${OBJECTDIR}/source/max31855.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/max31855.o.d" -o ${OBJECTDIR}/source/max31855.o source/max31855.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/keypad.o: source/keypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/keypad.o.d 
	@${RM} ${OBJECTDIR}/source/keypad.o 
	@${FIXDEPS} "${OBJECTDIR}/source/keypad.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/keypad.o.d" -o ${OBJECTDIR}/source/keypad.o source/keypad.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/main_work.o: source/main_work.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/main_work.o.d 
	@${RM} ${OBJECTDIR}/source/main_work.o 
	@${FIXDEPS} "${OBJECTDIR}/source/main_work.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/main_work.o.d" -o ${OBJECTDIR}/source/main_work.o source/main_work.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/vic.o: source/vic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/vic.o.d 
	@${RM} ${OBJECTDIR}/source/vic.o 
	@${FIXDEPS} "${OBJECTDIR}/source/vic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/vic.o.d" -o ${OBJECTDIR}/source/vic.o source/vic.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/serial.o: source/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/serial.o.d 
	@${RM} ${OBJECTDIR}/source/serial.o 
	@${FIXDEPS} "${OBJECTDIR}/source/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/serial.o.d" -o ${OBJECTDIR}/source/serial.o source/serial.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/source/__main.o: source/__main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/__main.o.d 
	@${RM} ${OBJECTDIR}/source/__main.o 
	@${FIXDEPS} "${OBJECTDIR}/source/__main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/__main.o.d" -o ${OBJECTDIR}/source/__main.o source/__main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/initialize.o: source/initialize.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/initialize.o.d 
	@${RM} ${OBJECTDIR}/source/initialize.o 
	@${FIXDEPS} "${OBJECTDIR}/source/initialize.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/initialize.o.d" -o ${OBJECTDIR}/source/initialize.o source/initialize.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/spi.o: source/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/spi.o.d 
	@${RM} ${OBJECTDIR}/source/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/source/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/spi.o.d" -o ${OBJECTDIR}/source/spi.o source/spi.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/timers.o: source/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/timers.o.d 
	@${RM} ${OBJECTDIR}/source/timers.o 
	@${FIXDEPS} "${OBJECTDIR}/source/timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/timers.o.d" -o ${OBJECTDIR}/source/timers.o source/timers.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/uarts.o: source/uarts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/uarts.o.d 
	@${RM} ${OBJECTDIR}/source/uarts.o 
	@${FIXDEPS} "${OBJECTDIR}/source/uarts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/uarts.o.d" -o ${OBJECTDIR}/source/uarts.o source/uarts.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/lcd.o: source/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/lcd.o.d 
	@${RM} ${OBJECTDIR}/source/lcd.o 
	@${FIXDEPS} "${OBJECTDIR}/source/lcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/lcd.o.d" -o ${OBJECTDIR}/source/lcd.o source/lcd.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/onewire.o: source/onewire.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/onewire.o.d 
	@${RM} ${OBJECTDIR}/source/onewire.o 
	@${FIXDEPS} "${OBJECTDIR}/source/onewire.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/onewire.o.d" -o ${OBJECTDIR}/source/onewire.o source/onewire.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/sched.o: source/sched.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/sched.o.d 
	@${RM} ${OBJECTDIR}/source/sched.o 
	@${FIXDEPS} "${OBJECTDIR}/source/sched.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/sched.o.d" -o ${OBJECTDIR}/source/sched.o source/sched.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/buzzer.o: source/buzzer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/buzzer.o.d 
	@${RM} ${OBJECTDIR}/source/buzzer.o 
	@${FIXDEPS} "${OBJECTDIR}/source/buzzer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/buzzer.o.d" -o ${OBJECTDIR}/source/buzzer.o source/buzzer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/circbuffer.o: source/circbuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/circbuffer.o.d 
	@${RM} ${OBJECTDIR}/source/circbuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/source/circbuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/circbuffer.o.d" -o ${OBJECTDIR}/source/circbuffer.o source/circbuffer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/eeprom.o: source/eeprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/eeprom.o.d 
	@${RM} ${OBJECTDIR}/source/eeprom.o 
	@${FIXDEPS} "${OBJECTDIR}/source/eeprom.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/eeprom.o.d" -o ${OBJECTDIR}/source/eeprom.o source/eeprom.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/i2c.o: source/i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/i2c.o.d 
	@${RM} ${OBJECTDIR}/source/i2c.o 
	@${FIXDEPS} "${OBJECTDIR}/source/i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/i2c.o.d" -o ${OBJECTDIR}/source/i2c.o source/i2c.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/systemfan.o: source/systemfan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/systemfan.o.d 
	@${RM} ${OBJECTDIR}/source/systemfan.o 
	@${FIXDEPS} "${OBJECTDIR}/source/systemfan.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/systemfan.o.d" -o ${OBJECTDIR}/source/systemfan.o source/systemfan.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/setup.o: source/setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/setup.o.d 
	@${RM} ${OBJECTDIR}/source/setup.o 
	@${FIXDEPS} "${OBJECTDIR}/source/setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/setup.o.d" -o ${OBJECTDIR}/source/setup.o source/setup.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/reflow.o: source/reflow.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/reflow.o.d 
	@${RM} ${OBJECTDIR}/source/reflow.o 
	@${FIXDEPS} "${OBJECTDIR}/source/reflow.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/reflow.o.d" -o ${OBJECTDIR}/source/reflow.o source/reflow.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/reflow_profiles.o: source/reflow_profiles.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/reflow_profiles.o.d 
	@${RM} ${OBJECTDIR}/source/reflow_profiles.o 
	@${FIXDEPS} "${OBJECTDIR}/source/reflow_profiles.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/reflow_profiles.o.d" -o ${OBJECTDIR}/source/reflow_profiles.o source/reflow_profiles.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/rtc.o: source/rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/rtc.o.d 
	@${RM} ${OBJECTDIR}/source/rtc.o 
	@${FIXDEPS} "${OBJECTDIR}/source/rtc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/rtc.o.d" -o ${OBJECTDIR}/source/rtc.o source/rtc.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/PID_v1.o: source/PID_v1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/PID_v1.o.d 
	@${RM} ${OBJECTDIR}/source/PID_v1.o 
	@${FIXDEPS} "${OBJECTDIR}/source/PID_v1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/PID_v1.o.d" -o ${OBJECTDIR}/source/PID_v1.o source/PID_v1.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/nvstorage.o: source/nvstorage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/nvstorage.o.d 
	@${RM} ${OBJECTDIR}/source/nvstorage.o 
	@${FIXDEPS} "${OBJECTDIR}/source/nvstorage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/nvstorage.o.d" -o ${OBJECTDIR}/source/nvstorage.o source/nvstorage.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/io.o: source/io.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/io.o.d 
	@${RM} ${OBJECTDIR}/source/io.o 
	@${FIXDEPS} "${OBJECTDIR}/source/io.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/io.o.d" -o ${OBJECTDIR}/source/io.o source/io.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/sensor.o: source/sensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/sensor.o.d 
	@${RM} ${OBJECTDIR}/source/sensor.o 
	@${FIXDEPS} "${OBJECTDIR}/source/sensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/sensor.o.d" -o ${OBJECTDIR}/source/sensor.o source/sensor.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/max31855.o: source/max31855.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/max31855.o.d 
	@${RM} ${OBJECTDIR}/source/max31855.o 
	@${FIXDEPS} "${OBJECTDIR}/source/max31855.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/max31855.o.d" -o ${OBJECTDIR}/source/max31855.o source/max31855.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/keypad.o: source/keypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/keypad.o.d 
	@${RM} ${OBJECTDIR}/source/keypad.o 
	@${FIXDEPS} "${OBJECTDIR}/source/keypad.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/keypad.o.d" -o ${OBJECTDIR}/source/keypad.o source/keypad.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/main_work.o: source/main_work.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/main_work.o.d 
	@${RM} ${OBJECTDIR}/source/main_work.o 
	@${FIXDEPS} "${OBJECTDIR}/source/main_work.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/main_work.o.d" -o ${OBJECTDIR}/source/main_work.o source/main_work.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/vic.o: source/vic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/vic.o.d 
	@${RM} ${OBJECTDIR}/source/vic.o 
	@${FIXDEPS} "${OBJECTDIR}/source/vic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/vic.o.d" -o ${OBJECTDIR}/source/vic.o source/vic.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/source/serial.o: source/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/serial.o.d 
	@${RM} ${OBJECTDIR}/source/serial.o 
	@${FIXDEPS} "${OBJECTDIR}/source/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"source/includes" -I"source/pic32/includes" -MMD -MF "${OBJECTDIR}/source/serial.o.d" -o ${OBJECTDIR}/source/serial.o source/serial.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/T962A_Controller.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -DICD3PlatformTool=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/T962A_Controller.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x37F   -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=ICD3PlatformTool=1,--defsym=_min_heap_size=1024,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/T962A_Controller.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/T962A_Controller.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=1024,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/T962A_Controller.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
