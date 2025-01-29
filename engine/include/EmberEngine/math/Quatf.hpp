#ifndef EE_Quatf_HPP
#define EE_Quatf_HPP

namespace EmberEngine
{
    class Vec3f;
    class Quatf
    {
        private:
            float x;
            float y;
            float z;
            float w;
        public:
            Quatf();
            Quatf(float x, float y, float z, float w);
            Quatf(const Quatf& other);
            Quatf& operator=(const Quatf& other);
        public: // -- Math -- //
            float length();
            Quatf normalize();
            Quatf conjugate();

            Quatf operator*(const Quatf& other);
            Quatf operator*=(const Quatf& other);
            Quatf operator*(const Vec3f& other);
            Quatf operator*=(const Vec3f& other);
        public: // -- Getters -- //
            float getX() const;
            float getY() const;
            float getZ() const;
            float getW() const;
        public: // -- Setters -- //
            void setX(float x);
            void setY(float y);
            void setZ(float z);
            void setW(float w);
    };
}

#endif