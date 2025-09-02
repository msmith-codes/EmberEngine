#pragma once

namespace Ember 
{
    class Scene
    {
        public:
            virtual void on_compose() {}
            virtual void on_update(float delta) {}
            virtual void on_render() {}
            virtual void on_dispose() {}
    };
}
