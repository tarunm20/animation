#include "Skeleton.h"

Skeleton::Skeleton() {}

Skeleton::~Skeleton() {}

void Skeleton::update() {
    this->root->update();
}

bool Skeleton::load(const char* file) {
    Tokenizer token;
    token.Open(file);
    token.FindToken("balljoint");

    this->root = new Joint();
    char temp[256];
    token.GetToken(temp);
    this->root->setName(temp);


    this->root->load(token, this);
    this->addJoint(this->root);

    token.Close();


    return true;

}

std::vector<Joint*> Skeleton::getJoints() {
    return this->joints;
}

Joint* Skeleton::getRoot() {
    return this->root;
}

void Skeleton::addJoint(Joint* j) {
    this->joints.push_back(j);
}

void Skeleton::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    if (this->root != nullptr) {
        printf("GETTING TO SKELETON");
        this->root->draw(viewProjMtx, shader);
    }
}

void Skeleton::hierarchy(Joint* joint, int level) {
    if (!joint) return;

    // Indent according to the level in the hierarchy
    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    // Print joint name
    printf("Joint: %s\n", joint->getName() ? joint->getName() : "Unnamed");

    // Print attributes
    for (int i = 0; i < level; i++) printf("  ");
    printf("  Offset: (%.2f, %.2f, %.2f)\n", joint->getOffset().x, joint->getOffset().y, joint->getOffset().z);

    for (int i = 0; i < level; i++) printf("  ");
    printf("  Pose: (%.2f, %.2f, %.2f)\n", joint->getPose().x, joint->getPose().y, joint->getPose().z);

    for (int i = 0; i < level; i++) printf("  ");
    printf("  DOFs: ");
    printf("DOF X: %.2f ", joint->getDOFX()->getValue());
    printf("DOF Y: %.2f ", joint->getDOFY()->getValue());
    printf("DOF Z: %.2f", joint->getDOFZ()->getValue());
    printf("\n");

    for (int i = 0; i < level; i++) printf("  ");
    printf("  Box Min: (%.2f, %.2f, %.2f)\n", joint->getBoxMin().x, joint->getBoxMin().y, joint->getBoxMin().z);

    for (int i = 0; i < level; i++) printf("  ");
    printf("  Box Max: (%.2f, %.2f, %.2f)\n", joint->getBoxMax().x, joint->getBoxMax().y, joint->getBoxMax().z);

    for (int i = 0; i < level; i++) printf("  ");
    printf("  Local Matrix:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < level + 2; j++) printf("  ");
        printf("%.2f %.2f %.2f %.2f\n", 
            joint->getLocalMat()[i][0], joint->getLocalMat()[i][1], joint->getLocalMat()[i][2], joint->getLocalMat()[i][3]);
    }

    for (int i = 0; i < level; i++) printf("  ");
    printf("  World Matrix:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < level + 2; j++) printf("  ");
        printf("%.2f %.2f %.2f %.2f\n", 
            joint->getWorldMat()[i][0], joint->getWorldMat()[i][1], joint->getWorldMat()[i][2], joint->getWorldMat()[i][3]);
    }

    for (int i = 0; i < level; i++) printf("  ");
    printf("  Parent: %s\n", joint->getParent() ? joint->getParent()->getName() : "None");

    for (int i = 0; i < level; i++) printf("  ");
    printf("  Number of Children: %zu\n", joint->getChildren().size());

    // Recursively print all child joints
    for (Joint* child : joint->getChildren()) {
        hierarchy(child, level + 1);
    }
}