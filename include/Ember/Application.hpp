#ifndef EMBER_Application_HPP_
#define EMBER_Application_HPP_

namespace Ember
{
    class Window;
    class Renderer;

    class Application
    {
        private: // Singleton Instance
            static Application* instance;
        protected:
            Window* window;
            Renderer* renderer;
        private:
            unsigned int currentFPS;
        public: // Singleton Constructor
            Application();
        public: // Singleton Destructor
            virtual ~Application();
        public: // Callbacks
            virtual void onStart() = 0;
            virtual void onUpdate(float delta) = 0;
            virtual void onRender() = 0;
            virtual void onEnd() = 0;
        public: // Methods
            void setInstance(Application* app);
            void run();
        public: // Static Methods
            static Application* getInstance();
            static unsigned int getFPS();
    };

    // This will only be defined by the main application.
    Application* createApplication();
}

#endif