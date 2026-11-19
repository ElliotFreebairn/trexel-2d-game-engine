#pragma once

// Custom includes
#include "asset_manager.h"
#include "game.h"
#include "game_actions.h"
#include "object_factory.h"
#include "input_state.h"
#include <filesystem>


class Trexel : public Game
{
public:
    Trexel();
    ~Trexel();

    void onUpdate(float detlaTime, const InputState& state) override;

public:
    static inline constexpr const char* MANIFEST_PATH = "/home/elliotf/Develop/Personal/trexel/data/asset_manifest.json";

protected:
    void onInit() override;
    void intialiseAssets(AssetManager& manager) override;
    void registerObjectTypes(ObjectFactory& factory) override;

private:

};
