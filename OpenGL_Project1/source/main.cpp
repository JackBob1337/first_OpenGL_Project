#include<iostream> // Подключаем библиотеку для ввода-вывода
#include<glad/glad.h> // Подключаем библиотеку GLAD для управления OpenGL функциями
#include<GLFW/glfw3.h> // Подключаем библиотеку GLFW для создания окон и работы с OpenGL

// Объявляем функции обратного вызова и обработки ввода
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Константы для размеров окна
const unsigned int SCR_WIDTH = 800; // Ширина окна
const unsigned int SCR_HEIGHT = 600; // Высота окна

// Источник шейдера для вершин (на языке GLSL)
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // Входной атрибут шейдера (позиция вершины)
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // Устанавливаем положение вершины
"}\0";

const char* fragmentShaderSource = "version 330 core\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n0";

int main() {
    // Инициализация библиотеки GLFW
    glfwInit();

    // Устанавливаем параметры версии OpenGL (запрашиваем OpenGL 3.3 Core Profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Создаем окно GLFW
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1; // Если окно не удалось создать, завершаем программу
    }

    glfwMakeContextCurrent(window); // Устанавливаем контекст окна как текущий

    // Инициализируем GLAD для загрузки указателей функций OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check fot shader compiler erros
    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

 
    // Устанавливаем callback для изменения размеров окна
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Определяем массив с вершинами треугольника
    float vertices[] = {
        -0.5f, -0.5f, 0.f, // Левая нижняя вершина
         0.5f, -0.5f, 0.f, // Правая нижняя вершина
         0.f,  0.f,   0.5f // Верхняя вершина
    };

    // Создаем Vertex Buffer Object (VBO) для хранения данных вершин
    unsigned int VBO;
    glGenBuffers(1, &VBO); // Генерируем буфер

    // Передаем данные вершин в видеопамять (но не привязываем буфер)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Главный цикл программы
    while (!glfwWindowShouldClose(window)) {

        // Обрабатываем ввод с клавиатуры
        processInput(window);

        glUseProgram(shaderProgram);

        // Очищаем экран цветом
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Устанавливаем цвет очистки
        glClear(GL_COLOR_BUFFER_BIT); // Очищаем цветовой буфер

        // Меняем буферы и проверяем события
        glfwSwapBuffers(window); // Меняем буферы (Double Buffering)
        glfwPollEvents(); // Обрабатываем события
    }

    // Завершаем работу GLFW
    glfwTerminate();
    return 0;
}

// Callback-функция для изменения размеров окна
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // Настраиваем область просмотра
}

// Обработка ввода с клавиатуры
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // Проверяем нажатие клавиши ESC
        glfwSetWindowShouldClose(window, true); // Закрываем окно
    }
}
