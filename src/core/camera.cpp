#include "camera.h"


Camera::Camera(Vector3 pos, int w, int h, double fov) {
    position = pos;
    width = w;
    height = h;
    fovH = fov;
    fovV = fov*h/w;
}

Vector3 Camera::screenToCamera(const Vector2 screenCoord) {
    return Vector3(
        ((2*(screenCoord.x+0.5) - width)/width)*std::tan(fovH),
        -((2*(screenCoord.y+0.5) - height)/height)*std::tan(fovV),
        -1.0
    );
}

Ray Camera::pixelToRay(const Vector2 screenCoord) {
    return Ray(position, screenToCamera(screenCoord).norm());
}