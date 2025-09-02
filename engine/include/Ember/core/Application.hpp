#pragma once

#include "Ember/scene/Scene.hpp"
#include <string>

namespace Ember
{
    class Application
    {
        private:
            static Application* instance;
        private: // -- Utilities -- //
            Scene* scene;
        public: // -- Constructor/Destructor -- //
            Application();
            virtual ~Application();
        public: // -- Methods -- //
            void run();
        public:
            static Application* get_singleton();
            static void set_singleton(Application* instance);
        public:
            static void change_scene(Scene* scene);
        protected:
            virtual void on_compose() = 0;
            virtual void on_update(float delta) = 0;
            virtual void on_render() = 0;
            virtual void on_dispose() = 0;
    };

    // To be defined in client.
    Application* create_application();
}

