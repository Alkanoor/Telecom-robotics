#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>


void convertBaudrate(char* numericVal, char* ref, char* forXbee);
int send(int fd, char* str, char* buf, __useconds_t waitMicros, size_t size);


int main(int argc, char* args[])
{
    //Usage : %s <serial port> <baud rate> <channel> <network id> <source address> <destination address>
    char serial[31], baudRate[12], chan[3], id[5], addr[5], dest[5], baudRateAfter[2], ch;
    memset(baudRate,0,12);
    memset(chan,0,3);
    memset(id,0,5);
    memset(addr,0,5);
    memset(dest,0,5);
    if(argc<7)
    {
        printf("Path to serial port :\n");
        fgets(serial,31,stdin);
        if(serial[strlen(serial)-1]!='\n')
            while((ch = getchar()) != '\n' && ch != EOF);
        else
            serial[strlen(serial)-1] = 0;

        printf("Baudrate :\n");
        fgets(baudRate,12,stdin);
        if(baudRate[strlen(baudRate)-1]!='\n')
            while((ch = getchar()) != '\n' && ch != EOF);
        else
            baudRate[strlen(baudRate)-1] = 0;

        printf("Channel (2 bytes max):\n");
        fgets(chan,3,stdin);
        if(chan[strlen(chan)-1]!='\n')
            while((ch = getchar()) != '\n' && ch != EOF);
        else
            chan[strlen(chan)-1] = 0;

        printf("Network Id (4 bytes max):\n");
        fgets(id,5,stdin);
        if(id[strlen(id)-1]!='\n')
            while((ch = getchar()) != '\n' && ch != EOF);
        else
            id[strlen(id)-1] = 0;

        printf("Address of current xbee (4 bytes max):\n");
        fgets(addr,5,stdin);
        if(addr[strlen(addr)-1]!='\n')
            while((ch = getchar()) != '\n' && ch != EOF);
        else
            addr[strlen(addr)-1] = 0;

        printf("Destination of current xbee (4 bytes max):\n");
        fgets(dest,5,stdin);
        if(dest[strlen(dest)-1]!='\n')
            while((ch = getchar()) != '\n' && ch != EOF);
        else
            dest[strlen(dest)-1] = 0;

        printf("Baudrate after :\n");
        char temp[12];
        fgets(temp,12,stdin);
        if(temp[strlen(temp)-1]!='\n')
            while((ch = getchar()) != '\n' && ch != EOF);
        else
            temp[strlen(temp)-1] = 0;
        convertBaudrate(temp,baudRate,baudRateAfter);
    }
    else
    {
        strncpy(serial,args[1],30);
        strncpy(baudRate,args[2],12);
        strncpy(chan,args[3],3);
        strncpy(id,args[4],5);
        strncpy(addr,args[5],5);
        strncpy(dest,args[6],5);
        char temp[12];
        if(argc>7)
            strncpy(temp,args[7],12);
        convertBaudrate(temp,baudRate,baudRateAfter);
    }

    struct termios tty, tty_old;
    memset(&tty, 0, sizeof(tty));

    int fd = open(serial,O_RDWR|O_NOCTTY|O_NDELAY|O_NONBLOCK);
    if(fd<0)
    {
        perror("Error during serial port opening ");
        return 1;
    }

    if(tcgetattr(fd, &tty) != 0)
    {
        perror("Error from tcgetattr ");
        return 1;
    }

    tty_old = tty;

    speed_t speed;
    int param = atoi(baudRate);
    switch(param)
    {
        case 1200:
            speed = B1200;
            break;
        case 2400:
            speed = B2400;
            break;
        case 4800:
            speed = B4800;
            break;
        case 9600:
            speed = B9600;
            break;
        case 19200:
            speed = B19200;
            break;
        case 38400:
            speed = B38400;
            break;
        case 57600:
            speed = B57600;
            break;
        case 115200:
            speed = B115200;
            break;
        default:
            printf("Unknown baudrate %d\n",param);
            return 1;
    }

    if(cfsetospeed(&tty, speed)<0)
    {
        perror("Error from cfsetospeed ");
        return 1;
    }
    if(cfsetispeed(&tty, speed)<0)
    {
        perror("Error from cfsetispeed ");
        return 1;
    }

    tty.c_cflag     &=  ~PARENB;            // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;

    tty.c_cflag     &=  ~CRTSCTS;           // no flow control
    tty.c_cc[VMIN]   =  0;                  // read doesn't block
    tty.c_cc[VTIME]  =  0;                  // 0.5 seconds read timeout
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines


    tcflush(fd, TCIFLUSH);
    if(tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        perror("Error from tcsetattr ");
        return 1;
    }

    char buf[16] = "";
    char cmd[16] = "";

    send(fd,"+++",buf,1100100,16);

    memcpy(cmd,"ATCH",4);
    memcpy(cmd+4,chan,strlen(chan));
    memcpy(cmd+4+strlen(chan),"\r",2);
    send(fd,cmd,buf,100000,16);

    memcpy(cmd,"ATID",4);
    memcpy(cmd+4,id,strlen(id));
    memcpy(cmd+4+strlen(id),"\r",2);
    send(fd,cmd,buf,100000,16);

    memcpy(cmd,"ATMY",4);
    memcpy(cmd+4,addr,strlen(addr));
    memcpy(cmd+4+strlen(addr),"\r",2);
    send(fd,cmd,buf,100000,16);

    memcpy(cmd,"ATDL",4);
    memcpy(cmd+4,dest,strlen(dest));
    memcpy(cmd+4+strlen(dest),"\r",2);
    send(fd,cmd,buf,100000,16);

    memcpy(cmd,"ATBD",4);
    memcpy(cmd+4,baudRateAfter,strlen(baudRateAfter));
    memcpy(cmd+4+strlen(baudRateAfter),"\r",2);
    send(fd,cmd,buf,100000,16);

    send(fd,"ATWR\r",buf,100000,16);
    send(fd,"ATCN\r",buf,100000,16);


    tcflush(fd, TCIFLUSH);
    if(tcsetattr(fd, TCSANOW, &tty_old) != 0)
    {
        perror("Error from tcsetattr ");
        return 1;
    }
    close(fd);

    return 0;
}

void convertBaudrate(char* numericVal, char* ref, char* forXbee)
{
    int param = atoi(numericVal);
    switch(param)
    {
        case 1200:
            sprintf(forXbee,"0");
            break;
        case 2400:
            sprintf(forXbee,"1");
            break;
        case 4800:
            sprintf(forXbee,"2");
            break;
        case 9600:
            sprintf(forXbee,"3");
            break;
        case 19200:
            sprintf(forXbee,"4");
            break;
        case 38400:
            sprintf(forXbee,"5");
            break;
        case 57600:
            sprintf(forXbee,"6");
            break;
        case 115200:
            sprintf(forXbee,"7");
            break;
        default:
            if(numericVal!=ref)
                convertBaudrate(ref,ref,forXbee);
            else
                printf("Unknown baudrate and reference baudrate %d",param);
            break;
    }
}

int send(int fd, char* str, char* buf, __useconds_t waitMicros, size_t size)
{
    char copy[30];
    sprintf(copy,"%s",str);
    unsigned int i=0;
    for(;i<30;i++)
        if(copy[i]=='\n'||copy[i]=='\r')
            copy[i] = ' ';
    if((int)write(fd,str,strlen(str))<(int)strlen(str))
    {
        char msg[50];
        sprintf(msg,"Error during send (%s)\n",copy);
        perror(msg);
        return -1;
    }
    printf("%s => ",copy);
    fflush(stdout);
    usleep(waitMicros);
    memset(buf,0,size);
    int n = read(fd,buf,size);
    if(n<0)
    {
        char msg[50];
        sprintf(msg,"Error during read after (%s)\n",copy);
        perror(msg);
        return 1;
    }
    printf("%s\n",buf);
    return 0;
}
