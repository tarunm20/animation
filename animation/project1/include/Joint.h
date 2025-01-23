#pragma once

#include "vector"
#include "core.h"
#include "DOF.h"
#include "Tokenizer.h"
#include "Cube.h"


class Skeleton;

class Joint {
private:
    char* name;

    glm::mat4 localMat;
    glm::mat4 worldMat;

    glm::vec3 offset;
    glm::vec3 pose;
    DOF* dofX;
    DOF* dofY;
    DOF* dofZ;

    glm::vec3 boxmin;
    glm::vec3 boxmax;

    Joint* parent;
    std::vector<Joint*> children;

    Cube* cube;

public:
    Joint();
    ~Joint();

    void setName(char* newName);
    char* getName();

    void setLocalMat(glm::mat4 newMat);
    glm::mat4 getLocalMat();

    void setWorldMat(glm::mat4 newMat);
    glm::mat4 getWorldMat();

    void setOffset(glm::vec3 newOff);
    glm::vec3 getOffset();

    void setPose(glm::vec3 newPose);
    glm::vec3 getPose();

    DOF* getDOFX() const;
    DOF* getDOFY() const;
    DOF* getDOFZ() const;

    void setDOFX(float newDofX);
    void setDOFY(float newDofY);
    void setDOFZ(float newDofZ);

    void setBoxMin(glm::vec3 boxMin);
    glm::vec3 getBoxMin();
    void setBoxMax(glm::vec3 boxMax);
    glm::vec3 getBoxMax();

    void setParent(Joint* parent);
    Joint* getParent();

    void addChild(Joint* child);
    std::vector<Joint*> getChildren();

    glm::mat4 getWorld();

    void computeLocal();
    void computeWorld();
    void update();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);

    bool load(Tokenizer &t, Skeleton* skel);

    void genCube();


};