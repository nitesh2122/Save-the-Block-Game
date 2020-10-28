#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

// actual vector representing the camera's direction
float lx = 0, lz = -1;
// XZ position of the camera
float x = 0, z = 5;

// where the block is (left = 1 or right = 0)
int lr = 0;
// help keeping track of obstacles and score
int zc = 0;
// is equal to 1 when a collison occurs i.e, game over
int gover = 0;



// change perspective when reshaped
void changePerspective(int w,int h);

// parameter for the glut idle function
// it will check for the gover parameter and will display accordingly
void loop();

// the display function 
void gamePlay();
// build the scene of the game
void scene();
// add obstables
void obstacle();
// add player
void player();
// movePlayer Func
void movePlayer(int key,int x,int y);

// display gameover screen
void gameOver();
// add space keybinding to restart game
void goverFunc(unsigned char key,int x,int y);

// utility func - string renderer
void RenderString(float x, float y, float z ,char *string);

int main(int argc, char **argv)
{
    // glut init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(720, 400);
    glutCreateWindow("Save the block");

    // event and callbacks
    glutDisplayFunc(gamePlay);
    glutReshapeFunc(changePerspective);
    glutIdleFunc(loop);

    // key binding
    glutSpecialFunc(movePlayer);
    glutKeyboardFunc(goverFunc);

    // Glut Main Loop
    glutMainLoop();

    return 1;
}


void changePerspective(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;

    float ratio = w * 1.0 / h;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45, ratio, 0, 100);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void loop()
{
    if (!gover)
    {
        gamePlay();
    }
    else
    {
        gameOver();
    }
}


void gamePlay()
{
    // background grey
    glClearColor(0.8, 0.8, 0.8, 0.8);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();
    // Set the camera
    gluLookAt(x, 7, z,
              x + lx, 7, z + lz,
              0, 7, 0);

    scene();
    obstacle();
    player();

    char score[15];
    sprintf(score, "Score : %d", zc/20);
    RenderString(-9, 35, -100, score);

    glutSwapBuffers();
}
void scene()
{
    // ground
    glBegin(GL_QUADS);
    // color - white
    glColor3f(1, 1, 1);
    glVertex3f(-100, 0, -100);
    glVertex3f(-100, 0, 100);
    glVertex3f(100, 0, 100);
    glVertex3f(100, 0, -100);

    // color - black
    glColor3f(0, 0, 0);
    glVertex3f(-10, 0, -100);
    glVertex3f(-10, 0, 100);
    glVertex3f(10, 0, 100);
    glVertex3f(10, 0, -100);
    glEnd();

    // line
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex3f(0, 0, -100);
    glVertex3f(0, 0, 100);
    glEnd();
}
void obstacle()
{
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(6.0f, 0.0f, (-100 + zc) % 100 - 100.0);
    glutSolidCube(4);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(-6.0f, 0.0f, (-80 + zc) % 100 - 100.0);
    glutSolidCube(4);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(6.0f, 0.0f, (-60 + zc) % 100 - 100.0);
    glutSolidCube(4);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(-6.0f, 0.0f, (-40 + zc) % 100 - 100.0);
    glutSolidCube(4);
    glPopMatrix();

    zc++;
    int l1 = (-80 + zc) % 100 - 100.0, l2 = (-40 + zc) % 100 - 100.0;
    int r1 = (-100 + zc) % 100 - 100.0, r2 = (-60 + zc) % 100 - 100.0;

    if ((l1 >= -24 && l1<=-17) || (l2 >= -24 && l2<=-17))
        if (lr == 1)
            gover = 1;
    if ((r1 >= -24 && r1<=-17) || (r2 >= -24 && r2<=-17))
        if (lr == 0)
            gover = 1;


    zc *= 1.0004;
}
void player()
{
    glPushMatrix();
    glColor3f(0, 1, 0);
    if (lr == 0)
        glTranslatef(6.0f, 0.0f, -20.0);
    else
        glTranslatef(-6.0f, 0.0f, -20.0);
    glutSolidCube(3);
    glPopMatrix();
}
void movePlayer(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
    {
        if (lr == 0)
            lr++;
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        if (lr == 1)
            lr = 0;
    }
}

void gameOver()
{
    glClearColor(0.8, 0.8, 0.8, 0.8);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();
    // Set the camera
    gluLookAt(x, 7, z,
              x + lx, 7, z + lz,
              0, 7, 0);

    glBegin(GL_QUADS);
    // color - white
    glColor3f(1, 0, 0);
    glVertex3f(-100, -100, -20);
    glVertex3f(-100, 100, -20);
    glVertex3f(100, 100, -20);
    glVertex3f(100, -100, -20);
    glEnd();

    RenderString(-4,20,-50,"Game Over");

    RenderString(-11,10,-50,"Press Space To Play Again");

    glutSwapBuffers();
}
void goverFunc(unsigned char key, int x, int y)
{
    if (key == 32)
        if (gover){
            gover = 0;
            zc = 0;
        }
}


void RenderString(float x, float y, float z ,char *string)
{
    glColor3f(0,0,0);
    glRasterPos3f(x, y, z);
    for (char *c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Updates the position
    }
}
