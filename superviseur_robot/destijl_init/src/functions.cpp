#include "../header/functions.h"

#define RESET 0
#define KILL_COM_ROBOT 1

char mode_start;
Camera maCamera ;
Jpg Image_envoi;
Image monImage ;
Image Image_arene;
Image Image_pos ;
Arene monArene;
Arene Arenezero ;
Position maPosition;

void write_in_queue(RT_QUEUE *, MessageToMon);

void f_server(void *arg) {
    
    /* INIT */
    RT_TASK_INFO info;
    rt_task_inquire(NULL, &info);
    printf("Init %s\n", info.name);
    rt_sem_p(&sem_barrier, TM_INFINITE);
    
    int err;
    err = run_nodejs("/usr/local/bin/node", "/home/pi/Interface_Robot/server.js");

    if (err < 0) {
        //printf("Failed to start nodejs: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    } else {
#ifdef _WITH_TRACE_
        printf("%s         : nodejs started\n", info.name);
#endif
        open_server();
       // printf("Server OK!\n");
        rt_sem_broadcast(&sem_serverOk);
    }
}

void f_sendToMon(void * arg) {
    int err;
    MessageToMon msg;

    /* INIT */
    RT_TASK_INFO info;
    rt_task_inquire(NULL, &info);
    printf("Init %s\n", info.name);
    rt_sem_p(&sem_barrier, TM_INFINITE);

#ifdef _WITH_TRACE_
    printf("%s      : waiting for sem_serverOk\n", info.name);
#endif
    rt_sem_p(&sem_serverOk, TM_INFINITE);
    while (1) {

#ifdef _WITH_TRACE_
        printf("%s : waiting for a message in queue\n", info.name);
#endif
        if (rt_queue_read(&q_messageToMon, &msg, sizeof (MessageToRobot), TM_INFINITE) >= 0) {
#ifdef _WITH_TRACE_
            printf("%s : message {%s,%s} in queue\n", info.name, msg.header, msg.data);
#endif

            send_message_to_monitor(msg.header, msg.data);
            free_msgToMon_data(&msg);
            rt_queue_free(&q_messageToMon, &msg);
        } else {
            printf("Error msg queue write: %s\n", strerror(-err));
        }
    }
}

void f_receiveFromMon(void *arg) {
    MessageFromMon msg;
    int err;

    /* INIT */
    RT_TASK_INFO info;
    rt_task_inquire(NULL, &info);
    printf("Init %s\n", info.name);
    rt_sem_p(&sem_barrier, TM_INFINITE);

#ifdef _WITH_TRACE_
    printf("%s : waiting for sem_serverOk\n", info.name);
#endif
    rt_sem_p(&sem_serverOk, TM_INFINITE);
    do {
#ifdef _WITH_TRACE_
        printf("%s : waiting for a message from monitor\n", info.name);
#endif
        err = receive_message_from_monitor(msg.header, msg.data);
#ifdef _WITH_TRACE_
        printf("%s: msg {header:%s,data=%s} received from UI\n", info.name, msg.header, msg.data);
#endif
        if (strcmp(msg.header, HEADER_MTS_COM_DMB) == 0) {
            if (msg.data[0] == OPEN_COM_DMB) { // Open communication supervisor-robot
#ifdef _WITH_TRACE_
                printf("%s: message open Xbee communication\n", info.name);
#endif
                rt_sem_v(&sem_openComRobot);
            }
        } else if (strcmp(msg.header, HEADER_MTS_DMB_ORDER) == 0) {
            if (msg.data[0] == DMB_START_WITHOUT_WD) { // Start robot
#ifdef _WITH_TRACE_
                printf("%s: message start robot\n", info.name);
#endif 
                rt_sem_v(&sem_startRobot);

            } else if ((msg.data[0] == DMB_GO_BACK)
                    || (msg.data[0] == DMB_GO_FORWARD)
                    || (msg.data[0] == DMB_GO_LEFT)
                    || (msg.data[0] == DMB_GO_RIGHT)
                    || (msg.data[0] == DMB_STOP_MOVE)) {

                rt_mutex_acquire(&mutex_move, TM_INFINITE);
                move = msg.data[0];
                rt_mutex_release(&mutex_move);
#ifdef _WITH_TRACE_
                printf("%s: message update movement with %c\n", info.name, move);
#endif

            } else if (msg.data[0] == DMB_START_WITH_WD) {

                rt_sem_v(&sem_startRobotWD);

            }
        } else if (strcmp(msg.header, HEADER_MTS_CAMERA) == 0) {

            if (msg.data[0] == CAM_OPEN) {

                rt_sem_v(&sem_openCamera);

            } else if (msg.data[0] == CAM_ASK_ARENA) {

                rt_mutex_acquire(&mutex_chercheArene, TM_INFINITE);
                chercheArene = 1;
                rt_mutex_release(&mutex_chercheArene);

            } else if (msg.data[0] == CAM_ARENA_CONFIRM) {

                rt_sem_v(&sem_areneOk);
                areneOk = 1;

            } else if (msg.data[0] == CAM_ARENA_INFIRM) {

                rt_sem_v(&sem_areneOk);
                areneOk = 0;

            } else if (msg.data[0] == CAM_COMPUTE_POSITION) {

                position = 1;
            } else if (msg.data[0] == CAM_STOP_COMPUTE_POSITION) {

                position = 0;
            } else if (msg.data[0] == CAM_CLOSE){
                rt_sem_v(&sem_closeCamera);
            
            }
        }
    } while (err > 0);

}

void f_openComRobot(void * arg) {
    int err;

    /* INIT */
    RT_TASK_INFO info;
    rt_task_inquire(NULL, &info);
    printf("Init %s\n", info.name);
    rt_sem_p(&sem_barrier, TM_INFINITE);

    while (1) {
#ifdef _WITH_TRACE_
        printf("%s   : waiting for sem_openComRobot\n", info.name);
#endif
        rt_sem_p(&sem_openComRobot, TM_INFINITE);
#ifdef _WITH_TRACE_
        printf("%s : sem_openComRobot arrived => open communication robot\n", info.name);
#endif
        err = open_communication_robot();
        if (err == 0) {
#ifdef _WITH_TRACE_
            printf("%s : the communication is opened\n", info.name);
#endif
            MessageToMon msg;
            set_msgToMon_header(&msg, HEADER_STM_ACK);
            write_in_queue(&q_messageToMon, msg);
            
        } else {
#ifdef _WITH_TRACE_
            printf("%s : there is a problem with the communication \n", info.name);
#endif
            MessageToMon msg;
            set_msgToMon_header(&msg, HEADER_STM_NO_ACK);
            write_in_queue(&q_messageToMon, msg);
        }
    }
}

void f_startRobot(void * arg) {
    int err;

    /* INIT */
    RT_TASK_INFO info;
    rt_task_inquire(NULL, &info);
    printf("Init %s\n", info.name);
    rt_sem_p(&sem_barrier, TM_INFINITE);

    while (1) {
#ifdef _WITH_TRACE_
        printf("%s     : waiting for  sem_startRobot\n", info.name);
#endif
        rt_sem_p(&sem_startRobot, TM_INFINITE);
#ifdef _WITH_TRACE_
        printf("%s : sem_startRobot arrived => Start robot\n", info.name);
#endif
        err = send_command_to_robot(DMB_START_WITHOUT_WD);
        if (err == 0) {
#ifdef _WITH_TRACE_
            printf("%s : the robot is started\n", info.name);
#endif
            rt_mutex_acquire(&mutex_robotStarted, TM_INFINITE);
            robotStarted = 1;
            rt_mutex_release(&mutex_robotStarted);
            MessageToMon msg;
            set_msgToMon_header(&msg, HEADER_STM_ACK);
            write_in_queue(&q_messageToMon, msg);
        } else {
            MessageToMon msg;
            set_msgToMon_header(&msg, HEADER_STM_NO_ACK);
            write_in_queue(&q_messageToMon, msg);
        }
    }
}

void f_move(void *arg) {
    /* INIT */
    RT_TASK_INFO info;
    rt_task_inquire(NULL, &info);
    printf("Init %s\n", info.name);
    rt_sem_p(&sem_barrier, TM_INFINITE);

    /* PERIODIC START */
#ifdef _WITH_TRACE_
    //printf("%s: start period\n", info.name);
#endif
    rt_task_set_periodic(NULL, TM_NOW, 100000000);
    while (1) {
#ifdef _WITH_TRACE_
        //printf("%s: Wait period \n", info.name);
#endif
        rt_task_wait_period(NULL);
#ifdef _WITH_TRACE_
        //printf("%s: Periodic activation\n", info.name);
        //printf("%s: move equals %c\n", info.name, move);
#endif
        rt_mutex_acquire(&mutex_robotStarted, TM_INFINITE);
        if (robotStarted) {
            rt_mutex_acquire(&mutex_move, TM_INFINITE);
            send_command_to_robot(move);
            rt_mutex_release(&mutex_move);
#ifdef _WITH_TRACE_
            //printf("%s: the movement %c was sent\n", info.name, move);
#endif            
        }
        rt_mutex_release(&mutex_robotStarted);
    }
}

void f_battery(void *arg) {
    /* INIT */
    RT_TASK_INFO info;
    rt_task_inquire(NULL, &info);
    printf("Init %s\n", info.name);
    rt_sem_p(&sem_barrier, TM_INFINITE);

    rt_task_set_periodic(NULL, TM_NOW, 500000000);

    while (1) {
        rt_task_wait_period(NULL);
#ifdef _WITH_TRACE_
        // mode verbose
        // printf("%s: start period\n", info.name);
#endif 

       
        rt_mutex_acquire(&mutex_robotStarted, TM_INFINITE);
        if (robotStarted) {

            // allocation message
            MessageToMon msg;
            int levelBat = 0;

            // acquisition valeur
            levelBat = send_command_to_robot(DMB_GET_VBAT);
            levelBat += 48; // passage code ascii

            // écriture message
            set_msgToMon_data(&msg, &levelBat);
            set_msgToMon_header(&msg, HEADER_STM_BAT);

            // envoi message
            write_in_queue(&q_messageToMon, msg);
        }
        rt_mutex_release(&mutex_robotStarted);
    }
}

void write_in_queue(RT_QUEUE *queue, MessageToMon msg) {
    void *buff;
    buff = rt_queue_alloc(&q_messageToMon, sizeof (MessageToMon));
    memcpy(buff, &msg, sizeof (MessageToMon));
    rt_queue_send(&q_messageToMon, buff, sizeof (MessageToMon), Q_NORMAL);
}

void send_command_to_robot_SAFE(char cmd, const char * arg) {

    static int cmpt = 0;

    rt_mutex_acquire(&mutex_send_command_to_robot, TM_INFINITE);

    switch (send_command_to_robot(cmd, arg)) {

        case ROBOT_OK:
            cmpt = 0;
            break;

        default:
            cmpt++;
            break;
    };


    if (cmpt >= 3) {
        // message(LOST_DMB)
        MessageToMon msg;
        set_msgToMon_header(&msg, HEADER_STM_LOST_DMB);
        set_msgToMon_data(&msg, NULL);
        write_in_queue(&q_messageToMon, msg);
        // close_communication_robot();
        // TO DO: fonction de reset/close_communication_robot
    }


    rt_mutex_release(&mutex_send_command_to_robot);
}

void f_openCamera(void *arg) {
    /* INIT */
    RT_TASK_INFO info ;
    rt_task_inquire(NULL, &info);
    printf("Init %s\n", info.name);
    rt_sem_p(&sem_barrier, TM_INFINITE);
   
    
    int err;

    while (1) {

        rt_sem_p(&sem_openCamera, TM_INFINITE);
        rt_mutex_acquire(&mutex_cam_Started, TM_INFINITE);
#ifdef _WITH_TRACE_
        printf("%s : sem_openCamera arrived => open camera\n", info.name);
#endif
        err = open_camera(&maCamera);
        printf("ordre cam recu\n");
        if (err == 0) {
#ifdef _WITH_TRACE_
            printf("%s : the camera is opened\n", info.name);
#endif
            
            
            cam_Started = 1;
         
         
            //MessageToMon msg;
            //send(&msg, HEADER_STM_ACK);
            //write_in_queue(&q_messageToMon, msg);
            send_message_to_monitor(HEADER_STM_ACK);
        } else {
#ifdef _WITH_TRACE_
            printf("%s : there is a problem with the camera\n", info.name);
#endif
            MessageToMon msg;
            set_msgToMon_header(&msg, HEADER_STM_NO_ACK);
            write_in_queue(&q_messageToMon, msg);
        }
          rt_mutex_release(&mutex_cam_Started);
    }
}


void f_closeCamera(void*arg){

/* INIT */
    RT_TASK_INFO info ;
    rt_task_inquire(NULL, &info);
    printf("Init %s\n", info.name);
    rt_sem_p(&sem_barrier, TM_INFINITE);
 int err;

    while (1) {

        rt_sem_p(&sem_closeCamera, TM_INFINITE);
        rt_mutex_acquire(&mutex_cam_Started, TM_INFINITE);
#ifdef _WITH_TRACE_
        printf("%s : sem_closeCamera arrived => close camera\n", info.name);
#endif
        close_camera(&maCamera);
        printf("ordre cam recu\n");
        send_message_to_monitor(HEADER_STM_ACK);
        
       
#ifdef _WITH_TRACE_
            printf("%s : the camera is closed\n", info.name);
#endif
             
            cam_Started = 0;
       
          rt_mutex_release(&mutex_cam_Started);
    }
}




void f_manageImage(void *arg) {
    /* INIT */
    RT_TASK_INFO info;
    rt_task_inquire(NULL, &info);
    printf("Init %s\n", info.name);
    rt_sem_p(&sem_barrier, TM_INFINITE);

    rt_task_set_periodic(NULL, TM_NOW, 100000000);

    int err;
    while (1) {
        rt_task_wait_period(NULL);
        rt_mutex_acquire(&mutex_cam_Started, TM_INFINITE);

        if (cam_Started) {

            get_image(&maCamera, &monImage);

            rt_mutex_acquire(&mutex_chercheArene, TM_INFINITE);


            if (chercheArene == 1) {
                printf("Je cherche une arene");
                err = detect_arena(&monImage, &monArene);

                if (err == 0) {

                    draw_arena(&monImage, &Image_arene, &monArene);
                    compress_image(&Image_arene, &Image_envoi);

                   
                    send_message_to_monitor(HEADER_STM_IMAGE, &Image_envoi);
                    rt_sem_p(&sem_areneOk, TM_INFINITE);

                   if (areneOk == 0) {

                       monArene = Arenezero ;

                  }


                } else {

                    MessageToMon msg;
                    set_msgToMon_header(&msg, HEADER_STM_NO_ACK);
                    write_in_queue(&q_messageToMon, msg);
                    

                }

             chercheArene = 0;   

            }
            rt_mutex_release(&mutex_chercheArene);
             if (position == 1) {

                        err = detect_position(&monImage, &maPosition, &monArene);

                        if (err == 0) {

                            maPosition.center.x = -1;
                            maPosition.center.y = -1;
                            maPosition.angle = 0;

                        }

                        draw_position(&monImage, &Image_pos, &maPosition);

                        monImage=Image_pos ;
                        send_message_to_monitor(HEADER_STM_POS,&maPosition);
                    }



           
           compress_image(&monImage, &Image_envoi);
           send_message_to_monitor(HEADER_STM_IMAGE, &Image_envoi); 
        }
        rt_mutex_release(&mutex_cam_Started);
    }
}

        
    

void f_startRobotWD(void * arg) {
    int err;
    int compteur ;
    /* INIT */
    RT_TASK_INFO info;
    rt_task_inquire(NULL, &info);
    printf("Init %s\n", info.name);
    rt_sem_p(&sem_barrier, TM_INFINITE);
    
    rt_task_set_periodic(NULL, TM_NOW, 1000000000);
    #ifdef _WITH_TRACE_
        printf("%s     : waiting for  sem_startRobotWD\n", info.name);
#endif
    rt_sem_p(&sem_startRobotWD,TM_INFINITE);
    #ifdef _WITH_TRACE_
        printf("%s     : sem reçu\n", info.name);
#endif
    err = send_command_to_robot(DMB_START_WITH_WD);
      #ifdef _WITH_TRACE_
        printf("%s     : miaou fait le chat\n", info.name);
#endif
    if (err == 0) {
        
            #ifdef _WITH_TRACE_
        printf("%s     : ca marche ! :o \n", info.name);
#endif
        rt_mutex_acquire(&mutex_robotStarted, TM_INFINITE);
        robotStarted = 1;
        rt_mutex_release(&mutex_robotStarted);
        MessageToMon msg;
        set_msgToMon_header(&msg, HEADER_STM_ACK);
        write_in_queue(&q_messageToMon, msg);
        

        while (1) {
            rt_task_wait_period(NULL);
            err = send_command_to_robot(DMB_RELOAD_WD);
            if (err == 0) {
                compteur = 0;

            } else {
                compteur += 1;

            }

            if (compteur == 3) {
                MessageToMon msg;
                set_msgToMon_header(&msg, HEADER_STM_LOST_DMB);
                write_in_queue(&q_messageToMon, msg);
                close_communication_robot();
            rt_mutex_acquire(&mutex_robotStarted, TM_INFINITE);
            robotStarted = 0;
            rt_mutex_release(&mutex_robotStarted);
            }

        }
        
    } else {
        MessageToMon msg;
        set_msgToMon_header(&msg, HEADER_STM_NO_ACK);
        write_in_queue(&q_messageToMon, msg);
    }

   

        
    }


int gestion_erreurs(int typeErreur) {

    RT_TASK_INFO info;
    rt_task_inquire(NULL, &info);
    printf("Init %s\n", info.name);
    rt_sem_p(&sem_barrier, TM_INFINITE);

    switch (typeErreur) {
        case RESET: // F6

            break;

        case KILL_COM_ROBOT: // F9

            break;

    }

}

