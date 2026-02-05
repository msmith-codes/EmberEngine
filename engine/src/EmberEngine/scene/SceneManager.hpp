#pragma once

#include <EmberEngine/scene/Scene.hpp>

#include <memory>
#include <string>
#include <unordered_map>

namespace EmberEngine
{
    class SceneManager
    {
        private:
            std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
            Scene* current_scene;
            std::string current_scene_name;
        public:
            static SceneManager& get_singleton();
        private:
            SceneManager() = default;
        public:
            ~SceneManager() = default;
        public:
            static void add_scene(const std::string& name, std::unique_ptr<Scene> scene);
            static void change_scene(const std::string& name);
            static void remove_scene(const std::string& name);
        public:
            static Scene* get_current_scene();
            static const std::string& get_current_scene_name();
        public:
            static void on_update(float delta);
            static void on_render();
    };
}

