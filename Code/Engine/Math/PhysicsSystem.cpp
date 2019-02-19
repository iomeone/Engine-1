//------------------------------------------------------------------------------------------------------------------------------
#include "Engine/Math/PhysicsSystem.hpp"
#include "Engine/Math/Collider2D.hpp"
#include "Engine/Renderer/Rgba.hpp"
#include "Engine/Math/CollisionHandler.hpp"
#include "Engine/Math/RigidBodyBucket.hpp"
#include "Engine/Math/MathUtils.hpp"

PhysicsSystem* g_physicsSystem = nullptr;

PhysicsSystem::PhysicsSystem()
{
	m_rbBucket = new RigidBodyBucket;
}

PhysicsSystem::~PhysicsSystem()
{

}


Rigidbody2D* PhysicsSystem::CreateRigidbody( eSimulationType simulationType )
{
	Rigidbody2D *rigidbody = new Rigidbody2D(this, simulationType);
	return rigidbody;
}

void PhysicsSystem::AddRigidbodyToVector(Rigidbody2D* rigidbody)
{
	m_rbBucket->m_RbBucket[rigidbody->GetSimulationType()].push_back(rigidbody);
}

void PhysicsSystem::DestroyRigidbody( Rigidbody2D* rigidbody )
{
	delete rigidbody;
	rigidbody = nullptr;
}

void PhysicsSystem::SetGravity( const Vec2& gravity )
{
	m_gravity = gravity;
}

void PhysicsSystem::CopyTransformsFromObjects()
{
	// reset per frame stuff; 
	// copy all transforms over;
	for(int rigidTypes = 0; rigidTypes < NUM_SIMULATION_TYPES; rigidTypes++)
	{
		int numRigidbodies = static_cast<int>(m_rbBucket->m_RbBucket[rigidTypes].size());

		for(int rigidbodyIndex = 0; rigidbodyIndex < numRigidbodies; rigidbodyIndex++)
		{
			if(m_rbBucket->m_RbBucket[rigidTypes][rigidbodyIndex] != nullptr)
			{
				m_rbBucket->m_RbBucket[rigidTypes][rigidbodyIndex]->m_transform = *m_rbBucket->m_RbBucket[rigidTypes][rigidbodyIndex]->m_object_transform;
			}
		}
	}
}

void PhysicsSystem::CopyTransformsToObjects()
{
	// figure out movement, apply to actual game object;
	for(int rigidTypes = 0; rigidTypes < NUM_SIMULATION_TYPES; rigidTypes++)
	{
		int numRigidbodies = static_cast<int>(m_rbBucket->m_RbBucket[rigidTypes].size());

		for(int rigidbodyIndex = 0; rigidbodyIndex < numRigidbodies; rigidbodyIndex++)
		{
			if(m_rbBucket->m_RbBucket[rigidTypes][rigidbodyIndex] != nullptr)
			{
				*m_rbBucket->m_RbBucket[rigidTypes][rigidbodyIndex]->m_object_transform = m_rbBucket->m_RbBucket[rigidTypes][rigidbodyIndex]->m_transform;
			}
		}
	}
}

void PhysicsSystem::Update( float deltaTime )
{
	CopyTransformsFromObjects(); 
	// debug: clear all frame information
	// such as if they are currently touching another object; 
	SetAllCollisionsToFalse();

	// we'll eventually switch to fixed-step updates, so we'll call down immediately to a run_step to make
	// that port easier; 
	RunStep( deltaTime );

	CopyTransformsToObjects();  
}

void PhysicsSystem::SetAllCollisionsToFalse()
{
	for(int rigidTypes = 0; rigidTypes < NUM_SIMULATION_TYPES; rigidTypes++)
	{
		int numRigidbodies = static_cast<int>(m_rbBucket->m_RbBucket[rigidTypes].size());

		for(int rigidbodyIndex = 0; rigidbodyIndex < numRigidbodies; rigidbodyIndex++)
		{
			if(m_rbBucket->m_RbBucket[rigidTypes][rigidbodyIndex] != nullptr)
			{
				m_rbBucket->m_RbBucket[rigidTypes][rigidbodyIndex]->m_collider->SetCollision(false);
			}
		}
	}
}

void PhysicsSystem::DebugRender( RenderContext* renderContext ) const
{
	for (int rbTypes = 0; rbTypes < NUM_SIMULATION_TYPES; rbTypes++)
	{
		int numRigidbodies = static_cast<int>(m_rbBucket->m_RbBucket[rbTypes].size());
		for(int rbIndex = 0; rbIndex < numRigidbodies; rbIndex++)
		{
			//Nullptr check first
			if(m_rbBucket->m_RbBucket[rbTypes][rbIndex] == nullptr)
			{
				continue;
			}

			eSimulationType simType = m_rbBucket->m_RbBucket[rbTypes][rbIndex]->GetSimulationType();

			switch( simType )
			{
			case TYPE_UNKOWN:
			break;
			case STATIC_SIMULATION:
			{
				if(m_rbBucket->m_RbBucket[rbTypes][rbIndex]->m_collider->m_inCollision)
				{
					m_rbBucket->m_RbBucket[rbTypes][rbIndex]->DebugRender(renderContext, Rgba::MAGENTA);
				}
				else
				{
					m_rbBucket->m_RbBucket[rbTypes][rbIndex]->DebugRender(renderContext, Rgba::YELLOW);
				}
			}
			break;
			case DYNAMIC_SIMULATION:
			{
				if(m_rbBucket->m_RbBucket[rbTypes][rbIndex]->m_collider->m_inCollision)
				{
					m_rbBucket->m_RbBucket[rbTypes][rbIndex]->DebugRender(renderContext, Rgba::RED);
				}
				else
				{
					m_rbBucket->m_RbBucket[rbTypes][rbIndex]->DebugRender(renderContext, Rgba::BLUE);
				}
			}
			break;
			case NUM_SIMULATION_TYPES:
			break;
			default:
			break;
			}
		}
	}
}

const Vec2& PhysicsSystem::GetGravity() const
{
	return m_gravity;
}

void PhysicsSystem::RunStep( float deltaTime )
{
	//First move all rigidbodies
	MoveAllDynamicObjects(deltaTime);

	//For A2
	//Check Static vs Static to mark as collided
	CheckStaticVsStaticCollisions();

	//Dynamic vs Static set 
	ResolveDynamicVsStaticCollisions(true);

	//Dynamic vs Dynamic set
	ResolveDynamicVsDynamicCollisions(true);

	//Dynamic vs static set with no resolution
	ResolveDynamicVsStaticCollisions(false);
}

void PhysicsSystem::MoveAllDynamicObjects(float deltaTime)
{
	int numObjects = static_cast<int>(m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION].size());
	for (int objectIndex = 0; objectIndex < numObjects; objectIndex++)
	{
		if(m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][objectIndex] != nullptr)
		{
			m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][objectIndex]->Move(deltaTime);
		}
	}
}

void PhysicsSystem::CheckStaticVsStaticCollisions()
{
	int numStaticObjects = static_cast<int>(m_rbBucket->m_RbBucket[STATIC_SIMULATION].size());

	//Set colliding or not colliding here
	for(int colliderIndex = 0; colliderIndex < numStaticObjects; colliderIndex++)
	{
		if(m_rbBucket->m_RbBucket[STATIC_SIMULATION][colliderIndex] == nullptr)
		{
			continue;
		}

		for(int otherColliderIndex = 0; otherColliderIndex < numStaticObjects; otherColliderIndex++)
		{
			//check condition where the other collider is nullptr
			if(m_rbBucket->m_RbBucket[STATIC_SIMULATION][otherColliderIndex] == nullptr)
			{
				continue;
			}

			//Make sure we aren't the same rigidbody
			if(m_rbBucket->m_RbBucket[STATIC_SIMULATION][colliderIndex] == m_rbBucket->m_RbBucket[STATIC_SIMULATION][otherColliderIndex])
			{
				continue;
			}

			Collision2D collision;
			if(m_rbBucket->m_RbBucket[STATIC_SIMULATION][colliderIndex]->m_collider->IsTouching(&collision, m_rbBucket->m_RbBucket[STATIC_SIMULATION][otherColliderIndex]->m_collider))
			{
				//Set collision to true
				m_rbBucket->m_RbBucket[STATIC_SIMULATION][colliderIndex]->m_collider->SetCollision(true);
				m_rbBucket->m_RbBucket[STATIC_SIMULATION][otherColliderIndex]->m_collider->SetCollision(true);
			}
		}
	}
}

void PhysicsSystem::ResolveDynamicVsStaticCollisions(bool canResolve)
{
	int numDynamicObjects = static_cast<int>(m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION].size());
	int numStaticObjects = static_cast<int>(m_rbBucket->m_RbBucket[STATIC_SIMULATION].size());

	//Set colliding or not colliding here
	for(int colliderIndex = 0; colliderIndex < numDynamicObjects; colliderIndex++)
	{
		if(m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][colliderIndex] == nullptr)
		{
			continue;
		}

		for(int otherColliderIndex = 0; otherColliderIndex < numStaticObjects; otherColliderIndex++)
		{
			//check condition where the other collider is nullptr
			if(m_rbBucket->m_RbBucket[STATIC_SIMULATION][otherColliderIndex] == nullptr)
			{
				continue;
			}

			Collision2D collision;
			if(m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][colliderIndex]->m_collider->IsTouching(&collision, m_rbBucket->m_RbBucket[STATIC_SIMULATION][otherColliderIndex]->m_collider))
			{
				//Set collision to true
				m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][colliderIndex]->m_collider->SetCollision(true);
				m_rbBucket->m_RbBucket[STATIC_SIMULATION][otherColliderIndex]->m_collider->SetCollision(true);

				//Push the object out based on the collision manifold
				if(collision.m_manifold.m_normal != Vec2::ZERO)
				{
					m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][colliderIndex]->m_transform.m_position += collision.m_manifold.m_normal * collision.m_manifold.m_penetration;
				}

				if(canResolve)
				{
					//Resolve
					Vec2 velocity = m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][colliderIndex]->m_velocity;
					
					float velocityOnNormal = GetDotProduct(velocity, collision.m_manifold.m_normal);
					if(velocityOnNormal > 0.f)
					{
						//Ignore this collision as we are already moving out of object
						return;
					}

					Vec2 tangent = collision.m_manifold.m_normal.GetRotated90Degrees();
					Vec2 tangentialVelocity = GetProjectedVector(velocity, tangent);

					float finalVelocityScale = -velocityOnNormal * (collision.m_Obj->m_rigidbody->m_material.restitution) * (collision.m_otherObj->m_rigidbody->m_material.restitution);

					//Comute final velocity along normal and add to existing velocity on tangent
					Vec2 fVelocityNormal = collision.m_manifold.m_normal * finalVelocityScale;
					Vec2 finalVelocity = fVelocityNormal + tangentialVelocity;

					m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][colliderIndex]->m_velocity = finalVelocity;
				}
			}
		}
	}
}

void PhysicsSystem::ResolveDynamicVsDynamicCollisions(bool canResolve)
{
	int numDynamicObjects = static_cast<int>(m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION].size());

	//Set colliding or not colliding here
	for(int colliderIndex = 0; colliderIndex < numDynamicObjects; colliderIndex++)
	{
		if(m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][colliderIndex] == nullptr)
		{
			continue;
		}

		for(int otherColliderIndex = 0; otherColliderIndex < numDynamicObjects; otherColliderIndex++)
		{
			//check condition where the other collider is nullptr
			if(m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][otherColliderIndex] == nullptr)
			{
				continue;
			}

			
			//Make sure we aren't the same rigidbody
			if(m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][colliderIndex] == m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][otherColliderIndex])
			{
				continue;
			}

			Collision2D collision;
			if(m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][colliderIndex]->m_collider->IsTouching(&collision, m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][otherColliderIndex]->m_collider))
			{
				//Set collision to true
				m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][colliderIndex]->m_collider->SetCollision(true);
				m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][otherColliderIndex]->m_collider->SetCollision(true);

				//Push the object out based on the collision manifold
				if(collision.m_manifold.m_normal != Vec2::ZERO)
				{
					float mass0 = m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][colliderIndex]->m_mass; 
					float mass1 = m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][otherColliderIndex]->m_mass; 
					float totalMass = mass0 + mass1;

					//Correction on system mass
					float correct0 = mass1 / totalMass;   // move myself along the correction normal
					float correct1 = mass0 / totalMass;  // move opposite along the normal

					m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][colliderIndex]->m_transform.m_position += collision.m_manifold.m_normal * collision.m_manifold.m_penetration * correct0;
					m_rbBucket->m_RbBucket[DYNAMIC_SIMULATION][otherColliderIndex]->m_transform.m_position += (collision.m_manifold.m_normal * -1) * collision.m_manifold.m_penetration * correct1;
				}

				if(canResolve)
				{
					//resolve
				}
			}
		}
	}
}