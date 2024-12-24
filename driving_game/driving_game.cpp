#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <iostream>

// Game state
struct GameState {
    float carX = 0.0f;
    float carZ = 0.0f;
    float carRotation = 0.0f;
    float speed = 0.0f;
    int score = 100;
    bool gameOver = false;
} gameState;

// Road parameters
const float ROAD_WIDTH = 10.0f;
const float ROAD_LENGTH = 100.0f;
const float S_CURVE_START = 20.0f;
const float S_CURVE_END = 60.0f;

// Camera parameters
float cameraDistance = 5.0f;
float cameraHeight = 2.0f;

// Function declarations
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void update(int value);
void drawRoad();
void drawCar();
void drawMirrors();
bool checkCollision();
void renderMirrorView(bool isLeft);

// Initialize OpenGL with improved lighting
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    // Set up ambient light
    GLfloat ambient_light[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
    
    // Set up directional light
    GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    // Enable color tracking
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

// Main game loop
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Driving Game");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0); // ~60 FPS
    
    glutMainLoop();
    return 0;
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Set background color to sky blue
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    
    // Set up main camera
    gluLookAt(gameState.carX - cameraDistance * sin(gameState.carRotation),
              cameraHeight,
              gameState.carZ - cameraDistance * cos(gameState.carRotation),
              gameState.carX,
              1.0f,
              gameState.carZ,
              0.0f,
              1.0f,
              0.0f);
    
    // Draw main scene
    drawRoad();
    drawCar();
    drawMirrors();
    
    // Draw score
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(10, 580);
    std::string score = "Score: " + std::to_string(gameState.score);
    for (char c : score) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glutSwapBuffers();
}

// Draw the S-curve road with improved visibility
void drawRoad() {
    // Draw grass
    glColor3f(0.2f, 0.8f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(-50.0f, 0.0f, -10.0f);
    glVertex3f(50.0f, 0.0f, -10.0f);
    glVertex3f(50.0f, 0.0f, ROAD_LENGTH + 10.0f);
    glVertex3f(-50.0f, 0.0f, ROAD_LENGTH + 10.0f);
    glEnd();
    
    // Draw main road surface
    glColor3f(0.35f, 0.35f, 0.35f);  // Lighter gray for better visibility
    glBegin(GL_QUADS);
    for (float z = 0; z < ROAD_LENGTH; z += 1.0f) {
        float xOffset = 0.0f;
        if (z > S_CURVE_START && z < S_CURVE_END) {
            xOffset = 10.0f * sin((z - S_CURVE_START) * M_PI / (S_CURVE_END - S_CURVE_START));
        }
        
        glNormal3f(0.0f, 1.0f, 0.0f);  // Add normal for better lighting
        glVertex3f(xOffset - ROAD_WIDTH/2, 0.01f, z);
        glVertex3f(xOffset + ROAD_WIDTH/2, 0.01f, z);
        glVertex3f(xOffset + ROAD_WIDTH/2, 0.01f, z + 1.0f);
        glVertex3f(xOffset - ROAD_WIDTH/2, 0.01f, z + 1.0f);
    }
    glEnd();
    
    // Draw road boundaries (white lines)
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    for (float z = 0; z < ROAD_LENGTH; z += 1.0f) {
        float xOffset = 0.0f;
        if (z > S_CURVE_START && z < S_CURVE_END) {
            xOffset = 10.0f * sin((z - S_CURVE_START) * M_PI / (S_CURVE_END - S_CURVE_START));
        }
        
        // Left boundary
        glVertex3f(xOffset - ROAD_WIDTH/2 - 0.15f, 0.02f, z);
        glVertex3f(xOffset - ROAD_WIDTH/2 + 0.15f, 0.02f, z);
        glVertex3f(xOffset - ROAD_WIDTH/2 + 0.15f, 0.02f, z + 1.0f);
        glVertex3f(xOffset - ROAD_WIDTH/2 - 0.15f, 0.02f, z + 1.0f);
        
        // Right boundary
        glVertex3f(xOffset + ROAD_WIDTH/2 - 0.15f, 0.02f, z);
        glVertex3f(xOffset + ROAD_WIDTH/2 + 0.15f, 0.02f, z);
        glVertex3f(xOffset + ROAD_WIDTH/2 + 0.15f, 0.02f, z + 1.0f);
        glVertex3f(xOffset + ROAD_WIDTH/2 - 0.15f, 0.02f, z + 1.0f);
    }
    glEnd();
    
    // Draw center line (dashed yellow line)
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    for (float z = 0; z < ROAD_LENGTH; z += 4.0f) {
        float xOffset = 0.0f;
        if (z > S_CURVE_START && z < S_CURVE_END) {
            xOffset = 10.0f * sin((z - S_CURVE_START) * M_PI / (S_CURVE_END - S_CURVE_START));
        }
        
        glVertex3f(xOffset - 0.1f, 0.02f, z);
        glVertex3f(xOffset + 0.1f, 0.02f, z);
        glVertex3f(xOffset + 0.1f, 0.02f, z + 2.0f);
        glVertex3f(xOffset - 0.1f, 0.02f, z + 2.0f);
    }
    glEnd();
}

// Rest of the functions remain the same as in the previous version
void drawMirrorsx() {
    if (gameState.carZ > S_CURVE_START && gameState.carZ < S_CURVE_END) {
        // Left mirror viewport
        glViewport(0, 400, 200, 150);
        renderMirrorView(true);
        
        // Right mirror viewport
        glViewport(600, 400, 200, 150);
        renderMirrorView(false);
        
        // Reset main viewport
        glViewport(0, 0, 800, 600);
    }
}

// [Previous includes and game state code remain the same until drawCar() function]

void drawTire(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Tire rubber (black)
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidTorus(0.1f, 0.2f, 20, 20);
    glPopMatrix();

    // Wheel rim (silver)
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(1.0f, 1.0f, 0.1f);
    glutSolidCube(0.3f);
    glPopMatrix();

    glPopMatrix();
}

void drawWindows() {
    // Windshield
    glColor4f(0.3f, 0.3f, 0.8f, 0.8f);  // Blue tinted glass
    glBegin(GL_QUADS);
    glVertex3f(-0.4f, 0.5f, 0.5f);
    glVertex3f(0.4f, 0.5f, 0.5f);
    glVertex3f(0.4f, 0.9f, 0.2f);
    glVertex3f(-0.4f, 0.9f, 0.2f);
    glEnd();

    // Rear window
    glBegin(GL_QUADS);
    glVertex3f(-0.4f, 0.5f, -0.5f);
    glVertex3f(0.4f, 0.5f, -0.5f);
    glVertex3f(0.4f, 0.9f, -0.2f);
    glVertex3f(-0.4f, 0.9f, -0.2f);
    glEnd();

    // Side windows
    glBegin(GL_QUADS);
    // Left side
    glVertex3f(-0.41f, 0.5f, 0.5f);
    glVertex3f(-0.41f, 0.5f, -0.5f);
    glVertex3f(-0.41f, 0.9f, -0.2f);
    glVertex3f(-0.41f, 0.9f, 0.2f);

    // Right side
    glVertex3f(0.41f, 0.5f, 0.5f);
    glVertex3f(0.41f, 0.5f, -0.5f);
    glVertex3f(0.41f, 0.9f, -0.2f);
    glVertex3f(0.41f, 0.9f, 0.2f);
    glEnd();
}

void drawHeadlights() {
    // Front headlights
    glColor3f(1.0f, 1.0f, 0.9f);
    glPushMatrix();
    glTranslatef(0.3f, 0.3f, 0.95f);
    glScalef(0.2f, 0.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.3f, 0.3f, 0.95f);
    glScalef(0.2f, 0.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Rear lights
    glColor3f(0.8f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.3f, 0.3f, -0.95f);
    glScalef(0.2f, 0.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.3f, 0.3f, -0.95f);
    glScalef(0.2f, 0.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawCarBody() {
    // Main body
    glColor3f(1.0f, 0.0f, 0.0f);  // Red color

    // Lower body
    glPushMatrix();
    glScalef(0.8f, 0.4f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Upper body (roof)
    glPushMatrix();
    glTranslatef(0.0f, 0.4f, 0.0f);
    glScalef(0.7f, 0.3f, 1.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Hood
    glBegin(GL_QUADS);
    glVertex3f(-0.4f, 0.2f, 1.0f);
    glVertex3f(0.4f, 0.2f, 1.0f);
    glVertex3f(0.4f, 0.4f, 0.5f);
    glVertex3f(-0.4f, 0.4f, 0.5f);
    glEnd();

    // Trunk
    glBegin(GL_QUADS);
    glVertex3f(-0.4f, 0.2f, -1.0f);
    glVertex3f(0.4f, 0.2f, -1.0f);
    glVertex3f(0.4f, 0.4f, -0.5f);
    glVertex3f(-0.4f, 0.4f, -0.5f);
    glEnd();
}

void drawGrille() {
    // Front grille
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, 0.99f);
    glScalef(0.4f, 0.2f, 0.01f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Grille details
    glColor3f(0.7f, 0.7f, 0.7f);
    for(float x = -0.15f; x <= 0.15f; x += 0.05f) {
        glBegin(GL_LINES);
        glVertex3f(x, 0.2f, 0.991f);
        glVertex3f(x, 0.4f, 0.991f);
        glEnd();
    }
}


void drawMirrors() {
    if (gameState.carZ > S_CURVE_START && gameState.carZ < S_CURVE_END) {
        // Left mirror viewport
        glViewport(0, 400, 200, 150);
        renderMirrorView(true);

        // Right mirror viewport
        glViewport(600, 400, 200, 150);
        renderMirrorView(false);

        // Reset main viewport
        glViewport(0, 0, 800, 600);
    }
}

void drawMirrorsxxxx() {
    // Side mirrors
    glColor3f(1.0f, 0.0f, 0.0f);  // Match car color

    // Left mirror
    glPushMatrix();
    glTranslatef(-0.45f, 0.5f, 0.3f);
    glScalef(0.1f, 0.15f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Right mirror
    glPushMatrix();
    glTranslatef(0.45f, 0.5f, 0.3f);
    glScalef(0.1f, 0.15f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawCar() {
    glPushMatrix();
    glTranslatef(gameState.carX, 0.4f, gameState.carZ);  // Adjusted height
    glRotatef(gameState.carRotation * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);

    // Draw all car components
    drawCarBody();
    drawWindows();
    drawHeadlights();
    drawGrille();
    //drawMirrors();

    // Draw tires
    drawTire(0.4f, 0.2f, 0.6f);   // Front right
    drawTire(-0.4f, 0.2f, 0.6f);  // Front left
    drawTire(0.4f, 0.2f, -0.6f);  // Rear right
    drawTire(-0.4f, 0.2f, -0.6f); // Rear left

    glPopMatrix();
}

void renderMirrorView(bool isLeft) {
    glPushMatrix();
    glLoadIdentity();
    
    float mirrorAngle = isLeft ? M_PI/4 : -M_PI/4;
    float mirrorX = gameState.carX + 2.0f * sin(gameState.carRotation + mirrorAngle);
    float mirrorZ = gameState.carZ + 2.0f * cos(gameState.carRotation + mirrorAngle);
    
    gluLookAt(mirrorX, 2.0f, mirrorZ,
              gameState.carX, 1.0f, gameState.carZ,
              0.0f, 1.0f, 0.0f);
    
    drawRoad();
    drawCar();
    
    glPopMatrix();
}

bool checkCollision() {
    float xOffset = 0.0f;
    if (gameState.carZ > S_CURVE_START && gameState.carZ < S_CURVE_END) {
        xOffset = 10.0f * sin((gameState.carZ - S_CURVE_START) * M_PI / (S_CURVE_END - S_CURVE_START));
    }
    
    return abs(gameState.carX - xOffset) > ROAD_WIDTH/2;
}

void update(int value) {
    if (!gameState.gameOver) {
        gameState.carZ += gameState.speed * cos(gameState.carRotation);
        gameState.carX += gameState.speed * sin(gameState.carRotation);
        
        if (checkCollision()) {
            gameState.score -= 5;
            if (gameState.score <= 0) {
                gameState.gameOver = true;
                std::cout << "Game Over!" << std::endl;
            }
        }
        
        if (gameState.carZ >= ROAD_LENGTH) {
            gameState.gameOver = true;
            std::cout << "Congratulations! Final score: " << gameState.score << std::endl;
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            gameState.speed = 0.1f; // 0.5f is very fast
            break;
        case 's':
            gameState.speed = -0.5f;
            break;
        case 'a':
            gameState.carRotation += 0.05f;
            break;
        case 'd':
            gameState.carRotation -= 0.05f;
            break;
        case ' ':
            gameState.speed = 0.0f;
            break;
        case 'r':
            gameState = GameState();
            break;
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w/h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

