#include <unistd.h>
#include <fcntl.h>
#include <stdio.h> 

int main()
{

    int fd;
    int ret = 0;
    char buff[100];

    fd = open("/dev/driverdrlr",O_RDWR);
    
    if(fd < 0) {
        printf("/dev/driverdrlr open failed (ret = %d)\n",fd);
        return 1;   
    };

    ret = read(fd,buff,10);
    printf("%s\n",buff);    
    ret = write(fd,"Hola Profe",11);

    ret = read(fd,buff,11);
    printf("%s\n",buff);

    close(fd);

    return 1;
}