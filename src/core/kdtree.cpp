#include "kdtree.h"

double median(std::vector<double> values) {
    int n = values.size() / 2;
    std::nth_element(values.begin(),values.begin()+n,values.end());
    double a = values[n];
    double b = values[n];

    if (values.size() % 2 == 0) {
        std::nth_element(values.begin(),values.begin()+n+1,values.end());
        double b = values[n+1];
        return (a+b)/2;
    } else {
        return a;
    }
}

KDTreeNode::KDTreeNode(BoundingBox box, std::vector<Shape*> geometry, int depth) : aabb(box){

    isLeaf = false;
    if (depth >= 20 || geometry.size() <= 1) {
        isLeaf = true;
        for (auto shape : geometry) {
            this->geometry.push_back((shape));
        }
        return;
    }

    std::vector<Shape*> leftGeometry;
    std::vector<Shape*> rightGeometry;

    double splitPoint = 0;
    Vector3 leftMax;
    Vector3 rightMin;

    int splitAxis = depth % 3;

    if (splitAxis == 0) {
        std::vector<double> xs;
        for (auto& g : geometry) {
            xs.push_back(g->aabb.min.x);
            xs.push_back(g->aabb.max.x);
        }
        splitPoint = median(xs);
        
        leftMax = Vector3(splitPoint,box.max.y,box.max.z);
        rightMin = Vector3(splitPoint,box.min.y, box.min.z);
    } else if (splitAxis == 1) {
        std::vector<double> ys;
        for (auto& g : geometry) {
            ys.push_back(g->aabb.min.y);
            ys.push_back(g->aabb.max.y);
        }
        splitPoint = median(ys);

        leftMax = Vector3(box.max.x,splitPoint,box.max.z);
        rightMin = Vector3(box.min.x,splitPoint, box.min.z);
    } else if (splitAxis == 2) {
        std::vector<double> zs;
        for (auto& g : geometry) {
            zs.push_back(g->aabb.min.z);
            zs.push_back(g->aabb.max.z);
        }
        splitPoint = median(zs);

        leftMax = Vector3(box.max.x,box.max.y,splitPoint);
        rightMin = Vector3(box.min.x, box.min.y,splitPoint);
    }

    BoundingBox leftBox(box.min, leftMax);
    BoundingBox rightBox(rightMin, box.max);

    for (auto shape : geometry) {
        if (leftBox.overlaps(shape->aabb)) {
            leftGeometry.push_back(shape);
        }
        if (rightBox.overlaps(shape->aabb)) {
            rightGeometry.push_back(shape);
        }
    }

    left = new KDTreeNode(leftBox, leftGeometry, (depth+1));
    right = new KDTreeNode(rightBox, rightGeometry, (depth+1));

}


std::vector<Shape*> KDTreeNode::findGeometry(Ray ray) {

    if (isLeaf) {
        return geometry;
    } else if (aabb.intersects(ray)) {
        std::vector<Shape*> leftGeo = left->findGeometry(ray);
        std::vector<Shape*> rightGeo = right->findGeometry(ray);
        leftGeo.insert(leftGeo.end(), rightGeo.begin(), rightGeo.end());

        return leftGeo;
    } else {
        return std::vector<Shape*>();
    }
}
