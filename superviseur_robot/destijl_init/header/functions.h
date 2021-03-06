/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   functions.h
 * Author: pehladik
 *
 * Created on 15 janvier 2018, 12:50
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/mman.h>
#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/mutex.h>
#include <alchemy/sem.h>
#include <alchemy/queue.h>

#include "../../src/monitor.h"    
#include "../../src/robot.h"
#include "../../src/image.h"
#include "../../src/message.h"

extern RT_TASK th_server;
extern RT_TASK th_sendToMon;
extern RT_TASK th_receiveFromMon;
extern RT_TASK th_openComRobot;
extern RT_TASK th_startRobot;
extern RT_TASK th_move;
extern RT_TASK th_battery;
extern RT_TASK th_openCamera;
extern RT_TASK th_manageImage;
extern RT_TASK th_closeCamera;

extern RT_MUTEX mutex_chercheArene ;
extern RT_MUTEX mutex_robotStarted;
extern RT_MUTEX mutex_move;
extern RT_MUTEX mutex_send_command_to_robot ;
extern RT_MUTEX mutex_cam_Started;



extern RT_SEM sem_barrier;
extern RT_SEM sem_openComRobot;
extern RT_SEM sem_serverOk;
extern RT_SEM sem_startRobot;
extern RT_SEM sem_openCamera ;
extern RT_SEM sem_areneOk ;
extern RT_SEM sem_startRobotWD;
extern RT_SEM sem_closeCamera;


extern RT_QUEUE q_messageToMon;

extern int etatCommMoniteur;
extern int robotStarted;
extern char move;
extern int chercheArene;
extern int position;
extern int areneOk;
extern int cam_Started;

extern int MSG_QUEUE_SIZE;

extern int PRIORITY_TSERVER;
extern int PRIORITY_TOPENCOMROBOT;
extern int PRIORITY_TMOVE;
extern int PRIORITY_TSENDTOMON;
extern int PRIORITY_TRECEIVEFROMMON;
extern int PRIORITY_TSTARTROBOT;
extern int PRIORITY_TSTARTROBOTWD;
extern int PRIORITY_TBATTERY ;
extern int PRIORITY_TOPENCAMERA ;
extern int PRIORITY_TMANAGEIMAGE;
extern int PRIORITY_TCLOSECAMERA ;

void f_server(void *arg);
void f_sendToMon(void *arg);
void f_receiveFromMon(void *arg);
void f_openComRobot(void * arg);
void f_move(void *arg);
void f_startRobot(void *arg);
void f_startRobotWD(void *arg);
void f_battery(void *arg) ;
void write_in_queue(RT_QUEUE *queue, MessageToMon msg);
void send_command_to_robot_SAFE(char cmd, const char * arg);
void f_openCamera(void * arg) ;
void f_manageImage(void *arg);
void f_closeCamera(void*arg);
#endif /* FUNCTIONS_H */


