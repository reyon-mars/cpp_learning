#include <GL/glut.h>   // simpler header for GLUT
#include <iostream>

// ✅ ADDED
#include <chrono>
#include <cmath>

// ---------------- NEW SMALL ADDITIONS ----------------

// handle window resize
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

// keyboard input (ESC to exit + NEW: toggle rotation)
bool rotate_triangle = false;

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        std::cout << "Exiting...\n";
        exit(0);
    }

    // ✅ ADDED: toggle rotation
    if (key == 'r' || key == 'R') {
        rotate_triangle = !rotate_triangle;
    }
}

// simple animation (refresh loop)
void timer(int) {
    glutPostRedisplay();              // request redraw
    glutTimerFunc(16, timer, 0);      // ~60 FPS
}

// ----------------------------------------------------

// ✅ ADDED: rotation + FPS tracking
float angle = 0.0f;

auto lastTime = std::chrono::high_resolution_clock::now();
int frames = 0;

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    // ✅ ADDED: apply rotation
    glLoadIdentity();
    if (rotate_triangle) {
        angle += 0.5f;
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
    }

    glBegin(GL_TRIANGLES);

        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f, -0.5f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(0.5f, -0.5f);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.0f, 0.5f);

    glEnd();

    // ✅ UPDATED: use double buffering
    glutSwapBuffers();

    // ✅ ADDED: FPS counter
    frames++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    float elapsed = std::chrono::duration<float>(currentTime - lastTime).count();

    if (elapsed >= 1.0f) {
        std::cout << "FPS: " << frames << "\n";
        frames = 0;
        lastTime = currentTime;
    }
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    // ✅ UPDATED: enable double buffering
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("OpenGL Test");

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutDisplayFunc(display);

    // ✅ ADDED
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();

    return 0;
}
