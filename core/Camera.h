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
        glm::vec3 cameraTarget;
        glm::vec3 cameraDirection;
        glm::vec3 up;
        glm::vec3 cameraRight;
        glm::vec3 cameraUp;

        glm::vec2 aspectRatio;
        glm::vec3 CameraMoveAxis;
        glm::mat4 view;
        glm::mat4 projection;
        void UpdateProjection();
    public:
        float speedMultiplier = 2.0f;

        Camera(glm::vec3 pos, glm::vec3 target, glm::vec2 aspectRatio);

        void Input(GLFWwindow *window, float standardSpeed);

        void ChangeAspectRatio(glm::vec2 aspectRatio);

        void TranslateCamera();

        void RotateCamera();

        glm::vec3 GetMoveAxis() const;

        glm::vec3 GetCameraPos() const;
        glm::vec3 GetCameraTarget() const;
        glm::vec3 GetCameraUp() const;
        glm::mat4 GetView() const;
        glm::mat4 GetProjection() const;
        glm::mat4 GetViewProjection() const;
    };
}


#endif //RAWENGINE_CAMERA_H
