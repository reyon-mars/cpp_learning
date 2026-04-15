#include <GL/glut.h>   // simpler header for GLUT
#include <iostream>

// ---------------- NEW SMALL ADDITIONS ----------------

// handle window resize
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

// keyboard input (ESC to exit)
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        std::cout << "Exiting...\n";
        exit(0);
    }
}

// simple animation (refresh loop)
void timer(int) {
    glutPostRedisplay();              // request redraw
    glutTimerFunc(16, timer, 0);      // ~60 FPS
}

// ----------------------------------------------------

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);

        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f, -0.5f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(0.5f, -0.5f);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.0f, 0.5f);

    glEnd();

    glFlush();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
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
