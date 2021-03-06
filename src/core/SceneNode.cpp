/*!
 * File SceneNode.cpp
 */

#include <core/SceneNode.h>

hpms::SceneNode* hpms::SceneNode::FindInTree(const std::string& name)
{
    SceneNode* res = nullptr;
    if (this->name == name)
    {
        res = this;
    } else
    {
        for (SceneNode* child : this->children)
        {
            res = child->FindInTree(name);
            if (res != nullptr)
            {
                break;
            }
        }
    }
    return res;
}

std::vector<hpms::Actor*> hpms::SceneNode::GetActorsList()
{
    std::vector<Actor*> ret;
    if (actor != nullptr)
    {
        ret.push_back(actor);
    }

    for (SceneNode* child : children)
    {
        std::vector<Actor*> childRet = child->GetActorsList();
        ret.insert(std::end(ret), std::begin(childRet), std::end(childRet));
    }

    return ret;
}

void hpms::SceneNode::DeleteAllActors()
{
    if (actor != nullptr)
    {
        hpms::SafeDelete(actor);
    }

    for (SceneNode* child : children)
    {
        child->DeleteAllActors();
    }
}

void hpms::SceneNode::UpdateTree(bool updateRoot)
{
    if (updateRoot)
    {
        const glm::mat4 rotMat = glm::toMat4(rotation);
        const glm::mat4 transMat = glm::translate(glm::mat4(1.0f), position);
        const glm::mat4 transScalMat = glm::scale(transMat, scale);
        localTransform = transScalMat * rotMat;

        if (parent != nullptr)
        {
            worldTransform = parent->worldTransform * localTransform;
        } else
        {
            worldTransform = localTransform;
        }

        if (actor != nullptr)
        {
            actor->SetWorldTransform(worldTransform);
        }
    }
    for (SceneNode* child : children)
    {
        child->UpdateTree();
    }
}

hpms::SceneNode* hpms::SceneNode::RemoveChild(const std::string& name)
{
    SceneNode* node = FindInTree(parent->GetName());
    if (node != nullptr)
    {
        unsigned int index = 0;
        for (SceneNode* n : node->parent->children)
        {
            if (n->GetName() == name)
            {
                node->parent->children.erase(node->parent->children.begin() + index);
            }
            index++;
        }
    }
    return node;
}
