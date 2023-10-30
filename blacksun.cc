/*****************************************************************************************/
/*black sun : es un ejemplo interactivo que muestra como se utilizan algunas de los metodos
que implementa allegro para el manejo de graficas en 3d y 2d los algoritmos no se
encuantran optimizados salvo el ocultamiento de caras
y el disparo de la nave, que se implemento utilizando una lista simplemente enlazada

                                                   by Enzo Lepe
                                                   *************************************/

#include <stdio.h>
#include <math.h>
#include <iostream.h>

#include "allegro.h"
//libreria de primitivas graficas
#include "3dworld.h"
//libreria que contiene el modelo de la eronave
#include "aeronave.h"
//libreria que contiene las estrellas
//que se desplazan en 2d
#include "stars.h"
//libreria que contien las barras
#include "consola.h"


//definicion de profundidad de color
#define bpp 16
//definicion de la velocidad maxima
//solo sirve para la graficacion de las llamas
#define vel_max 1.8
//define la maxima escala que se le puede aplicar a la aeronave
#define max_escala 1.3
//define la escala minima que se puede aplicar a la aeronave
#define min_escala 0.5
// posicion original de toso los objetos en el espacio
#define minz 200


//definicion del bitmap de double buffering
BITMAP *buffer;
//definicion de la paleta que se utilizara
PALETTE pal;

//velocidades de rotado en los distintos ejes
int  Rx=0, Ry=0, Rz = 0;
//velocidad de rotado de la dona sobre la piramide
int rz = 0;
//altura inicial de la dona sobre la piramide
int dy = - 800;
//escala en la cual se encuentras los objetos en el espacio
float escala = 0.6;
//punttos de incio de todos objetos en el espacio
int x = 0, y = 150, z = minz;
//setea la velocidad inicial de la aeronave
float velocidad = 0;

/*VERTEX es uns estructura con cuatro datos (x,y,z)
que pepresentan un vertice en el espacio y son del tipo fixed, se simplifico el esquema
que presenta allegro de V3D y V3D_f por uno mas simple sin manejo de texturas, el otro valor
es "c" que representa el valor del color, y es del tipo de los queridos enteros*/
/*es piso es un poligono grande de 512 x 512 que cuenta con una textura inmensa
warning: no utilizar texturas con esta , de 512 x 512*/
VERTEX piso[] = {{ -256<<16, 0<<16, 256<<16,45},
                 {  256<<16, 0<<16, 256<<16,45},
                 {  256<<16, 0<<16,-256<<16,45},
                 { -256<<16, 0<<16,-256<<16,45},};
                 
//un poligono peque¤o que implementa un arbol*/
VERTEX arbol_p[] ={{-8<<16,  0<<16,0<<16,0},
                {-8<<16,-16<<16,0<<16,0},
                { 8<<16,-16<<16,0<<16,0},
                { 8<<16,  0<<16,0<<16,0}};


/*metodo que actualiza los valores globales de interaccion*/
void ingreso();
/*imprime la leyenda de las teclas */
void leyenda(BITMAP *buffer);


int main(void)
{
 /*texturas de los objetos en el espacio*/
 BITMAP *textura;//aeronave
 BITMAP *tex;//piso
 BITMAP *foto;//fondo de las pantalla
 BITMAP *edificio_tex;//texturas de los edificios
 BITMAP *edificio_tex2;
 BITMAP *arbol_tex;//textura del arbol que implementa mascaras

 BARRA *speed = new BARRA(100, 560, 8, 5, 1); //inicializa la barra de velocidad
 /*objeto de la aeronave*/
 aeronave *aero;
 /*objeto plano*/
 plano *p;
 /*piramide del centro*/
 piramide *centro;
 /*los edificios*/
 cubo_tex *edificio_1, *edificio_2, *edificio_3, *edificio_4, *edificio_5;
 /*las torres que rodean la piramide*/
 tapa *torre1, *torre2, *torre3, *torre4;
 /*setea la dona que esta sobre la piramide*/
 dona *d;
 /*setea el plano del arbol*/
 plano *arbol;
 //las estrellas en 2d*/
 STARS *s;
 /*matrices para cada uno de los objetos en el espacio*/
 /*matrix matrix de iteraccion de la eronave*/
 /*matrixt la del plano*/
 /*y el resto de cada uno de los objetos*/
 MATRIX matrix, matrixt, matrix_aux1, matrix_aux2;
 MATRIX matrix_aux3, matrix_aux4, matrix_aux5, matrixd;
 MATRIX matrixt1, matrixt2, matrixt3, matrixt4;
 MATRIX matrixa;

 allegro_init();
 install_timer();
 install_mouse();
 install_keyboard();

    /* seleccion de modo grafico*/
    set_color_depth(bpp);

    if (set_gfx_mode(GFX_AUTODETECT, 800, 600, 0, 0) != 0)
    {
     set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
     allegro_message("imposible inicializar modo grafico\n%s\n", allegro_error);
      return 1;
    }

     set_palette(pal);
     /*carga cada una de las texturas*/
     textura       = load_bitmap("textura.pcx", pal);
     tex           = load_bitmap("piso.pcx", pal);
     edificio_tex  = load_bitmap("edificio.pcx",pal);
     edificio_tex2 = load_bitmap("edificio2.pcx",pal);
     arbol_tex     = load_bitmap("arbol.pcx",pal);

        /* doble bufer de animacion*/
     buffer = create_bitmap(SCREEN_W, SCREEN_H);
     /*carga el bitmap de fondo*/
     foto = load_bitmap("fondo.pcx", pal);

     /* setea el punto de vision para la proyeccion*/
     set_projection_viewport(0, 0, SCREEN_W, SCREEN_H);

     /*inicializa la aeronave*/
     aero = new aeronave(textura);
     /*inicializa el plano*/
     p = new plano(tex, POLYTYPE_PTEX, piso[0], piso[1], piso[2], piso[3]);
     /*inicializa la piramide*/
     centro = new piramide(NULL, 80, 80, 80, POLYTYPE_GCOL);

     /*inicializa la dona sobre la piramide*/
     d = new dona(NULL, 40,40, 10, POLYTYPE_GRGB);
     
     /*se inicializan las torres*/
     torre1 = new tapa(NULL,5,5,120, POLYTYPE_GCOL);
     torre2 = new tapa(NULL,5,5,120, POLYTYPE_GCOL);
     torre3 = new tapa(NULL,5,5,120, POLYTYPE_GCOL);
     torre4 = new tapa(NULL,5,5,120, POLYTYPE_GCOL);

     //se inicializan los edificios
     edificio_1 = new cubo_tex(edificio_tex2, 32, 32 , 150);
     edificio_2 = new cubo_tex(edificio_tex, 32, 32 , 100);
     edificio_3 = new cubo_tex(edificio_tex, 32, 32 , 80);
     edificio_4 = new cubo_tex(edificio_tex, 32, 32 , 64);
     edificio_5 = new cubo_tex(edificio_tex, 32, 32 , 100);

     /*inicializa el arbol*/
     arbol = new plano(arbol_tex, POLYTYPE_PTEX_MASK, arbol_p[0], arbol_p[1], arbol_p[2], arbol_p[3]);
     /*inicializa las estrellas*/
     s = new STARS(SCREEN_W, SCREEN_H, 5);
     
     /*setea la profundidad de campo de los disparos
     en otras palabras hasta que profundidad seran dibujados*/
     /*NOTA Z+ el fondo de la pantalla y Z-apunta hacia nosotros*/
     aero->set_rango_lazer(800);
     aero->set_velocidad_z(4);
     
     //declara el modo de textto flotante*/
     text_mode(-1);

     for(;;)
        {
         rz+= 2; //este valor sirve para rotar la corona da la intextividad de fondo*/
         //pega la foto de las estrellas en el bitmap de double buffering
         blit(foto, buffer, 0, 0, 0, 0, foto->w, foto->h);
         //matrix que da movilidad a la aeronave
         get_transformation_matrix(&matrix, ftofix(escala), 0, 0, Rz<<16, x<<16, y<<16, z<<16);
         //matrix de campo, o sea el plano
         get_transformation_matrix(&matrixt, ftofix(0.6), 0, Ry<<16, 0, 0, 148<<16, 320<<16);

         //esto es tedioso y de seguto hay otra forma de hacerlo
         //pero bueno yo lo hize asi y asi esta
         /*estas son las matrices que trasladan, rotan los objetos en el plano
          los edificios, las torres, el arbol y la dona
          recordad que 3dworld.h solo entrega primitivas centradas en el origen*/
         
         //EDIFICIO 1
         get_transformation_matrix(&matrix_aux1, 1<<16, 0<<16, 0,0 , 200<<16, -32<<16, 170<<16);
         matrix_mul(&matrix_aux1, &matrixt, &matrix_aux1);
         edificio_1->transforma(&matrix_aux1);

         //EDIFICIO 2
         
         get_transformation_matrix(&matrix_aux2, 1<<16, 0, 0,0 , -200<<16, -32<<16, 130<<16);
         matrix_mul(&matrix_aux2, &matrixt, &matrix_aux2);
         edificio_2->transforma(&matrix_aux2);

         //EDIFICIO 3
         get_transformation_matrix(&matrix_aux3, 1<<16, 0, 0,0 , 120<<16, -32<<16, 110<<16);
         matrix_mul(&matrix_aux3, &matrixt, &matrix_aux3);
         edificio_3->transforma(&matrix_aux3);
         
         //EDIFICIO 4
         get_transformation_matrix(&matrix_aux4, 1<<16, 0, 0,0 , 150<<16, -32<<16, -150<<16);
         matrix_mul(&matrix_aux4, &matrixt, &matrix_aux4);
         edificio_4->transforma(&matrix_aux4);

         //EDIFICIO 5
         get_transformation_matrix(&matrix_aux5, 1<<16, 0, 0,0 , -220<<16, -32<<16, -170<<16);
         matrix_mul(&matrix_aux5, &matrixt, &matrix_aux5);
         edificio_5->transforma(&matrix_aux5);

         //dona
         get_transformation_matrix(&matrixd, 1<<16, 0, rz<<16, 0, 0<<16, dy <<16, 0<<16);
         matrix_mul(&matrixd, &matrixt, &matrixd);
         d->transforma(&matrixd);
         
         //torre1
         get_transformation_matrix(&matrixt1, 1<<16, 0, 0, 0, 60<<16, -60<<16, 60<<16);
         matrix_mul(&matrixt1, &matrixt, &matrixt1);
         torre1->transforma(&matrixt1);

         //torre2
         get_transformation_matrix(&matrixt2, 1<<16, 0, 0, 0, -60<<16, -60<<16, 60<<16);
         matrix_mul(&matrixt2, &matrixt, &matrixt2);
         torre2->transforma(&matrixt2);
         
         //torre3
         get_transformation_matrix(&matrixt3, 1<<16, 0, 0, 0, -60<<16, -60<<16, -60<<16);
         matrix_mul(&matrixt3, &matrixt, &matrixt3);
         torre3->transforma(&matrixt3);

         //torre4
         get_transformation_matrix(&matrixt4, 1<<16, 0, 0, 0, 60<<16, -60<<16, -60<<16);
         matrix_mul(&matrixt4, &matrixt, &matrixt4);
         torre4->transforma(&matrixt4);

         //un arbolito
         get_transformation_matrix(&matrixa, 1<<16, 0, 0, 0, -100<<16, 0<<16, -110<<16);
         matrix_mul(&matrixa, &matrixt, &matrixa);
         arbol->transforma(&matrixa);
         
         
         /*aumenta o disminuye la velocidad de la nave*/
         if(key[KEY_N] && velocidad<vel_max)
           {
            velocidad+=0.05;
           aero->set_velocidad(velocidad);
           }

         if(velocidad>0 && key[KEY_M])
           {
            velocidad-=0.05;
            aero->set_velocidad(-velocidad);
           }
         //metodo que pinta las estrellas
         s->pinta(buffer);

         //no pinta ningun lazer solo los introduce en la lista de disparos
         if(key[KEY_SPACE]) aero->lazer();
         
         /*transforma la eronave, osea la rota, traslada y escala*/
         aero->transforma(&matrix);
         /*el metodo lazer no necesita una matrix ya que construye una propia que moviliza
           los disparos hasta el rango*/
         aero->transforma_lazer();
         
         /*transforma el plano y la piramide*/
         p->transforma(&matrixt);
         centro->transforma(&matrixt);

         /*render de las figuras
         plano, piramide, edicios*/
         p->render_plano(buffer, 0);
         centro->render_piramide(buffer, 0);
         edificio_1->render_cubo_tex(buffer,0);
         edificio_2->render_cubo_tex(buffer,0);
         edificio_3->render_cubo_tex(buffer,0);
         edificio_4->render_cubo_tex(buffer,0);
         edificio_5->render_cubo_tex(buffer,0);

         //renderiza las torres
         torre1->render_tapa(buffer,0);
         torre2->render_tapa(buffer,0);
         torre3->render_tapa(buffer,0);
         torre4->render_tapa(buffer,0);
         
         /*renderiza las donas*/
         d->render_dona(buffer,0);
         
         /*renderiza la aeronave y sus disparos*/
         aero->render_aeronave(buffer);
         aero->render_lazer(buffer);

         /*renderiza el arbol*/
         arbol->render_plano(buffer,0);

         //se grafican todos los graficos 2d
         leyenda(buffer);
         /*grafica una barra de representacion de la velocidad*/
         speed->pinta(buffer,velocidad*100);
         
         /*grafica el bitmap de double buffering en el objeto pantalla: screen*/
         blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);
         /*si presionas la tecla escape te le mueres no...*/
         if(key[KEY_ESC]) break;
         /*hace iterar a las estrellas*/
         s->itera();
         /*actualiza las variables de posiones de la nave y el plano*/
         ingreso();
         /*hace bajar la corona y cuando llega a y=-150 la para y no baja mas*/
         /*recordad que Y- esta arriba y Y+ abaja, solo X permanece como tiene que se
         /*<------------------->
            x-                 x+*/
         if(dy<=-150) dy+=1;
        }
 /*aqui tiene que terminar con todo*/
 destroy_bitmap(foto);//fondo de las pantalla
 destroy_bitmap(edificio_tex);//texturas de los edificios
 destroy_bitmap(edificio_tex2);
 destroy_bitmap(arbol_tex);//textura del arbol que implementa mascaras
 destroy_bitmap(textura);
 destroy_bitmap(tex);
 destroy_bitmap(buffer);
 free(speed);
 free(aero);
 free(p);
 free(centro);

 free(edificio_1);
 free(edificio_2);
 free(edificio_3);
 free(edificio_4);
 free(edificio_5);

 free(torre1);
 free(torre2);
 free(torre3);
 free(torre4);

 free(d);
 free(arbol);
 //las estrellas en 2d*/
 free(s);
 return(1);
}
END_OF_MAIN();


/*---------------------------------------------------------------------------------------*/
/*actualiza las variables que permiten mover la nave y el plano*/
/*---------------------------------------------------------------------------------------*/
void ingreso()
{
  /*teclas que modifican el factor de escala*/
  if(key[KEY_LEFT]&&escala<=max_escala) escala+= 0.01;
  if(key[KEY_RIGHT]&&escala>=min_escala) escala-= 0.01;
  /*teclas que desplazan el objeto en y*/
  if(key[KEY_W]) y-= 3;
  if(key[KEY_S]) y+= 3;
  /*teclas que desplazan el objeto en x*/
  if(key[KEY_A]) x-= 3;
  if(key[KEY_D]) x+= 3;
  /*teclas que desplazan el objeto en z*/
  if(key[KEY_R]) z+= 3;
  if(key[KEY_F]&&z>=minz) z-= 3;
  /*teclas que rotan el objeto sobre z*/
  if(key[KEY_Q]) Rz+= 5;
  if(key[KEY_E]) Rz-= 5;

  if(key[KEY_Z]) Ry+= 2;
  if(key[KEY_X]) Ry-= 2;

}
/*---------------------------------------------------------------------------------------*/
/*leyenda es un metodo que desplega informacion acerca de lo referido en ingreso
  ose las teclas que utilizan la interactuar en blacksun*/
/*---------------------------------------------------------------------------------------*/
void leyenda(BITMAP *buffer)
{
 int dy = 200;
 char buf[100];

         sprintf(buf,"      Programa ejemplo para el uso de 3dworld.h y aeronave.h by enzo lepe     ");
         textout(buffer, font, buf, 100, 5, makecol16(255, 255, 64));

         sprintf(buf,"UP: W");
         textout(buffer, font, buf, 0, 20+dy, makecol16(255,64 ,10 ));

         sprintf(buf,"DOWN: S");
         textout(buffer, font, buf, 0, 30+dy, makecol16(255,64 ,20 ));

         sprintf(buf,"LEFT: A");
         textout(buffer, font, buf, 0, 40+dy, makecol16(255,64 ,30 ));

         sprintf(buf,"RIGHT: D");
         textout(buffer, font, buf, 0, 50+dy, makecol16(255,64 ,50 ));

         sprintf(buf,"ROLL LEFT : Q");
         textout(buffer, font, buf, 0, 60+dy, makecol16(255,64 ,60 ));

         sprintf(buf,"ROLL RIGHT : E");
         textout(buffer, font, buf, 0, 70+dy, makecol16(255,64 ,70 ));

         sprintf(buf,"Z IN: R");
         textout(buffer, font, buf, 0, 80+dy, makecol16(255,64 ,80 ));

         sprintf(buf,"Z OUT: F");
         textout(buffer, font, buf, 0, 90+dy, makecol16(255,64 ,90 ));

         sprintf(buf,"SPEED: N");
         textout(buffer, font, buf, 0, 100+dy, makecol16(255,64 ,100 ));

         sprintf(buf,"ZOOM IN: RIGHT");
         textout(buffer, font, buf, 0, 110+dy, makecol16(255,64 ,110 ));

         sprintf(buf,"ZOOM OUT: LEFT");
         textout(buffer, font, buf, 0, 120+dy, makecol16(255,64 ,120 ));

         sprintf(buf,"SHOOT: SPACE BAR");
         textout(buffer, font, buf, 0, 130+dy, makecol16(255,64 ,130 ));

         sprintf(buf,"ROTATE PLANE: +Z, -X");
         textout(buffer, font, buf, 0, 140+dy, makecol16(255,64 ,140 ));
}
/*---------------------------------------------------------------------------------------*/
