#include "object_defs.h"
#include "game_object_def.h"

namespace ObjectDefs
{
    static std::map<std::string, GameObjectDef> objectDefinitions;

    GameObjectDef& getObjectDefinition(const std::string& objectDef)
    {
        return objectDefinitions.at(objectDef);
    }

    void loadObjectDefinitions(const std::string& path)
    {

    }
}
