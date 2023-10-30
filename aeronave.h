#include <stdio.h>
#include <math.h>
#include <iostream.h>
#include "allegro.h"

//define la distancia hasta la cual el disparo sera dibujado
#define rango 120


//            -Y !   / Z
//               !  /
//               ! /
// _ _ _ _ _ _ _ !_____________________ X
// X             /
//              /!
//             / !
//            /  !
//           /   !
//      -Z  /    ! Y
/*****************************************************************************************/
class nodo
      {
    public:

      nodo *siguiente;
      triangulo *p;
      nodo(VERTEX v);
      };
/*----------------------------------------------------------------------------------------*/
nodo::nodo(VERTEX v)
{
 fixed x, y, z;
 fixed dx, dy, dz;

 x = v.x;
 y = v.y;
 z = v.z;
 dx = 3<<16;
 dy = 4<<16;
 dz = 15<<16;
 

 VERTEX plano_vertex[] = {{v.x, v.y, v.z, makecol16(0,0,0)},    //esta es la punta del ca¤o¤
                          {x + dx, y, z + dz, makecol16(255,0,0)},
                          {x - dx, y + dy, z + dz, makecol16(192,0,0)}};
 siguiente = NULL;

 p = new triangulo(NULL, POLYTYPE_GCOL, plano_vertex[0], plano_vertex[1], plano_vertex[2]);
}
/*****************************************************************************************/
class listaenlazada
{
  public:
   int tamano;
   nodo  *primero;

   listaenlazada();
   void insertar(VERTEX v);
   void eliminar();
   void modificar_coordenadas(int dz);
   void render_disparos(BITMAP *buffer);
};

/*---------------------------------------------------------------------------------------*/
listaenlazada::listaenlazada()
{
  primero = NULL;
  tamano = 0;
}

/*---------------------------------------------------------------------------------------*/
/*pone los  nodos que serian los disparos que entraron en el tiempo t al principio, y los
del tiempo t+1, t+2, t+3 .... t+n en los sucesivos lugaren en la lista*/
void listaenlazada::insertar(VERTEX v)
{
     nodo *newnodo = new nodo(v);

     if(tamano==0)
     {
      primero = newnodo; //si el primero en la lista
      tamano++;
     }

     else if (tamano==1)
     {
      primero->siguiente=newnodo; // si es el segundo en la lista
      tamano++;
     }

     else
     {
      nodo *aux = primero;
      while(aux!= NULL)
      {
        if(aux->siguiente == NULL)
          {
           aux->siguiente = newnodo;
           tamano++;
           break;
          }
       aux=aux->siguiente;
      }
     }

}
/*---------------------------------------------------------------------------------------*/
void listaenlazada::eliminar()
{
    if(tamano > 0 )
      {
       nodo *aux;
       aux = primero;
       primero = primero->siguiente;
       tamano--;
       free(aux);
      }
}
/*---------------------------------------------------------------------------------------*/
void listaenlazada::modificar_coordenadas(int dz)
{

//se mueven todos los disparos en un delta hacia el fondo de la pantalla
 int i,j;
 nodo *aux = NULL;
 MATRIX desplaza;

  if(tamano>0)
    {
     //aplica una transformacion para mover el disparo//
     get_transformation_matrix(&desplaza,1<<16, 0, 0, 0, 0, 0, dz<<16);

     aux = primero;
     //se actualizan todos los disparos en el espacio 3d
     for(i=0;i<tamano;i++)
        {
         for(j=0;j<3;j++)
            {
             aux->p->modificar_coordenadas(&desplaza);
             //se aplica una transformacion para proyectar los puntos
             //ya que modificar coordenadas es incapaz de esto
             aux->p->transforma(&identity_matrix);
            }
         aux = aux->siguiente;
        }
    }
 }
/*----------------------------------------------------------------------------------------*/
void listaenlazada::render_disparos(BITMAP *buffer)
{
//renderiza los planos de disparo
 int i,j;
 nodo *aux = NULL;


 aux = primero;
 //se actualizan todos los disparos en el espacio 3d
 for(i=0;i<tamano;i++)
    {
     aux->p->render_triangulo(buffer,0);
     aux = aux->siguiente;
    }
 }


/*****************************************************************************************/
class aeronave
      {
      cubo *CUBO;
      plano *ala_izquierda, *ala_derecha, *aleron_izq, *aleron_der;
      tapa *motor_izq, *motor_der;
      tapa *misil_izq, *misil_der;
      corona *llama_izq, *llama_der;
      listaenlazada *disparos;
      //cabina
      triangulo *t0, *t1, *t2, *t3, *t4, *t5, *t6, *t7;
      int rango_lazer;
      int velocidad_disparos;
      
      public:
      aeronave(BITMAP *textura);
      void transforma(MATRIX *m);
      void set_rango_lazer(int r){if(r>0) rango_lazer = r;};
      void transforma_lazer();
      void lazer();
      void render_aeronave(BITMAP *buffer);
      void render_lazer(BITMAP *buffer);
      void set_velocidad(float velocidad);
      void set_velocidad_z(int dz) {velocidad_disparos = dz;};
      };
/*----------------------------------------------------------------------------------------*/
aeronave::aeronave(BITMAP *textura)
{
 MATRIX  matrix1;

 //vertices de las alas
 VERTEX vertex1[] = {{ -15<<16, 0<<16,  5<<16,makecol16(192,192,255)},
                    { -32<<16, 5<<16, -15<<16,makecol16(0,0,192)},
                    { -26<<16, 5<<16, -26<<16,makecol16(0,0,0)},
                    { -15<<16, 0<<16, -20<<16,makecol16(128,128,255)}};

 VERTEX vertex2[] = {{ 15<<16, 0<<16,  5<<16,makecol16(192,192,255)},
                    { 32<<16, 5<<16, -15<<16,makecol16(0,0,192)},
                    { 26<<16, 5<<16, -26<<16,makecol16(0,0,0)},
                    { 15<<16, 0<<16, -20<<16,makecol16(128,128,255)}};

 //vertices de la cabina
 VERTEX vertex[] ={{ 0<<16, -3<<16,  0<<16,makecol16(0,0,0)},//0
                   {-5<<16, -3<<16,  5+16<<16,makecol16(0,0,192)},//1
                   {-3<<16, -8<<16,  5+16<<16,makecol16(0,0,192)},//2
                   { 3<<16, -8<<16,  5+16<<16,makecol16(0,0,192)},//3
                   { 5<<16, -3<<16,  5+16<<16,makecol16(0,0,192)},//4
                   { 0<<16,  4<<16,  5+16<<16,makecol16(0,0,192)},//5
                   { 0<<16,  0<<16, 25+16<<16,makecol16(255,255,255)}};//6


 //triangulo(BITMAP *t, int type, VERTEX V0, VERTEX V1, VERTEX V2);

 t0 =  new triangulo(NULL, POLYTYPE_GCOL, vertex[0], vertex[2], vertex[1]);
 t1 =  new triangulo(NULL, POLYTYPE_GCOL, vertex[0], vertex[3], vertex[2]);
 t2 =  new triangulo(NULL, POLYTYPE_GCOL, vertex[0], vertex[4], vertex[3]);
 t3 =  new triangulo(NULL, POLYTYPE_GCOL, vertex[6], vertex[4], vertex[5]);
 t4 =  new triangulo(textura, POLYTYPE_ATEX_MASK, vertex[6], vertex[3], vertex[4]);
 t5 =  new triangulo(textura, POLYTYPE_ATEX_MASK, vertex[6], vertex[1], vertex[2]);
 t6 =  new triangulo(NULL, POLYTYPE_GCOL, vertex[6], vertex[5], vertex[1]);
 t7 =  new triangulo(textura, POLYTYPE_PTEX_MASK, vertex[6], vertex[2], vertex[3]);

 //plano(BITMAP *t, int type, VERTEX V0, VERTEX V1, VERTEX V2, VERTEX V3);
 ala_izquierda = new plano(NULL,POLYTYPE_GCOL, vertex1[0], vertex1[1], vertex1[2], vertex1[3]);
 ala_derecha   = new plano(NULL,POLYTYPE_GCOL, vertex2[0], vertex2[1], vertex2[2], vertex2[3]);

 VERTEX vertex3[] = {{ -12<<16,  0-5<<16,  0-12<<16,makecol16(192,192,192)},
                     { -12<<16,-10-5<<16, -5-12<<16,makecol16(192,192,192)},
                     { -12<<16,-10-4<<16,-10-12<<16,makecol16(0,0,0)},
                     { -12<<16,  0-4<<16, -5-12<<16,makecol16(192,192,192)},
                     {  12<<16,  0-5<<16,  0-12<<16,makecol16(192,192,192)},
                     {  12<<16,-10-5<<16, -5-12<<16,makecol16(192,192,192)},
                     {  12<<16,-10-4<<16,-10-12<<16,makecol16(0,0,0)},
                     {  12<<16,  0-4<<16, -5-12<<16,makecol16(192,192,192)}};
 


 aleron_izq = new plano(NULL,POLYTYPE_GCOL, vertex3[0], vertex3[1], vertex3[2], vertex3[3]);
 aleron_der = new plano(NULL,POLYTYPE_GCOL, vertex3[4], vertex3[5], vertex3[6], vertex3[7]);

 //cubo(BITMAP *textura, int LARGO, int ANCHO, int ALTO, int tr);
 //CUERPO DE LA NAVE
 CUBO = new cubo(NULL, 10, 40, 5, POLYTYPE_GCOL);
 // tapa(BITMAP *textura, int LARGO, int ANCHO, int ALTO, int4 tr);
 //MOTORES A AMBOS LADOS DE LA AVION
 motor_izq = new tapa(NULL, 5, 5, 35, POLYTYPE_GCOL);
 motor_der = new tapa(NULL, 5, 5, 35, POLYTYPE_GCOL);
 //MISILES A AMBOS LADOS DE LA NAVE
 misil_izq = new tapa(NULL, 2, 2, 30, POLYTYPE_FLAT);
 misil_der = new tapa(NULL, 2, 2, 30, POLYTYPE_FLAT);
 //llama de los mototes
 llama_izq = new corona(NULL,4, 4, 15, POLYTYPE_GCOL);
 llama_der = new corona(NULL,4, 4, 15, POLYTYPE_GCOL);

  get_transformation_matrix(&matrix1, 1<<16, -64<<16, 2<<16 ,0<<16,-10<<16, 0<<16, -5<<16);
  motor_izq->modificar_coordenadas(&matrix1);
  get_transformation_matrix(&matrix1, 1<<16, -64<<16, 2<<16 ,0<<16, 10<<16, 0<<16, -5<<16);
  motor_der->modificar_coordenadas(&matrix1);

  get_transformation_matrix(&matrix1, 1<<16, -64<<16, 2<<16 ,0<<16,-23<<16, 4<<16, -12<<16);
  misil_izq->modificar_coordenadas(&matrix1);
  
  get_transformation_matrix(&matrix1, 1<<16, -64<<16, 2<<16 ,0<<16, 23<<16, 4<<16, -12<<16);
  misil_der->modificar_coordenadas(&matrix1);

  get_transformation_matrix(&matrix1, 1<<16, -64<<16, 2<<16 ,0<<16, -10<<16, 0<<16, -28<<16);
  llama_izq->modificar_coordenadas(&matrix1);
  get_transformation_matrix(&matrix1, 1<<16, -64<<16, 2<<16 ,0<<16, 10<<16, 0<<16, -28<<16);
  llama_der->modificar_coordenadas(&matrix1);

  rango_lazer = rango;

  disparos = new listaenlazada();
}
/*--------------------------------------------------------------------------------------*/

void aeronave::transforma(MATRIX *m)
{
 CUBO->transforma(m);
 motor_izq->transforma(m);
 motor_der->transforma(m);
 misil_izq->transforma(m);
 misil_der->transforma(m);
 llama_izq->transforma(m);
 llama_der->transforma(m);
 ala_derecha->transforma(m);
 ala_izquierda->transforma(m);
 aleron_izq->transforma(m);
 aleron_der->transforma(m);
 t0->transforma(m);
 t1->transforma(m);
 t2->transforma(m);
 t3->transforma(m);
 t4->transforma(m);
 t5->transforma(m);
 t6->transforma(m);
 t7->transforma(m);
}

/*----------------------------------------------------------------------------------------*/
void aeronave::render_aeronave(BITMAP *buffer)
{
 CUBO->render_cubo(buffer,0);
 motor_izq->render_tapa(buffer,0);
 motor_der->render_tapa(buffer,0);
 misil_izq->render_tapa(buffer,0);
 misil_der->render_tapa(buffer,0);
 llama_izq->render_corona(buffer,0);
 llama_der->render_corona(buffer,0);
 ala_derecha->render_plano(buffer,0);
 ala_izquierda->render_plano(buffer,0);
 aleron_izq->render_plano(buffer,0);
 aleron_der->render_plano(buffer,0);

 //CABINA
 t0->render_triangulo(buffer,1);
 t1->render_triangulo(buffer,1);
 t2->render_triangulo(buffer,1);
 t3->render_triangulo(buffer,1);
 t4->render_triangulo(buffer,1);
 t5->render_triangulo(buffer,1);
 t6->render_triangulo(buffer,1);
 t7->render_triangulo(buffer,1);
}
/*----------------------------------------------------------------------------------------*/
void aeronave::set_velocidad(float velocidad)
{
 MATRIX M;
 get_transformation_matrix(&M, 1<<16, 0<<16, 0<<16 ,0<<16, 0<<16, 0<<16, ftofix(-velocidad));
 llama_izq->set_vertex(&M);
 llama_der->set_vertex(&M);
}
/*----------------------------------------------------------------------------------------*/
/*se introducen los nuevos disparos a la lista*/
/*los disparos son untroducidos de izquierda a derecha
  izq 0->der 1->izq 2->der 2->NULL etc*/
void aeronave::lazer()
{
 char buf[50];
 //se toma el vertice del medio de cada uno de los ca¤ones
 VERTEX aux_izquierdo = misil_izq->get_vertex();
 VERTEX aux_derecho = misil_der->get_vertex();
 //se agregan a la lista ambos diaparos
 disparos->insertar(aux_izquierdo);
 disparos->insertar(aux_derecho);

}
/*----------------------------------------------------------------------------------------*/
void aeronave::transforma_lazer()
{
 disparos->modificar_coordenadas(velocidad_disparos);
}
/*----------------------------------------------------------------------------------------*/
void aeronave::render_lazer(BITMAP *buffer)
{
 char buf[50];

   VERTEX aux_izq = misil_izq->get_vertex();
   VERTEX aux_der = misil_der->get_vertex();

 if(disparos->tamano>0)
   {
     if(disparos->primero->p->profundidad(0)<rango_lazer<<16)
       {
        disparos->render_disparos(buffer);
       }
     else
       {
       //elimina los dos primeros ca¤os derecho e izquierdo
        disparos->eliminar();
        disparos->eliminar();
        disparos->render_disparos(buffer);
       }
   }
}
/******************************************************************************************/
