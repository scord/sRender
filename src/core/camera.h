#pragma once

#include <cmath>
#include "smath.h"

class Camera {
    public:
        Vector3 position;
        int width;
        int height;
        double fovH;
        double fovV;
        Camera(Vector3 pos, int w, int h, double fov);
        Vector3 screenToCamera(const Vector2 screenCoord);
        Ray pixelToRay(const Vector2 screenCoord);
        
};