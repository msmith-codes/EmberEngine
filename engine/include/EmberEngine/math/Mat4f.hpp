#ifndef EE_Mat4f_HPP
#define EE_Mat4f_HPP

namespace EmberEngine
{
    class Mat4f
    {
        private:
            float m[4][4];
        public:
            Mat4f();
            Mat4f(const Mat4f& other);
            Mat4f& operator=(const Mat4f& other);
        public: // -- Math -- //
            void initIdentity();
            Mat4f operator*(const Mat4f& other);
        public: // -- Getters -- //
            float* getMatrix();
            float get(int x, int y);
        public: // -- Setters -- //
            void setMatrix(float* matrix);
            void set(int x, int y, float value);
    };
}

#endif