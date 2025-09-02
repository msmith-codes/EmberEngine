#pragma once

#include "Ember/scene/Scene.hpp"
#include "raylib.h"

class GameScene : public Ember::Scene
{
    private:
    public:
        void on_compose() override;
        void on_update(float delta) override;
        void on_render() override;
        void on_dispose() override;
};
