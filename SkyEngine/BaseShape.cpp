#include "BaseShape.h"

int Plane::next_id = 0;
int BoxCollider::next_id = 0;
int SphereCollider::next_id = 0;
int CylinderCollider::next_id = 0;
int CapsuleCollider::next_id = 0;
int ConeCollider::next_id = 0;
int CompoundShape::next_id = 0;
int ConvexHull::next_id = 0;
int TriangleMesh::next_id = 0;
int HeightField::next_id = 0;
int SoftBody::next_id = 0;
/*
int MultiSphere::next_id = 0;
int ConvexPointCloud::next_id = 0;
*/

void BaseShape::InitiateRigidBody(btDynamicsWorld* dynamicsWorld)
{
    createCollisionShape();
    // Add boxShape to the collisionShapes array
    collisionShapes.push_back(collisionShape);


    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(glmToBullet(Position));

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    isDynamic = (massValue != 0.f);

    localInertia = btVector3(1.0, 1.0, 1.0);
    collisionShape->calculateLocalInertia(massValue, localInertia);

    //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(massValue, myMotionState, collisionShape, localInertia);
    rigidBody = new btRigidBody(rbInfo);
    rigidBody->setFriction(.8f);

    //add the body to the dynamics world
    dynamicsWorld->addRigidBody(rigidBody);
}

void BaseShape::UpdateRigidBody()
{
    if (rigidBody) {
        btTransform btTrans;

        if (isDynamic) {
            rigidBody->getMotionState()->getWorldTransform(btTrans);
        }
        else {
            btTrans = rigidBody->getWorldTransform();
        }

        // Update position
        btVector3 btPos = btTrans.getOrigin();
        Position = glm::vec3(btPos.x(), btPos.y(), btPos.z());

        // Update rotation
        btQuaternion btRot = btTrans.getRotation();
        glm::quat glmRot(btRot.w(), btRot.x(), btRot.y(), btRot.z());
        Rotation = glm::eulerAngles(glmRot);
    }
}

void CompoundShape::ObjMenu(string name)
{
    ImGui::SetItemDefaultFocus();
    // Bring this menu up if object is selected

    ImGui::Begin(name.c_str());

    if (ImGui::BeginChild("Child Window", ImVec2(600, 200), false))
    {
        if (rigidBody) { rigidBodyEnabled = rigidBody->getActivationState() != DISABLE_SIMULATION; }
        if (ImGui::Checkbox("Rigidbody Enabled?", &rigidBodyEnabled))
        {
            setRigidBodyEnabled(rigidBodyEnabled);
        }
        btScalar newMass = massValue;


        btTransform currentTransform;

        glm::vec3 currentPosition;

        if (rigidBody != nullptr) {
            currentTransform = rigidBody->getWorldTransform();
            currentPosition = bulletToGlm(currentTransform.getOrigin());
        }

        if (ImGui::DragFloat3("Compound Pos", (float*)&currentPosition, .5f)) {
            // Update the object's position
            if (isDynamic && rigidBodyEnabled)
            {
                rigidBody->setActivationState(DISABLE_SIMULATION);
                btTransform newTransform = currentTransform;
                newTransform.setOrigin(glmToBullet(currentPosition));
                rigidBody->setWorldTransform(newTransform);
                setRigidBodyEnabled(rigidBodyEnabled);
            }
            else if (!isDynamic)
            {
                btTransform newTransform = currentTransform;
                newTransform.setOrigin(glmToBullet(currentPosition));
                rigidBody->setWorldTransform(newTransform);
            }
        }

        ScaleUniform("Global Size", (float*)&Size, 1.0f, 0.0f, 100.0f);

        glm::quat currentRotation = bulletToGlm(currentTransform.getRotation());

        if (ImGui::DragFloat4("Global Rot", (float*)&currentRotation, .05, -1.0f, 1.0f)) {
            // Update the object's rotation
            if (isDynamic && rigidBodyEnabled)
            {
                rigidBody->setActivationState(DISABLE_SIMULATION);
                currentRotation = glm::normalize(currentRotation);
                btTransform newTransform = rigidBody->getWorldTransform();
                newTransform.setRotation(glmToBullet(currentRotation));
                rigidBody->setWorldTransform(newTransform);
                setRigidBodyEnabled(rigidBodyEnabled);
            }
            else if (!isDynamic)
            {
                btTransform newTransform = currentTransform;
                newTransform.setRotation(glmToBullet(currentRotation));
                rigidBody->setWorldTransform(newTransform);
            }
        }

        if (ImGui::DragFloat("Object Mass", &massValue)) { setMass(massValue); }

        if (ImGui::Button("Delete"))
        {
            Destroyed = true;
        }

        ImGui::EndChild();
    }
    ImGui::End();  // handle selection
}



/// <summary>
/// Axis-Aligned Bounding Box (AABB)
/// 
/// The most efficient and easiest way to detect collision
/// Simply checks whether 2 objects overlap or not
///  ___________
/// |           |
/// |           |
/// |           |
/// |           |
/// |___________|
/// 
///            ___________
///           |           |
///           |           |
///           |           |
///           |           |
///           |___________|
///       Not overlapping
/// 
/// 
///   ___________
///  |           |
///  |           |
///  |       ____|______
///  |      |    |      |
///  |______|____|      |
///         |           |
///         |           |
///         |___________|
///        is Overlapping
///           
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
bool AABB(GameObject& a, GameObject& b)
{
    float objhWidth = a.Size.x / 2.0f;
    float objhHeight = a.Size.y / 2.0f;
    float objhDepth = a.Size.z / 2.0f;
    float halfWidth = b.Size.x / 2.0f;
    float halfHeight = b.Size.y / 2.0f;
    float halfDepth = b.Size.z / 2.0f;

    float cWidth = b.Size.x;
    float cHeight = b.Size.y;
    float cDepth = b.Size.z;

    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), b.Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

    // collision x-axis?
    bool collisionX = (a.Position.x - objhWidth) + a.Size.x >= (b.Position.x - halfWidth) &&
        (b.Position.x - halfWidth) + b.Size.x >= (a.Position.x - objhWidth);

    // collision y-axis?
    bool collisionY = (a.Position.y - objhHeight) + a.Size.y >= (b.Position.y - halfHeight) &&
        (b.Position.y - halfHeight) + b.Size.y >= (a.Position.y - objhHeight);

    // collision z-axis?
    bool collisionZ = (a.Position.z - objhDepth) + a.Size.z >= (b.Position.z - halfDepth) &&
        (b.Position.z - halfDepth) + b.Size.z >= (a.Position.z - objhDepth);
    // collision only if on both axes
    return collisionX && collisionY && collisionZ;
}
