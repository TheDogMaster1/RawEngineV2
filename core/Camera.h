//
// Created by Gebruiker on 9-10-2026.
//

#ifndef RAWENGINE_CAMERA_H
#define RAWENGINE_CAMERA_H


#include <glm/gtc/matrix_transform.hpp>
#include <Glfw//glfw3.h>

namespace Core {
    class Camera {
    private:
        glm::vec3 cameraPos;
        glm::vec3 cameraDirection;
        glm::vec3 cameraFront;
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight;
        glm::vec3 cameraUp;

        float pitch = 0.0f;
        float yaw = -90.0f;
        float lastX = 400, lastY = 300;
        float zoom = 45.0f;
        bool firstMouse = true;

        glm::vec2 aspectRatio;
        glm::mat4 view;
        glm::mat4 projection;
        void UpdateProjection();
        void UpdateVectors();
    public:
        float speedMultiplier = 2.0f;

        Camera(glm::vec3 pos, glm::vec2 aspectRatio);

        void MouseInput(GLFWwindow *window, double xpos, double ypos);

        void ScrollInput(GLFWwindow *window, double xoffset, double yoffset);

        void Input(GLFWwindow *window, float standardSpeed);

        void ChangeAspectRatio(glm::vec2 aspectRatio);

        void SetFirstMouse(bool input);

        void ControlCamera();

        glm::vec3 GetCameraPos() const;
        glm::vec3 GetCameraUp() const;
        glm::mat4 GetView() const;
        glm::mat4 GetProjection() const;
        glm::mat4 GetViewProjection() const;
    };
}


#endif //RAWENGINE_CAMERA_H
