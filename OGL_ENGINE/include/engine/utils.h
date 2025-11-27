
#include <RigidBox/RigidBox.h>
#include <engine/CollisionBox.h>
#include <engine/camera.h>
#include <engine/model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

static bool intersect(CollisionBox a, CollisionBox b)
{
    return (a.min.x/2 <= b.max.x / 2 && a.max.x / 2 >= b.min.x / 2) &&
           (a.min.y / 2 <= b.max.y / 2 && a.max.y / 2 >= b.min.y / 2) &&
           (a.min.z / 2 <= b.max.z / 2 && a.max.z / 2 >= b.min.z / 2);
}

void detectColls(CollisionBox *coll, string name, Camera *cam, bool renderCollBox, void (*callback)(string))
{
    glm::mat4 projection = glm::perspective(glm::radians(cam->Zoom), (float)800 / 600, 0.1f, 100.0f);
    glm::mat4 view = cam->GetViewMatrix();
    if (intersect(*coll, cam->collbox))
        callback(name);
    if (intersect(*coll, cam->collbox2) && name!="")
        cam->notFrontMove = true;
    if (renderCollBox)
        coll->draw(view, projection);
    
}

void detectColls(CollisionBox* coll, string name, CollisionBox* coll2,string name2, Camera* cam,bool renderCollBox, void (*callback)(string,string))
{
    glm::mat4 projection = glm::perspective(glm::radians(cam->Zoom), (float)800 / 600, 0.1f, 100.0f);
    glm::mat4 view = cam->GetViewMatrix();

    if (intersect(*coll, *coll2)&&name!=name2)
        callback(name,name2);
    if (renderCollBox)
        coll->draw(view, projection); coll2->draw(view, projection);

}


//Colisiones de cajas moradas
void detectColls(map<int, pair<string, CollisionBox>> collboxes, Camera *cam, bool renderCollBox, void (*callback)(string))
{
    glm::mat4 projection = glm::perspective(glm::radians(cam->Zoom), (float)800 / 600, 0.1f, 100.0f);
    glm::mat4 view = cam->GetViewMatrix();
    for (pair<int, pair<string, CollisionBox>> colls : collboxes)
    {

        if (intersect(colls.second.second, cam->collbox))
            callback(colls.second.first);       
        if (intersect(colls.second.second, cam->collbox2)&& colls.second.first!="Tormenta")
            cam->notFrontMove = true;
        if (renderCollBox)
            colls.second.second.draw(view, projection);
    }
}

void detectColls(rbEnvironment *env, void (*callback)(string, string))
{
    for (rbContact c : env->Contacts())
    {
        if (c.Body[0]->IsNotFixed() && c.Body[1]->IsNotFixed())
        {
            string name1 = c.Body[0]->getName();
            string name2 = c.Body[1]->getName();
            callback(name1, name2);
        }
    }
}