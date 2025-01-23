#include "Joint.h"
#include "Skeleton.h"
#include "Cube.h"

// HELPERS
void printMat4(const glm::mat4& mat) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%.2f ", mat[i][j]);
        }
        printf("\n");
    }
}


Joint::Joint() {
    this->offset = glm::vec3(0.0f);
    this->localMat = glm::mat4(1.0f);
    this->worldMat = glm::mat4(1.0f);
    this->boxmin = glm::vec3(-0.1f);
    this->boxmax = glm::vec3(0.1f);
    this->pose = glm::vec3(0.0f);
    this->dofX = new DOF(Axis::X);
    this->dofY = new DOF(Axis::Y);
    this->dofZ = new DOF(Axis::Z);

}

Joint::~Joint() {

}

void Joint::setName(char* newName) {
    name = new char[strlen(newName) + 1];
    strcpy(name, newName);
}

char* Joint::getName() {
    return this->name;
}

void Joint::setLocalMat(glm::mat4 newMat) {
    this->localMat = newMat;
}

glm::mat4 Joint::getLocalMat() {
    return this->localMat;
}

void Joint::setWorldMat(glm::mat4 newMat) {
    this->worldMat = newMat;
}

glm::mat4 Joint::getWorldMat() {
    return this->worldMat;
}

void Joint::setOffset(glm::vec3 newOff) {
    this->offset = newOff;
}

glm::vec3 Joint::getOffset() {
    return this->offset;
}

void Joint::setPose(glm::vec3 newPose) {
    this->pose = newPose;
}

glm::vec3 Joint::getPose() {
    return this->pose;
}

DOF* Joint::getDOFX() const {
    return dofX;
}

DOF* Joint::getDOFY() const {
    return dofY;
}

DOF* Joint::getDOFZ() const {
    return dofZ;
}

void Joint::setDOFX(float newDofX) {
    dofX->SetValue(newDofX);
}

void Joint::setDOFY(float newDofY) {
    dofY->SetValue(newDofY);
}

void Joint::setDOFZ(float newDofZ) {
    dofZ->SetValue(newDofZ);
}

void Joint::setBoxMin(glm::vec3 boxMin) {
    this->boxmin = boxMin;
}

glm::vec3 Joint::getBoxMin() {
    return this->boxmin;
}

void Joint::setBoxMax(glm::vec3 boxMax) {
    this->boxmax = boxMax;
}

glm::vec3 Joint::getBoxMax() {
    return this->boxmax;
}

void Joint::setParent(Joint* parent) {
    this->parent = parent;
}

Joint* Joint::getParent() {
    return this->parent;
}

void Joint::addChild(Joint* child) {
    this->children.push_back(child);
}

std::vector<Joint*> Joint::getChildren() {
    return this->children;
}

glm::mat4 Joint::getWorld() {
    return this->worldMat;
}

void Joint::computeLocal() {

    this->localMat = glm::translate(this->localMat, this->offset);

    float angleZ = dofZ->getValue();
    this->localMat = glm::rotate(this->localMat, angleZ, glm::vec3(0.0f, 0.0f, 1.0f));

    float angleY = dofY->getValue();
    this->localMat = glm::rotate(this->localMat, angleY, glm::vec3(0.0f, 1.0f, 0.0f));

    float angleX = dofX->getValue();
    this->localMat = glm::rotate(this->localMat, angleX, glm::vec3(1.0f, 0.0f, 0.0f));



}


void Joint::computeWorld() {
    if (this->parent) {
        this->worldMat = this->parent->getWorldMat() * this->localMat;
    } else {
        this->worldMat = this->localMat;
    }
}

void Joint::update() {
    this->computeLocal();
    this->computeWorld();

    for (Joint* child : this->children) {
        child->update();
    }
}

bool Joint::load(Tokenizer &t, Skeleton* skel) {
    t.FindToken("{");

    while (1) {
        char temp[256];
        t.GetToken(temp);

        if (strcmp(temp, "offset") == 0) {
            this->offset.x = t.GetFloat();
            this->offset.y = t.GetFloat();
            this->offset.z = t.GetFloat();
        } else if (strcmp(temp, "boxmin") == 0) {
            this->boxmin.x = t.GetFloat();
            this->boxmin.y = t.GetFloat();
            this->boxmin.z = t.GetFloat();
        } else if (strcmp(temp, "boxmax") == 0) {
            this->boxmax.x = t.GetFloat();
            this->boxmax.y = t.GetFloat();
            this->boxmax.z = t.GetFloat();
        } else if (strcmp(temp, "rotxlimit") == 0) {
            // DOF* dof = new DOF(Axis::X, t.GetFloat(), t.GetFloat());
            // this->addDOF(dof);
            this->getDOFX()->SetMinMax((float)t.GetFloat(), (float)t.GetFloat());
        } else if (strcmp(temp, "rotylimit") == 0) {
            // DOF* dof = new DOF(Axis::Y, t.GetFloat(), t.GetFloat());
            // this->addDOF(dof);
            this->getDOFY()->SetMinMax((float)t.GetFloat(), (float)t.GetFloat());

        } else if (strcmp(temp, "rotzlimit") == 0) {
            // DOF* dof = new DOF(Axis::Z, t.GetFloat(), t.GetFloat());
            // this->addDOF(dof);
            this->getDOFZ()->SetMinMax((float)t.GetFloat(), (float)t.GetFloat());

        } else if (strcmp(temp, "pose") == 0) {
            float x = t.GetFloat();
            float y = t.GetFloat();
            float z = t.GetFloat();

            this->setPose(glm::vec3(x,y,z));

            this->setDOFX(pose.x);
            this->setDOFY(pose.y);
            this->setDOFZ(pose.z);
            


        } else if (strcmp(temp, "balljoint") == 0) {
            t.GetToken(temp);
            Joint *jnt = new Joint();
            // printf("%s\n", temp);
            jnt->setName(temp);
            // printf("%s\n", jnt->getName());
            jnt->setParent(this);
            jnt->load(t, skel);
            this->addChild(jnt);
            
            skel->addJoint(jnt);
        } else if (strcmp(temp, "}") == 0) {
            return true;
        } else {
            t.SkipLine();
        }
    }
}


void Joint::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    this->cube->draw(viewProjMtx, shader);
}