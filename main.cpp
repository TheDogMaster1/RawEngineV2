#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

#include "imgui_internal.h"
#include "core/mesh.h"
#include "core/assimpLoader.h"
#include "core/texture.h"
#include "core/Camera.h"
#include "core/LightObj.h"
//#define MAC_CLION
#define VSTUDIO

#ifdef MAC_CLION
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

#ifdef VSTUDIO
// Note: install imgui with:
//     ./vcpkg.exe install imgui[glfw-binding,opengl3-binding]
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif

int g_width = 800;
int g_height = 600;

float cameraSpeed = 1.0f;

double currentTime = glfwGetTime();
double finishFrameTime = 0.0;
float deltaTime = 0.0f;
bool holdingRMB = false;

GLenum drawMode = GL_TRIANGLES;

Core::Camera camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec2(g_width, g_height));

Core::LightObj light1(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-2.0f, 0.0f, 0.0f), 0.1f);

Core::LightObj light2(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(2.0f, 0.0f, 0.0f), 0.1f);

std::vector<Core::LightObj*> lights;

void CameraCallBack(GLFWwindow *window, double xpos, double ypos) {
    if (holdingRMB) {
        camera.MouseInput(window, xpos, ypos);
    }
    else
        camera.SetFirstMouse(true);
}

void MouseButtonCallBack(GLFWwindow *window, int button, int action, int mods);

void ScrollCallBack(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ScrollInput(window, xoffset, yoffset);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   camera.Input(window, cameraSpeed * deltaTime);

}

void framebufferSizeCallback(GLFWwindow *window,
                             int width, int height) {
    g_width = width;
    g_height = height;
    glViewport(0, 0, width, height);
    camera.ChangeAspectRatio(glm::vec2(g_width, g_height));
}

std::string readFileToString(const std::string &filePath) {
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        printf("Could not open file: %s\n", filePath.c_str());
        return "";
    }
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

GLuint generateShader(const std::string &shaderPath, GLuint shaderType) {
    printf("Loading shader: %s\n", shaderPath.c_str());
    const std::string shaderText = readFileToString(shaderPath);
    const GLuint shader = glCreateShader(shaderType);
    const char *s_str = shaderText.c_str();
    glShaderSource(shader, 1, &s_str, nullptr);
    glCompileShader(shader);
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Error! Shader issue [%s]: %s\n", shaderPath.c_str(), infoLog);
    }
    return shader;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(g_width, g_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, CameraCallBack);
    glfwSetMouseButtonCallback(window, MouseButtonCallBack);
    glfwSetScrollCallback(window, ScrollCallBack);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    //Setup platforms
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 400");

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const GLuint modelVertexShader = generateShader("shaders/modelVertex.vs", GL_VERTEX_SHADER);
    const GLuint fragmentShader = generateShader("shaders/fragment.fs", GL_FRAGMENT_SHADER);
    const GLuint textureShader = generateShader("shaders/texture.fs", GL_FRAGMENT_SHADER);

    int success;
    char infoLog[512];
    const unsigned int modelShaderProgram = glCreateProgram();
    glAttachShader(modelShaderProgram, modelVertexShader);
    glAttachShader(modelShaderProgram, fragmentShader);
    glLinkProgram(modelShaderProgram);
    glGetProgramiv(modelShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(modelShaderProgram, 512, NULL, infoLog);
        printf("Error! Making Shader Program: %s\n", infoLog);
    }
    const unsigned int textureShaderProgram = glCreateProgram();
    glAttachShader(textureShaderProgram, modelVertexShader);
    glAttachShader(textureShaderProgram, textureShader);
    glLinkProgram(textureShaderProgram);
    glGetProgramiv(textureShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(textureShaderProgram, 512, NULL, infoLog);
        printf("Error! Making Shader Program: %s\n", infoLog);
    }

    glDeleteShader(modelVertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(textureShader);

    core::Mesh quad = core::Mesh::generateQuad();
    core::Model quadModel({quad});

    quadModel.translate(glm::vec3(0,0,-2.5));
    quadModel.scale(glm::vec3(5, 5, 1));


    core::Model suzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");

    glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r,
                 clearColor.g, clearColor.b, clearColor.a);

    glm::vec4 objectColor(1, 0, 0, 1);

    lights.push_back(&light1);
    lights.push_back(&light2);

    GLint objectColor0Uniform = glGetUniformLocation(modelShaderProgram, "uniformColor");
    GLint objectTextUniform = glGetUniformLocation(modelShaderProgram, "tex");
    GLint objectLightAmountUniform = glGetUniformLocation(modelShaderProgram, "lightsAmount");
    glUseProgram(modelShaderProgram);
    glUniform1i(objectLightAmountUniform, lights.size());
    glBindVertexArray(0);

    for (int i = 0; i < static_cast<int>(lights.size()); i++) {
        std::string lightUniformString = "lights[" + std::to_string(i) + "].";
        std::cout << (lightUniformString+ "Color").c_str() << std::endl;
        glUseProgram(modelShaderProgram);
        GLint lightColor0Uniform = glGetUniformLocation(modelShaderProgram, (lightUniformString + "color").c_str());
        GLint lightPosition0Uniform = glGetUniformLocation(modelShaderProgram, (lightUniformString + "position").c_str());
        GLint lightAI0Uniform = glGetUniformLocation(modelShaderProgram, (lightUniformString + "ambientIntensity").c_str());
        glUniform4fv(lightColor0Uniform, 1, &lights[i]->GetColor()[0]);
        glUniform3fv(lightPosition0Uniform, 1, &lights[i]->GetPosition()[0]);
        glUniform1f(lightAI0Uniform, lights[i]->GetAmbientIntensity());
        glBindVertexArray(0);
    }

    glm::vec3 lightPos = light1.GetPosition();
    glm::vec4 lightColor = light1.GetColor();

    // suzanne.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(45.0f));
    glm::vec3 suzanneScale(1, 1, 1);
    glm::vec3 suzannePosition(0, 0, 0);
    float suzanneRotation = 0.0f;


    GLint mvpMatrixUniform = glGetUniformLocation(modelShaderProgram, "mvpMatrix");
    GLint mMatrixUniform = glGetUniformLocation(modelShaderProgram, "mMatrix");
    GLint textureModelUniform = glGetUniformLocation(textureShaderProgram, "mvpMatrix");
    GLint textureUniform = glGetUniformLocation(textureShaderProgram, "text");

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Raw Engine v2");
        ImGui::Text("Hello :)");
        if (ImGui::Button("Switch Draw Mode")) {
            if (drawMode == GL_TRIANGLES) {
                drawMode = GL_LINES;
            }
            else {
                drawMode = GL_TRIANGLES;
            }
        }
        if (ImGui::InputFloat3("LightPos", &lightPos[0])) {
            light1.SetPos(lightPos);
        }
        if (ImGui::ColorPicker4("LightColor", &lightColor[0])) {
            light1.SetColor(lightColor);
        }
        ImGui::InputFloat("Speed", &cameraSpeed);
        ImGui::End();
        ImGui::Begin("SuzanneInput");
        ImGui::InputFloat3("SuzanneScale", &suzanneScale[0]);
        ImGui::InputFloat3("SuzannePosition", &suzannePosition[0]);
        ImGui::InputFloat("SuzanneRotation", &suzanneRotation);
        // ImGui::ColorPicker4("objectColor", &objectColor[0]);
        ImGui::End();


        processInput(window);
        suzanne.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(suzanneRotation));
        suzanne.translate(suzannePosition);
        suzanne.scale(suzanneScale);

        camera.ControlCamera();
        glUseProgram(textureShaderProgram);
        glUniformMatrix4fv(textureModelUniform, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjection() * quadModel.getModelMatrix()));
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(textureUniform, 0);
        glBindTexture(GL_TEXTURE_2D, cmgtGatoTexture.getId());
        quadModel.render(drawMode);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);

        glUseProgram(modelShaderProgram);
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjection() * suzanne.getModelMatrix()));
        glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, glm::value_ptr(suzanne.getModelMatrix()));

        // glUniform4fv(objectColor0Uniform, 1, &objectColor[0]);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(objectTextUniform, 0);
        glBindTexture(GL_TEXTURE_2D, cmgtGatoTexture.getId());
        suzanne.render(drawMode);
        glBindVertexArray(0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
        finishFrameTime = glfwGetTime();
        deltaTime = static_cast<float>(finishFrameTime - currentTime);
        currentTime = finishFrameTime;
    }

    glDeleteProgram(modelShaderProgram);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

    void MouseButtonCallBack(GLFWwindow *window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            holdingRMB = true;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
            holdingRMB = false;
        }
    }


