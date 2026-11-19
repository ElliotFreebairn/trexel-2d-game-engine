#pragma once

#include <string>

class Scene;

/*
 * Converts a Scene to/from a JSON file
 *
 * {
 *  "objects": [
 *      { "def": "grass_block", "position": { "x": 500.0, "y": 600.0 }, "size": { "x": 50.0, "y": 50.0 } },
 *      ...
 *  ]
 * }
 *
 * "def" is used to spawn it again on load, everything else is only saved when it differs from the definition
 */
class SceneSerializer
{
public:
    static void save(Scene& scene, const std::string& path);

    static bool load(Scene& scene, const std::string& path);
};
