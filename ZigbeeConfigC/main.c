#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


void printChoices()
{
    printf("\n**************What do you want to do ?**************\n");
    printf("1-read xbee configuration\n");
    printf("2-change xbee configuration\n");
    printf("3-send file to current xbee (read config before)\n");
    printf("?-quit\n");
}

int main(int argc, char* args[])
{
    if(argc<3)
    {
        printf("Usage : %s <serial port> <baud rate>\n",args[0]);
        return 1;
    }

    int choice = 1, pid;
    char choiceStr[2], ch, baudrate[20], filename[50], sizeBloc[5], ack[3], numberTries[5];
    while(choice<=3&&choice>0)
    {
        printChoices();
        fgets(choiceStr,2,stdin);
        if(choiceStr[strlen(choiceStr)-1]!='\n')
            while((ch = getchar()) != '\n' && ch != EOF);
        else
            choiceStr[strlen(choiceStr)-1] = 0;
        choice = atoi(choiceStr);

        pid = fork();
        if(pid<0)
        {
            perror("Error forking ");
            return 1;
        }
        else if(pid)
            wait(NULL);
        else
        {
            char* args2[9];
            switch(choice)
            {
                case 1:
                    args2[0] = "./readConfig";
                    args2[1] = args[1];
                    args2[2] = args[2];
                    args2[3] = NULL;
                    execv("readConfig",args2);
                    break;
                case 2:
                    args2[0] = "./changeConfig";
                    args2[1] = args[1];
                    args2[2] = args[2];
                    args2[3] = NULL;
                    execv("changeConfig",args2);
                    break;
                case 3:
                    printf("Baudrate :\n");
                    fgets(baudrate,20,stdin);
                    if(baudrate[strlen(baudrate)-1]!='\n')
                        while((ch = getchar()) != '\n' && ch != EOF);
                    else
                        baudrate[strlen(baudrate)-1] = 0;

                    printf("File to send :\n");
                    fgets(filename,50,stdin);
                    if(filename[strlen(filename)-1]!='\n')
                        while((ch = getchar()) != '\n' && ch != EOF);
                    else
                        filename[strlen(filename)-1] = 0;

                    printf("Size of bloc :\n");
                    fgets(sizeBloc,5,stdin);
                    if(sizeBloc[strlen(sizeBloc)-1]!='\n')
                        while((ch = getchar()) != '\n' && ch != EOF);
                    else
                        sizeBloc[strlen(sizeBloc)-1] = 0;

                    printf("Acknowledge ?\n");
                    fgets(ack,3,stdin);
                    if(ack[strlen(ack)-1]!='\n')
                        while((ch = getchar()) != '\n' && ch != EOF);
                    else
                        ack[strlen(ack)-1] = 0;

                    int ackInt = atoi(ack);
                    if(ackInt)
                    {
                        printf("Number tries :\n");
                        fgets(numberTries,5,stdin);
                        if(numberTries[strlen(numberTries)-1]!='\n')
                            while((ch = getchar()) != '\n' && ch != EOF);
                        else
                            numberTries[strlen(numberTries)-1] = 0;
                    }
                    else
                        sprintf(numberTries,"1");

                    args2[0] = "./sendFile";
                    args2[1] = args[1];
                    args2[2] = baudrate;
                    args2[3] = filename;
                    args2[4] = sizeBloc;
                    args2[5] = ack;
                    args2[6] = numberTries;
                    args2[7] = NULL;
                    execv("sendFile",args2);
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}
