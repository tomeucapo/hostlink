/*
   hostlink.c

   Modul per a interpretar i preparar comandes tipus Host Link
   per autòmats Omron CPM/CQM

   Smart Software 1993/99 (C)
   Tomeu Capó i Capó 1999 (C)

   Last Revision: 29/09/1999
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <curses.h>
#include "hostlink.h"
#include "propies.h"

/* 
   Calcula el FCS d'una cadena de caràcters 
   per a fer el checksum.
*/

int calculaFCS(char *cad)
{
    int i,r=0;
    for(i=0;i<=strlen(cad)-1;i++) 
       r=(r^cad[i]);

    return r;
}

/*
   Funció per a definir un PLC nou
*/

p_plc designa_plc(int model, int node)
{
      p_plc p;

      if((p=(p_plc) malloc(sizeof(p_plc)+1))==NULL) {
        printf("error al assignar memoria\n");
        return 0;
      }
    
      p->model = model;
      p->node = node;

      p->darrera_comanda = NULL;
      p->trama = (char *) malloc(255);
      return p;
}

/*
   Torna el model del PLC amb el qual s'està treballant
   abans de cridar-lo es necessari enviar-li al PLC la comanda
   de petició de model.   
*/

char *modelPLC(p_plc p)
{
     if(p->darrera_comanda==NULL)
       return 0;
}

/*
   Funció que torna el bloc d'inici de la comanda de
   resposta PLC->PC, sense la terminació i el FCS.
*/

char *agafa_inici(char *com_hl)
{
     int i,l;
     char *paquet;
     
     l = strlen(com_hl)-5;
     i = 0;

     paquet=(char *) malloc(l);
     
     while(i<=l) {
          paquet[i]=com_hl[i];
          i++;
     }

     paquet[i]='\0';

     return paquet;
}

/* 
   Ens torna el número de node del PLC que ens envia
   la comanda.
*/

int agafa_node(char *com_hl)
{
    int node = 0;
    node = node*10+(com_hl[1]-48);
    node = node*10+(com_hl[2]-48);
    return node;
}

/* 
   Ens torna el codi FCS rebut en hexadecimal
*/
 
int agafa_FCS(char *com_hl)
{
    int p_FCS,FCS=0;
    
    p_FCS=strlen(com_hl)-4;

    FCS=FCS*16+(com_hl[p_FCS]-48);
    FCS=FCS*16+(com_hl[p_FCS+1]-48);
    
    return FCS;
}

int finalitzacio(char *com_hl)
{
    int FIN = 0;
    FIN = FIN*16+(com_hl[5]-48);
    FIN = FIN*16+(com_hl[6]-48);
    return FIN;
}

/***********************************************************************/

int que_es(char *com_hl)
{
    int l;
  
    l = strlen(com_hl);

    if(com_hl[0]=='@' && com_hl[l-2]=='*')
      return 1;
    else if(com_hl[l-1]=='|')
      return COMANDA;
    else
      return -1;
}

int comprova_comanda(p_plc p, char *com_hl)
{
    if (p->node!=agafa_node(com_hl))
       return NO_NODE;

    if (finalitzacio(com_hl)!=0)
       return NO_FIN;

    return 0;
}

int llegeix_comanda(p_plc p,char *com_hl)
{
    switch(que_es(com_hl)) {
          COMANDA:comprova_comanda(com_hl);
                  break;
             2:if (comprova_comanda(com_hl)!=0)
                     afegeix_a_trama(com_hl);
                  else
                     return
                  break;
            -1:return ERROR;
    }
}

char *darrera_trama(p_plc p)
{
     return p->trama;
}
