
/* Respostes de la funció interpreta_comanda */

#define NO_COMANDA 0xF001
#define NO_NODE    0xF002
#define NO_FCS     0xF003
#define NO_FIN     0xF004
#define TROS_TRAMA 0xE001
#define COMANDA_OK 0x0

/* Estructura que utilitzarem */

struct s_plc {
       int node;
       int model;
       char *darrera_comanda;
       char *trama;
};

typedef struct s_plc *p_plc,r_plc;

p_plc designa_plc(int, int);
char *darrera_trama(p_plc);
