#pragma once

#include "../smath.h"
#include "pga.h"

static Vector3 pointToVector(PGA3D p) {
    return Vector3(p[13], p[12], p[11])/p[14];
}
static PGA3D toPoint(Vector3 v) {
    return point(v.x, v.y, v.z);
}

static PGA3D toLine(Vector3 vp, Vector3 vd) {
    PGA3D p = toPoint(vp);
    PGA3D d = toPoint(vd);
    return p & d;
}



static Vector3 rotateVector(Vector3 v, Vector3 centre, Vector3 axis, float rotation) {
    PGA3D c = point(centre.x,centre.y,centre.z);
    PGA3D d = point(axis.x, axis.y, axis.z);
    PGA3D line = c & d;
    PGA3D rot = rotor( rotation, line );
    PGA3D p = point(v.x, v.y, v.z);

    PGA3D rotated_p = rot * p * ~rot;

    return pointToVector(rotated_p);
}

