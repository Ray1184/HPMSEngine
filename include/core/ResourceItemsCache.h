/**
 * File ResourceItemsCache.h
 */
#pragma once

#include <unordered_map>
#include <sstream>
#include <core/Texture.h>
#include <core/AdvModelItem.h>


namespace hpms
{

    class ResourceItemsCache : public HPMSObject
    {
    private:
        ResourceItemsCache()
        {};

        ResourceItemsCache(const ResourceItemsCache&);

        ResourceItemsCache& operator=(const ResourceItemsCache&);

        std::unordered_map<std::string, Texture*> textCache;
        std::unordered_map<std::string, AdvModelItem*> modelsCache;

        inline void FreeTextures()
        {
            for (auto entry : textCache)
            {
                hpms::SafeDelete(entry.second);
            }
            textCache.clear();
        }

        inline void FreeModels()
        {
            for (auto entry : modelsCache)
            {
                hpms::SafeDelete(entry.second);
            }
            modelsCache.clear();
        }


    public:
        static ResourceItemsCache& Instance();

        Texture* GetTexture(const std::string& name);

        AdvModelItem* GetModel(const std::string& name);



        inline void FreeAll()
        {
            FreeTextures();
            FreeModels();
        }

        inline const std::string Name() const override
        {
            return "ResourceItemsCache";
        }

    };
}
