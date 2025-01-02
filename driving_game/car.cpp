#include <GL/glut.h>
#include <cmath>

// Camera position and orientation
float cameraX = 0.0f, cameraY = 5.0f, cameraZ = 15.0f;
float cameraRotX = 20.0f, cameraRotY = 0.0f;

// Car position and orientation
float carX = 0.0f, carY = 0.0f, carZ = 0.0f;
float carRotation = 0.0f;
float carSpeed = 0.0f;

// Mouse handling
int lastMouseX = 0, lastMouseY = 0;
bool mouseDown = false;

void drawWheel() {
    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.2f);
    
    // Tire
    glutSolidTorus(0.2f, 0.4f, 20, 20);
    
    // Wheel hub - using a scaled cube instead of cylinder
    glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix();
    glScalef(0.4f, 0.1f, 0.4f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPopMatrix();
}

void drawCar() {
    glPushMatrix();
    
    // Position and rotate the car
    glTranslatef(carX, carY, carZ);
    glRotatef(carRotation, 0.0f, 1.0f, 0.0f);
    
    // Main body
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glScalef(3.0f, 1.0f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Hood
    glColor3f(0.8f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.8f, 0.5f, 0.0f);
    glScalef(1.5f, 0.5f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Wheels
    glPushMatrix();
    // Front-right wheel
    glTranslatef(1.0f, -0.5f, 1.0f);
    drawWheel();
    
    // Front-left wheel
    glTranslatef(0.0f, 0.0f, -2.0f);
    drawWheel();
    
    // Back-left wheel
    glTranslatef(-2.0f, 0.0f, 0.0f);
    drawWheel();
    
    // Back-right wheel
    glTranslatef(0.0f, 0.0f, 2.0f);
    drawWheel();
    glPopMatrix();
    
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Set camera position
    gluLookAt(cameraX, cameraY, cameraZ,
              carX, carY, carZ,
              0.0f, 1.0f, 0.0f);
    
    // Draw ground grid
    glColor3f(0.5f, 0.5f, 0.5f);
    for(float i = -20; i <= 20; i += 1.0f) {
        glBegin(GL_LINES);
        glVertex3f(-20.0f, 0.0f, i);
        glVertex3f(20.0f, 0.0f, i);
        glVertex3f(i, 0.0f, -20.0f);
        glVertex3f(i, 0.0f, 20.0f);
        glEnd();
    }
    
    drawCar();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w/h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    float moveX = sin(carRotation * M_PI / 180.0f);
    float moveZ = cos(carRotation * M_PI / 180.0f);
    
    switch(key) {
        case 'w':
            carX += moveX * 0.1f;
            carZ -= moveZ * 0.1f;
            break;
        case 's':
            carX -= moveX * 0.1f;
            carZ += moveZ * 0.1f;
            break;
        case 'a':
            carRotation += 2.0f;
            break;
        case 'd':
            carRotation -= 2.0f;
            break;
        case 27: // ESC key
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if(mouseDown) {
        cameraRotY += (x - lastMouseX) * 0.5f;
        cameraRotX += (y - lastMouseY) * 0.5f;
        
        // Limit vertical rotation
        if(cameraRotX > 89.0f) cameraRotX = 89.0f;
        if(cameraRotX < -89.0f) cameraRotX = -89.0f;
        
        // Update camera position
        float radius = 15.0f;
        cameraX = carX + radius * cos(cameraRotY * M_PI / 180.0f) * cos(cameraRotX * M_PI / 180.0f);
        cameraY = carY + radius * sin(cameraRotX * M_PI / 180.0f);
        cameraZ = carZ + radius * sin(cameraRotY * M_PI / 180.0f) * cos(cameraRotX * M_PI / 180.0f);
        
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON) {
        if(state == GLUT_DOWN) {
            mouseDown = true;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            mouseDown = false;
        }
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Set up lighting
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Car Visualization");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    
    glutMainLoop();
    return 0;
}
