/************************************************************************

  comm.c

  Modul per a gestionar l'entrada/sortida sèrie a travers de linux

  Tomeu Capó i Capó 1999 (C)

  Last revision: 28/09/1999 


 ************************************************************************/

#include <termios.h>


struct c_port {
       char *disp;
       int descriptor;
       struct termios cfg_vella;
       struct termios cfg_nova;
       char in_buffer[255];
       char out_buffer[255];
};

typedef struct c_port *p_port, s_port;

p_port defineix_port(char *,speed_t,int);
void obre_port(p_port);
void tanca_port(p_port);

void envia_comanda(p_port, char *);
int rebre_comanda(p_port);
char *in_buffer(p_port);
