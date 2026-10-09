//
// Created by Gebruiker on 9-10-2026.
//

#include "Camera.h"
#include <GLFW/glfw3.h>

namespace Core {
    Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec2 aspectRatio) {
        this->cameraPos = pos;
        this->cameraTarget = target;
        this->aspectRatio = aspectRatio;
        cameraDirection = glm::normalize(cameraPos - cameraTarget);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        cameraUp = glm::cross(cameraDirection, cameraRight);
        CameraMoveAxis = glm::vec3(0.0f, 0.0f, 0.0f);
        view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        projection = glm::perspective(glm::radians(45.0f), float(aspectRatio.x) / float(aspectRatio.y), 0.1f, 100.0f);

    }

    void Camera::UpdateProjection() {
        projection = glm::perspective(glm::radians(45.0f), float(aspectRatio.x) / float(aspectRatio.y), 0.1f, 100.0f);
    }
    void Camera::Input(GLFWwindow *window, float standardSpeed) {
        standardSpeed = 0.01f * speedMultiplier;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            CameraMoveAxis.z -= standardSpeed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            CameraMoveAxis.z += standardSpeed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            CameraMoveAxis.x -= standardSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            CameraMoveAxis.x += standardSpeed;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            CameraMoveAxis.y += standardSpeed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            CameraMoveAxis.y -= standardSpeed;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            speedMultiplier = 2;
        else
            speedMultiplier = 1;

    }

    void Camera::ChangeAspectRatio(glm::vec2 aspectRatio) {
        this->aspectRatio = aspectRatio;
       UpdateProjection();
    }

    void Camera::TranslateCamera() {
        view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    }

    void Camera::RotateCamera() {
        cameraPos += CameraMoveAxis;
        cameraTarget += CameraMoveAxis;
    }

    glm::vec3 Camera::GetMoveAxis() const {
        return this->CameraMoveAxis;
    }

    glm::vec3 Camera::GetCameraPos() const {
        return this->cameraPos;
    }
    glm::vec3 Camera::GetCameraTarget() const {
        return this->cameraTarget;
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
}