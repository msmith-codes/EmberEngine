#pragma once

namespace EmberEngine
{
    class Scene
    {
        private:
            bool active;
        public:
            Scene() = default;
            virtual ~Scene() = default;
        public:
            virtual void on_compose() {}
            virtual void on_dispose() {}
            virtual void on_update(float delta) {}
            virtual void on_render() {}
        public:
            bool is_active() const { return this->active; }
            void set_active(bool active) { this->active = active; }
    };
}

