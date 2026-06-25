#include <GL/glut.h>
#include <iostream>
#include <chrono>

namespace {

bool  rotate_triangle = false;
float angle           = 0.0f;
int   frame_count     = 0;

auto last_time = std::chrono::steady_clock::now();

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int /*x*/, int /*y*/) {
    switch (key) {
        case 27:
            std::cout << "Exiting...\n";
            std::exit(0);
        case 'r':
        case 'R':
            rotate_triangle = !rotate_triangle;
            std::cout << "Rotation: " << (rotate_triangle ? "ON" : "OFF") << '\n';
            break;
        default:
            break;
    }
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (rotate_triangle) {
        angle = std::fmod(angle + 0.5f, 360.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
    }

    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(-0.5f, -0.5f);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex2f( 0.5f, -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex2f( 0.0f,  0.5f);
    glEnd();

    glutSwapBuffers();

    ++frame_count;
    const auto now    = std::chrono::steady_clock::now();
    const float delta = std::chrono::duration<float>(now - last_time).count();
    if (delta >= 1.0f) {
        std::cout << "FPS: " << frame_count << '\n';
        frame_count = 0;
        last_time   = now;
    }
}

} // anonymous namespace

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Triangle");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
