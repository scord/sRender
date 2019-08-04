#include <vector>
#include "geometry.h"
#include <algorithm>
#include <memory>

class KDTreeNode {
public:
    KDTreeNode() {};
    KDTreeNode(BoundingBox box, std::vector<Shape*> geometry, int splitAxis);
    BoundingBox aabb;
    KDTreeNode* left;
    KDTreeNode* right;
    std::vector<Shape*> geometry;
    bool isLeaf;
    double splitPoint;
    int splitAxis;
    double intersect(Ray ray, Vector3& near, Vector3 &far, Shape* &intersectedGeometry) ;
    double intersect(Ray ray, Shape* &intersectedGeometry) ;
    std::vector<Shape*> findGeometry(Ray ray);
};
