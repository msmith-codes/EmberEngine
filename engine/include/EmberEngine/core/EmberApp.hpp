#ifndef EE_EmberApp_HPP
#define EE_EmberApp_HPP

#include <string>

namespace EmberEngine
{
    class EmberApp
    {
        public:
            EmberApp();
        protected:
            virtual void onCompose() = 0;
            virtual void onUpdate(float dt) = 0;
            virtual void onRender() = 0;
            virtual void onDispose() = 0;  
        public:
            void run();
        
    };
}

#endif