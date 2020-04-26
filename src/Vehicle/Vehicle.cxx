/*
 *  Vehicle.cxx
 *  Copyright 2020 ItJustWorksTM
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
//======================================================================================
//
// Copyright (c) 2008-2020 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/DecalSet.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/Constraint.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RaycastVehicle.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include "Vehicle.hxx"

using namespace Urho3D;

const float CHASSIS_WIDTH = 2.6f;

void Vehicle::RegisterObject(Context* context) {
    context->RegisterFactory<Vehicle>();
    URHO3D_ATTRIBUTE("Controls Yaw", float, controls_.yaw_, 0.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Controls Pitch", float, controls_.pitch_, 0.0f, AM_DEFAULT);
}

Vehicle::Vehicle(Urho3D::Context* context) : LogicComponent(context) {
    SetUpdateEventMask(USE_FIXEDUPDATE | USE_POSTUPDATE);
    engineForce_ = 0.0f; // NOT CORRECT VALUES
    brakingForce_ = 50.0f;
    maxEngineForce_ = 2500.0f;
    wheelRadius_ = 0.5f;
    suspensionRestLength_ = 0.6f;
    wheelWidth_ = 0.4f;
    suspensionStiffness_ = 14.0f;
    suspensionDamping_ = 2.0f;
    suspensionCompression_ = 4.0f;
    wheelFriction_ = 1000.0f;
    rollInfluence_ = 0.12f;
}

Vehicle::~Vehicle() = default;

void Vehicle::Init() {
    auto* vehicle = node_->CreateComponent<RaycastVehicle>();
    vehicle->Init();
    auto* hullBody = node_->GetComponent<RigidBody>();
    hullBody->SetMass(800.0f);
    hullBody->SetLinearDamping(0.2f); // Some air resistance
    hullBody->SetAngularDamping(0.5f);
    hullBody->SetCollisionLayer(1);
    // This function is called only from the main program when initially creating the vehicle, not on scene load
    auto* cache = GetSubsystem<ResourceCache>();
    auto* hullObject = node_->CreateComponent<StaticModel>();
    // Setting-up collision shape
    auto* hullColShape = node_->CreateComponent<CollisionShape>();
    Vector3 v3BoxExtents = Vector3::ONE;
    hullColShape->SetBox(v3BoxExtents);
    node_->SetScale(Vector3(2.3f, 1.0f, 4.0f));
    hullObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    hullObject->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
    hullObject->SetCastShadows(true);
    float connectionHeight = -0.4f;
    bool isFrontWheel = true;
    Vector3 wheelDirection(0, -1, 0);
    Vector3 wheelAxle(-1, 0, 0);
    // We use not scaled coordinates here as everything will be scaled.
    // Wheels are on bottom at edges of the chassis
    // Note we don't set wheel nodes as children of hull (while we could) to avoid scaling to affect them.
    float wheelX = CHASSIS_WIDTH / 2.0f - wheelWidth_;
    // Front left
    connectionPoints_[0] = Vector3(-wheelX, connectionHeight, 2.5f - GetWheelRadius() * 2.0f);
    // Front right
    connectionPoints_[1] = Vector3(wheelX, connectionHeight, 2.5f - GetWheelRadius() * 2.0f);
    // Back left
    connectionPoints_[2] = Vector3(-wheelX, connectionHeight, -2.5f + GetWheelRadius() * 2.0f);
    // Back right
    connectionPoints_[3] = Vector3(wheelX, connectionHeight, -2.5f + GetWheelRadius() * 2.0f);
    const Color LtBrown(0.972f, 0.780f, 0.412f);
    for (int id = 0; id < sizeof(connectionPoints_) / sizeof(connectionPoints_[0]); id++) {
        Node* wheelNode = GetScene()->CreateChild();
        Vector3 connectionPoint = connectionPoints_[id];
        // Front wheels are at front (z > 0)
        // back wheels are at z < 0
        // Setting rotation according to wheel position
        bool isFrontWheel = connectionPoints_[id].z_ > 0.0f;
        wheelNode->SetRotation(connectionPoint.x_ >= 0.0 ? Quaternion(0.0f, 0.0f, -90.0f) : Quaternion(0.0f, 0.0f, 90.0f));
        wheelNode->SetWorldPosition(node_->GetWorldPosition() + node_->GetWorldRotation() * connectionPoints_[id]);
        vehicle->AddWheel(wheelNode, wheelDirection, wheelAxle, suspensionRestLength_, wheelRadius_, isFrontWheel);
        vehicle->SetWheelSuspensionStiffness(id, suspensionStiffness_);
        vehicle->SetWheelDampingRelaxation(id, suspensionDamping_);
        vehicle->SetWheelDampingCompression(id, suspensionCompression_);
        vehicle->SetWheelFrictionSlip(id, wheelFriction_);
        vehicle->SetWheelRollInfluence(id, rollInfluence_);
        wheelNode->SetScale(Vector3(1.0f, 0.65f, 1.0f));
        auto* pWheel = wheelNode->CreateComponent<StaticModel>();
        pWheel->SetModel(cache->GetResource<Model>("Models/Cylinder.mdl"));
        pWheel->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
        pWheel->SetCastShadows(true);
    }
    vehicle->ResetWheels();
}
void Vehicle::resetDifferential() {
    auto* vehicle = node_->GetComponent<RaycastVehicle>();
    engineForce_ = 0;
    vehicle->SetEngineForce(0, 0);
    vehicle->SetEngineForce(2, 0);
    vehicle->SetEngineForce(1, 0);
    vehicle->SetEngineForce(3, 0);
}
void Vehicle::setDifferential(float engineForce, const unsigned int control) {
    auto* vehicle = node_->GetComponent<RaycastVehicle>();
    if (control == 8) {
        engineForce = engineForce * -10;
    } else {
        engineForce = engineForce * 10;
    }
    vehicle->SetEngineForce(1, engineForce);
    vehicle->SetEngineForce(3, engineForce);
    vehicle->SetEngineForce(0, -engineForce);
    vehicle->SetEngineForce(2, -engineForce);
}
void Vehicle::FixedUpdate(float timeStep) {
    float accelerator = 0.0f;
    bool brake = false;
    auto* vehicle = node_->GetComponent<RaycastVehicle>();
    // Read controls

    if (controls_.buttons_ & CTRL_LEFT) {
        setDifferential(maxEngineForce_, CTRL_LEFT);
    }
    if (controls_.buttons_ & CTRL_RIGHT) {
        setDifferential(maxEngineForce_, CTRL_RIGHT);
    }
    if (controls_.buttons_ & CTRL_FORWARD) {
        resetDifferential();
        accelerator = 1.0f;
    }
    if (controls_.buttons_ & CTRL_BACK) {
        resetDifferential();
        accelerator = -0.5f;
    }
    if (controls_.buttons_ & CTRL_BRAKE) {
        brake = true;
    }

    // apply forces
    engineForce_ = maxEngineForce_ * accelerator;
    vehicle->SetEngineForce(0, engineForce_);
    vehicle->SetEngineForce(1, engineForce_);
    vehicle->SetEngineForce(3, engineForce_);
    vehicle->SetEngineForce(2, engineForce_);

    for (int i = 0; i < vehicle->GetNumWheels(); i++) {
        if (brake) {
            vehicle->SetBrake(i, brakingForce_ * 4);
        } else {
            vehicle->SetBrake(i, 0.0f);
        }
    }
}
void Vehicle::PostUpdate(float timeStep) {
    auto* vehicle = node_->GetComponent<RaycastVehicle>();
    auto* vehicleBody = node_->GetComponent<RigidBody>();
    Vector3 velocity = vehicleBody->GetLinearVelocity();
    Vector3 accel = (velocity - prevVelocity_) / timeStep;
    float planeAccel = Vector3(accel.x_, 0.0f, accel.z_).Length();
    prevVelocity_ = velocity;
}
