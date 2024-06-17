#include <glad/glad.h> //place before any depdency used OpenGL
#include <GLFW/glfw3.h>

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

using namespace std;
using namespace chrono;

const GLuint SCREEN_WIDTH = 640;
const GLuint SCREEN_HEIGHT = 640;

//_setup variable
GLFWwindow *window;

//_mainloop variable
time_point
    _process_start_time = high_resolution_clock::now(),
    _process_end_time = high_resolution_clock::now(),
    _lates_process_time = high_resolution_clock::now(),
    _lates_render_time = high_resolution_clock::now();

milliseconds _process_duration, elapsed_time;
float fps = 60;

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};
unsigned int VBO;

//_terminate variable

int _setup() {

    auto output = freopen("output.txt", "w", stdout);
    auto error = freopen("error.txt", "w", stderr);
    cout << "file has ben created" << endl;
    

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raimess OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Open the input file named "input.txt"
    ifstream inputFile("src/Shader/default.vert");

    // Check if the file is successfully opened
    if (!inputFile.is_open()) {
        cerr << "Error opening the file!" << endl;
        return 1;
    }

    string content;

    string line; // Declare a string variable to store each
                 // line of the file
    // standard output stream
    while (getline(inputFile, line)) {
        // cout << line << endl; // Print the current line
        content += line + "\n";
    }

    const char *vertexShaderSource = content.c_str();
    cout << vertexShaderSource;

    // Close the file
    inputFile.close();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    return 1;
}

void _gamelogic() {
    this_thread::sleep_for(2ms);
}

void _render() {
    // fps limit
    if (duration_cast<milliseconds>(_process_start_time.time_since_epoch() - _lates_render_time.time_since_epoch()) < 1000ms / fps) {
        cout << "render skiped at " << high_resolution_clock::now().time_since_epoch().count() << endl;
        return;
    }
    _lates_render_time = high_resolution_clock::now();
    cout << "rendering at " << high_resolution_clock::now().time_since_epoch().count() << endl;

    // do render here
    this_thread::sleep_for(2ms);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
}

void _eventHandler() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    glfwPollEvents();
}

void _mainloop() {
    while (!glfwWindowShouldClose(window)) {
        _process_start_time = high_resolution_clock::now();
        elapsed_time = duration_cast<milliseconds>(_process_start_time.time_since_epoch() - _lates_process_time.time_since_epoch());
        _lates_process_time = _process_start_time;
        // if (elapsed_time < 1000ms / fps) {
        //     auto sleep_time = (1000ms / fps) - elapsed_time;
        //     this_thread::sleep_for(sleep_time);
        //     _process_start_time = high_resolution_clock::now();
        //     cout << "sleep: " << sleep_time.count() << " ms at - " << high_resolution_clock::now().time_since_epoch().count() << endl;
        // } else
        //     cout << "sleep: " << 0 << " ms at - " << high_resolution_clock::now().time_since_epoch().count() << endl;
        // call the process here
        this_thread::sleep_for(2ms);
        _render();
        _gamelogic();
        _eventHandler();

        _process_end_time = high_resolution_clock::now();
        _process_duration = duration_cast<milliseconds>(_process_end_time - _process_start_time);
        cout << "process time: " << _process_duration.count() << " ms at - " << high_resolution_clock::now().time_since_epoch().count() << endl;
    }
}

void _terminate() {
    fclose(stdout);
    fclose(stderr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main() {
    if (!_setup())
        return -1;
    _mainloop();
    _terminate();

    return 0;
}