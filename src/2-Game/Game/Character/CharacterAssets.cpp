#include "CharacterAssets.hpp"

#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include <sre/Mesh.hpp>
#include <sre/Material.hpp>
#include <sre/Shader.hpp>

#include <Core/Transform.hpp>
#include <Core/Model/Model.hpp>
#include <Core/Model/ModelPose.hpp>
#include <Core/Model/AnimationClip.hpp>

using namespace std;
using namespace glm;
using namespace sre;
using namespace fg::core;
using namespace fg::game;

const vec3 BODY_SIZE { 0.5f, 0.6f, 0.3f };
const vec3 LEG_SIZE { 0.2f, 0.5f, 0.2f };
const vec3 ARM_SIZE { 0.4f, 0.15f, 0.15f };


Transform mat4ToTransform(const mat4& m) {
    vec3 scale;
    quat rotation;
    vec3 translation;
    vec3 skew;
    vec4 perspective;
    decompose(m, scale, rotation, translation, skew, perspective);

    return Transform{ translation, scale, rotation };
}


mat4 boxTransform(vec3 pivotPoint, vec3 position, vec3 size, vec3 rotationDegrees) {
    mat4 matrix { 1.0f };
    matrix = matrix * translate(mat4{ 1.0 }, position);
    matrix = matrix * mat4_cast(quat{ radians(rotationDegrees) });
    matrix = matrix * scale(mat4{ 1.0 }, size);
    matrix = matrix * translate(mat4{ 1.0 }, -pivotPoint);
    return matrix;
}


shared_ptr<ModelPose> createModelPose( 
    shared_ptr<Model> model, 
    vec3 torsoRotation, 
    float torsoYOffset, 
    vec3 rightLegRotation, 
    vec3 leftLegRotation,
    vec3 rightArmRotation, 
    vec3 leftArmRotation 
) {

    const vec3 centerPosition { 0, LEG_SIZE.y, 0.0, };
    const vec3 rightLegPosition { BODY_SIZE.x / 4, LEG_SIZE.x / 2.0, 0.0, };
    const vec3 leftLegPosition { -BODY_SIZE.x / 4, LEG_SIZE.x / 2.0, 0.0, };
    const vec3 rightArmPosition { BODY_SIZE.x / 2.5, BODY_SIZE.y / 1.5, 0.0, };
    const vec3 leftArmPosition { -BODY_SIZE.x / 2.5, BODY_SIZE.y / 1.5, 0.0, };

    mat4 center = boxTransform(vec3{0}, centerPosition, vec3{1}, vec3{0});

    mat4 torsoAnchor = boxTransform(vec3{0, 0, 0}, vec3{ 0, torsoYOffset, 0 }, vec3{1}, torsoRotation);

    mat4 torso = boxTransform(vec3{0, -0.5, 0}, vec3{0}, BODY_SIZE, vec3{0});

    mat4 rightLeg = boxTransform(vec3{0, 0.5, 0}, rightLegPosition, LEG_SIZE, rightLegRotation);

    mat4 leftLeg = boxTransform(vec3{0, 0.5, 0}, leftLegPosition, LEG_SIZE, leftLegRotation);

    mat4 rightArm = boxTransform(vec3{-0.5, 0, 0}, rightArmPosition, ARM_SIZE, rightArmRotation);

    mat4 leftArm = boxTransform(vec3{0.5, 0, 0}, leftArmPosition, ARM_SIZE, leftArmRotation);

    unordered_map<int, Transform> meshTransforms {
        { 0, mat4ToTransform(center * torsoAnchor * torso) },
        { 1, mat4ToTransform(center * rightLeg) },
        { 2, mat4ToTransform(center * leftLeg) },
        { 3, mat4ToTransform(center * torsoAnchor * rightArm) },
        { 4, mat4ToTransform(center * torsoAnchor * leftArm) }
    };
    
    shared_ptr<ModelPose> pose = make_shared<ModelPose>(model, meshTransforms);

    return pose;
}


shared_ptr<Model> createModel() {

    shared_ptr<Material> skinMaterial = Shader::getStandardBlinnPhong()->createMaterial();
    skinMaterial->setColor(Color{ 0.3f, 0.6f, 0.9f});
    skinMaterial->setSpecularity(sre::Color(1, 1, 1, 50));

    shared_ptr<Model> model = make_shared<Model>();

    model->addMesh(Mesh::create().withCube(0.5f).build(), skinMaterial);
    model->addMesh(Mesh::create().withCube(0.5f).build(), skinMaterial);
    model->addMesh(Mesh::create().withCube(0.5f).build(), skinMaterial);
    model->addMesh(Mesh::create().withCube(0.5f).build(), skinMaterial);
    model->addMesh(Mesh::create().withCube(0.5f).build(), skinMaterial);

    return model;
}


CharacterAssets::CharacterAssets()
    :   model(createModel()), 

        idleAnimation(
            AnimationClip::Builder(model, createModelPose(model, vec3{0}, 0.0f, vec3{0}, vec3{0}, vec3{0, 0, -60}, vec3{0, 0, 60}), 1.5f)
            .addFrame(0.5f, createModelPose(model, vec3{0}, -0.03f, vec3{0}, vec3{0}, vec3{0, 0, -55}, vec3{0, 0, 55}))
            .addFrame(1.0f, createModelPose(model, vec3{0}, 0.0f, vec3{0}, vec3{0}, vec3{0, 0, -60}, vec3{0, 0, 60}))
            .enableLooping()
            .build()
        ),

        runAnimation(
            AnimationClip::Builder(model, createModelPose(model, vec3{-10, 0, 0}, 0.0f, vec3{45, 0, 0}, vec3{-45, 0, 0}, vec3{0, 80, -30}, vec3{0, 80, 10}), 0.5f)
            .addFrame(0.5f, createModelPose(model, vec3{-10, 0, 0}, 0.0f, vec3{-45, 0, 0}, vec3{45, 0, 0}, vec3{0, -80, -10}, vec3{0, -80, 30}))
            .addFrame(1.0f, createModelPose(model, vec3{-10, 0, 0}, 0.0f, vec3{45, 0, 0}, vec3{-45, 0, 0}, vec3{0, 80, -30}, vec3{0, 80, 10}))
            .enableLooping()
            .build()
        ),

        jumpAnimation(
            AnimationClip::Builder(model, createModelPose(model, vec3{10, 0, 0}, 0.0f, vec3{-60, 0, 0}, vec3{60, 0, 0}, vec3{0, 80, -30}, vec3{0, 70, 30}), 0.5f)
            .addFrame(0.5f, createModelPose(model, vec3{10, 0, 0}, 0.0f, vec3{-45, 0, 0}, vec3{45, 0, 0}, vec3{0, 60, -25}, vec3{0, 60, 25}))
            .addFrame(1.0f, createModelPose(model, vec3{10, 0, 0}, 0.0f, vec3{-60, 0, 0}, vec3{60, 0, 0}, vec3{0, 80, -30}, vec3{0, 70, 30}))
            .enableLooping()
            .build()
        ),

        fallAnimation(
            AnimationClip::Builder(model, createModelPose(model, vec3{10, 0, 0}, 0.0f, vec3{-60, 0, 0}, vec3{60, 0, 0}, vec3{0, 0, 35}, vec3{0, 0, -35}), 0.5f)
            .addFrame(0.5f, createModelPose(model, vec3{10, 0, 0}, 0.0f, vec3{-45, 0, 0}, vec3{45, 0, 0}, vec3{0, 0, 50}, vec3{0, 0, -50}))
            .addFrame(1.0f, createModelPose(model, vec3{10, 0, 0}, 0.0f, vec3{-60, 0, 0}, vec3{60, 0, 0}, vec3{0, 0, 35}, vec3{0, 0, -35}))
            .enableLooping()
            .build()
        )
{ }