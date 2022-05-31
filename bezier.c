#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#define size(x)  (sizeof(x) / sizeof((x)[0]))

// Largura e altura da janela
GLfloat windowWidth;
GLfloat windowHeight;

typedef struct{
    double x;
    double y;
}Point;

const Point make_point(double x, double y) 
{
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

// pontos
Point points[4];
const int STEP = 100;

void make_all_points()
{
    points[0] = make_point(30,39);
    points[1] = make_point(105,200);
    points[2] = make_point(178,115);
    points[3] = make_point(225,225);
}

void draw_lines()
{
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < size(points);i++)
        glVertex2d(points[i].x,points[i].y);
    glEnd();
}

Point new_point(Point p1,Point p2,double t)
{
    double d = sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
    double ang = atan2(p2.y - p1.y, p2.x - p1.x);
    return make_point(p1.x+cos(ang)*t*d,p1.y+sin(ang)*t*d);
}

Point bezier(double t,Point *p,int s)
{
    while(s-->2)
    {
        Point np[0<s?s:0];
        for(int i = 0; i <= s;i++)
        {
            Point p1 = p[i];
            Point p2 = p[i+1];
            np[i] = new_point(p1,p2,t);
        }
        p = np;
    }
    return new_point(p[0],p[1],t);    
}

void draw_bezier()
{
    double step = 1.0/STEP;
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i <= STEP;i++)
    {
        Point p = bezier(step * i,points,size(points));
        glVertex2d(p.x,p.y);
    }
    glEnd();
}


// Função callback chamada para fazer o desenho
void Desenha(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
                
    // Limpa a janela de visualização com a cor de fundo especificada
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 1.0f);

    draw_lines();

    glColor3f(1.0f, 1.0f, 1.0f);

    draw_bezier();
    // Executa os comandos OpenGL
    glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void Inicializa (void)
{   
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Evita a divisao por zero
    if(h == 0) h = 1;
                        
    // Especifica as dimensões da Viewport
    glViewport(0, 0, w, h);

    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Estabelece a janela de seleção (left, right, bottom, top)     
    if (w <= h)  {
        windowHeight = 250.0f*h/w;
        windowWidth = 250.0f;
    }
    else  { 
        windowWidth = 250.0f*w/h;
        windowHeight = 250.0f;
    }
    
    gluOrtho2D(0.0f, windowWidth, 0.0f, windowHeight);
}

// Programa Principal 
int main(int argc, char** argv)
{
    make_all_points();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400,350);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Anima");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    Inicializa();
    glutMainLoop();
    return 0;
}
