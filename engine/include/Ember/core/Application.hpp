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
            static Application* getInstance();
            static void setInstance(Application* instance);
        public:
            static void changeScene(const std::string& sceneName);
            static void setDefaultScene(const std::string& sceneName);
        protected:
            virtual void onCreate() = 0;
            virtual void onUpdate(float delta) = 0;
            virtual void onRender() = 0;
            virtual void onShutdown() = 0;
    };

    Application* createApplication();
}

#endif