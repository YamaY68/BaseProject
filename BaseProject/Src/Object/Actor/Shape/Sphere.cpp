#include "Sphere.h"  
#include<memory>
#include "../Collider/ColliderSphere.h"  
#include"../Collider/ColliderInfo.h"
#include"../Collider/ColliderBase.h"

Sphere::Sphere(void)  
    :
	ShapeBase(),
	radius_(100.0f)
{  
}


Sphere::Sphere(float radius)
	:
	ShapeBase(),
	radius_(radius)
{
}



Sphere::~Sphere(void)  
{  
}  

void Sphere::SubInit(void)  
{  

}  

void Sphere::SubUpdate(void)  
{  
}  

void Sphere::SubDraw(void)  
{  
   //DrawSphere3D(trans_.pos,  
   //    radius_,  
   //    16, GetColor(255, 0, 0), GetColor(0, 255, 255), false);  
}  

void Sphere::SubRelease(void)  
{  
}  

void Sphere::InitCollider(void)  
{  
   ColliderInfo info = {  
       SHAPE::SPHERE,  
       &trans_,  
       TAG::NONE,  
       Layer::NONE,  
       0,  
       VGet(0.0f, 0.0f, 0.0f),  
       VGet(0.0f, 0.0f, 0.0f),  
       true,
       false,  
   };  
   std::shared_ptr<ColliderSphere>collider=
	   std::make_shared<ColliderSphere>(info, radius_, this);
   ownColliders_.emplace(static_cast<int>(SHAPE::SPHERE), collider);
}
