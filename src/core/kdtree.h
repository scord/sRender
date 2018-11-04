#include <vector>
#include "geometry.h"
#include <algorithm>
#include <memory>

class KDTreeNode {
public:
    KDTreeNode(BoundingBox box, std::vector<Shape*> geometry, int splitAxis);
    BoundingBox aabb;
    KDTreeNode* left;
    KDTreeNode* right;
    std::vector<Shape*> geometry;
    bool isLeaf;

    std::vector<Shape*> findGeometry(Ray ray);
};
