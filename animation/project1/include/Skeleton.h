#pragma once

#include "Joint.h"
#include <glm/gtx/string_cast.hpp>

class Skeleton {
private:
    Joint* root;
    std::vector<Joint*> joints;

public:
    Skeleton();
    ~Skeleton();

    Joint* getRoot();

    void addJoint(Joint* j);

    std::vector<Joint*> getJoints();

    void update();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);

    bool load(const char* file);

    void hierarchy(Joint* joint, int level);
};