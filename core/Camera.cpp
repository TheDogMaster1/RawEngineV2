//
// Created by Gebruiker on 9-10-2026.
//

#include "Camera.h"
#include <GLFW/glfw3.h>

namespace Core {
    Camera::Camera(glm::vec3 pos, glm::vec2 aspectRatio) {
        this->cameraPos = pos;
        this->aspectRatio = aspectRatio;
        UpdateVectors();
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(45.0f), float(aspectRatio.x) / float(aspectRatio.y), 0.1f, 100.0f);
    }

    void Camera::MouseInput(GLFWwindow *window, double xpos, double ypos) {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
     UpdateVectors();
    }

    void Camera::UpdateProjection() {
        projection = glm::perspective(glm::radians(45.0f), float(aspectRatio.x) / float(aspectRatio.y), 0.1f, 100.0f);
    }
    void Camera::Input(GLFWwindow *window, float speed) {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += speed * speedMultiplier * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= speed * speedMultiplier * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos += speed * speedMultiplier * cameraRight;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos -= speed * speedMultiplier * cameraRight;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            cameraPos += speed * speedMultiplier * cameraUp;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            cameraPos -= speed * speedMultiplier * cameraUp;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            speedMultiplier = 2;
        else
            speedMultiplier = 1;
    }

    void Camera::ChangeAspectRatio(glm::vec2 aspectRatio) {
        this->aspectRatio = aspectRatio;
       UpdateProjection();
    }

    void Camera::ControlCamera() {
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    glm::vec3 Camera::GetCameraPos() const {
        return this->cameraPos;
    }
    glm::vec3 Camera::GetCameraUp() const {
        return this->cameraUp;
    }
    glm::mat4 Camera::GetView() const {
        return this->view;
    }
    glm::mat4 Camera::GetProjection() const {
        return this->projection;
    }
    glm::mat4 Camera::GetViewProjection() const {
        return this->projection * this->view;
    }

    void Camera::UpdateVectors() {
        cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraDirection.y = sin(glm::radians(pitch));
        cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(cameraDirection);
        cameraRight = glm::normalize(glm::cross(up, cameraFront));
        cameraUp = glm::cross(cameraFront, cameraRight);
    }
}
