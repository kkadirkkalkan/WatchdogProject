#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#define DATA_SIZE 1000
/**
 * i holds the number of the child process. It comes from watchdog.c
 */
int i;
int signum;
/**
 * In order to create processpu_file.
 */
FILE *fptr;
/**
 * path of the processout_file comes from watchdog.c. I hold it with this string.
 */
char processout_path[DATA_SIZE];



/**
 * I defined handler function for SIGINT
 *
 */
void int_handle(int sig);
/**
 * I defined handler function for SIGHUP
 *
 */
void hup_handle(int sig);
/**
 * I defined handler function for SIGILL
 *
 */
void ill_handle(int sig);
/**
 * I defined handler function for SIGTRAP
 *
 */
void trap_handle(int sig);
/**
 * I defined handler function for SIGFPE
 *
 */
void fpe_handle(int sig);
/**
 * I defined handler function for SIGSEGV
 *
 */
void segv_handle(int sig);
/**
 * I defined handler function for SIGTERM
 *
 */
void term_handle(int sig);
/**
 * I defined handler function for SIGXCPU
 *
 */
void xcpu_handle(int sig);

/**
 * In the main first I take the number of child i and processout_path from the watchdog.c
 * I defined temparr string to write processout_file something.
 * There are signal functions to understand child is takes signal or not.
 * When the children initiate the process.c I write to processout_file to say Px is eaiting for a signal.
 *
 */

int main(int argc, char *argv[]) {

    i = atoi(argv[1]);
    sprintf(processout_path,"%s",argv[2]);

    char temparr[DATA_SIZE];




    signal(SIGHUP, hup_handle);
    signal(SIGINT, int_handle);
    signal(SIGILL, ill_handle);
    signal(SIGTRAP, trap_handle);
    signal(SIGFPE, fpe_handle);
    signal(SIGSEGV, segv_handle);
    signal(SIGTERM, term_handle);
    signal(SIGXCPU, xcpu_handle);




    sprintf(temparr, "P%d is waiting for a signal\n", i);
    fptr =fopen(processout_path, "a");

    fputs(temparr, fptr);
    fclose(fptr);


    while (1);















    return 0;
}
/**
 * If the child takes SIGINT signal it comes to this handler.
 * Handler writes to prcoessout_file to inform child received this signal
 */
void int_handle(int sig){

    char temp[DATA_SIZE];

    sprintf(temp, "P%d received signal %d\n",i,  sig);

    fptr =fopen(processout_path, "a");
    fputs(temp, fptr);
    fclose(fptr);
}
/**
 * If the child takes SIGHUP signal it comes to this handler.
 * Handler writes to prcoessout_file to inform child received this signal
 */
void hup_handle(int sig){
    char temp[DATA_SIZE];

    sprintf(temp, "P%d received signal %d\n",i,  sig);
    fptr =fopen(processout_path, "a");
    fputs(temp, fptr);
    fclose(fptr);

}
/**
 * If the child takes SIGILL signal it comes to this handler.
 * Handler writes to prcoessout_file to inform child received this signal
 */
void ill_handle(int sig){
    char temp[DATA_SIZE];

    sprintf(temp, "P%d received signal %d\n",i,  sig);
    fptr =fopen(processout_path, "a");
    fputs(temp, fptr);
    fclose(fptr);

}
/**
 * If the child takes SIGTRAP signal it comes to this handler.
 * Handler writes to prcoessout_file to inform child received this signal
 */
void trap_handle(int sig){
    char temp[DATA_SIZE];

    sprintf(temp, "P%d received signal %d\n",i,  sig);
    fptr =fopen(processout_path, "a");
    fputs(temp, fptr);
    fclose(fptr);

}
/**
 * If the child takes SIGFPE signal it comes to this handler.
 * Handler writes to prcoessout_file to inform child received this signal
 */
void fpe_handle(int sig){
    char temp[DATA_SIZE];

    sprintf(temp, "P%d received signal %d\n",i,  sig);
    fptr =fopen(processout_path, "a");
    fputs(temp, fptr);
    fclose(fptr);

}
/**
 * If the child takes SIGSEGV signal it comes to this handler.
 * Handler writes to prcoessout_file to inform child received this signal
 */
void segv_handle(int sig){
    char temp[DATA_SIZE];

    sprintf(temp, "P%d received signal %d\n",i,  sig);
    fptr =fopen(processout_path, "a");
    fputs(temp, fptr);
    fclose(fptr);

}
/**
 * If the child takes SIGTERM signal it comes to this handler.
 * Handler writes to prcoessout_file to inform child received this signal
 * Also child terminates thanks to this signal
 */
void term_handle(int sig){
    char temp[DATA_SIZE];

    sprintf(temp, "P%d received signal %d, terminating gracefully\n",i,  sig);
    fptr =fopen(processout_path, "a");
    fputs(temp, fptr);
    fclose(fptr);
    exit(0);
}
/**
 * If the child takes SIGXCPU signal it comes to this handler.
 * Handler writes to prcoessout_file to inform child received this signal
 */
void xcpu_handle(int sig){
    char temp[DATA_SIZE];

    sprintf(temp, "P%d received signal %d\n",i,  sig);
    fptr =fopen(processout_path, "a");
    fputs(temp, fptr);
    fclose(fptr);

}