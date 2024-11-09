#define STB_IMAGE_IMPLEMENTATION
#include <GL/glut.h>
#include "stb_image.h"
#include <cstdio>
//#include "SDL2/SDL.h"

float angleX = 0.0f;
float angleY = 0.0f;
float angleZ = 0.0f;
float X = 0.5f;

bool rotating = false;

//SDL_AudioDeviceID audioDevice;

// Lighting settings
GLfloat ambientLight[] = { 0.3f, 0.5f, 0.9f, 1.0f };
GLfloat diffuseLight[] = { 1.1f, 1.1f, 1, 1 };
GLfloat specularLight[] = { 0.7f, 0.7f, 0.7f, 0.1f };
GLfloat lightPos[] = { 0.5f, 0.0f, 1.5f, 0.5f };
GLfloat shininess = 100.0f;

// Material settings
GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat black[] = { 0.3f, 0.3f, 0.3f, 1.0f };

GLuint backgroundTexture;

// Function to render text at a given position
void renderText(float x, float y, const char* text) {
    glColor3f(1.0f, 1.0f, 1.0f); // Set text color to white
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

// Setup lighting based on type
void setupLighting(int type) {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    switch (type) {
        case 1:  // Ambient only
            glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
            glLightfv(GL_LIGHT0, GL_SPECULAR, black);
            break;
        case 2:  // Diffuse only
            glLightfv(GL_LIGHT0, GL_AMBIENT, black);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
            glLightfv(GL_LIGHT0, GL_SPECULAR, black);
            break;
        case 3:  // Specular only
            glLightfv(GL_LIGHT0, GL_AMBIENT, black);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
            glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
            break;
        case 4:  // Combined (Ambient + Diffuse + Specular)
            glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
            glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
            break;
        default:
            break;
    }

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glShadeModel(GL_SMOOTH);
}

// Setup material properties
void setupMaterial() {
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void loadBackgroundImage(const char* filePath) {
    int width, height, channels;
    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 0);
    if (data) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glGenTextures(1, &backgroundTexture);
        glBindTexture(GL_TEXTURE_2D, backgroundTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    } else {
        printf("Failed to load texture\n");
    }
}

// Render background
void renderBackground() {
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glEnable(GL_TEXTURE_2D);

    // Switch to orthographic projection for the background
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

// Draw a sphere
void drawSphere() {
    glColor3f(0.8f, 0.8f, 0.8f);
    glutSolidSphere(0.4f, 50, 50);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render Background in 2D Orthographic Projection without lighting
    glDisable(GL_LIGHTING);  // Disable lighting for the background
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    renderBackground();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_LIGHTING);  // Re-enable lighting for 3D objects

    // Clear Depth Buffer before drawing spheres to avoid layering issues
    glClear(GL_DEPTH_BUFFER_BIT);

    // Set Perspective Projection for Spheres with lighting
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.5, 10.0,   // Camera position (eye)
              0.0, 0.0, 0.0,    // Look-at point
              0.0, 1.0, 0.0);   // Up vector



    // Draw Earth Sphere
    glPushMatrix();
    glTranslatef(-2.0f, 0.0f, -10.0f);  // Position Earth
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glScalef(8, 8, 8);         // Scale Earth to be larger than the Moon
    setupLighting(4);                   // Combined lighting
    setupMaterial();
    drawSphere();
    glPopMatrix();

    // Disable lighting and set text color to white
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);  // Set text color to white
    renderText(-1.3f, -1.9f, "Earth");  // Position Earth label under the Earth sphere

    // Draw Moon Sphere
    glPushMatrix();
    glTranslatef(3, 3, -10.0f);   // Position Moon
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glScalef(3, 3, 3);         // Scale Moon to be smaller than the Earth
    setupLighting(2);                   // Combined lighting
    setupMaterial();
    drawSphere();
    glPopMatrix();

    // Disable lighting and set text color to white
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);  // Set text color to white
    renderText(1.2f, 0.6f, "Moon");    // Position Moon label under the Moon sphere

    glutSwapBuffers();
}



// Handle window resizing
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}



void autorotate(int value) {
    if (rotating) {
        angleX += 0.5f;
        angleY += 0.5f;
        angleZ += 0.5f;

        if (angleX > 360) angleX -= 360;
        if (angleY > 360) angleY -= 360;
        if (angleZ > 360) angleZ -= 360;

        glutPostRedisplay();
        glutTimerFunc(16, autorotate, 0);
    }
    else{
        glutPostRedisplay();
    }
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            angleX -= 5.0f;
            break;
        case GLUT_KEY_DOWN:
            angleX += 5.0f;
            break;
        case GLUT_KEY_LEFT:
            angleY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            angleY += 5.0f;
            break;
    }
    glutPostRedisplay();
}

void handleKey(unsigned char key, int x, int y) {
    switch (key) {
        case 's':
        case 'S':
            angleX = 0.0f;
            angleY = 0.0f;
            angleZ = 0.0f;
            rotating = false;  // Stop continuous rotation
            break;

        case 'a':  // Start continuous auto-rotation
        case 'A':
            rotating = true;
            glutTimerFunc(0, autorotate, 0);
            break;

        case 'o': // open face
        case 'O':
            X += 0.5f;
            break;

        case 'c':  // close face
        case 'C':
            X -= 0.5f;
            break;

        case 'Z':
        case 'z':
            angleZ += 5.0f;
            break;

        case 'X':
        case 'x':
            angleZ -= 5.0f;
            break;

        case 27:  // ESC to exit
            exit(0);
    }
    glutPostRedisplay();
}

//// Function to initialize SDL audio
//void initAudio(const char* audioFilePath) {
//    SDL_Init(SDL_INIT_AUDIO);
//
//    // Load the audio file
//    SDL_AudioSpec wavSpec;
//    Uint32 wavLength;
//    Uint8* wavBuffer;
//
//    if (SDL_LoadWAV(audioFilePath, &wavSpec, &wavBuffer, &wavLength) == NULL) {
//        printf("Could not open WAV file: %s\n", SDL_GetError());
//        return;
//    }
//
//    // Open an audio device
//    audioDevice = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
//    SDL_QueueAudio(audioDevice, wavBuffer, wavLength); // Queue the audio
//    SDL_PauseAudioDevice(audioDevice, 0); // Start playback
//
//    SDL_FreeWAV(wavBuffer); // Free the WAV buffer
//}

int main(int argc, char** argv) {
    // Initialize SDL for audio playback
//    SDL_Init(SDL_INIT_AUDIO);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Universe");

    glEnable(GL_DEPTH_TEST);

//    initAudio("C:/Users/Nermeen Kamal/OneDrive/Desktop/presentation/SpaceShuttleIntCo PE1075907.wav");

    loadBackgroundImage("C:/Users/Nermeen Kamal/CLionProjects/OpenGLProject/uni.png");

    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(handleKey);
    glutReshapeFunc(reshape);

    glutMainLoop();

    glDeleteTextures(1, &backgroundTexture);

//    SDL_CloseAudioDevice(audioDevice);
//    SDL_Quit();

    return 0;
}

