#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>



#define DATA_SIZE 1000


/**
 *number of processes taken from command line
 */
int Processnum;
/**
 * In order to create file for watchdog_out and process_out
 */
FILE *fptr;
FILE *fptr2;

/**
 * When watchdog process take sigterm signal the program will terminate so there is a sigterm signal handler
 * @param sig is the parameter of signal handler
 */
void term_handle(int sig);
/**
 * In order to nanasleep to sleep 0.3 sec we defined this struct
 */
struct timespec delta = {0 /*secs*/, 300000000 /*nanosecs*/}; //0.3 sec
/**
 * watchdog takes the watchdog_outputPath and process_outputPath from command line so we defined string for taking these lines
 */
char watchdog_outputPath[DATA_SIZE];
char process_outputPath[DATA_SIZE];


/**
 *
 * Main function first turns command line processnum string to integer.
 * It puts the command line watchdog and process output path to the deafined strings.
 * There is a signal function to detect the sigterm signal sended from executor.cpp.
 * I defined myfifo in order to reach pipe.
 * the string cwd takes the current directory and it combines with /process. Then we will use this string in execlp.
 * In pidlist we store the processer's ids.
 * In order to write to pipe we nedd create two strings Px and id of x. I created Px string in temparr and id of x in temp.
 * I will write process and watchdog outputs files via this  temparr2 string.
 * inte is the string which convert integer number i to string in order to put execlp.
 * In the lines 108-111 I opened the  process and watchdog outputs files with w flag to clear its content if it is full.
 * In line 114 I forked processnum times processes. In order to fork in correct order I used nanosleep.
 * When the child process nre forked the fork()will return 0 for child and fork() will return id of the child for parent.
 * So we can puts the ids of the forked childs to pidlist array.
 * Then I turn to int i to string and sent it to process.c program.
 * With exec each childs initiate the process.c program. The parameters are i number of the childs and process_outpath.
 * In 125-129 I write to pipe the parent process
 * In line 135-144 I write to pipe the child processes. Also I write to watchdog_out file to say child is created.
 * dead holds to id of the terminated processes. Wait(NULL) turns the id of the terminated process.
 * deadid holds the number of the terminated child.
 * there are two options:the number of the terminated child is 1 or not 1.
 * Firts I will explain not 1 option.
 * If the deadid>1  I write to watchdog_output file to say it is killed and reastrating.
 * Then I create it again and initiate process.c program with exec.
 * Then I write to its new id to watchdog_output file and send it to pipe.
 * If deadid==1. I made all of the aboves for P1. For other childs:
 * First I sent SIGTERM signal to kill them and put wait in order to they don't enter to waid in parent process.
 * Then I forked all of them again and initiate process.c program with execlp(). Nanosleep for the forking correct order.
 * Then I write their new ids to watchdog_output file and send them to pipe.
 */

int main(int argc, char *argv[]) {

    Processnum = atoi(argv[1]);

    sprintf(process_outputPath, "%s", argv[2]);
    sprintf(watchdog_outputPath, "%s", argv[3]);


    signal(SIGTERM, term_handle);



    int fd;
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0644);
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    char path[100];

    sprintf(path, "%s", "/process");
    strcat(cwd, path);

    pid_t pidlist[Processnum+1];

    char temp[30];
    char temparr[30];
    char temparr2[DATA_SIZE];
    char inte[30];




    fptr=fopen(watchdog_outputPath, "w");
    fclose(fptr);
    fptr2=fopen(process_outputPath, "w");
    fclose(fptr2);


    for (int i = 1; i <= Processnum; i++) {
        nanosleep(&delta, &delta);
        if ((pidlist[i] = fork()) <= 0) {

            sprintf(inte, "%d", i);

            execlp(cwd, "./process", inte,process_outputPath, NULL);

        }}


    snprintf(temparr, 30, "P%d", 0);
    sprintf(temp, " %d", getpid());
    strcat(temparr, temp);
    fd = open(myfifo, O_WRONLY);
    write(fd, temparr, 30);





    for (int k = 1; k <= Processnum; k++){

        snprintf(temparr, 30, "P%d", k);
        sprintf(temp, " %d", pidlist[k]);
        strcat(temparr, temp);
        write(fd, temparr, 30);
        sprintf(temparr2, "P%d is started and it has a pid of %d\n", k, pidlist[k]);
        fptr = fopen(watchdog_outputPath, "a");
        fputs(temparr2, fptr);
        fclose(fptr);




    }







    pid_t dead;


    while(1){
        dead=wait(NULL);
        int deadid;

        for(int i=1;i<=Processnum;i++){
           if(pidlist[i]==dead)
               deadid = i;
        }


       if(deadid==1){

            fptr = fopen(watchdog_outputPath, "a");
           sprintf(temparr2, "P%d is killed, all processes must be killed\n", deadid);
            fputs(temparr2, fptr);
           fclose(fptr);
           fptr = fopen(watchdog_outputPath, "a");
            sprintf(temparr2, "Restarting all processes\n");
            fputs(temparr2, fptr);
           fclose(fptr);




           for(int i=2;i<=Processnum;i++) {

               kill(pidlist[i], SIGTERM);
               wait(NULL);
           }

           if ((pidlist[deadid] = fork()) <= 0) {

               sprintf(inte, "%d", deadid);
               execlp(cwd, "./process", inte,process_outputPath, NULL);
           }







           for(int i=2;i<=Processnum;i++) {
               nanosleep(&delta, &delta);
               if ((pidlist[i] = fork()) <= 0) {

                   sprintf(inte, "%d", i);

                   execlp(cwd, "./process", inte,process_outputPath, NULL);

               }}

           fptr = fopen(watchdog_outputPath, "a");
           sprintf(temparr2, "P%d is started and it has a pid of %d\n", deadid, pidlist[deadid]);
           fputs(temparr2, fptr);
           fclose(fptr);

           snprintf(temparr, 30, "P%d", deadid);
           sprintf(temp, " %d", pidlist[deadid]);
           strcat(temparr, temp);
           write(fd, temparr, 30);




           for(int i=2;i<=Processnum;i++) {

               fptr = fopen(watchdog_outputPath, "a");
               sprintf(temparr2, "P%d is started and it has a pid of %d\n", i, pidlist[i]);
               fputs(temparr2, fptr);
               fclose(fptr);

               snprintf(temparr, 30, "P%d", i);
               sprintf(temp, " %d", pidlist[i]);
               strcat(temparr, temp);
               write(fd, temparr, 30);
           }


        }



        else if(deadid>1) {

           fptr = fopen(watchdog_outputPath, "a");
           sprintf(temparr2, "P%d is killed\n", deadid);
           fputs(temparr2, fptr);
           fclose(fptr);
           fptr = fopen(watchdog_outputPath, "a");
           sprintf(temparr2, "Restarting P%d\n", deadid);
           fputs(temparr2, fptr);
           fclose(fptr);

           if ((pidlist[deadid] = fork()) <= 0) {

               sprintf(inte, "%d", deadid);
               execlp(cwd, "./process", inte,process_outputPath, NULL);
           }

           fptr = fopen(watchdog_outputPath, "a");

           sprintf(temparr2, "P%d is started and it has a pid of %d\n", deadid, pidlist[deadid]);

           fputs(temparr2, fptr);
           fclose(fptr);
           snprintf(temparr, 30, "P%d", deadid);
           sprintf(temp, " %d", pidlist[deadid]);
           strcat(temparr, temp);
           write(fd, temparr, 30);


       }

}










    return 0;
}
/**
 *
 * When the watchdog process takes SIGTERM signal I used this handler.
 * In this handler I write to watchdog_out file to say watchdog is terminated.
 * and watchdog.c program terminates.
 */
void term_handle(int sig){
    char temparr4[DATA_SIZE];
    sleep(3);
    sprintf(temparr4, "Watchdog is terminating gracefully\n");
    fptr = fopen(watchdog_outputPath, "a");
    fputs(temparr4, fptr);
    fclose(fptr);

    exit(0);
}