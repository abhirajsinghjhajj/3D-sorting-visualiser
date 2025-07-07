#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<conio.h>
#include<string.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX 10

typedef struct sphere {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat r;
} sphere;

void bubble_sort();
void selection_sort();
void insertion_sort();
void animate_swap();
void shuffle_array();
void sphere_draw(sphere s);
void draw_grid();
void algo_menu(int choice);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void special(int key, int x, int y);
int is_sorted();

int k = 0;
int num_elements = MAX;
int paused = 0;
int original_a[MAX];
sphere s[MAX];
int a[MAX];
int global_i = 0, global_j = 0;
int sorting = 0;
int swap_in_progress = 0;
int swap_i, swap_j;
GLfloat target_x1, target_x2;
GLfloat speed = 15.0;

int bubble_state = 0;
int selection_state = 0;
int selection_min_idx = 0;
int insertion_state = 0;

enum { BUBBLE_SORT, SELECTION_SORT, INSERTION_SORT };
int current_algo = BUBBLE_SORT;

GLfloat camera_angle = 45.0;
GLfloat camera_elevation = 30.0;
GLfloat camera_distance = 800.0;

int mouse_button = -1;
int mouse_x, mouse_y;

GLUquadric* quad;

int show_sorted_message = 0;

void initialise() {
    global_i = global_j = swap_in_progress = 0;
    bubble_state = selection_state = insertion_state = 0;
    selection_min_idx = 0;
    GLfloat max_a = 0.0;
    for (int i = 0; i < num_elements; i++) {
        if (a[i] > max_a) max_a = a[i];
    }
    if (max_a == 0) max_a = 1.0;
    GLfloat scaling_factor = 60.0 / max_a;
    if (num_elements == 1) {
        s[0].x = 0.0;
        s[0].y = 0.0;
        s[0].z = 0.0;
        s[0].r = a[0] * scaling_factor;
    }
    else {
        GLfloat start_x = -400.0;
        GLfloat end_x = 400.0;
        GLfloat spacing = (end_x - start_x) / (num_elements - 1);
        for (int i = 0; i < num_elements; i++) {
            s[i].x = start_x + i * spacing;
            s[i].y = 0.0;
            s[i].z = 0.0;
            s[i].r = a[i] * scaling_factor;
        }
    }
}

void draw_grid() {
    glPushMatrix();
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINES);
    for (GLfloat x = -400.0; x <= 400.0; x += 50.0) {
        glVertex3f(x, -100.0, -400.0);
        glVertex3f(x, -100.0, 400.0);
    }
    for (GLfloat z = -400.0; z <= 400.0; z += 50.0) {
        glVertex3f(-400.0, -100.0, z);
        glVertex3f(400.0, -100.0, z);
    }
    glEnd();
    glPopMatrix();
}

void sphere_draw(sphere s) {
    glPushMatrix();
    glTranslatef(s.x, s.y, s.z);
    gluSphere(quad, s.r, 20, 20);
    glPopMatrix();
}

void bubble_sort() {
    if (sorting == 1 && current_algo == BUBBLE_SORT) {
        if (global_i < num_elements - 1) {
            if (global_j < num_elements - global_i - 1) {
                if (bubble_state == 0) {
                    if (a[global_j] > a[global_j + 1]) {
                        swap_in_progress = 1;
                        swap_i = global_j;
                        swap_j = global_j + 1;
                        target_x1 = s[swap_j].x;
                        target_x2 = s[swap_i].x;
                        bubble_state = 1;
                    }
                    else {
                        global_j++;
                    }
                }
                else if (bubble_state == 1) {
                    if (!swap_in_progress) {
                        bubble_state = 0;
                        global_j++;
                    }
                }
            }
            else {
                global_i++;
                global_j = 0;
            }
        }
        else {
            sorting = 0;
            global_i = 0;
        }
    }
}

void selection_sort() {
    if (sorting == 1 && current_algo == SELECTION_SORT) {
        if (global_i < num_elements - 1) {
            if (selection_state == 0) {
                selection_min_idx = global_i;
                for (int j = global_i + 1; j < num_elements; j++) {
                    if (a[j] < a[selection_min_idx]) selection_min_idx = j;
                }
                selection_state = 1;
            }
            else if (selection_state == 1) {
                if (selection_min_idx != global_i && !swap_in_progress) {
                    swap_in_progress = 1;
                    swap_i = global_i;
                    swap_j = selection_min_idx;
                    target_x1 = s[swap_j].x;
                    target_x2 = s[swap_i].x;
                    selection_state = 2;
                }
                else {
                    selection_state = 3;
                }
            }
            else if (selection_state == 2) {
                if (!swap_in_progress) {
                    selection_state = 3;
                }
            }
            else if (selection_state == 3) {
                global_i++;
                selection_state = 0;
            }
        }
        else {
            sorting = 0;
            global_i = 0;
        }
    }
}

void insertion_sort() {
    static int key = 0;
    static int j = 0;
    if (sorting == 1 && current_algo == INSERTION_SORT) {
        if (global_i < num_elements) {
            if (insertion_state == 0) {
                key = a[global_i];
                j = global_i - 1;
                insertion_state = 1;
            }
            else if (insertion_state == 1) {
                if (j >= 0 && a[j] > key && !swap_in_progress) {
                    swap_in_progress = 1;
                    swap_i = j + 1;
                    swap_j = j;
                    target_x1 = s[swap_j].x;
                    target_x2 = s[swap_i].x;
                    a[j + 1] = a[j];
                    j--;
                    insertion_state = 2;
                }
                else {
                    insertion_state = 3;
                }
            }
            else if (insertion_state == 2) {
                if (!swap_in_progress) {
                    insertion_state = 1;
                }
            }
            else if (insertion_state == 3) {
                a[j + 1] = key;
                global_i++;
                insertion_state = 0;
            }
        }
        else {
            sorting = 0;
            global_i = global_j = 0;
        }
    }
}

void animate_swap() {
    if (swap_in_progress) {
        static GLfloat animation_progress = 0.0;
        animation_progress += 0.1;
        if (s[swap_i].x < target_x1) {
            s[swap_i].x += speed;
            if (s[swap_i].x > target_x1) s[swap_i].x = target_x1;
        }
        else if (s[swap_i].x > target_x1) {
            s[swap_i].x -= speed;
            if (s[swap_i].x < target_x1) s[swap_i].x = target_x1;
        }
        if (s[swap_j].x < target_x2) {
            s[swap_j].x += speed;
            if (s[swap_j].x > target_x2) s[swap_j].x = target_x2;
        }
        else if (s[swap_j].x > target_x2) {
            s[swap_j].x -= speed;
            if (s[swap_j].x < target_x2) s[swap_j].x = target_x2;
        }
        s[swap_i].y = 20.0 * sin(animation_progress);
        s[swap_j].y = 20.0 * sin(animation_progress);
        if (fabs(s[swap_i].x - target_x1) < 0.01 && fabs(s[swap_j].x - target_x2) < 0.01) {
            int temp = a[swap_i];
            a[swap_i] = a[swap_j];
            a[swap_j] = temp;
            sphere temp_s = s[swap_i];
            s[swap_i] = s[swap_j];
            s[swap_j] = temp_s;
            s[swap_i].y = s[swap_j].y = 0.0;
            swap_in_progress = 0;
            animation_progress = 0.0;
        }
    }
}

void shuffle_array() {
    for (int i = num_elements - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
}

void algo_menu(int choice) {
    current_algo = choice;
    sorting = 0;
    global_i = global_j = swap_in_progress = 0;
    bubble_state = selection_state = insertion_state = 0;
    selection_min_idx = 0;
    for (int i = 0; i < num_elements; i++) {
        a[i] = original_a[i];
        s[i].y = 0.0;
    }
    initialise();
    glutPostRedisplay();
}

void create_menu() {
    glutCreateMenu(algo_menu);
    glutAddMenuEntry("Bubble Sort", BUBBLE_SORT);
    glutAddMenuEntry("Selection Sort", SELECTION_SORT);
    glutAddMenuEntry("Insertion Sort", INSERTION_SORT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouse_button = GLUT_LEFT_BUTTON;
            mouse_x = x;
            mouse_y = y;
        }
        else {
            mouse_button = -1;
        }
    }
}

void motion(int x, int y) {
    if (mouse_button == GLUT_LEFT_BUTTON) {
        int dx = x - mouse_x;
        int dy = y - mouse_y;
        camera_angle += dx * 0.2;
        camera_elevation += dy * -0.2;
        if (camera_elevation < 5.0) camera_elevation = 5.0;
        if (camera_elevation > 85.0) camera_elevation = 85.0;
        mouse_x = x;
        mouse_y = y;
        printf("Camera: angle=%f, elevation=%f\n", camera_angle, camera_elevation);
        glutPostRedisplay();
    }
}

void special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        camera_angle -= 5.0;
        break;
    case GLUT_KEY_RIGHT:
        camera_angle += 5.0;
        break;
    case GLUT_KEY_UP:
        camera_elevation += 5.0;
        if (camera_elevation > 85.0) camera_elevation = 85.0;
        break;
    case GLUT_KEY_DOWN:
        camera_elevation -= 5.0;
        if (camera_elevation < 5.0) camera_elevation = 5.0;
        break;
    }
    printf("Camera: angle=%f, elevation=%f\n", camera_angle, camera_elevation);
    glutPostRedisplay();
}

void display(void) {
    printf("Display called, k=%d\n", k);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLfloat rad_angle = camera_angle * M_PI / 180.0;
    GLfloat rad_elevation = camera_elevation * M_PI / 180.0;
    gluLookAt(
        camera_distance * cos(rad_elevation) * sin(rad_angle),
        camera_distance * sin(rad_elevation),
        camera_distance * cos(rad_elevation) * cos(rad_angle),
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );
    glEnable(GL_DEPTH_TEST);
    draw_grid();
    glColor3f(1.0, 1.0, 1.0);
    if (k == 0) {
        printf("Rendering text: Press Enter to start\n");
        glRasterPos3f(-450, 360, -600);
        const char* credits1 = "Made By-:";
        for (size_t i = 0; i < strlen(credits1); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, credits1[i]);
        }
        glRasterPos3f(-450, 320, -600);
        const char* credits2 = "Abhiraj Singh Jhajj, 102217094";
        for (size_t i = 0; i < strlen(credits2); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, credits2[i]);
        }
        glRasterPos3f(-450, 280, -600);
        const char* credits3 = "Nishtha Singla, 102217205";
        for (size_t i = 0; i < strlen(credits3); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, credits3[i]);
        }
        glRasterPos3f(-450, 240, -600);
        const char* credits4 = "Submitted to-:";
        for (size_t i = 0; i < strlen(credits4); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, credits4[i]);
        }
        glRasterPos3f(-450, 200, -600);
        const char* credits5 = "Dr.Jyoti";
        for (size_t i = 0; i < strlen(credits5); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, credits5[i]);
        }
        glRasterPos3f(-100, 0, -600);
        const char* msg = "Press Enter to start";
        for (size_t i = 0; i < strlen(msg); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
        }
    }
    else {
        printf("Rendering text: %s\n", sorting ? "Sorting..." : "Press s to sort, r to randomize, o to restore, Esc to quit");
        glRasterPos3f(-450, 250, -600);
        const char* msg = sorting ? "Sorting..." : "Press s to sort, r to shuffle, o to restore, Esc to quit";
        for (size_t i = 0; i < strlen(msg); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, msg[i]);
        }
        if (show_sorted_message > 0) {
            glRasterPos3f(-450, 230, -600);
            const char* sorted_msg = "Array is already sorted";
            for (size_t i = 0; i < strlen(sorted_msg); i++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, sorted_msg[i]);
            }
            show_sorted_message--;
        }
        for (int i = 0; i < num_elements; i++) {
            if (swap_in_progress && (i == swap_i || i == swap_j)) {
                glColor3f(1.0, 0.0, 0.0);
                s[i].r *= 1.2;
                sphere_draw(s[i]);
                s[i].r /= 1.2;
            }
            else {
                glColor3f(0.0, 0.0, 1.0);
                sphere_draw(s[i]);
            }
            char value_str[16];
            snprintf(value_str, sizeof(value_str), "%d", a[i]);
            glColor3f(1.0, 1.0, 1.0);
            glRasterPos3f(s[i].x - 10.0, s[i].y + s[i].r + 20.0, s[i].z);
            for (size_t j = 0; j < strlen(value_str); j++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, value_str[j]);
            }
        }
        if (k == 1 && sorting == 1 && !paused) {
            switch (current_algo) {
            case BUBBLE_SORT: bubble_sort(); break;
            case SELECTION_SORT: selection_sort(); break;
            case INSERTION_SORT: insertion_sort(); break;
            }
        }
        if (swap_in_progress && !paused) {
            animate_swap();
        }
    }
    glutSwapBuffers();
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 13) k = 1;
    if (k == 1) {
        switch (key) {
        case 27: exit(0);
        case 's':
            if (is_sorted()) {
                show_sorted_message = 100;
            }
            else {
                sorting = 1;
                swap_in_progress = 0;
                global_i = global_j = 0;
                bubble_state = selection_state = insertion_state = 0;
                selection_min_idx = 0;
            }
            break;
        case 'r':
            shuffle_array();
            initialise();
            break;
        case 'o':
            for (int i = 0; i < num_elements; i++) {
                a[i] = original_a[i];
                s[i].y = 0.0;
            }
            initialise();
            break;
        case 'p': if (sorting == 1) paused = 1; break;
        case 'c': if (sorting == 1) paused = 0; break;
        }
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 3000.0);
    glMatrixMode(GL_MODELVIEW);
}

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_pos[] = { 0.0, 500.0, 500.0, 1.0 };
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
}

int is_sorted() {
    for (int i = 0; i < num_elements - 1; i++) {
        if (a[i] > a[i + 1]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char** argv) {
    srand(time(NULL));
    printf("Enter the number of elements (1 to %d): ", MAX);
    if (scanf("%d", &num_elements) != 1) {
        printf("Invalid input. Using default %d.\n", MAX);
        num_elements = MAX;
    }
    if (num_elements < 1 || num_elements > MAX) {
        printf("Invalid number. Using default %d.\n", MAX);
        num_elements = MAX;
    }
    printf("Enter %d integers:\n", num_elements);
    for (int i = 0; i < num_elements; i++) {
        if (scanf("%d", &a[i]) != 1) {
            printf("Invalid input for element %d. Using default value 1.\n", i);
            a[i] = 1;
        }
        original_a[i] = a[i];
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(900, 600);
    glutCreateWindow("3D Sorting Visualizer");
    init();
    glutTimerFunc(0, timer, 0);
    initialise();
    create_menu();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutReshapeFunc(reshape);
    glutMainLoop();
    gluDeleteQuadric(quad);
    return 0;
}