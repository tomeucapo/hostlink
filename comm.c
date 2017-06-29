/************************************************************************

  comm.c

  Modul per a gestionar l'entrada/sortida sèrie a travers de linux

  Smart Software 1993/99 (C)
  Tomeu Capó i Capó 1999 (C)

  Last revision: 28/09/1999

 ************************************************************************/

#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include "comm.h"
#include "propies.h"

p_port defineix_port(char *disp,speed_t velocitat,int tdades)
{
       p_port p;
       
       if((p=(p_port) malloc(sizeof(s_port)+1))==NULL) {
         printf("error al reservar la memoria\n");
         return 0;
       }
       
       p->disp = duplica_cad(disp);
       bzero(&p->cfg_nova,sizeof(p->cfg_nova));
       p->cfg_nova.c_cflag = velocitat | CRTSCTS | tdades | CREAD; 

       p->cfg_nova.c_iflag = ICRNL;           // Converteix el caracter CR->NULL
       p->cfg_nova.c_oflag = 0;               // Sortida tipus RAW
       p->cfg_nova.c_lflag = ICANON;
}

void obre_port(p_port p)
{
     p->descriptor = open(p->disp, O_RDWR | O_NOCTTY);

     if (p->descriptor<0)
        perror(p->disp);
     else {
        tcgetattr(p->descriptor,&p->cfg_vella);
        tcflush(p->descriptor,TCIFLUSH);
        tcsetattr(p->descriptor,TCSANOW,&p->cfg_nova);
     } 
}

void tanca_port(p_port p)
{
     tcsetattr(p->descriptor,TCSANOW,&p->cfg_vella);
     close(p->descriptor);
}

void envia_comanda(p_port p,char *comanda)
{
//     bcopy("@00MS5E*\r\0",&p->out_buffer,10);
     bcopy(comanda,&p->out_buffer,strlen(comanda));

     if(fork()==0)
       write(p->descriptor,p->out_buffer,strlen(comanda));
}

int rebre_comanda(p_port p)
{
     int res;
   
     if(fork()==0) 
       res = read(p->descriptor,p->in_buffer,255);
}

char *in_buffer(p_port p)
{
     return p->in_buffer;
}
