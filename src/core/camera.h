#pragma once

#include <cmath>
#include "smath.h"
#include "geometry.h"

class Camera {
    public:
        vec3 position;
        int width;
        int height;
        double fovH;
        double fovV;
        Camera(vec3 pos, int w, int h, double fov);
        vec3 screenToCamera(const vec2 screenCoord);
        Ray pixelToRay(const vec2 screenCoord);
        Ray pixelToRay(const vec2 screenCoord, vec2 centre, double focalLength, double focalDistance);
};