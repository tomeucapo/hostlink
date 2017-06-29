
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyS10"

int CalculaFCS(char *cads)
{
    int i,r;

    r=0;

    for(i=0;i<=strlen(cads)-1;i++) {
       r=(r ^ cads[i]);
       printf("%c,",cads[i]);
    }

    return r;
}

main()
{
     int fd,c,res,i;
     struct termios oldtio,newtio;
     char buf[255],ins[10];

     printf("Obrint el port...\n"); 
    
     fd = open(MODEMDEVICE,O_RDWR | O_NOCTTY);
     
     if (fd<0) {
        perror(MODEMDEVICE);
	exit(-1);
     }
 
         
     tcgetattr(fd,&oldtio);                     // Guarda els valors actuals del port
     bzero(&newtio,sizeof(newtio));             // Buida els valors per iniciar-los
     printf("Configurant el port...\n");

     // Inicialitzar el tipus de transmisió de dades
     // 
     // OMROM CPM1: 9600,7,E,2
     //

     newtio.c_cflag = BAUDRATE | CRTSCTS | CS7 | CSTOPB | PARENB | CLOCAL | CREAD;
     
     newtio.c_iflag = ICRNL;         
     newtio.c_oflag = 0;
     newtio.c_lflag = ICANON;         // Tipus d'entrada canònica
     
     // Inicialitzar el temps d'espera 
     
     newtio.c_cc[VTIME] = 10;
     newtio.c_cc[VMIN]  = 5;
     
     tcflush(fd,TCIFLUSH);
     tcsetattr(fd,TCSANOW,&newtio);
     
     bcopy("@00MM40*\r\0",&buf,10);
     bcopy("@00MM",&ins,6);

     printf("%s %2x\n",ins,CalculaFCS(ins));

     printf("Enviant petició d'estat...\n");
     write(fd,buf,10); 
           
     printf("Esperant resposta...\n");
     res = read(fd,buf,255);

     buf[res]=0;
     printf("resposta: %s",buf); 
    
     tcsetattr(fd,TCSANOW,&oldtio);
}
