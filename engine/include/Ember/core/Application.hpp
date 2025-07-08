#ifndef EE_Application_HPP
#define EE_Application_HPP

#include <string>

namespace Ember
{
    class Application
    {
        private:
            static Application* instance;
        public: // -- Constructor/Destructor -- //
            Application();
            virtual ~Application();
        public: // -- Methods -- //
            void run();
        public:
            static Application* get_singleton();
            static void set_singleton(Application* instance);
        public:
            static void change_scene(const std::string& sceneName);
            static void set_default_scene(const std::string& sceneName);
        protected:
            virtual void on_create() = 0;
            virtual void on_update(float delta) = 0;
            virtual void on_render() = 0;
            virtual void on_shutdown() = 0;
    };

    // To be defined in client.
    Application* create_application();
}

#endif