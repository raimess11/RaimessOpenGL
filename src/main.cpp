#include <glad/glad.h> //place before any depdency used OpenGL
#include <GLFW/glfw3.h>

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include <shader.h>

using namespace std;
using namespace chrono;

const GLuint SCREEN_WIDTH = 640;
const GLuint SCREEN_HEIGHT = 640;

//_setup variable
GLFWwindow *window;
unsigned int VAO;

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

string readFile(string path) {
    std::ifstream in(path);
    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    in.close();
    return contents;
}

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
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    // vertex array object
    // Make sure Bind VAO before VBO
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // Vertex Buffer Object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Shader
    Shader shader0("src/Shader/default.vert", "src/Shader/default.frag");
    Shader shader("src/Shader/default.vert", "src/Shader/default.frag");
    shader.bind();
    cerr << shader.ID;

    // config Vertex Attribut
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    return 1;
}

void _gamelogic() {
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
    // draw
    glDrawArrays(GL_TRIANGLES, 0, 3);
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