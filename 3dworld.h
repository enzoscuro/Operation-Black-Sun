#include <stdio.h>
#include <math.h>
#include <iostream.h>

#include "allegro.h"


/*estructura de un punto en 3d
el manejo de las textura y colores se hace de una forma interna*/
typedef struct VERTEX
        {
        fixed x,y,z;
        long c;
        };
/******************************************************************************************/
/**********************************C L A S E    L I N E************************************/
/******************************************************************************************/
class linea
        {
         int dx;
        //contiene los vertices del plano centrado en el origen en R3
         V3D puntos[4];
        //puntos de salida despues de ser transformados
         V3D puntos_t[8];
        //apunta a los vertices
         V3D *puntost;
        //angulo de rotacion
         int tipo_render;
        //numero de vertices
         int num_v;

         public:                                                                                                                                                                                                                                                                                                                                                                                                 int num_vertices;
            linea(int largo, int color1, int color2);
            void transforma(MATRIX *M);
            void modificar_coordenadas(MATRIX *M);
            void render_linea(BITMAP *buffer);
            int normal();
          };
/*----------------------------------------------------------------------------------------*/
//solo sirve para renderes con texturas
linea::linea(int largo, int color1, int color2)
{
 dx = largo;
 num_v = 4;

 puntost = puntos_t;

 puntos[0].x = -2<<16;  puntos[0].y = 0;  puntos[0].z = -dx/2<<16;
 puntos[1].x =  2<<16;  puntos[1].y = 0;  puntos[1].z = -dx/2<<16;
 puntos[2].x =  2<<16;  puntos[2].y = 0;  puntos[2].z =  dx/2<<16;
 puntos[3].x = -2<<16;  puntos[3].y = 0;  puntos[3].z =  dx/2<<16;

 puntost[0].c = color1;
 puntost[1].c = color2;
 puntost[2].c = color2;
 puntost[3].c = color1;

}

/*----------------------------------------------------------------------------------------*/
//proyecta y proyecta los puntos del espacio de 3d al espacio de dispositivo
void linea::transforma(MATRIX *M)
{
 int i;
 //construye una matrix de transformacion
 for(i=0; i<num_v; i++)
    {
     apply_matrix(M,puntos[i].x, puntos[i].y, puntos[i].z, &puntost[i].x, &puntost[i].y, &puntost[i].z);
     persp_project(puntost[i].x, puntost[i].y, puntost[i].z, &puntost[i].x, &puntost[i].y);
    }
}

/*----------------------------------------------------------------------------------------*/
void linea::modificar_coordenadas(MATRIX *M)
{
 int i;
 //construye una matrix de transformacion
 for(i=0; i<num_v; i++)
    {
     apply_matrix(M,puntos[i].x, puntos[i].y, puntos[i].z, &puntos[i].x, &puntos[i].y, &puntos[i].z);
    }
}

/*----------------------------------------------------------------------------------------*/


//z_normal = 1  activa ocultacion de caras
//z_normal = 0  desactiva ocultacion de caras
void linea::render_linea(BITMAP *buffer)
{
     quad3d(buffer, POLYTYPE_GCOL, NULL, &puntost[0], &puntost[1], &puntost[2], &puntost[3]);
}
/*----------------------------------------------------------------------------------------*/
int linea::normal()
{
 return fixtoi(polygon_z_normal(&puntost[0], &puntost[1], &puntost[2]));
}

/******************************************************************************************/
/***********************************C L A S E    P L A NO**********************************/
/******************************************************************************************/
class plano
          {
            public:                                                                                                                                                                                                                                                                                                                                                                                                 int num_vertices;

            //textura del plano//
            BITMAP *textura;
            //contiene los vertices del plano centrado en el origen en R3
            V3D puntos[4];
            //puntos de salida despues de ser transformados
            V3D puntos_t[8];
            //apunta a los vertices
            V3D *puntost;
            //angulo de rotacion
            int tipo_render;
            //numero de vertices
            int num_v;

            plano(BITMAP *t, int type, VERTEX V0, VERTEX V1, VERTEX V2, VERTEX V3);
            void transforma(MATRIX *M);
            void modificar_coordenadas(MATRIX *M);
            void render_plano(BITMAP *buffer,int z_normal);
            int normal();
            void set_render(int tr) {tipo_render = tr;};
          };

/*----------------------------------------------------------------------------------------*/
//solo sirve para renderes con texturas
plano::plano(BITMAP *t, int type, VERTEX V0, VERTEX V1, VERTEX V2, VERTEX V3)
{
 tipo_render = type;

 if(t!=NULL)
   {
    textura = create_bitmap(t->w, t->h);
    clear_bitmap(textura);
   }
 textura = t;
  
 num_v = 4;

 puntost = puntos_t;

 puntos[0].x = V0.x;  puntos[0].y = V0.y;  puntos[0].z = V0.z;
 puntos[1].x = V1.x;  puntos[1].y = V1.y;  puntos[1].z = V1.z;
 puntos[2].x = V2.x;  puntos[2].y = V2.y;  puntos[2].z = V2.z;
 puntos[3].x = V3.x;  puntos[3].y = V3.y;  puntos[3].z = V3.z;

 puntost[0].u =          0<<16; puntost[0].v =          0<<16; puntost[0].c = V0.c;
 puntost[1].u = textura->w<<16; puntost[1].v =          0<<16; puntost[1].c = V1.c;
 puntost[2].u = textura->w<<16; puntost[2].v = textura->h<<16; puntost[2].c = V2.c;
 puntost[3].u =          0<<16; puntost[3].v = textura->h<<16; puntost[3].c = V3.c;

}

/*----------------------------------------------------------------------------------------*/
//proyecta y proyecta los puntos del espacio de 3d al espacio de dispositivo
void plano::transforma(MATRIX *M)
{
 int i;
 //construye una matrix de transformacion
 for(i=0; i<num_v; i++)
    {
     apply_matrix(M,puntos[i].x, puntos[i].y, puntos[i].z, &puntost[i].x, &puntost[i].y, &puntost[i].z);
     persp_project(puntost[i].x, puntost[i].y, puntost[i].z, &puntost[i].x, &puntost[i].y);
    }
}

/*----------------------------------------------------------------------------------------*/
void plano::modificar_coordenadas(MATRIX *M)
{
 int i;
 //construye una matrix de transformacion
 for(i=0; i<num_v; i++)
    {
     apply_matrix(M,puntos[i].x, puntos[i].y, puntos[i].z, &puntos[i].x, &puntos[i].y, &puntos[i].z);
    }
}

/*----------------------------------------------------------------------------------------*/

//z_normal = 1  activa ocultacion de caras
//z_normal = 0  desactiva ocultacion de caras
void plano::render_plano(BITMAP *buffer,int z_normal)
{
  if(z_normal)
    {
     if(polygon_z_normal(&puntost[0], &puntost[1], &puntost[2])>0)
     quad3d(buffer, tipo_render, textura, &puntost[0], &puntost[1], &puntost[2], &puntost[3]);
    }
  else quad3d(buffer, tipo_render, textura, &puntost[0], &puntost[1], &puntost[2], &puntost[3]);
}
/*----------------------------------------------------------------------------------------*/
int plano::normal()
{
 return fixtoi(polygon_z_normal(&puntost[0], &puntost[1], &puntost[2]));
}
/******************************************************************************************/
/***********************************C L A S E    P L A NO**********************************/
/******************************************************************************************/
class triangulo
          {
            public:                                                                                                                                                                                                                                                                                                                                                                                                int num_vertices;
            //textura del plano//
            BITMAP *textura;
            //contiene los vertices del plano centrado en el origen en R3
            V3D puntos[3];
            //puntos de salida despues de ser transformados
            V3D puntos_t[6];
            //apunta a los vertices
            V3D *puntost;
            //angulo de rotacion
            int tipo_render;
            //numero de vertices
            int num_v;
            
            triangulo(BITMAP *t, int type, VERTEX V0, VERTEX V1, VERTEX V2);
            void transforma(MATRIX *M);
            void modificar_coordenadas(MATRIX *M);
            void render_triangulo(BITMAP *buffer,int z_normal);
            int normal();
            void set_render(int tr) {tipo_render = tr;};
            fixed profundidad(int selectvertex) {if(selectvertex<3&&selectvertex>=0) return puntos[selectvertex].z;};
          };

/*----------------------------------------------------------------------------------------*/
//solo sirve para renderes con texturas
triangulo::triangulo(BITMAP *t, int type, VERTEX V0, VERTEX V1, VERTEX V2)
{
 tipo_render = type;

 if(t!=NULL)
   {
    textura = create_bitmap(t->w, t->h);
    clear_bitmap(textura);
   }
 textura = t;
  
 num_v= 3;

 puntost = puntos_t;

 puntos[0].x = V0.x;  puntos[0].y = V0.y;  puntos[0].z = V0.z;
 puntos[1].x = V1.x;  puntos[1].y = V1.y;  puntos[1].z = V1.z;
 puntos[2].x = V2.x;  puntos[2].y = V2.y;  puntos[2].z = V2.z;

 puntost[0].u =          0<<16; puntost[0].v =          0<<16; puntost[0].c = V0.c;
 puntost[1].u = textura->w<<16; puntost[1].v =          0<<16; puntost[1].c = V1.c;
 puntost[2].u = textura->w<<16; puntost[2].v = textura->h<<16; puntost[2].c = V2.c;
}

/*----------------------------------------------------------------------------------------*/
//proyecta y proyecta los puntos del espacio de 3d al espacio de dispositivo
void triangulo::transforma(MATRIX *M)
{
 int i;
 //construye una matrix de transformacion
 for(i=0; i<num_v; i++)
    {
     apply_matrix(M,puntos[i].x, puntos[i].y, puntos[i].z, &puntost[i].x, &puntost[i].y, &puntost[i].z);
     persp_project(puntost[i].x, puntost[i].y, puntost[i].z, &puntost[i].x, &puntost[i].y);
    }
}

/*----------------------------------------------------------------------------------------*/
void triangulo::modificar_coordenadas(MATRIX *M)
{
 int i;
 //construye una matrix de transformacion
 for(i=0; i<num_v; i++)
    {
     apply_matrix(M,puntos[i].x, puntos[i].y, puntos[i].z, &puntos[i].x, &puntos[i].y, &puntos[i].z);
    }
}

/*----------------------------------------------------------------------------------------*/

//z_normal = 1  activa ocultacion de caras
//z_normal = 0  desactiva ocultacion de caras
void triangulo::render_triangulo(BITMAP *buffer,int z_normal)
{
  if(z_normal)
    {
     if(polygon_z_normal(&puntost[0], &puntost[1], &puntost[2])<0)
     triangle3d(buffer, tipo_render, textura, &puntost[0], &puntost[1], &puntost[2]);
    }
  else triangle3d(buffer, tipo_render, textura, &puntost[0], &puntost[1], &puntost[2]);
}
/*----------------------------------------------------------------------------------------*/
int triangulo::normal()
{
 return fixtoi(polygon_z_normal(&puntost[0], &puntost[1], &puntost[2]));
}



/******************************************************************************************/
/*******************************C L A S E    D O N A***************************************/
/******************************************************************************************/
//hace una dona centrada en el origen*/
class dona
      {
       //dimensiones de la dona//
       int dx;
       int dz;
       int dy;
       int num_v;
       int tipo_render;
       VERTEX vertice[16];
       plano *p0 , *p1, *p2, *p3, *p4, *p5, *p6, *p7;

      public:
      dona(BITMAP *textura, int LARGO, int ANCHO, int ALTO, int tr);
      void transforma(MATRIX *M);
      void modificar_coordenadas(MATRIX *M);
      void render_dona(BITMAP *buffer, int normal);
      void set_render(int tr);
      };
/*---------------------------------------------------------------------------------------*/
dona::dona(BITMAP *textura, int LARGO, int ANCHO, int ALTO, int tr)
{
 dx = LARGO;
 dz = ANCHO;
 dy = ALTO;

 tipo_render = tr;

 num_v = 16;
 //se inicializan los vertices posteriores de la dona para altura dy/2
 vertice[0].x =   -dx<<16; vertice[0].y = dy/2<<16; vertice[0].z =  dz/2<<16; vertice[0].c = makecol16(255,255,128);
 vertice[1].x =   -dx<<16; vertice[1].y = dy/2<<16; vertice[1].z = -dz/2<<16; vertice[1].c = makecol16(255,255,128);
 vertice[2].x = -dx/2<<16; vertice[2].y = dy/2<<16; vertice[2].z =   -dz<<16; vertice[2].c = makecol16(192,192,192);
 vertice[3].x =  dx/2<<16; vertice[3].y = dy/2<<16; vertice[3].z =   -dz<<16; vertice[3].c = makecol16(192,192,192);

 vertice[4].x =    dx<<16; vertice[4].y = dy/2<<16; vertice[4].z = -dz/2<<16; vertice[4].c = makecol16(255,255,128);
 vertice[5].x =    dx<<16; vertice[5].y = dy/2<<16; vertice[5].z =  dz/2<<16; vertice[5].c = makecol16(255,255,128);
 vertice[6].x =  dx/2<<16; vertice[6].y = dy/2<<16; vertice[6].z =    dz<<16; vertice[6].c = makecol16(192,192,192);
 vertice[7].x =  -dx/2<<16; vertice[7].y = dy/2<<16; vertice[7].z =    dz<<16; vertice[7].c = makecol16(192,192,192);

 //se inicializan los vertices para -dy/2
 vertice[8].x =   -dx<<16; vertice[8].y = -dy/2<<16; vertice[8].z =  dz/2<<16; vertice[8].c = makecol16(255,255,128);
 vertice[9].x =   -dx<<16; vertice[9].y = -dy/2<<16; vertice[9].z = -dz/2<<16; vertice[9].c = makecol16(255,255,128);
 vertice[10].x = -dx/2<<16; vertice[10].y = -dy/2<<16; vertice[10].z =   -dz<<16; vertice[10].c = makecol16(192,192,192);
 vertice[11].x =  dx/2<<16; vertice[11].y = -dy/2<<16; vertice[11].z =   -dz<<16; vertice[11].c = makecol16(192,192,192);

 vertice[12].x =    dx<<16; vertice[12].y = -dy/2<<16; vertice[12].z = -dz/2<<16; vertice[12].c = makecol16(255,255,128);
 vertice[13].x =    dx<<16; vertice[13].y = -dy/2<<16; vertice[13].z =  dz/2<<16; vertice[13].c = makecol16(255,255,128);
 vertice[14].x =  dx/2<<16; vertice[14].y = -dy/2<<16; vertice[14].z =    dz<<16; vertice[14].c = makecol16(192,192,192);
 vertice[15].x =  -dx/2<<16; vertice[15].y = -dy/2<<16; vertice[15].z =    dz<<16; vertice[15].c = makecol16(192,192,192);

 //se llaman a los constructores de los distintos planos
 p0 = new plano(textura, tipo_render, vertice[0], vertice[8], vertice[9], vertice[1]);
 p1 = new plano(textura, tipo_render, vertice[1], vertice[9], vertice[10], vertice[2]);
 p2 = new plano(textura, tipo_render, vertice[2], vertice[10], vertice[11], vertice[3]);
 p3 = new plano(textura, tipo_render, vertice[3], vertice[11], vertice[12], vertice[4]);
 p4 = new plano(textura, tipo_render, vertice[4], vertice[12], vertice[13], vertice[5]);
 p5 = new plano(textura, tipo_render, vertice[5], vertice[13], vertice[14], vertice[6]);
 p6 = new plano(textura, tipo_render, vertice[6], vertice[14], vertice[15], vertice[7]);
 p7 = new plano(textura, tipo_render, vertice[7], vertice[15], vertice[8], vertice[0]);


}
/*----------------------------------------------------------------------------------------*/
void dona::transforma(MATRIX *M)
{
 p0->transforma(M);
 p1->transforma(M);
 p2->transforma(M);
 p3->transforma(M);
 p4->transforma(M);
 p5->transforma(M);
 p6->transforma(M);
 p7->transforma(M);

}
/*----------------------------------------------------------------------------------------*/
void dona::modificar_coordenadas(MATRIX *M)
{
 p0->modificar_coordenadas(M);
 p1->modificar_coordenadas(M);
 p2->modificar_coordenadas(M);
 p3->modificar_coordenadas(M);
 p4->modificar_coordenadas(M);
 p5->modificar_coordenadas(M);
 p6->modificar_coordenadas(M);
 p7->modificar_coordenadas(M);

}
/*----------------------------------------------------------------------------------------*/


//normal 1 informacion sobre vertices
//normal 0 no informacion

void dona::render_dona(BITMAP *buffer,int  normal)
{
 char buf[50];
 p0->render_plano(buffer, 0);
 p1->render_plano(buffer, 0);
 p2->render_plano(buffer, 0);
 p3->render_plano(buffer, 0);
 p4->render_plano(buffer, 0);
 p5->render_plano(buffer, 0);
 p6->render_plano(buffer, 0);
 p7->render_plano(buffer, 0);

 if(normal==1)
 {
  sprintf(buf,"Normal polygono 0:%d", p0->normal());
 textout(buffer, font, buf, 10, 20,makecol16(255,20,20));

  sprintf(buf,"Normal polygono 1:%d", p1->normal());
 textout(buffer, font, buf, 10, 30,makecol16(245,30,30));

  sprintf(buf,"Normal polygono 2:%d", p2->normal());
 textout(buffer, font, buf, 10, 40,makecol16(235,40,40));

  sprintf(buf,"Normal polygono 3:%d", p3->normal());
 textout(buffer, font, buf, 10, 50,makecol16(225,50,50));

  sprintf(buf,"Normal polygono 4:%d", p4->normal());
 textout(buffer, font, buf, 10, 60,makecol16(215,60,60));

 sprintf(buf,"Normal polygono 5:%d", p5->normal());
 textout(buffer, font, buf, 10, 70,makecol16(205,70,70));

  sprintf(buf,"Normal polygono 6:%d", p6->normal());
 textout(buffer, font, buf, 10, 80,makecol16(195,80,80));

  sprintf(buf,"Normal polygono 7:%d", p7->normal());
 textout(buffer, font, buf, 10, 90,makecol16(185,90,90));
 }
}
/*---------------------------------------------------------------------------------------*/
void dona::set_render(int tr)
{
 tipo_render = tr;
 p0->set_render(tipo_render);
 p1->set_render(tipo_render);
 p2->set_render(tipo_render);
 p3->set_render(tipo_render);
 p4->set_render(tipo_render);
 p5->set_render(tipo_render);
 p6->set_render(tipo_render);
 p7->set_render(tipo_render);

}

/******************************************************************************************/
/*******************************C L A S E    T A P A***************************************/
/******************************************************************************************/
//hace una dona centrada en el origen*/
class tapa
      {
       //dimensiones de la dona//
       int dx;
       int dz;
       int dy;
       int num_v;
       int tipo_render;
       VERTEX vertice[16];
       VERTEX punto_medio;
       VERTEX punto_medio_aux;
       //p8, p9 tapa de dona
       plano *p0 , *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;
       //triangulos de dona
       triangulo *t0, *t1, *t2, *t3, *t4, *t5, *t6, *t7;
      public:
      tapa(BITMAP *textura, int LARGO, int ANCHO, int ALTO, int tr);
      void transforma(MATRIX *M);
      void modificar_coordenadas(MATRIX *M);
      void render_tapa(BITMAP *buffer, int normal);
      void set_render(int tr);
      VERTEX get_vertex() {return (punto_medio_aux);};
      };
/*---------------------------------------------------------------------------------------*/
tapa::tapa(BITMAP *textura, int LARGO, int ANCHO, int ALTO, int tr)
{
 dx = LARGO;
 dz = ANCHO;
 dy = ALTO;

 tipo_render = tr;

 num_v = 16;
 
 punto_medio.x = 0; punto_medio.y = dy/2; punto_medio.z = 0; punto_medio.c = 0;
 //se inicializan los vertices posteriores de la dona para altura dy/2
 vertice[0].x =   -dx<<16; vertice[0].y = dy/2<<16; vertice[0].z =  dz/2<<16; vertice[0].c = makecol16(128,128,128);
 vertice[1].x =   -dx<<16; vertice[1].y = dy/2<<16; vertice[1].z = -dz/2<<16; vertice[1].c = makecol16(128,128,128);
 vertice[2].x = -dx/2<<16; vertice[2].y = dy/2<<16; vertice[2].z =   -dz<<16; vertice[2].c = makecol16(128,128,128);
 vertice[3].x =  dx/2<<16; vertice[3].y = dy/2<<16; vertice[3].z =   -dz<<16; vertice[3].c = makecol16(128,128,128);

 vertice[4].x =    dx<<16; vertice[4].y = dy/2<<16; vertice[4].z = -dz/2<<16; vertice[4].c = makecol16(128,128,128);
 vertice[5].x =    dx<<16; vertice[5].y = dy/2<<16; vertice[5].z =  dz/2<<16; vertice[5].c = makecol16(128,128,128);
 vertice[6].x =  dx/2<<16; vertice[6].y = dy/2<<16; vertice[6].z =    dz<<16; vertice[6].c = makecol16(128,128,128);
 vertice[7].x =  -dx/2<<16; vertice[7].y = dy/2<<16; vertice[7].z =    dz<<16; vertice[7].c = makecol16(128,128,128);

 //se inicializan los vertices para -dy/2
 vertice[8].x =   -dx<<16; vertice[8].y = -dy/2<<16; vertice[8].z =  dz/2<<16; vertice[8].c = makecol16(64,64,64);
 vertice[9].x =   -dx<<16; vertice[9].y = -dy/2<<16; vertice[9].z = -dz/2<<16; vertice[9].c = makecol16(64,64,64);
 vertice[10].x = -dx/2<<16; vertice[10].y = -dy/2<<16; vertice[10].z =   -dz<<16; vertice[10].c = makecol16(64,64,64);
 vertice[11].x =  dx/2<<16; vertice[11].y = -dy/2<<16; vertice[11].z =   -dz<<16; vertice[11].c = makecol16(64,64,64);

 vertice[12].x =    dx<<16; vertice[12].y = -dy/2<<16; vertice[12].z = -dz/2<<16; vertice[12].c = makecol16(64,64,64);
 vertice[13].x =    dx<<16; vertice[13].y = -dy/2<<16; vertice[13].z =  dz/2<<16; vertice[13].c = makecol16(64,64,64);
 vertice[14].x =  dx/2<<16; vertice[14].y = -dy/2<<16; vertice[14].z =    dz<<16; vertice[14].c = makecol16(64,64,64);
 vertice[15].x =  -dx/2<<16; vertice[15].y = -dy/2<<16; vertice[15].z =    dz<<16; vertice[15].c = makecol16(64,64,64);

 //se llaman a los constructores de los distintos planos
 p0 = new plano(textura, tipo_render, vertice[8], vertice[9], vertice[1], vertice[0]);
 p1 = new plano(textura, tipo_render, vertice[9], vertice[10], vertice[2], vertice[1]);
 p2 = new plano(textura, tipo_render, vertice[10], vertice[11], vertice[3], vertice[2]);
 p3 = new plano(textura, tipo_render, vertice[11], vertice[12], vertice[4], vertice[3]);
 p4 = new plano(textura, tipo_render, vertice[12], vertice[13], vertice[5], vertice[4]);
 p5 = new plano(textura, tipo_render, vertice[13], vertice[14], vertice[6], vertice[5]);
 p6 = new plano(textura, tipo_render, vertice[14], vertice[15], vertice[7], vertice[6]);
 p7 = new plano(textura, tipo_render, vertice[15], vertice[8], vertice[0], vertice[7]);
 
 p8 = new plano(textura, tipo_render, vertice[7], vertice[2], vertice[3], vertice[6]);
 p9 = new plano(textura, tipo_render, vertice[15], vertice[14], vertice[11], vertice[10]);

 t0 = new triangulo(textura, tipo_render, vertice[0], vertice[2], vertice[1]);
 t1 = new triangulo(textura, tipo_render, vertice[0], vertice[7], vertice[2]);
 t2 = new triangulo(textura, tipo_render, vertice[6], vertice[5], vertice[3]);
 t3 = new triangulo(textura, tipo_render, vertice[5], vertice[4], vertice[3]);

 t4 = new triangulo(textura, tipo_render, vertice[8], vertice[9], vertice[10]);
 t5 = new triangulo(textura, tipo_render, vertice[8], vertice[10], vertice[15]);
 t6 = new triangulo(textura, tipo_render, vertice[14], vertice[11], vertice[13]);
 t7 = new triangulo(textura, tipo_render, vertice[13], vertice[11], vertice[12]);
}
/*----------------------------------------------------------------------------------------*/
void tapa::transforma(MATRIX *M)
{
 apply_matrix(M, punto_medio.x, punto_medio.y, punto_medio.z,&punto_medio_aux.x,&punto_medio_aux.y,&punto_medio_aux.z);
 p0->transforma(M);
 p1->transforma(M);
 p2->transforma(M);
 p3->transforma(M);
 p4->transforma(M);
 p5->transforma(M);
 p6->transforma(M);
 p7->transforma(M);
 p8->transforma(M);
 p9->transforma(M);
 t0->transforma(M);
 t1->transforma(M);
 t2->transforma(M);
 t3->transforma(M);
 t4->transforma(M);
 t5->transforma(M);
 t6->transforma(M);
 t7->transforma(M);
}
/*----------------------------------------------------------------------------------------*/

void tapa::modificar_coordenadas(MATRIX *M)
{
 apply_matrix(M, punto_medio.x, punto_medio.y, punto_medio.z,&punto_medio.x,&punto_medio.y,&punto_medio.z);
 p0->modificar_coordenadas(M);
 p1->modificar_coordenadas(M);
 p2->modificar_coordenadas(M);
 p3->modificar_coordenadas(M);
 p4->modificar_coordenadas(M);
 p5->modificar_coordenadas(M);
 p6->modificar_coordenadas(M);
 p7->modificar_coordenadas(M);
 p8->modificar_coordenadas(M);
 p9->modificar_coordenadas(M);
 t0->modificar_coordenadas(M);
 t1->modificar_coordenadas(M);
 t2->modificar_coordenadas(M);
 t3->modificar_coordenadas(M);
 t4->modificar_coordenadas(M);
 t5->modificar_coordenadas(M);
 t6->modificar_coordenadas(M);
 t7->modificar_coordenadas(M);

}
/*----------------------------------------------------------------------------------------*/
//normal 1 informacion sobre vertices
//normal 0 no informacion

void tapa::render_tapa(BITMAP *buffer,int  normal)
{
 char buf[50];
 p0->render_plano(buffer, 1);
 p1->render_plano(buffer, 1);
 p2->render_plano(buffer, 1);
 p3->render_plano(buffer, 1);
 p4->render_plano(buffer, 1);
 p5->render_plano(buffer, 1);
 p6->render_plano(buffer, 1);
 p7->render_plano(buffer, 1);
 p8->render_plano(buffer, 1);
 p9->render_plano(buffer, 1);
 t0->render_triangulo(buffer, 1);
 t1->render_triangulo(buffer, 1);
 t2->render_triangulo(buffer, 1);
 t3->render_triangulo(buffer, 1);
 t4->render_triangulo(buffer, 1);
 t5->render_triangulo(buffer, 1);
 t6->render_triangulo(buffer, 1);
 t7->render_triangulo(buffer, 1);


 if(normal==1)
 {
  sprintf(buf,"Normal polygono 0:%d", p0->normal());
 textout(buffer, font, buf, 10, 20,makecol16(255,20,20));

  sprintf(buf,"Normal polygono 1:%d", p1->normal());
 textout(buffer, font, buf, 10, 30,makecol16(245,30,30));

  sprintf(buf,"Normal polygono 2:%d", p2->normal());
 textout(buffer, font, buf, 10, 40,makecol16(235,40,40));

  sprintf(buf,"Normal polygono 3:%d", p3->normal());
 textout(buffer, font, buf, 10, 50,makecol16(225,50,50));

  sprintf(buf,"Normal polygono 4:%d", p4->normal());
 textout(buffer, font, buf, 10, 60,makecol16(215,60,60));

 sprintf(buf,"Normal polygono 5:%d", p5->normal());
 textout(buffer, font, buf, 10, 70,makecol16(205,70,70));

  sprintf(buf,"Normal polygono 6:%d", p6->normal());
 textout(buffer, font, buf, 10, 80,makecol16(195,80,80));

  sprintf(buf,"Normal polygono 7:%d", p7->normal());
 textout(buffer, font, buf, 10, 90,makecol16(185,90,90));
 }
}
/*----------------------------------------------------------------------------------------*/
void tapa::set_render(int tr)
{
 tipo_render = tr;
 p0->set_render(tipo_render);
 p1->set_render(tipo_render);
 p2->set_render(tipo_render);
 p3->set_render(tipo_render);
 p4->set_render(tipo_render);
 p5->set_render(tipo_render);
 p6->set_render(tipo_render);
 p7->set_render(tipo_render);
 p8->set_render(tipo_render);
 p9->set_render(tipo_render);
 t0->set_render(tipo_render);
 t1->set_render(tipo_render);
 t2->set_render(tipo_render);
 t3->set_render(tipo_render);
 t4->set_render(tipo_render);
 t5->set_render(tipo_render);
 t6->set_render(tipo_render);
 t7->set_render(tipo_render);
}
/******************************************************************************************/
/*******************************C L A S E    C U B O***************************************/
/******************************************************************************************/
//hace una dona centrada en el origen*/
class cubo
      {
       //dimensiones de la dona//
       int dx;
       int dz;
       int dy;
       int num_v;
       int tipo_render;
       VERTEX vertice[8];
       plano *p0 , *p1, *p2, *p3, *p4, *p5;
      public:
      cubo(BITMAP *textura, int LARGO, int ANCHO, int ALTO, int tr);
      void transforma(MATRIX *M);
      void modificar_coordenadas(MATRIX *M);
      void render_cubo(BITMAP *buffer, int normal);
      void set_render(int tr);
      };
/*---------------------------------------------------------------------------------------*/
cubo::cubo(BITMAP *textura, int LARGO, int ANCHO, int ALTO, int tr)
{
 dx = LARGO;
 dz = ANCHO;
 dy = ALTO;

 tipo_render = tr;

 num_v = 8;
 //se inicializan los vertices posteriores de la dona para altura dy/2
 vertice[0].x =   -dx/2<<16; vertice[0].y = -dy/2<<16; vertice[0].z = -dz/2<<16; vertice[0].c = makecol16(64,64,64);
 vertice[1].x =   -dx/2<<16; vertice[1].y =  dy/2<<16; vertice[1].z = -dz/2<<16; vertice[1].c = makecol16(64,64,64);
 vertice[2].x =    dx/2<<16; vertice[2].y =  dy/2<<16; vertice[2].z = -dz/2<<16; vertice[2].c = makecol16(64,64,64);
 vertice[3].x =    dx/2<<16; vertice[3].y = -dy/2<<16; vertice[3].z = -dz/2<<16; vertice[3].c = makecol16(64,64,64);

 vertice[4].x =   -dx/2<<16; vertice[4].y = -dy/2<<16; vertice[4].z =  dz/2<<16; vertice[4].c = makecol16(128,128,128);
 vertice[5].x =   -dx/2<<16; vertice[5].y =  dy/2<<16; vertice[5].z =  dz/2<<16; vertice[5].c = makecol16(128,128,128);
 vertice[6].x =    dx/2<<16; vertice[6].y =  dy/2<<16; vertice[6].z =  dz/2<<16; vertice[6].c = makecol16(128,128,128);
 vertice[7].x =    dx/2<<16; vertice[7].y = -dy/2<<16; vertice[7].z =  dz/2<<16; vertice[7].c = makecol16(128,128,128);

 //se llaman a los constructores de los distintos planos
 p0 = new plano(textura, tipo_render, vertice[0], vertice[3], vertice[2], vertice[1]);
 p1 = new plano(textura, tipo_render, vertice[7], vertice[4], vertice[5], vertice[6]);
 p2 = new plano(textura, tipo_render, vertice[4], vertice[0], vertice[1], vertice[5]);
 p3 = new plano(textura, tipo_render, vertice[3], vertice[7], vertice[6], vertice[2]);

 p4 = new plano(textura, tipo_render, vertice[0], vertice[4], vertice[7], vertice[3]);
 p5 = new plano(textura, tipo_render, vertice[1], vertice[2], vertice[6], vertice[5]);
 
}
/*----------------------------------------------------------------------------------------*/
void cubo::transforma(MATRIX *M)
{
 p0->transforma(M);
 p1->transforma(M);
 p2->transforma(M);
 p3->transforma(M);
 p4->transforma(M);
 p5->transforma(M);
}
/*----------------------------------------------------------------------------------------*/
void cubo::modificar_coordenadas(MATRIX *M)
{
 p0->modificar_coordenadas(M);
 p1->modificar_coordenadas(M);
 p2->modificar_coordenadas(M);
 p3->modificar_coordenadas(M);
 p4->modificar_coordenadas(M);
 p5->modificar_coordenadas(M);
}
/*----------------------------------------------------------------------------------------*/
//normal 1 informacion sobre vertices
//normal 0 no informacion

void cubo::render_cubo(BITMAP *buffer,int  normal)
{
 char buf[50];
 p0->render_plano(buffer, 1);
 p1->render_plano(buffer, 1);
 p2->render_plano(buffer, 1);
 p3->render_plano(buffer, 1);
 p4->render_plano(buffer, 1);
 p5->render_plano(buffer, 1);

 if(normal==1)
 {
  sprintf(buf,"Normal polygono 0:%d", p0->normal());
 textout(buffer, font, buf, 10, 20,makecol16(255,20,20));

  sprintf(buf,"Normal polygono 1:%d", p1->normal());
 textout(buffer, font, buf, 10, 30,makecol16(245,30,30));

  sprintf(buf,"Normal polygono 2:%d", p2->normal());
 textout(buffer, font, buf, 10, 40,makecol16(235,40,40));

  sprintf(buf,"Normal polygono 3:%d", p3->normal());
 textout(buffer, font, buf, 10, 50,makecol16(225,50,50));

  sprintf(buf,"Normal polygono 4:%d", p4->normal());
 textout(buffer, font, buf, 10, 60,makecol16(215,60,60));

 sprintf(buf,"Normal polygono 5:%d", p5->normal());
 textout(buffer, font, buf, 10, 70,makecol16(205,70,70));
 }
}
/*----------------------------------------------------------------------------------------*/
void cubo::set_render(int tr)
{
 tipo_render = tr;
 p0->set_render(tipo_render);
 p1->set_render(tipo_render);
 p2->set_render(tipo_render);
 p3->set_render(tipo_render);
 p4->set_render(tipo_render);
 p5->set_render(tipo_render);
}
/******************************************************************************************/
/*********************************C L A S E    C O R O N A*********************************/
/******************************************************************************************/
//hace una dona centrada en el origen*/
class corona
      {
       //dimensiones de la dona//
       int dx;
       int dz;
       int dy;
       int num_v;
       int tipo_render;
       VERTEX vertice[16];
       triangulo *t0 , *t1, *t2, *t3, *t4, *t5, *t6, *t7;

      public:
      corona(BITMAP *textura, int LARGO, int ANCHO, int ALTO, int tr);
      void transforma(MATRIX *M);
      void modificar_coordenadas(MATRIX *M);
      void render_corona(BITMAP *buffer, int normal);
      void set_render(int tr);
      void set_vertex(MATRIX *m);
      };
/*---------------------------------------------------------------------------------------*/
corona::corona(BITMAP *textura, int LARGO, int ANCHO, int ALTO, int tr)
{
 dx = LARGO;
 dz = ANCHO;
 dy = ALTO;

 tipo_render = tr;

 num_v = 16;
 //se inicializan los vertices posteriores de la dona para altura dy/2
 vertice[0].x =   -dx<<16; vertice[0].y = dy/2<<16; vertice[0].z =  dz/2<<16; vertice[0].c = makecol16(247,209,57);
 vertice[1].x =   -dx<<16; vertice[1].y = dy/2<<16; vertice[1].z = -dz/2<<16; vertice[1].c = makecol16(247,209,57);
 vertice[2].x = -dx/2<<16; vertice[2].y = dy/2<<16; vertice[2].z =   -dz<<16; vertice[2].c = makecol16(247,209,57);
 vertice[3].x =  dx/2<<16; vertice[3].y = dy/2<<16; vertice[3].z =   -dz<<16; vertice[3].c = makecol16(247,209,57);

 vertice[4].x =    dx<<16; vertice[4].y = dy/2<<16; vertice[4].z = -dz/2<<16; vertice[4].c = makecol16(247,209,57);
 vertice[5].x =    dx<<16; vertice[5].y = dy/2<<16; vertice[5].z =  dz/2<<16; vertice[5].c = makecol16(247,209,57);
 vertice[6].x =  dx/2<<16; vertice[6].y = dy/2<<16; vertice[6].z =    dz<<16; vertice[6].c = makecol16(247,209,57);
 vertice[7].x =  -dx/2<<16; vertice[7].y = dy/2<<16; vertice[7].z =    dz<<16; vertice[7].c = makecol16(247,209,57);

 //se inicializan los vertices para -dy/2
 vertice[8].x =    -dx<<16; vertice[8].y =    dy/2<<16; vertice[8].z =     0<<16; vertice[8].c = makecol16(64,0,0);
 vertice[9].x =  -dx/2<<16; vertice[9].y =    dy/2<<16; vertice[9].z = -dz/2<<16; vertice[9].c = makecol16(64,0,0);
 vertice[10].x =    0<<16; vertice[10].y =   dy/2<<16; vertice[10].z =   -dz<<16; vertice[10].c = makecol16(64,0,0);
 vertice[11].x =  dx/2<<16; vertice[11].y =  dy/2<<16; vertice[11].z = -dz/2<<16; vertice[11].c = makecol16(64,0,0);

 vertice[12].x =    dx<<16; vertice[12].y =  dy/2<<16; vertice[12].z =    0<<16; vertice[12].c = makecol16(64,0,0);
 vertice[13].x =  dx/2<<16; vertice[13].y =  dy/2<<16; vertice[13].z = dz/2<<16; vertice[13].c = makecol16(64,0,0);
 vertice[14].x =     0<<16; vertice[14].y =  dy/2<<16; vertice[14].z =   dz<<16; vertice[14].c = makecol16(64,0,0);
 vertice[15].x =  -dx/2<<16; vertice[15].y =  dy/2<<16; vertice[15].z = dz/2<<16; vertice[15].c = makecol16(64,0,0);

 //se llaman a los constructores de los distintos planos
 t0 = new triangulo(textura, tipo_render, vertice[0], vertice[1], vertice[8]);
 t1 = new triangulo(textura, tipo_render, vertice[1], vertice[2], vertice[9]);
 t2 = new triangulo(textura, tipo_render, vertice[2], vertice[3], vertice[10]);
 t3 = new triangulo(textura, tipo_render, vertice[3], vertice[4], vertice[11]);
 t4 = new triangulo(textura, tipo_render, vertice[4], vertice[5], vertice[12]);
 t5 = new triangulo(textura, tipo_render, vertice[5], vertice[6], vertice[13]);
 t6 = new triangulo(textura, tipo_render, vertice[6], vertice[7], vertice[14]);
 t7 = new triangulo(textura, tipo_render, vertice[7], vertice[0], vertice[15]);


}
/*----------------------------------------------------------------------------------------*/
void corona::transforma(MATRIX *M)
{
 t0->transforma(M);
 t1->transforma(M);
 t2->transforma(M);
 t3->transforma(M);
 t4->transforma(M);
 t5->transforma(M);
 t6->transforma(M);
 t7->transforma(M);


}
/*----------------------------------------------------------------------------------------*/
void corona::modificar_coordenadas(MATRIX *M)
{
 t0->modificar_coordenadas(M);
 t1->modificar_coordenadas(M);
 t2->modificar_coordenadas(M);
 t3->modificar_coordenadas(M);
 t4->modificar_coordenadas(M);
 t5->modificar_coordenadas(M);
 t6->modificar_coordenadas(M);
 t7->modificar_coordenadas(M);
}
/*----------------------------------------------------------------------------------------*/
//normal 1 informacion sobre vertices
//normal 0 no informacion

void corona::render_corona(BITMAP *buffer,int  normal)
{
 char buf[50];
 t0->render_triangulo(buffer, 0);
 t1->render_triangulo(buffer, 0);
 t2->render_triangulo(buffer, 0);
 t3->render_triangulo(buffer, 0);
 t4->render_triangulo(buffer, 0);
 t5->render_triangulo(buffer, 0);
 t6->render_triangulo(buffer, 0);
 t7->render_triangulo(buffer, 0);

 if(normal==1)
 {
  sprintf(buf,"Normal polygono 0:%d", t0->normal());
 textout(buffer, font, buf, 10, 20,makecol16(255,20,20));

  sprintf(buf,"Normal polygono 1:%d", t1->normal());
 textout(buffer, font, buf, 10, 30,makecol16(245,30,30));

  sprintf(buf,"Normal polygono 2:%d", t2->normal());
 textout(buffer, font, buf, 10, 40,makecol16(235,40,40));

  sprintf(buf,"Normal polygono 3:%d", t3->normal());
 textout(buffer, font, buf, 10, 50,makecol16(225,50,50));

  sprintf(buf,"Normal polygono 4:%d", t4->normal());
 textout(buffer, font, buf, 10, 60,makecol16(215,60,60));

 sprintf(buf,"Normal polygono 5:%d", t5->normal());
 textout(buffer, font, buf, 10, 70,makecol16(205,70,70));

  sprintf(buf,"Normal polygono 6:%d", t6->normal());
 textout(buffer, font, buf, 10, 80,makecol16(195,80,80));

  sprintf(buf,"Normal polygono 7:%d", t7->normal());
 textout(buffer, font, buf, 10, 90,makecol16(185,90,90));
 }
}
/*----------------------------------------------------------------------------------------*/
void corona::set_render(int tr)
{
 tipo_render = tr;
 t0->set_render(tipo_render);
 t1->set_render(tipo_render);
 t2->set_render(tipo_render);
 t3->set_render(tipo_render);
 t4->set_render(tipo_render);
 t5->set_render(tipo_render);
 t6->set_render(tipo_render);
 t7->set_render(tipo_render);
}

/*----------------------------------------------------------------------------------------*/
void corona::set_vertex(MATRIX *m)
{
  //se aplica una matriz de transformacion a cada uno de los vertices del cuadrado con el fin
  //de hace crecer el triangulo
  apply_matrix(m, t0->puntos[2].x, t0->puntos[2].y ,t0->puntos[2].z , &t0->puntos[2].x, &t0->puntos[2].y, &t0->puntos[2].z);
  apply_matrix(m, t1->puntos[2].x, t1->puntos[2].y ,t1->puntos[2].z , &t1->puntos[2].x, &t1->puntos[2].y, &t1->puntos[2].z);
  apply_matrix(m, t2->puntos[2].x, t2->puntos[2].y ,t2->puntos[2].z , &t2->puntos[2].x, &t2->puntos[2].y, &t2->puntos[2].z);
  apply_matrix(m, t3->puntos[2].x, t3->puntos[2].y ,t3->puntos[2].z , &t3->puntos[2].x, &t3->puntos[2].y, &t3->puntos[2].z);
  apply_matrix(m, t4->puntos[2].x, t4->puntos[2].y ,t4->puntos[2].z , &t4->puntos[2].x, &t4->puntos[2].y, &t4->puntos[2].z);
  apply_matrix(m, t5->puntos[2].x, t5->puntos[2].y ,t5->puntos[2].z , &t5->puntos[2].x, &t5->puntos[2].y, &t5->puntos[2].z);
  apply_matrix(m, t6->puntos[2].x, t6->puntos[2].y ,t6->puntos[2].z , &t6->puntos[2].x, &t6->puntos[2].y, &t6->puntos[2].z);
  apply_matrix(m, t7->puntos[2].x, t7->puntos[2].y ,t7->puntos[2].z , &t7->puntos[2].x, &t7->puntos[2].y, &t7->puntos[2].z);
  
}
/******************************************************************************************/
/**************************** C L A S E    P I R A M I D E*********************************/
/******************************************************************************************/
//hace una piramide centrada en el origen*/
class piramide
      {
       //dimensiones de la dona//
       int dx;
       int dz;
       int dy;
       int num_v;
       int tipo_render;
       VERTEX vertice[5];
       //p8, p9 tapa de dona
       plano *base;
       triangulo *t0, *t1, *t2, *t3;
      public:
      piramide(BITMAP *textura, int LARGO, int ANCHO, int ALTO, int tr);
      void transforma(MATRIX *M);
      void modificar_coordenadas(MATRIX *M);
      void render_piramide(BITMAP *buffer, int normal);
      void set_render(int tr);
      };
/*---------------------------------------------------------------------------------------*/
piramide::piramide(BITMAP *textura, int LARGO, int ANCHO, int ALTO, int tr)
{
 dx = LARGO;
 dz = ANCHO;
 dy = ALTO;

 tipo_render = tr;

 num_v = 5;
 //se inicializan los vertices posteriores de la dona para altura dy/2
 vertice[0].x =   dx/2<<16; vertice[0].y =    0<<16; vertice[0].z =  dz/2<<16; vertice[0].c = makecol16(255,0,0);
 vertice[1].x =  -dx/2<<16; vertice[1].y =    0<<16; vertice[1].z =  dz/2<<16; vertice[1].c = makecol16(0,255,0);
 vertice[2].x =  -dx/2<<16; vertice[2].y =    0<<16; vertice[2].z = -dz/2<<16; vertice[2].c = makecol16(0,0,255);
 vertice[3].x =   dx/2<<16; vertice[3].y =    0<<16; vertice[3].z = -dz/2<<16; vertice[3].c = makecol16(255,255,255);
 vertice[4].x =      0<<16; vertice[4].y =  -dy<<16; vertice[4].z = 0 <<16; vertice[4].c = makecol16(255,0,0);

 //se llaman a los constructores de los distintos planos
 base = new plano(textura, tipo_render, vertice[0], vertice[1], vertice[2], vertice[3]);
 t0 = new triangulo(textura, tipo_render, vertice[0], vertice[4], vertice[3]);
 t1 = new triangulo(textura, tipo_render, vertice[3], vertice[4], vertice[2]);
 t2 = new triangulo(textura, tipo_render, vertice[2], vertice[4], vertice[1]);
 t3 = new triangulo(textura, tipo_render, vertice[1], vertice[4], vertice[0]);

 
}
/*----------------------------------------------------------------------------------------*/
void piramide::transforma(MATRIX *M)
{
 base->transforma(M);
 t0->transforma(M);
 t1->transforma(M);
 t2->transforma(M);
 t3->transforma(M);
}
/*----------------------------------------------------------------------------------------*/
void piramide::modificar_coordenadas(MATRIX *M)
{
 base->modificar_coordenadas(M);
 t0->modificar_coordenadas(M);
 t1->modificar_coordenadas(M);
 t2->modificar_coordenadas(M);
 t3->modificar_coordenadas(M);

}

/*----------------------------------------------------------------------------------------*/
//normal 1 informacion sobre vertices
//normal 0 no informacion

void piramide::render_piramide(BITMAP *buffer,int  normal)
{
 char buf[50];
 base->render_plano(buffer, 1);
 t0->render_triangulo(buffer, 1);
 t1->render_triangulo(buffer, 1);
 t2->render_triangulo(buffer, 1);
 t3->render_triangulo(buffer, 1);


 if(normal==1)
 {
  sprintf(buf,"Normal polygono 0:%d", base->normal());
 textout(buffer, font, buf, 10, 20,makecol16(255,20,20));

  sprintf(buf,"Normal polygono 1:%d", t0->normal());
 textout(buffer, font, buf, 10, 30,makecol16(245,30,30));

  sprintf(buf,"Normal polygono 2:%d", t1->normal());
 textout(buffer, font, buf, 10, 40,makecol16(235,40,40));

  sprintf(buf,"Normal polygono 3:%d", t2->normal());
 textout(buffer, font, buf, 10, 50,makecol16(225,50,50));

  sprintf(buf,"Normal polygono 4:%d", t3->normal());
 textout(buffer, font, buf, 10, 60,makecol16(215,60,60));

 }
}
/*---------------------------------------------------------------------------------------*/
void piramide::set_render(int tr)
{
 tipo_render = tr;
 base->set_render(tipo_render);
 t0->set_render(tipo_render);
 t1->set_render(tipo_render);
 t2->set_render(tipo_render);
 t3->set_render(tipo_render);
}

/******************************************************************************************/
/*******************************C L A S E    C U B O_T E X*********************************/
/******************************************************************************************/
//hace una dona centrada en el origen*/
class cubo_tex
      {
       //dimensiones de la dona//
       int dx;
       int dz;
       int dy;
       int num_v;
       int tipo_render;
       VERTEX vertice[8];
       plano *p0 , *p1, *p2, *p3, *p4, *p5;

      public:
      cubo_tex(BITMAP *textura, int LARGO, int ANCHO, int ALTO);
      void transforma(MATRIX *M);
      void modificar_coordenadas(MATRIX *M);
      void render_cubo_tex(BITMAP *buffer, int normal);
      };
/*---------------------------------------------------------------------------------------*/
cubo_tex::cubo_tex(BITMAP *textura, int LARGO, int ANCHO, int ALTO)
{
 dx = LARGO;
 dz = ANCHO;
 dy = ALTO;

 num_v = 8;
 //se inicializan los vertices posteriores de la dona para altura dy/2
 vertice[0].x =   -dx/2<<16; vertice[0].y = -dy/2<<16; vertice[0].z = -dz/2<<16; vertice[0].c = makecol16(64,64,64);
 vertice[1].x =   -dx/2<<16; vertice[1].y =  dy/2<<16; vertice[1].z = -dz/2<<16; vertice[1].c = makecol16(64,64,64);
 vertice[2].x =    dx/2<<16; vertice[2].y =  dy/2<<16; vertice[2].z = -dz/2<<16; vertice[2].c = makecol16(64,64,64);
 vertice[3].x =    dx/2<<16; vertice[3].y = -dy/2<<16; vertice[3].z = -dz/2<<16; vertice[3].c = makecol16(64,64,64);

 vertice[4].x =   -dx/2<<16; vertice[4].y = -dy/2<<16; vertice[4].z =  dz/2<<16; vertice[4].c = makecol16(128,128,128);
 vertice[5].x =   -dx/2<<16; vertice[5].y =  dy/2<<16; vertice[5].z =  dz/2<<16; vertice[5].c = makecol16(128,128,128);
 vertice[6].x =    dx/2<<16; vertice[6].y =  dy/2<<16; vertice[6].z =  dz/2<<16; vertice[6].c = makecol16(128,128,128);
 vertice[7].x =    dx/2<<16; vertice[7].y = -dy/2<<16; vertice[7].z =  dz/2<<16; vertice[7].c = makecol16(128,128,128);

 //se llaman a los constructores de los distintos planos
 p0 = new plano(textura, POLYTYPE_PTEX, vertice[0], vertice[3], vertice[2], vertice[1]);
 p1 = new plano(textura, POLYTYPE_PTEX, vertice[7], vertice[4], vertice[5], vertice[6]);
 p2 = new plano(textura, POLYTYPE_PTEX, vertice[4], vertice[0], vertice[1], vertice[5]);
 p3 = new plano(textura, POLYTYPE_PTEX, vertice[3], vertice[7], vertice[6], vertice[2]);

 p4 = new plano(NULL   , POLYTYPE_GCOL, vertice[0], vertice[4], vertice[7], vertice[3]);
 p5 = new plano(textura, POLYTYPE_PTEX, vertice[1], vertice[2], vertice[6], vertice[5]);
 
}
/*----------------------------------------------------------------------------------------*/
void cubo_tex::transforma(MATRIX *M)
{
 p0->transforma(M);
 p1->transforma(M);
 p2->transforma(M);
 p3->transforma(M);
 p4->transforma(M);
 p5->transforma(M);
}
/*----------------------------------------------------------------------------------------*/
void cubo_tex::modificar_coordenadas(MATRIX *M)
{
 p0->modificar_coordenadas(M);
 p1->modificar_coordenadas(M);
 p2->modificar_coordenadas(M);
 p3->modificar_coordenadas(M);
 p4->modificar_coordenadas(M);
 p5->modificar_coordenadas(M);
}
/*----------------------------------------------------------------------------------------*/
//normal 1 informacion sobre vertices
//normal 0 no informacion

void cubo_tex::render_cubo_tex(BITMAP *buffer,int  normal)
{
 char buf[50];
 p0->render_plano(buffer, 1);
 p1->render_plano(buffer, 1);
 p2->render_plano(buffer, 1);
 p3->render_plano(buffer, 1);
 p4->render_plano(buffer, 1);
 p5->render_plano(buffer, 1);

 if(normal==1)
 {
  sprintf(buf,"Normal polygono 0:%d", p0->normal());
 textout(buffer, font, buf, 10, 20,makecol16(255,20,20));

  sprintf(buf,"Normal polygono 1:%d", p1->normal());
 textout(buffer, font, buf, 10, 30,makecol16(245,30,30));

  sprintf(buf,"Normal polygono 2:%d", p2->normal());
 textout(buffer, font, buf, 10, 40,makecol16(235,40,40));

  sprintf(buf,"Normal polygono 3:%d", p3->normal());
 textout(buffer, font, buf, 10, 50,makecol16(225,50,50));

  sprintf(buf,"Normal polygono 4:%d", p4->normal());
 textout(buffer, font, buf, 10, 60,makecol16(215,60,60));

 sprintf(buf,"Normal polygono 5:%d", p5->normal());
 textout(buffer, font, buf, 10, 70,makecol16(205,70,70));
 }
}
/*----------------------------------------------------------------------------------------*/

