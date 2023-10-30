#include<iostream.h>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#define MAX_STARS 200

class STARS
       {
               int x[MAX_STARS];
               int y[MAX_STARS];
               int z[MAX_STARS];
               int max_speed;
               int largo;
               int alto;
               public:
                              STARS(int l, int a, int m);
                              void pinta(BITMAP *FONDO);
                              void itera();
       };
/********************************************************************/
STARS::STARS(int l, int a, int m)
       {
       int i;
       largo = l;
       alto  = a;
       max_speed=m;
       
          for(i=0;i<MAX_STARS; i++)
             {
             x[i] = random()%largo;
             y[i] = random()%alto;
             z[i] = random()%(m)+1;
             }
       }
/********************************************************************/
void STARS::pinta(BITMAP *FONDO)
       {
       int i;
       for(i=0;i<MAX_STARS; i++)
          {
           _putpixel16(FONDO, x[i], y[i], makecol16(255-2*z[i], 255-2*z[i], 255-2*z[i]));
          }
       }
/********************************************************************/
void STARS::itera()
       {
       int i;
       for(i=0;i<MAX_STARS;i++)
          {
           x[i]+= z[i];
          }
          
       if(x[i]>=largo)
         {
                 x[i] = random()%largo;
                 y[i] = random()%alto;
                 z[i] = rand()%(max_speed)+1;
         }
       }

