#include"classes.hpp"

#define FLOOR_SIZE 1.0
#define TILES_NUM 10.0
Camera camera;
Floor flr(10,.9);
int recursion_level;
int image_width;
std::vector<Object*> objects;
std::vector<PointLight> pointLights;
std::vector<SpotLight> spotLights;

bool fullscreen = false;

void initScene()
{
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    // Initialize camera outside the cube, looking at its center
    camera.position = Vec3(2.0f * FLOOR_SIZE, 2.0f * FLOOR_SIZE, FLOOR_SIZE * 2.0f);
    camera.target = Vec3(0,0,0);
    camera.up = Vec3(0, 0, 1.0f);
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.apply();
    flr.draw();

    


    glutSwapBuffers();
}

void reshape(int w, int h)
{
    if (h == 0)
        h = 1;
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
    case 'r':
        camera.position = Vec3(2.0f * FLOOR_SIZE, 2.0f * FLOOR_SIZE, FLOOR_SIZE * 2.0f);
        camera.target = Vec3(0, 0, 0);
        camera.up = Vec3(0, 0, 1.0f);
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
void loadData() {
    std::ifstream sceneFile("scene.txt");
    if (!sceneFile.is_open()) {
        std::cerr << "ERROR: Could not open 'scene.txt'. Make sure it is in the correct directory." << std::endl;
        return;
    }

    // Read global scene settings
    sceneFile >> recursion_level >> image_width;

    // Read all geometric objects
    int num_objects;
    sceneFile >> num_objects;
    for (int i = 0; i < num_objects; ++i) {
        std::string object_type;
        sceneFile >> object_type;

        Object* newObject = nullptr;

        if (object_type == "sphere") {
            Vec3 center;
            double radius;
            sceneFile >> center.x >> center.y >> center.z >> radius;
            newObject = new Sphere(center, radius);

        } else if (object_type == "triangle") {
            Vec3 p1, p2, p3;
            sceneFile >> p1.x >> p1.y >> p1.z;
            sceneFile >> p2.x >> p2.y >> p2.z;
            sceneFile >> p3.x >> p3.y >> p3.z;
            newObject = new Triangle(p1, p2, p3);

        } else if (object_type == "general") {
            double coeffs[10];
            Vec3 ref_point;
            double l, w, h;
            for(int j = 0; j < 10; ++j) sceneFile >> coeffs[j];
            sceneFile >> ref_point.x >> ref_point.y >> ref_point.z;
            sceneFile >> l >> w >> h;
            newObject = new General(coeffs, ref_point, l, w, h);
        }

        // If an object was created, read its material properties
        if (newObject) {
            double r, g, b, amb, diff, spec, rec;
            int shine;
            sceneFile >> r >> g >> b;
            newObject->setColor(r, g, b);
            sceneFile >> amb >> diff >> spec >> rec;
            newObject->setCoEfficients(amb, diff, spec, rec);
            sceneFile >> shine;
            newObject->setShine(shine);
            objects.push_back(newObject);
        }
    }

    // Read point light sources
    int num_point_lights;
    sceneFile >> num_point_lights;
    for (int i = 0; i < num_point_lights; ++i) {
        Vec3 pos;
        double r, g, b;
        sceneFile >> pos.x >> pos.y >> pos.z;
        sceneFile >> r >> g >> b;
        pointLights.push_back(PointLight(pos, r, g, b));
    }

    // Read spot light sources
    int num_spot_lights;
    sceneFile >> num_spot_lights;
    for (int i = 0; i < num_spot_lights; ++i) {
        Vec3 pos, dir;
        double r, g, b, angle;
        sceneFile >> pos.x >> pos.y >> pos.z;
        sceneFile >> r >> g >> b;
        sceneFile >> dir.x >> dir.y >> dir.z;
        sceneFile >> angle;
        PointLight pl(pos, r, g, b);
        spotLights.push_back(SpotLight(pl, dir, angle));
    }

    sceneFile.close();

    // Manually add the floor object as per assignment specification
    Object* floor = new Floor(1000, 20); // Floor width 1000, tile width 20
    floor->setCoEfficients(0.4, 0.2, 0.2, 0.2); // Set some default coefficients
    floor->setShine(5); // Set some default shine
    objects.push_back(floor);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Task1 & Task3: Camera and Bouncing Ball");
    initScene();
    
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
