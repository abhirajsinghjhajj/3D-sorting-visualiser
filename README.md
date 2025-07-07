# 🧠 3D Sorting Algorithm Visualizer

> A C++ OpenGL/GLUT-based interactive visualizer for sorting algorithms in a 3D space.

---

## 📌 Project Overview

The **3D Sorting Algorithm Visualizer** is an OpenGL + GLUT based desktop application designed to help students and learners understand sorting algorithms through interactive 3D visualization. It demonstrates:

- 🫧 Bubble Sort  
- 📥 Insertion Sort  
- 📌 Selection Sort  

Each element is visualized as a **3D sphere**, whose size corresponds to the value it represents. The algorithm animations are rendered step-by-step, with **smooth transitions**, color changes, and real-time camera control for immersive understanding.

---

## ✨ Features

- 🔴 Highlighted swaps with red color
- 🎥 Real-time 3D animation with smooth transitions
- 🧭 Interactive camera controls (mouse drag + arrow keys)
- 🕹️ Right-click menu for algorithm selection
- 🎛️ Keyboard input for sorting control
- 🟫 Grid-based XZ plane for spatial context

---

## 🕹️ Controls

| Key            | Action                                 |
|----------------|----------------------------------------|
| `Enter`        | Start visualization                    |
| `s`            | Start sorting                          |
| `r`            | Randomize array                        |
| `o`            | Restore original array                 |
| `p`            | Pause sorting                          |
| `c`            | Continue/resume sorting                |
| `Esc`          | Exit the application                   |
| Arrow Keys     | Adjust camera view                     |
| Mouse (Drag)   | Rotate camera (angle and elevation)    |
| Right-click    | Open GLUT menu (choose algorithm)      |

---

## 🔧 Technical Implementation

- Language: **C++**
- Libraries: **OpenGL**, **GLUT**
- Rendering:
  - **Double buffering** for smooth frame updates
  - **Depth testing** for 3D occlusion
  - **Ambient and diffuse lighting** for clarity
  - **Timer function** for ~60 FPS redraw (`glutPostRedisplay` every 16ms)

- Sphere positioning:
  - X-axis spaced
  - Y-axis for animations
  - Radius scaled based on max array value

- Camera control using `gluLookAt`, adjustable via:
  - Mouse drag (angle & elevation)
  - Arrow keys

---

## 🧩 User-Defined Functions

| Function          | Description |
|-------------------|-------------|
| `initialise`      | Initializes sphere positions and sizes |
| `draw_grid`       | Draws reference grid on XZ plane |
| `sphere_draw`     | Renders a sphere using `gluSphere` |
| `bubble_sort`     | Bubble Sort with visual state machine |
| `selection_sort`  | Selection Sort with animated swaps |
| `insertion_sort`  | Insertion Sort with shifting animation |
| `animate_swap`    | Animates x-axis movement & y-axis bouncing |
| `randomise_array` | Randomizes array with values 1–10 |
| `algo_menu`       | Switches algorithm via GLUT menu |
| `create_menu`     | Initializes right-click menu |
| `mouse`           | Handles mouse press for camera |
| `motion`          | Updates camera angle during drag |
| `special`         | Handles arrow key camera control |
| `display`         | Main rendering function |
| `timer`           | Forces refresh every 16ms |
| `keyboard`        | Processes all keyboard inputs |
| `reshape`         | Handles window resize |
| `init`            | Sets OpenGL lighting, depth, color |

---

## 📚 Educational Utility

This project bridges theoretical algorithm concepts with **real-time 3D visual feedback**, targeting:

- 👨‍🎓 Computer Science students
- 🧑‍🏫 Educators teaching algorithms/OpenGL
- 👨‍💻 Enthusiasts learning graphics or animations

---

## 🚀 Future Enhancements

- Add Quick Sort, Merge Sort
- Comparison highlighting with color codes
- Sound effects for swaps
- Larger dataset visualization
- Improved GUI overlays or annotations

---

## 🛠️ Requirements

- C++ compiler (e.g. g++, clang++)
- OpenGL & GLUT (e.g. FreeGLUT or GLUT on Windows/Linux/macOS)

---

## 🧠 Author

**Abhiraj Singh Jhajj**  
📫 [LinkedIn](https://www.linkedin.com/in/abhirajsingh-jhajj-88a647286/)  
📧 [Gmail](mailto:abhirajjhajj04@gmail.com)

---

> ⭐️ Star this repository if you found it helpful!  
> 🎓 Happy Sorting & Visualizing!

