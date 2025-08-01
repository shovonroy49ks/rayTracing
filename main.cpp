#include"classes.hpp"

#define TILES_SIZE 15
Camera camera;
bool fullscreen = false;

void initScene()
{
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.apply();

    // Draw checkered floor (y=0)
    
    for (int i = 0; i < TILES_SIZE ; ++i)
    {
        for (int j = 0; j < TILES_SIZE; ++j)
        {
            if ((i + j) % 2 == 0)
                glColor3f(1.0f, 1.0f, 1.0f);
            else
                glColor3f(0.2f, 0.2f, 0.2f);
            glBegin(GL_QUADS);
            glVertex3f(i, 0.0f, j);
            glVertex3f(i + 1, 0.0f, j);
            glVertex3f(i + 1, 0.0f, j + 1);
            glVertex3f(i, 0.0f, j + 1);
            glEnd();
        }
    }


    glutSwapBuffers();
}

void reshape(int w, int h)
{
    float ratio = (float)w / (float)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // ESC
        exit(0);
    
    case 'w':
        camera.moveUpLocal(+0.5f);
        break;
    case 's':
        camera.moveUpLocal(-0.5f);
        break;
    case 'f':
        if (fullscreen)
        {
            glutReshapeWindow(800, 600);
            glutPositionWindow(100, 100);
        }
        else
        {
            glutFullScreen();
        }
        fullscreen = !fullscreen;
        break;

    case '1':
        camera.yaw(-5.0f); // look left
        break;
    case '2':
        camera.yaw(5.0f); // look right
        break;
    case '3':
        camera.pitch(5.0f); // look up
        break;
    case '4':
        camera.pitch(-5.0f); // look down
        break;
    case '5':
        camera.roll(+5.0f); // tilt cw
        break;
    case '6':
        camera.roll(-5.0f); // tilt ccw
        break;
    }
    glutPostRedisplay();
}
void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        camera.moveForward(0.5f);
        break;
    case GLUT_KEY_DOWN:
        camera.moveForward(-0.5f);
        break;
    case GLUT_KEY_LEFT:
        camera.moveRight(-0.5f);
        break;
    case GLUT_KEY_RIGHT:
        camera.moveRight(0.5f);
        break;
    case GLUT_KEY_PAGE_UP:
        camera.moveUpGlobal(0.5f);
        break;
    case GLUT_KEY_PAGE_DOWN:
        camera.moveUpGlobal(-0.5f);
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (state != GLUT_DOWN)
        return;
    if (button == 3)
    { // scroll up
        camera.moveForward(0.5f);
        glutPostRedisplay();
    }
    else if (button == 4)
    { // scroll down
        camera.moveForward(-0.5f);
        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Task1 & Task3: Camera and Bouncing Ball");
    initScene();
    // Initialize camera outside the cube, looking at its center
    camera.position = Vec3(2.0f * TILES_SIZE, 2.0f * TILES_SIZE, TILES_SIZE * 2.0f);
    camera.target = Vec3(TILES_SIZE/ 2.0f, TILES_SIZE / 2.0f, TILES_SIZE / 2.0f);
    camera.up = Vec3(-1.0f, 1.0f, -1.0f);
    // Initialize ball

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
