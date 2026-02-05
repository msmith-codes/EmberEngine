#include <EmberEngine/scene/SceneManager.hpp>

#include <EmberEngine/core/Logger.hpp>

namespace EmberEngine
{
    SceneManager& SceneManager::get_singleton()
    {
        static SceneManager singleton;
        return singleton;
    }

    void SceneManager::add_scene(const std::string& name, std::unique_ptr<Scene> scene)
    {
        SceneManager& self = SceneManager::get_singleton();
        if(self.scenes.find(name) != self.scenes.end()) {
            Logger::error("SceneManager", "Scene with name '" + name + "' already exists!");
            return;
        }

        self.scenes[name] = std::move(scene);
    }

    void SceneManager::change_scene(const std::string &name)
    {
        SceneManager& self = SceneManager::get_singleton();
        auto it = self.scenes.find(name);
        if(it == self.scenes.end()) {
            Logger::error("SceneManager", "Scene with name '" + name + "' does not exists!");
            return;
        }

        if(self.current_scene) {
            self.current_scene->on_dispose();
            self.current_scene->set_active(false);
            }

        self.current_scene = it->second.get();
        self.current_scene_name = name;
        self.current_scene->set_active(true);
        self.current_scene->on_compose();
    }

    void SceneManager::remove_scene(const std::string &name)
    {
        SceneManager& self = SceneManager::get_singleton();
        auto it = self.scenes.find(name);
        if(it == self.scenes.end()) {
            Logger::error("SceneManager", "Scene with name '" + name + "' does not exists!");
            return;
        }

        if(self.current_scene_name == name) {
            self.current_scene->on_dispose();
            self.current_scene = nullptr;
            self.current_scene_name = "";
            }

        self.scenes.erase(it);
        }

    Scene* SceneManager::get_current_scene()
    {
        SceneManager& self = SceneManager::get_singleton();
        return self.current_scene;
    }

    const std::string& SceneManager::get_current_scene_name()
    {
        SceneManager& self = SceneManager::get_singleton();
        return self.current_scene_name;
    }

    void SceneManager::on_update(float delta)
    {
        SceneManager& self = SceneManager::get_singleton();
        if(self.current_scene && self.current_scene->is_active()) {
            self.current_scene->on_update(delta);
        }
    }

    void SceneManager::on_render()
    {
        SceneManager& self = SceneManager::get_singleton();
        if(self.current_scene && self.current_scene->is_active()) {
            self.current_scene->on_render();
        }
    }
}