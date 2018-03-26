#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/8d22333d/main.o \
	${OBJECTDIR}/_ext/fe436812/functions.o \
	${OBJECTDIR}/_ext/c07c2ecd/image.o \
	${OBJECTDIR}/_ext/c07c2ecd/message.o \
	${OBJECTDIR}/_ext/c07c2ecd/monitor.o \
	${OBJECTDIR}/_ext/c07c2ecd/robot.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-D_GNU_SOURCE -D_REENTRANT -fasynchronous-unwind-tables -D__MERCURY__ -I/usr/xenomai/include/alchemy -g -D_WITH_TRACE_ -I/usr/xenomai/include/ -I/usr/xenomai/include/mercury -MMD -MP
CXXFLAGS=-D_GNU_SOURCE -D_REENTRANT -fasynchronous-unwind-tables -D__MERCURY__ -I/usr/xenomai/include/alchemy -g -D_WITH_TRACE_ -I/usr/xenomai/include/ -I/usr/xenomai/include/mercury -MMD -MP

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/elonmusk

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/elonmusk: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/elonmusk ${OBJECTFILES} ${LDLIBSOPTIONS} -D_GNU_SOURCE -D_REENTRANT -fasynchronous-unwind-tables -D__MERCURY__ -I/usr/xenomai/include/alchemy -L/usr/xenomai/lib -lalchemy -lcopperplate -lmercury -L/opt/vc/lib -I/usr/local/include -lopencv_highgui -lopencv_core -lopencv_imgproc -Wl,--no-as-needed -lalchemy -lcopperplate /usr/xenomai/lib/xenomai/bootstrap.o -Wl,--wrap=main -Wl,--dynamic-list=/usr/xenomai/lib/dynlist.ld -L/usr/xenomai/lib -lmercury -lpthread -lrt -Wl,-rpath /usr/xenomai/lib -lopencv_highgui -lopencv_core -lopencv_imgcodecs -lraspicam_cv -lopencv_imgproc -lpthread

${OBJECTDIR}/_ext/8d22333d/main.o: ../../TP\ temps\ reel/superviseur_robot/destijl_init/main.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/8d22333d
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/8d22333d/main.o ../../TP\ temps\ reel/superviseur_robot/destijl_init/main.cpp

${OBJECTDIR}/_ext/fe436812/functions.o: ../../TP\ temps\ reel/superviseur_robot/destijl_init/src/functions.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/fe436812
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fe436812/functions.o ../../TP\ temps\ reel/superviseur_robot/destijl_init/src/functions.cpp

${OBJECTDIR}/_ext/c07c2ecd/image.o: ../../TP\ temps\ reel/superviseur_robot/src/image.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c07c2ecd
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c07c2ecd/image.o ../../TP\ temps\ reel/superviseur_robot/src/image.cpp

${OBJECTDIR}/_ext/c07c2ecd/message.o: ../../TP\ temps\ reel/superviseur_robot/src/message.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c07c2ecd
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c07c2ecd/message.o ../../TP\ temps\ reel/superviseur_robot/src/message.cpp

${OBJECTDIR}/_ext/c07c2ecd/monitor.o: ../../TP\ temps\ reel/superviseur_robot/src/monitor.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c07c2ecd
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c07c2ecd/monitor.o ../../TP\ temps\ reel/superviseur_robot/src/monitor.cpp

${OBJECTDIR}/_ext/c07c2ecd/robot.o: ../../TP\ temps\ reel/superviseur_robot/src/robot.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c07c2ecd
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c07c2ecd/robot.o ../../TP\ temps\ reel/superviseur_robot/src/robot.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
