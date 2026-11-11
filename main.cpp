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
#include "core/Transform.h"
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

core::LightObj light1(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-2.0f, 0.0f, 0.0f), 1, 256, 0);

core::LightObj light2(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(2.0f, 0.0f, 0.0f), 1, 256, 0);

core::LightObj light3(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 2.0f, 0.0f), 1, 256, 0);

float ambientIntensity = 0.1f;

std::vector<core::LightObj*> lights;

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


void LoadTextures(const unsigned int shaderProgram, core::Texture &difTexture, core::Texture &normalTexture ) {
    GLint difTextureUniform = glGetUniformLocation(shaderProgram, "difTex");
    GLint normalTextureUniform = glGetUniformLocation(shaderProgram, "normalTex");
    // glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(difTextureUniform, 0);
    glBindTexture(GL_TEXTURE_2D, difTexture.getId());
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(normalTextureUniform, 1);
    glBindTexture(GL_TEXTURE_2D, normalTexture.getId());
    // glBindVertexArray(0);
}

/**
 * make sure to put this between ImGui::Begin and End.
 */
void ChangeObjectTransforms(core::Model* object,const std::string name) {
    if (ImGui::CollapsingHeader((name + "Input").c_str())) {
        if (ImGui::InputFloat3((name + "Scale").c_str(), &object->transform.scale[0])) {
            object->transform.Scale(object->transform.scale);
        }
        if (ImGui::InputFloat3((name + "Position").c_str(), &object->transform.position[0])) {
            object->transform.Translate(object->transform.position);
        }
        if (ImGui::InputFloat3((name + "Rotation").c_str(), &object->transform.rotation[0])) {
            object->transform.Rotate(object->transform.rotation);
        }
    }
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
    const GLuint lightObjShader = generateShader("shaders/lightObj.frag", GL_FRAGMENT_SHADER);

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
    const unsigned int lightObjShaderProgram = glCreateProgram();
    glAttachShader(lightObjShaderProgram, modelVertexShader);
    glAttachShader(lightObjShaderProgram, lightObjShader);
    glLinkProgram(lightObjShaderProgram);
    glGetProgramiv(lightObjShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(lightObjShaderProgram, 512, NULL, infoLog);
        printf("Error! Making Shader Program: %s\n", infoLog);
    }

    glDeleteShader(modelVertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(lightObjShader);

    core::Mesh quad = core::Mesh::generateQuad();
    core::Model quadModel({quad});

    quadModel.transform.Translate(glm::vec3(0,0,-2.5));
    quadModel.transform.Rotate(glm::vec3(0, 0, 180));
    quadModel.transform.Scale(glm::vec3(5, 5, 1));


    core::Model suzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    core::Model sphere = core::AssimpLoader::loadModel("models/BlenderSphere.obj");
    core::Model lightSphere = core::AssimpLoader::loadModel("models/BlenderSphere.obj");
    core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");
    core::Texture donQuixoteTexture("Textures/LCB_Sinner_Don_Quixote_Profile.png");
    core::Texture donQuixoteNormalTexture("Textures/LCB_Sinner_Don_Quixote_Profile_normal.png");
    core::Texture brickDifTexture("Textures/red_brick_diff_4k.png");
    core::Texture brickNormalTexture("Textures/red_brick_nor_gl_4k.png");
    core::Texture hexDifTexture("textures/hexDiff.png");
    core::Texture hexNormTexture("textures/hexNorm.png");

    glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r,
                 clearColor.g, clearColor.b, clearColor.a);

    lights.push_back(&light1);
    lights.push_back(&light2);
    lights.push_back(&light3);

    GLint objectLightAmountUniform = glGetUniformLocation(modelShaderProgram, "lightsAmount");
    glUseProgram(modelShaderProgram);
    glUniform1i(objectLightAmountUniform, lights.size());
    glBindVertexArray(0);

    GLint mvpMatrixUniform = glGetUniformLocation(modelShaderProgram, "mvpMatrix");
    GLint mMatrixUniform = glGetUniformLocation(modelShaderProgram, "mMatrix");
    GLint lightMvpMatrixUniform = glGetUniformLocation(lightObjShaderProgram, "mvpMatrix");
    GLint lightMMatrixUniform = glGetUniformLocation(lightObjShaderProgram, "mMatrix");
    GLint camPosUniform = glGetUniformLocation(modelShaderProgram, "camPos");

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Raw Engine v2");
        if (ImGui::CollapsingHeader("test")) {
            ImGui::Text("Hello :)");
        }
        if (ImGui::Button("Switch Draw Mode")) {
            if (drawMode == GL_TRIANGLES) {
                drawMode = GL_LINES;
            }
            else {
                drawMode = GL_TRIANGLES;
            }
        }
        // if (ImGui::InputFloat3("LightPos", &lightPos[0])) {
        //     light1.SetPos(lightPos);
        // }
        // if (ImGui::ColorPicker4("LightColor", &lightColor[0])) {
        //     light1.SetColor(lightColor);
        // }
        ImGui::InputFloat("Speed", &cameraSpeed, 1);
        ImGui::SliderFloat("Ambient Intensity", &ambientIntensity, 0, 1);
        ImGui::BeginTabBar("Stuff");
        if (ImGui::BeginTabItem("Objects")) {
            ChangeObjectTransforms(&sphere, "Sphere");
            ChangeObjectTransforms(&suzanne, "Suzanne");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
            for (int i = 0; i < static_cast<int>(lights.size()); i++) {
                ImGui::PushID(i);

                glm::vec4 lightColor = lights[i]->GetColor();
                float lightIntensity = lights[i]->GetIntensity();
                float lightSpecInt = lights[i]->GetSpecularIntensity();
                float lightType = lights[i]->GetType();

                std::string lightGuiName = "Light" + std::to_string(i+1);
                if (ImGui::CollapsingHeader(lightGuiName.c_str())) {
                    if (lightType == 0) {
                        // glm::vec3 lightPos = lights[i]->transform.GetPosition();
                        if (ImGui::InputFloat3("LightPos", &lights[i]->transform.position[0])) {
                            lights[i]->transform.Translate(lights[i]->transform.position);
                        }
                    }
                    else if (lightType == 1) {
                        // glm::vec3 lightDir = lights[i]->transform.GetRotation();
                        if (ImGui::InputFloat3("LightDir", &lights[i]->transform.rotation[0])) {
                            lights[i]->transform.Rotate(lights[i]->transform.rotation);
                        }
                    }
                    if (ImGui::InputFloat("Intensity", &lightIntensity, 1)) {
                        lights[i]->SetIntensity(lightIntensity);
                    }
                    if (ImGui::InputFloat("Specular Intensity", &lightSpecInt)) {
                        lights[i]->SetSpecularIntensity(lightSpecInt);
                    }
                    if (ImGui::ColorPicker4("LightColor", &lightColor[0])) {
                        lights[i]->SetColor(lightColor);
                    }
                    if ( ImGui::InputFloat("LightType", &lightType)) {
                        lights[i]->SetType(lightType);
                    }
                }

                std::string lightUniformString = "lights[" + std::to_string(i) + "].";
                glUseProgram(modelShaderProgram);
                GLint lightColorUniform = glGetUniformLocation(modelShaderProgram, (lightUniformString + "color").c_str());
                GLint lightPositionUniform = glGetUniformLocation(modelShaderProgram, (lightUniformString + "position").c_str());
                GLint lightIntensityUniform = glGetUniformLocation(modelShaderProgram, (lightUniformString + "intensity").c_str());
                GLint lightSpecIntensityUniform = glGetUniformLocation(modelShaderProgram, (lightUniformString + "specularIntensity").c_str());
                GLint lightTypeUniform = glGetUniformLocation(modelShaderProgram, (lightUniformString + "type").c_str());
                glUniform1f(lightTypeUniform, lightType);
                glUniform4fv(lightColorUniform, 1, &lights[i]->GetColor()[0]);
                if (lightType == 0) {
                    glUniform3fv(lightPositionUniform, 1, &lights[i]->transform.position[0]);
                }
                else if (lightType == 1) {
                    glUniform3fv(lightPositionUniform, 1, &lights[i]->transform.GetFowDir()[0]);
                }
                glUniform1f(lightIntensityUniform, lights[i]->GetIntensity());
                glUniform1f(lightSpecIntensityUniform, lights[i]->GetSpecularIntensity());
                glBindVertexArray(0);
                glUseProgram(lightObjShaderProgram);

                GLint lightDebugColUniform = glGetUniformLocation(lightObjShaderProgram, "lightColor");
                lightSphere.transform.Translate(lights[i]->transform.position);
                lightSphere.transform.Scale(glm::vec3(lights[i]->GetIntensity() * 0.2f));
                glUniformMatrix4fv(lightMvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjection() * lightSphere.transform.getModelMatrix()));
                glUniformMatrix4fv(lightMMatrixUniform, 1, GL_FALSE, glm::value_ptr(lightSphere.transform.getModelMatrix()));
                glUniform4fv(lightDebugColUniform, 1, &lights[i]->GetColor()[0]);
                if (lightType != 1) {
                    lightSphere.render(GL_LINES);
                }
                else {
                    suzanne.render(GL_LINES);
                }
                glBindVertexArray(0);
                ImGui::PopID();
        }


        ImGui::End();



        processInput(window);

        camera.ControlCamera();
        glUseProgram(modelShaderProgram);
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjection() * quadModel.transform.getModelMatrix()));
        glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, glm::value_ptr(quadModel.transform.getModelMatrix()));
        // glActiveTexture(GL_TEXTURE0);
        // glUniform1i(objectTextUniform, 0);
        // glBindTexture(GL_TEXTURE_2D, cmgtGatoTexture.getId());
        LoadTextures(modelShaderProgram,donQuixoteTexture, brickNormalTexture);
        quadModel.render(drawMode);
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjection() * sphere.transform.getModelMatrix()));
        glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, glm::value_ptr(sphere.transform.getModelMatrix()));
        LoadTextures(modelShaderProgram,donQuixoteTexture, hexNormTexture);
        sphere.render(drawMode);
        glBindVertexArray(0);

        glUseProgram(modelShaderProgram);
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjection() * suzanne.transform.getModelMatrix()));
        glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, glm::value_ptr(suzanne.transform.getModelMatrix()));
        glUniform3fv(camPosUniform, 1, &camera.GetCameraPos()[0]);
        // glUniform4fv(objectColor0Uniform, 1, &objectColor[0]);
        // glActiveTexture(GL_TEXTURE0);
        // glUniform1i(objectTextUniform, 0);
        // glBindTexture(GL_TEXTURE_2D, donQuixoteTexture.getId());
        LoadTextures(modelShaderProgram, brickDifTexture, brickNormalTexture);

        GLint lightAI0Uniform = glGetUniformLocation(modelShaderProgram, "ambientIntensity");
        glUniform1f(lightAI0Uniform, ambientIntensity);
        // suzanne.render(drawMode);
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
    glDeleteProgram(lightObjShaderProgram);
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


