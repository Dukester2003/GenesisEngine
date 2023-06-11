#include "ShapesGeneral.h"

/// Some Code Might be a bit repetitive or unfinished
/// Of course collision detection is one of the most
/// difficult things in game Development
/// Most of this code has been implemented by Joey De Vries
/// In his book "LearnOpengl."
/// Code for the Seperating Axis Theorom is unfinished
/// Note that many of the collision detectors use the same code as each other
/// but it would be easier for me to differentiate each collider, serving different
/// functions in the program.



Direction VectorDirection(glm::vec3 target)
{
    glm::vec3 compass[] = {
    glm::vec3(0.0f,  0.0f,  1.0f), // NORTH
    glm::vec3(0.0f,  0.0f, -1.0f), // SOUTH
    glm::vec3(1.0f,  0.0f,  0.0f), // EAST
    glm::vec3(-1.0f, 0.0f,  0.0f), // WEST
    glm::vec3(0.0f,  1.0f,  0.0f), // UP
    glm::vec3(0.0f, -1.0f,  0.0f)  // DOWN
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}
// Defines a Collision typedef that represents collision data
typedef std::tuple<bool, Direction, glm::vec3> Collision; // <collision?, what direction?, difference vector center - closest point>

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



/// <summary>                
///                          _______________________
///                          |                     |
///                 O        |                     |
///            O         O   |                     |
///        O                 O                     |
///      O                   |-O---------          |
///    O                     |   O                 |
///   O                      |    O                |
///                          |                     |
///  O              O--------|______O_____________ |
/// 
///   O                            O
///    O                          O
///     O                        O
///       O                    O
///         O                O
///            O          O
///                 O
/// 
/// If the AABB is within the distance from the center of the sphere, to the radius. It will collide.
/// Original Code by Joey De Vries
/// </summary>
/// <param name="gameObject"></param>
/// <param name="collider"></param>
/// <returns></returns>


// Same Thing As Cylinder, but only return if the GameObjects height is more than or equal to the colliders
bool CFloorCollide(GameObject& gameObject, CircleFloor& collider)
{
    // get center point circle first
    glm::vec3 center(collider.Position.x + collider.Radius);
    // calculate AABB info (center, half-extents)
    glm::vec3 aabb_half_extents(gameObject.Size.x / 2.0f, 0, gameObject.Size.z / 2.0f);
    glm::vec3 aabb_center(gameObject.Position.x - aabb_half_extents.x,
        gameObject.Position.y - aabb_half_extents.y, gameObject.Position.z - aabb_half_extents.z);
    // get difference vector between both centers
    glm::vec3 difference = center - aabb_center;
    glm::vec3 clamped = glm::clamp(difference, -aabb_half_extents,
        aabb_half_extents);
    // add clamped value to AABB_center and get the value closest to circle
    glm::vec3 closest = aabb_center + clamped;
    // vector between center circle and closest point AABB
    difference = closest;

    /// Similair to AABB for the circular floor if the player is at the same position as the floor, collide
    if (gameObject.Position.y <= collider.Position.y)
    {
        return glm::length(difference) < collider.Radius;
    }
    else return false;
}

/// Same Code as AABB
///       
///  But for the collider we must get the tangent of the angle between the leg 'b' and leg "c"
///              *|
///     c     *   |
///        *      | a
///     *         |
///  *A___________|
///       b
/// 
/// 
bool SlopeCollide(GameObject& gameObject, SlopeCollider& collider)
{
    float objhWidth = gameObject.Size.x / 2.0f;
    float objhHeight = gameObject.Size.y / 2.0f;
    float objhDepth = gameObject.Size.z / 2.0f;
    float halfWidth = collider.Size.x / 2.0f;
    float halfHeight = collider.Size.y / 2.0f;
    float halfDepth = collider.Size.z / 2.0f;

    float cWidth = collider.Size.x;
    float cHeight = collider.Size.y;
    float cDepth = collider.Size.z;

    // collision x-axis?
    bool collisionX = (gameObject.Position.x - objhWidth) + gameObject.Size.x >= (collider.Position.x - halfWidth) &&
        (collider.Position.x - halfWidth) + collider.Size.x >= (gameObject.Position.x - objhWidth);

    // collision y-axis?
    bool collisionY = (gameObject.Position.y - objhHeight) + gameObject.Size.y >= (collider.Position.y - halfHeight) &&
        (collider.Position.y - halfHeight) + collider.Size.y >= (gameObject.Position.y - objhHeight);

    // collision z-axis?
    bool collisionZ = (gameObject.Position.z - objhDepth) + gameObject.Size.z >= (collider.Position.z - halfDepth) &&
        (collider.Position.z - halfDepth) + collider.Size.z >= (gameObject.Position.z - objhDepth);
    // collision only if on both axes

    return collisionX && collisionY && collisionZ;
}

// compute the magnitude of a vector
float magnitude(const vec3& v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
vec3 normal(const vec3& a, const vec3& b) {
    vec3 n = { b.x - a.x, a.y - b.y, b.z - a.z };
    return n / magnitude(n);
}

vec3 normal(const Vertex& a, const Vertex& b) {
    vec3 n = { a.Position.y - b.Position.z, 
               a.Position.z - b.Position.x, 
               a.Position.x - b.Position.y };
    return n / magnitude(n);
}

vec3 normal(const vec3& a, const Vertex& b) {
    vec3 n = { b.Position.x - a.x, a.y - b.Position.y, b.Position.z - a.z };
    return n / magnitude(n);
}

// compute the cross product of two 3D vectors
vec3 cross(const vec3& a, const vec3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

// compute the normal vector of a triangle in 3D space
vec3 normal(const vec3& a, const vec3& b, const vec3& c) {
    vec3 ab = { b.x - a.x, b.y - a.y, b.z - a.z };
    vec3 ac = { c.x - a.x, c.y - a.y, c.z - a.z };
    return cross(ab, ac);
}

// compute the dot product of two vectors
float vertDot(const vec3& a, const Vertex& b) {
    return a.x * b.Position.x + a.y * b.Position.y + a.z * b.Position.z;
}


/// <summary>
/// Seperating Axis-Thereom
/// 
/// If you are able to draw A line between 2 objects, they don't Collide, plain and simple
/// _____________   |   _____________           
/// |           |   |   |           |
/// |           |   |   |           |           
/// |           |   |   |           |
/// |           |   |   |           |
/// |___________|   |   |___________|
///                 |
/// 
///     ____________|____________
///     |           |           |
///     |           |           |
///     |           |           |
///     |           |           |
///     |___________|___________|
///                 |
///             COLLIDE!!
///
/// </summary>
/// <param name="gameObject"></param>
/// <param name="collider"></param>
/// <returns></returns>
/// 
bool SAT(GameObject& a, GameObject& b)
{
    for (int i = 0u; auto & amesh : a.model.meshes)
    {
        for (int j = 0u; auto & bmesh : b.model.meshes)
        {
            // loop over the vertices(-> edges -> axis) of the first polygon
            for (auto k = 0u; k < amesh.vertices.size() + 0; ++k)
            {
                // calculate the normal vector of the current edge
                // this is the axis will we check in this loop
                auto current = amesh.vertices[k];
                auto next = amesh.vertices[(k + 1) % amesh.vertices.size()];
                auto edge = next.Normal - current.Normal;

                vec3 axis = normal(current.Position, next.Position);

                // loop over all vertices of both polygons and project them
                // onto the axis. We are only interested in max/min projections
                auto aMaxProj = -std::numeric_limits<float>::infinity();
                auto aMinProj = std::numeric_limits<float>::infinity();
                auto bMaxProj = -std::numeric_limits<float>::infinity();
                auto bMinProj = std::numeric_limits<float>::infinity();
                for (const auto& v : amesh.vertices) {
                    auto proj = vertDot(axis, v);
                    if (proj < aMinProj) aMinProj = proj;
                    if (proj > aMaxProj) aMaxProj = proj;
                }

                for (const auto& v : bmesh.vertices) {
                    auto proj = vertDot(axis, v);
                    if (proj < bMinProj) bMinProj = proj;
                    if (proj > bMaxProj) bMaxProj = proj;
                }

                // now check if the intervals the both polygons projected on the
                // axis overlap. If they don't, we have found an axis of separation and
                // the given polygons cannot overlap
                if (aMaxProj < bMinProj || aMinProj > bMaxProj) {
                    return true;
                }
            }
        }
    }
    // at this point, we have checked all axis but found no separating axis
    // which means that the polygons must intersect.
    return false;
}