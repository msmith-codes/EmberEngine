#pragma once

namespace EmberEngine
{
    class Application
    {
        public:
            Application() = default;
            virtual ~Application() = default;
        public:
            void run();
        protected:
            virtual void on_compose() = 0;
            virtual void on_dispose() = 0;
            virtual void on_update(float delta) = 0;
            virtual void on_render() = 0;
            virtual bool is_running() = 0;
    };

    Application* create_application();
}