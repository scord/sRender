#include "kdtree.h"
#include <assert.h>
#include <iostream>

double median(std::vector<double> values) {

    auto minmax = std::minmax_element(values.begin(), values.end());
    return (*minmax.second+*minmax.first)/2;

}

KDTreeNode::KDTreeNode(BoundingBox box, std::vector<Shape*> geometry, int depth) : aabb(box){

    isLeaf = false;
    if (depth >=24|| geometry.size() <= 1) {
        isLeaf = true;
        this->geometry = geometry;
        return;
    }

    std::vector<Shape*> leftGeometry;
    std::vector<Shape*> rightGeometry;

    splitPoint = 0;
    Vector3 leftMax;
    Vector3 rightMin;

    splitAxis = depth % 3;

    if (splitAxis == 0) {
        splitPoint = (box.min.x + box.max.x)/2.0;
        leftMax = Vector3(splitPoint,box.max.y,box.max.z);
        rightMin = Vector3(splitPoint,box.min.y, box.min.z);
    } else if (splitAxis == 1) {
        splitPoint = (box.min.y + box.max.y)/2.0;
        leftMax = Vector3(box.max.x,splitPoint,box.max.z);
        rightMin = Vector3(box.min.x,splitPoint, box.min.z);
    } else if (splitAxis == 2) {
        splitPoint = (box.min.z + box.max.z)/2.0;
        leftMax = Vector3(box.max.x, box.max.y, splitPoint);
        rightMin = Vector3(box.min.x, box.min.y, splitPoint);
    }

    BoundingBox leftBox(box.min, leftMax);
    BoundingBox rightBox(rightMin, box.max);

    for (auto* shape : geometry) {
        if (rightBox.overlaps(shape->aabb))
            rightGeometry.push_back(shape);
        if (leftBox.overlaps(shape->aabb))
            leftGeometry.push_back(shape);
    }

    left = new KDTreeNode(leftBox, leftGeometry, (depth+1));
    right = new KDTreeNode(rightBox, rightGeometry, (depth+1));

}

double KDTreeNode::intersect(Ray ray, Vector3& near, Vector3& far, Shape* & intersectedGeometry) {

    if (isLeaf) {
 

        double minT = 99999;

        for (auto shape : geometry) {
            double t = shape->Intersect(ray);
            if (t < minT && t > EPS) {
                minT = t;
                intersectedGeometry = shape;
            }
        }

        if (minT < 99999)
            return minT;
        else 
            return -1;
    } else {

        double n, f;

        if (splitAxis == 0) {
            n = near.x;
            f = far.x;
        } else if (splitAxis == 1) {
            n = near.y;
            f = far.y;
        } else if (splitAxis == 2) {
            n = near.z;
            f = far.z;
        }

        double t = 0;

        if (n <= splitPoint) {
            // left
            if (f > splitPoint) {
                Vector3 newFar = near + (far - near) * ((splitPoint - n) / (f - n));
                t = left->intersect(ray, near, newFar, intersectedGeometry);
                Shape* intersectedGeometry2;
                double t2 = right->intersect(ray, newFar, far, intersectedGeometry2);
                if (t <= 0 || (t2 > EPS && t2 < t)) {
                    t = t2;
                    intersectedGeometry = intersectedGeometry2;
                }
            } else
                t = left->intersect(ray, near, far, intersectedGeometry);

        } else {
            // right
            if (f <= splitPoint) {
                Vector3 newFar = near + (far - near) * ((splitPoint - n) / (f - n));
                t = right->intersect(ray, near, newFar, intersectedGeometry);
            
                Shape* intersectedGeometry2;
                double t2 = left->intersect(ray, newFar, far,  intersectedGeometry2);
                
                if (t <= 0 || (t2 > EPS && t2 < t)) {
                    t = t2;
                    intersectedGeometry = intersectedGeometry2;
                }
            } else
                t = right->intersect(ray, near, far, intersectedGeometry);

        }

        return t;
    }
    
}

double KDTreeNode::intersect(Ray ray, Shape* & intersectedGeometry) {
    std::vector<Vector3> intersections = aabb.intersect(ray);

    if (intersections.size() <= 1) {
        return -1;
    }
  //  std::cout << intersections.size() << "\n";
   // assert(intersections.size() == 2);
    Vector3 near = intersections[0];
    Vector3 far = intersections[1];

    return intersect(ray, near, far, intersectedGeometry);
}


std::vector<Shape*> KDTreeNode::findGeometry(Ray ray) {

    

    if (isLeaf) {
        std::vector<Vector3> intersections = aabb.intersect(ray);

        if (intersections.size() == 0)
            return std::vector<Shape*>();
        else
            return geometry;
    } else {
        std::vector<Vector3> intersections = aabb.intersect(ray);

        if (intersections.size() == 0)
            return std::vector<Shape*>();

        std::vector<double> splitCheck0 {intersections[0].x, intersections[0].y, intersections[0].z};
        std::vector<double> splitCheck1 {intersections[1].x, intersections[1].y, intersections[1].z};

        std::vector<Shape*> leftGeo;
        std::vector<Shape*> rightGeo;

        if (splitCheck0[splitAxis] <= splitPoint || splitCheck1[splitPoint] <= splitPoint) {
            leftGeo = left->findGeometry(ray);
        }

        if (splitCheck0[splitAxis] >= splitPoint || splitCheck1[splitPoint] >= splitPoint) {
            rightGeo = right->findGeometry(ray);
        }


        leftGeo.reserve(leftGeo.size() + rightGeo.size());
        leftGeo.insert(leftGeo.end(), rightGeo.begin(), rightGeo.end());

        return leftGeo;
    } 
}
