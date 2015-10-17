#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>



int main(int argc, char* args[])
{
    if(argc<7)
    {
        printf("Usage : %s <serial port> <baud rate> <file to send> <size bloc : 1-4096> <ACK> <number tries 1-100>\n",args[0]);
        return 1;
    }

    struct termios tty, tty_old;
    memset(&tty, 0, sizeof(tty));

    int fd = open(args[1],O_RDWR|O_NOCTTY|O_NDELAY|O_NONBLOCK);
    if(fd<0)
    {
        perror("Error during serial port opening ");
        return 1;
    }
    int fd2 = open(args[3],O_RDONLY);
    if(fd2<0)
    {
        perror("Error during file to send opening");
        return 1;
    }

    if(tcgetattr(fd, &tty) != 0)
    {
        perror("Error from tcgetattr ");
        return 1;
    }

    tty_old = tty;

    speed_t speed;
    int param = atoi(args[2]);
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

    int sizeBlocs = atoi(args[4]);
    int ack = atoi(args[5]);
    int numberTries = atoi(args[6]);
    if(sizeBlocs<=0||sizeBlocs>4096)
    {
        sizeBlocs = 2048;
        printf("Size blocs set to 2048\n");
    }
    if(numberTries<=0||numberTries>100)
    {
        numberTries = 10;
        printf("Number tries set to 10\n");
    }

    int nbRead = 1, nbWrite, nbTries = 0;
    char buf[4096];
    char onReception[2];
    memset(onReception,0,2);
    while(nbTries!=numberTries&&nbRead>0)
    {
        nbRead = read(fd2,buf,sizeBlocs);
        nbWrite = write(fd,buf,nbRead);
        if(nbWrite!=nbRead)
        {
            perror("Data not written entierely ");
            return 1;
        }
        else
            printf("%d bytes printed\n",nbWrite);
        usleep(1000000*nbRead/param*8);
        if(ack&&nbRead>0)
        {
            usleep(1000000*4/param*8);
            if(read(fd,onReception,2)<0)
            {
                perror("Read ack failed ");
                return 1;
            }
            else
            {
                int nbReceived = onReception[0]+((int)onReception[1]<<8);
                if(nbReceived!=nbWrite)
                    nbTries++;
                else
                    nbTries=0;
            }
        }
    }
    printf("***************Send finished****************\n");

    tcflush(fd, TCIFLUSH);
    if(tcsetattr(fd, TCSANOW, &tty_old) != 0)
    {
        perror("Error from tcsetattr ");
        return 1;
    }
    close(fd);

    return 0;
}
