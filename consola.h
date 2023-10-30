#include<iostream.h>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#define MAX_COLOR 20


class BARRA
      {
      int xpos;
      int ypos;
      int deltay;
      int deltax;//cuantas lineas pinta de cada color si dx=1 20 lineas de colres diferentes
                 //si dx es 2, 40 lineas de cada dos lineas se cambia color
                 //si dx es 3, 60 lineas, tres lineas por color
                 //si dx es 4, 80 lineas, cuatro lineas por color

      int color_map[MAX_COLOR];
      int sentido; //1 derecha
      char buf[3];
      
      public:
      BARRA(int x, int y, int dx, int dy, int s);
      int pinta(BITMAP *FONDO, int valor);
      };

/*------------------------------------------------------------------------------*/
  BARRA::BARRA(int x, int y, int dx, int dy, int s)
         {
          xpos = x;
          ypos = y;
          deltax = dx;
          deltay = dy;
          sentido = s;
          sprintf(buf,"max");

               color_map[0] = makecol16(0,0,0);
               color_map[1] = makecol16(75,17,17);
               color_map[2] = makecol16(119,25,25);
               color_map[3] = makecol16(129,29,29);
               color_map[4] = makecol16(155,37,37);
               color_map[5] = makecol16(174,42,42);
               color_map[6] = makecol16(200,50,50);
               color_map[7] = makecol16(200,96,50);
               color_map[8] = makecol16(209,113,71);
               color_map[9] = makecol16(215,113,73);
               color_map[10] = makecol16(211,156,69);
               color_map[11] = makecol16(219,169,89);
               color_map[12] = makecol16(219,220,98);
               color_map[13] = makecol16(227,228,124);
               color_map[14] = makecol16(232,233,143);
               color_map[15] = makecol16(237,238,166);
               color_map[16] = makecol16(242,243,191);
               color_map[17] = makecol16(248,248,218);
               color_map[18] = makecol16(252,252,234);
               color_map[19] = makecol16(255,255,255);
         }
/*-------------------------------------------------------------------------------*/
int BARRA::pinta(BITMAP *FONDO, int valor)
         {
          int i;
          int jcolor=0;

         if(sentido==1)
            {
              textout(FONDO, font, "speed ",xpos-45, ypos-1, color_map[19]);
              rect(FONDO, xpos-1, ypos-1, xpos+MAX_COLOR*deltax+1, ypos+deltay+1, makecol(255, 255, 64));
              for(i=0;i<=MAX_COLOR*deltax;i++)
                 {
                  if(i%deltax==0) jcolor++;
                  if(i==MAX_COLOR*deltax) textout(FONDO , font, buf, xpos+i-25, ypos-1 , color_map[0]);
                  line(FONDO, xpos+i, ypos, xpos+i, ypos+deltay, color_map[jcolor-1]);
                  if(i==valor) return 0;
                 }
            }

           else
            {
             textout(FONDO, font, "speed ",xpos+5, ypos-1, color_map[19]);
             rect(FONDO, xpos+1, ypos-1, xpos-MAX_COLOR*deltax-1, ypos+deltay+1, makecol(255, 255, 64));
             for(i=0;i<=MAX_COLOR*deltax;i++)
                 {
                  if(i%deltax==0) jcolor++;
                  if(i==MAX_COLOR*deltax) textout(FONDO , font, buf, xpos-i+4, ypos-1, color_map[0]);
                  line(FONDO, xpos-i, ypos, xpos-i, ypos+deltay, color_map[jcolor]);
                  if(i==valor) return 0;
                 }
            }

          }
/********************************************************************************/
/*                                                                              */
/********************************************************************************/
class CONSOLA
       {
       public:
        int turno;
        int potencia;
        int angulo;
        int viento;
       
        BITMAP *SKIN;
        BITMAP *SKIN_aux;
        BARRA *potenciaplayer1;
        BARRA *potenciaplayer2;

        BARRA *vientooeste;
        BARRA *vientoeste;
        char bufp1[5];
        char bufp2[5];

        CONSOLA(BITMAP *bitmap_consola);
        void actualizar(int t, int p, int a, int v);
        BITMAP *pintar();
       };

/*----------------------------------------------------------------------------------*/
CONSOLA::CONSOLA(BITMAP *bitmap_consola)
       {
        SKIN_aux = bitmap_consola;
        SKIN = create_bitmap(320, 80);
        int turno = 0;
        int potencia = 0;
        int angulo = 0;
        int viento = 0;

        potenciaplayer1 = new BARRA(100, 36, 2, 6, 1);
        potenciaplayer2 = new BARRA(220, 36, 2, 6, -1);
        vientooeste = new BARRA(130, 5, 3, 5,-1);
        vientoeste = new BARRA(185, 5, 3, 5,1);
       }
/*---------------------------------------------------------------------------------*/
void CONSOLA::actualizar(int t, int p, int a, int v)
     {
     turno = t;
     potencia = p;
     angulo = a;
     viento = v;
     }
/*---------------------------------------------------------------------------------*/
//  turno 2 va con barra 1, y turno 1 va con barra  0
//2->1;1->0
BITMAP *CONSOLA::pintar()
     {
      clear(SKIN);
      blit(SKIN_aux, SKIN, 0, 0, 0, 0, SKIN->w, SKIN->h);
      if(turno==1)
              {
               potenciaplayer1->pinta(SKIN,potencia);
               potenciaplayer2->pinta(SKIN,0);
               
               sprintf(bufp1,"%d",angulo);
               textout(SKIN, font, bufp1, 89, 17, makecol16(0, 192, 0));
               sprintf(bufp2,"%d",90);
               textout(SKIN, font, bufp2, 230-16, 17, makecol16(0, 192, 0));
              }
              
      if(turno==2)
              {
               potenciaplayer1->pinta(SKIN, 0);
               potenciaplayer2->pinta(SKIN, potencia);
               sprintf(bufp2,"%d",angulo);
               textout(SKIN, font, bufp2, 230-16, 17, makecol16(0, 192, 0));
               sprintf(bufp1,"%d",90);
               textout(SKIN, font, bufp1, 89, 17, makecol16(0, 192, 0));

              }
              
      if(viento>0)
              {
               vientoeste->pinta(SKIN, 30*viento);
               vientooeste->pinta(SKIN, 0);
              }
       else
              {
               vientoeste->pinta(SKIN, 0);
               vientooeste->pinta(SKIN, 30*viento);
              }
              
              
       return SKIN;
      }







