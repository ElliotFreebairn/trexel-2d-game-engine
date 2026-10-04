#pragma once

#include <string>

class Scene;
class GameObjectRegistry;

/*
 * Converts a Scene to/from a JSON file
 */
class SceneSerializer
{
public:
    static void save(const Scene& scene, const std::string& path);

    static bool load(Scene& scene, const std::string& path, const GameObjectRegistry& registry);
};
