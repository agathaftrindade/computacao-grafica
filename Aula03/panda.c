//header

/* #include<GL/gl.h> */
#include<GL/glut.h>
#include<stdio.h>

//globals

GLuint panda;
float pandarot = 180;
char ch='1';

/* #define BACKGROUND_R 1.0
/* #define BACKGROUND_G 1.0 */
/* #define BACKGROUND_B 1.0 */
#define BACKGROUND_R 1
#define BACKGROUND_G 1
#define BACKGROUND_B 1

#define OBJECT_R 0
#define OBJECT_G 0
#define OBJECT_B 0
/* #define OBJECT_R 0.16470588 */
/* #define OBJECT_G 0.74901961 */
/* #define OBJECT_B 0.15686275 */

#define OBJECT_ROTATION_Y 1.1

#define OBJECT_SCALE 3

#define OBJECT_POINT_SIZE 2

//.obj loader

void loadObj(char *fname)
{
    FILE *fp; // cria um ponteiro arquivo
    int read;
    GLfloat x, y, z;
    char ch;
    panda=glGenLists(1);//#
    fp=fopen(fname, "r"); //Abre um arquivo para leitura.

    if(!fp)
        {
            printf("nao pode abrir o arquivo %s\n", fname);
            exit(1);
        }
    glPointSize(OBJECT_POINT_SIZE); // Tamanho do diametro do ponto
    glNewList(panda, GL_COMPILE); // Define a lista de exibição

    glPushMatrix(); // Essas rotinas push e pop podem ser legalmente armazenadas em cache em uma lista de exibição.
    glBegin(GL_POINTS);
    //glDisable(GL_POINT_SMOOTH);
    //glEnable(GL_POINT_SMOOTH);
    while(!(feof(fp)))
        {
            read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
            if(read==4&&ch=='v')
                {
                    glVertex3f(x,y,z); // recebe as coordenadas e desenha o objeto na janela.
                }
        }
    glEnd();

    glPopMatrix(); // Essas rotinas push e pop podem ser legalmente armazenadas em cache em uma lista de exibição.
    glEndList();
    fclose(fp); // fecha arquivo
}

void reshape(int w,int h)
{
    glViewport(0,0,w,h); // Define a janela de Visualização e local onde a imagem será visualizada
	glMatrixMode(GL_PROJECTION); // Aplica operações de matriz subseqüentes à pilha de matriz de projeção.
	glLoadIdentity(); //#
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW); //#
}

void drawObject()
{
 	glPushMatrix(); // Essas rotinas push e pop podem ser legalmente armazenadas em cache em uma lista de exibição.

 	glTranslatef(0, -25, -105); //Desenha o objeto descolado
 	glColor3f(OBJECT_R, OBJECT_G, OBJECT_B); // Mudar a cor dos pontos do objeto
 	glScalef(OBJECT_SCALE, OBJECT_SCALE, OBJECT_SCALE); // Muda o tamanho do objeto
 	glRotatef(pandarot, 0, 1, 0); //Gira o objeto a redor do eixo Y
 	glRotatef(270, 1, 0, 0);
 	glCallList(panda);
 	glPopMatrix(); // Essas rotinas push e pop podem ser legalmente armazenadas em cache em uma lista de exibição.
 	pandarot=pandarot + OBJECT_ROTATION_Y;
 	if(pandarot > 360)
        pandarot=pandarot-360; // Gira o objeto em 360 graus
}

void display(void)
{
   	glClearColor(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, 1.0); // Define a cor de fundo da janela de visualização
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Limpa a janela de visualização com a cor de fundo especificada
   	glLoadIdentity();
   	drawObject(); // função projetada para definir a exibicao do objeto
   	glutSwapBuffers(); //trocar os buffers

}

int main(int argc,char **argv)

{
	glutInit(&argc,argv); //glutInit é usado para inicializar a biblioteca GLUT.
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH); // GLUT_DOUBLE  Este método é geralmente utilizado para produzir efeitos de animação
	glutInitWindowSize(800,450);  //Define tamanho da janela.
	glutInitWindowPosition(10,10); // Define a posicao da janela.
	glutCreateWindow("Carregar Objeto");// Cria uma janela com o nome "Nome do programa".
	glutReshapeFunc(reshape); //#
    glutDisplayFunc(display); //#
	glutIdleFunc(display); //#
    loadObj("data/panda.obj");// busca o arquivo tipo objeto
	glutMainLoop(); //#
	return 0;

}
