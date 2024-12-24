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

// Initialize OpenGL
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Set up light
    GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
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

// Draw the S-curve road
void drawRoad() {
    glColor3f(0.5f, 0.5f, 0.5f);
    
    glBegin(GL_QUADS);
    for (float z = 0; z < ROAD_LENGTH; z += 1.0f) {
        float xOffset = 0.0f;
        if (z > S_CURVE_START && z < S_CURVE_END) {
            xOffset = 10.0f * sin((z - S_CURVE_START) * M_PI / (S_CURVE_END - S_CURVE_START));
        }
        
        glVertex3f(xOffset - ROAD_WIDTH/2, 0.0f, z);
        glVertex3f(xOffset + ROAD_WIDTH/2, 0.0f, z);
        glVertex3f(xOffset + ROAD_WIDTH/2, 0.0f, z + 1.0f);
        glVertex3f(xOffset - ROAD_WIDTH/2, 0.0f, z + 1.0f);
    }
    glEnd();
    
    // Draw road boundaries
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    for (float z = 0; z < ROAD_LENGTH; z += 1.0f) {
        float xOffset = 0.0f;
        if (z > S_CURVE_START && z < S_CURVE_END) {
            xOffset = 10.0f * sin((z - S_CURVE_START) * M_PI / (S_CURVE_END - S_CURVE_START));
        }
        
        glVertex3f(xOffset - ROAD_WIDTH/2, 0.1f, z);
        glVertex3f(xOffset - ROAD_WIDTH/2, 0.1f, z + 1.0f);
        
        glVertex3f(xOffset + ROAD_WIDTH/2, 0.1f, z);
        glVertex3f(xOffset + ROAD_WIDTH/2, 0.1f, z + 1.0f);
    }
    glEnd();
}

// Draw the player's car
void drawCar() {
    glPushMatrix();
    glTranslatef(gameState.carX, 1.0f, gameState.carZ);
    glRotatef(gameState.carRotation * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);
    
    // Simple car model
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidCube(1.0f);
    
    glPopMatrix();
}

// Draw rearview mirrors
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

// Render view for each mirror
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

// Check if car is off the road
bool checkCollision() {
    float xOffset = 0.0f;
    if (gameState.carZ > S_CURVE_START && gameState.carZ < S_CURVE_END) {
        xOffset = 10.0f * sin((gameState.carZ - S_CURVE_START) * M_PI / (S_CURVE_END - S_CURVE_START));
    }
    
    return abs(gameState.carX - xOffset) > ROAD_WIDTH/2;
}

// Update game state
void update(int value) {
    if (!gameState.gameOver) {
        // Update car position
        gameState.carZ += gameState.speed * cos(gameState.carRotation);
        gameState.carX += gameState.speed * sin(gameState.carRotation);
        
        // Check for collision
        if (checkCollision()) {
            gameState.score -= 5;
            if (gameState.score <= 0) {
                gameState.gameOver = true;
                std::cout << "Game Over!" << std::endl;
            }
        }
        
        // Check for finish
        if (gameState.carZ >= ROAD_LENGTH) {
            gameState.gameOver = true;
            std::cout << "Congratulations! Final score: " << gameState.score << std::endl;
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            gameState.speed = 0.5f;
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
            // Reset game
            gameState = GameState();
            break;
    }
}

// Handle window reshaping
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w/h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

