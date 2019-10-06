#include "camera.h"


Camera::Camera(vec3 pos, int w, int h, double fov) {
    position = pos;
    width = w;
    height = h;
    fovH = fov;
    fovV = fov*h/w;
}

vec3 Camera::screenToCamera(const vec2 screenCoord) {
    return vec3(
        2*(screenCoord.x/width-0.5)*std::tan(fovH),
        -2*(screenCoord.y/height-0.5)*std::tan(fovV),
        -1.0
    );
}


Ray Camera::pixelToRay(const vec2 screenCoord) {
    return Ray(position, screenToCamera(screenCoord).norm());
}



Ray Camera::pixelToRay(const vec2 screenCoord, vec2 centre, double focalLength, double focalDistance) {
    vec3 imagePlanePos = position + screenToCamera(screenCoord).norm()*focalLength;
    return Ray(imagePlanePos, ((position + screenToCamera(centre).norm()*focalDistance) - imagePlanePos).norm());
}