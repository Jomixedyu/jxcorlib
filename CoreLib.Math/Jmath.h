#pragma once
#include <cmath>
#include <string>

namespace jmath
{
    const inline constexpr float pi = 3.1415926f;
    const inline constexpr float deg2rad = 0.017453292519943f;
    const inline constexpr float rad2deg = 57.295779513082320876798154814105f;

    template<typename T> constexpr T Radians(T degree) { return deg2rad * degree; }
    template<typename T> constexpr T Degrees(T rad) { return rad2deg * rad; }
    template<typename T> constexpr T Clamp(T x, T min, T max)
    {
        return std::min(std::max(x, min), max);
    }
    template<typename T>
    constexpr bool FloatEqual(T const& x, T const& y)
    {
        return std::abs(x - y) <= std::numeric_limits<T>::epsilon();
    }

    template<typename T>
    struct Vector4
    {
        T x, y, z, w;

        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

        const T* get_value_ptr() const { return &this->x; }

        static constexpr int column_count = 1;
        static constexpr int row_count = 4;

        T& operator[](int index) { return *(&x + index); }
        const T& operator[](int index) const { return *(&x + index); }

    };
    template<typename T> std::string to_string(const Vector4<T>& v)
    {
        std::string s;
        s.reserve(64);
        s.append("{x: "); s.append(std::to_string(v.x)); s.append(", ");
        s.append("y: "); s.append(std::to_string(v.y)); s.append(", ");
        s.append("z: "); s.append(std::to_string(v.z)); s.append(", ");
        s.append("w: "); s.append(std::to_string(v.w)); s.append("}");
        return s;
    }

    using Vector4f = Vector4<float>;
    using Vector4d = Vector4<double>;
    using Vector4i = Vector4<int>;

    template<typename T>
    struct Vector3
    {
        T x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

        const T* get_value_ptr() const { return &this->x; }

        static constexpr int column_count = 1;
        static constexpr int row_count = 3;

        T& operator[](int index) { return *(&x + index); }
        const T& operator[](int index) const { return *(&x + index); }

        static Vector3 StaticUp() { return Vector3{ T(0), T(1), T(0) }; }
        static Vector3 StaticRight() { return Vector3{ T(1), T(0), T(0) }; }
        //right handle?
        static Vector3 StaticForward() { return Vector3{ T(0), T(0), T(1) }; }
        static Vector3 StaticZero() { return Vector3{ T(0), T(0), T(0) }; }
        static Vector3 StaticOne() { return Vector3{ T(1), T(1), T(1) }; }

        Vector3& operator +=(Vector3 v) { x += v.x; y += v.y; z += v.z; return *this; }
        Vector3& operator +=(T v) { x += v; y += v; z += v; return *this; }
        Vector3& operator -=(Vector3 v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
        Vector3& operator -=(T v) { x -= v; y -= v; z -= v; return *this; }
        Vector3& operator *=(T v) { x *= v; y *= v; z *= v; return *this; }
        Vector3& operator /=(T v) { x /= v; y /= v; z /= v; return *this; }
        Vector3 operator-() { return Vector3(-x, -y, -z); }

        operator Vector4<T>() const { return Vector4<T>{ T(x), T(y), T(z), T(0) }; }
    };
    template<typename T> inline Vector3<T> operator+(Vector3<T> l, T r) { return Vector3<T>(l.x + r, l.y + r, l.z + r); }
    template<typename T> inline Vector3<T> operator+(T l, Vector3<T> r) { return Vector3<T>(l + r.x, l + r.y, l + r.z); }
    template<typename T> inline Vector3<T> operator+(Vector3<T> l, Vector3<T> r) { return Vector3<T>(l.x + r.x, l.y + r.y, l.z + r.z); }
    template<typename T> inline Vector3<T> operator-(Vector3<T> l, T r) { return Vector3<T>(l.x - r, l.y - r, l.z - r); }
    template<typename T> inline Vector3<T> operator-(T l, Vector3<T> r) { return Vector3<T>(l - r.x, l - r.y, l - r.z); }
    template<typename T> inline Vector3<T> operator-(Vector3<T> l, Vector3<T> r) { return Vector3<T>(l.x - r.x, l.y - r.y, l.z - r.z); }
    template<typename T> inline Vector3<T> operator/(Vector3<T> v3, T f) { return Vector3<T>(v3.x / f, v3.y / f, v3.z / f); }
    template<typename T> inline Vector3<T> operator/(T f, Vector3<T> v3) { return Vector3<T>(f / v3.x, f / v3.y, f / v3.z); }
    template<typename T> inline Vector3<T> operator*(Vector3<T> v3, T f) { return Vector3<T>(v3.x * f, v3.y * f, v3.z * f); }
    template<typename T> inline Vector3<T> operator*(T f, Vector3<T> v3) { return Vector3<T>(v3.x * f, v3.y * f, v3.z * f); }
    template<typename T> inline Vector3<T> operator*(Vector3<T> l, Vector3<T> r) { return Vector3<T>(l.x * r.x, l.y * r.y, l.z * r.z); }
    template<typename T> inline bool operator==(Vector3<T> l, Vector3<T> r) { return l.x == r.x && l.y && r.y && l.z && r.z; }

    template<typename T> std::string to_string(const Vector3<T>& v)
    {
        std::string s;
        s.reserve(64);
        s.append("{x: "); s.append(std::to_string(v.x)); s.append(", ");
        s.append("y: "); s.append(std::to_string(v.y)); s.append(", ");
        s.append("z: "); s.append(std::to_string(v.z)); s.append("}");
        return s;
    }

    using Vector3f = Vector3<float>;
    using Vector3d = Vector3<double>;
    using Vector3i = Vector3<int>;

    template<typename T>
    struct Vector2
    {
        T x, y;

        Vector2() : x(0), y(0) {}
        Vector2(T _x, T _y) : x(_x), y(_y) {}

        const T* get_value_ptr() const { return &this->x; }

        static constexpr int column_count = 1;
        static constexpr int row_count = 2;

        T& operator[](int index) { return *(&x + index); }
        const T& operator[](int index) const { return *(&x + index); }

        static Vector2 StaticZero() { return Vector2{ T(0), T(0) }; }
        static Vector2 StaticOne() { return Vector2{ T(1), T(1) }; }
        static Vector2 StaticUp() { return Vector2{ T(0), T(1) }; }
        static Vector2 StaticRight() { return Vector2{ T(1), T(0) }; }

        Vector2 operator-() { return Vector2(-x, -y); }
        Vector2 operator+=(Vector2 r) { x += r.x; y += r.y; return *this; }
        Vector2 operator+=(T r) { x += r; y += r; return *this; }
        Vector2 operator-=(Vector2 r) { x -= r.x; y -= r.y; return *this; }
        Vector2 operator-=(T r) { x -= r; y -= r; return *this; }
        Vector2 operator*=(T r) { x *= r; y *= r; return *this; }
        Vector2 operator/=(T r) { x /= r; y /= r; return *this; }

        operator Vector3<T>() const { return Vector3<T>{ x, y, T(0) }; }
    };


    template<typename T> inline Vector2<T> operator*(Vector2<T> a, T b) { return Vector2<T>(a.x * b, a.y * b); }
    template<typename T> inline Vector2<T> operator*(T a, Vector2<T> b) { return Vector2<T>(a * b.x, a * b.y); }
    template<typename T> inline Vector2<T> operator*(Vector2<T> a, Vector2<T> b) { return Vector2<T>(a.x * b.x, a.y * b.y); }
    template<typename T> inline Vector2<T> operator/(Vector2<T> a, T b) { return Vector2<T>(a.x / b, a.y / b); }
    template<typename T> inline Vector2<T> operator/(T a, Vector2<T> b) { return Vector2<T>(a / b.x, a / b.y); }
    template<typename T> inline Vector2<T> operator+(Vector2<T> a, T b) { return Vector2<T>(a.x + b, a.y + b); }
    template<typename T> inline Vector2<T> operator+(T a, Vector2<T> b) { return Vector2<T>(a + b.x, a + b.y); }
    template<typename T> inline Vector2<T> operator+(Vector2<T> a, Vector2<T> b) { return Vector2<T>(a.x + b.x, a.y + b.y); }
    template<typename T> inline Vector2<T> operator-(Vector2<T> a, T b) { return Vector2<T>(a.x - b, a.y - b); }
    template<typename T> inline Vector2<T> operator-(T a, Vector2<T> b) { return Vector2<T>(a - b.x, a - b.y); }
    template<typename T> inline Vector2<T> operator-(Vector2<T> a, Vector2<T> b) { return Vector2<T>(a.x - b.x, a.y - b.y); }
    template<typename T> inline bool operator==(Vector2<T> a, Vector2<T> b) { return a.x == b.x && a.y == b.y; }

    template<typename T> std::string to_string(Vector2<T> v)
    {
        std::string s;
        s.reserve(64);
        s.append("{x: "); s.append(std::to_string(v.x)); s.append(", ");
        s.append("y: "); s.append(std::to_string(v.y)); s.append("}");
        return s;
    }

    using Vector2f = Vector2<float>;
    using Vector2d = Vector2<double>;
    using Vector2i = Vector2<int>;

    template<typename T>
    struct Matrix2
    {
        Vector2<T> M[2];

        Matrix2(
            T x1, T y1,
            T x2, T y2) {
            M[0] = Vector2<T>(x1, x2);
            M[1] = Vector2<T>(y1, y2);
        }
        Matrix2(Vector2<T> x, Vector2<T> y) { M[0] = x; M[1] = y; }

        static constexpr int column_count = 2;
        static constexpr int row_count = 2;

        const T* get_value_ptr() const { return &M[0].x; }

        Vector2<T>& operator[](int i) { return M[i]; }
        const Vector2<T>& operator[](int i) const { return M[i]; }

        static Matrix2 StaticScalar(T k = T(1))
        {
            return Matrix2(
                T(k), T(0),
                T(0), T(k)
            );
        }
    };

    template<typename T>
    struct Matrix3
    {
        Vector3<T> M[3];

        const T* get_value_ptr() const { return &M[0].x; }

        Matrix3(
            T x1, T y1, T z1,
            T x2, T y2, T z2,
            T x3, T y3, T z3
        ) {
            M[0] = Vector3<T>(x1, x2, x3);
            M[1] = Vector3<T>(y1, y2, y3);
            M[2] = Vector3<T>(z1, z2, z3);
        }
        Matrix3(Vector3<T> x, Vector3<T> y, Vector3<T> z) { M[0] = x; M[1] = y; M[2] = z; }

        static constexpr int column_count = 3;
        static constexpr int row_count = 3;

        Vector3<T>& operator[](int i) { return M[i]; }
        const Vector3<T>& operator[](int i) const { return M[i]; }

        static Matrix3 StaticScalar(T k = T(1))
        {
            return Matrix3(
                T(k), T(0), T(0),
                T(0), T(k), T(0),
                T(0), T(0), T(k)
            );
        }
    };

    template<typename T>
    struct Matrix4
    {
        Vector4<T> M[4];

        const T* get_value_ptr() const { return &M[0].x; }

        Matrix4(
            T x1, T y1, T z1, T w1,
            T x2, T y2, T z2, T w2,
            T x3, T y3, T z3, T w3,
            T x4, T y4, T z4, T w4
        ) {
            M[0] = Vector4<T>(x1, x2, x3, x4);
            M[1] = Vector4<T>(y1, y2, y3, y4);
            M[2] = Vector4<T>(z1, z2, z3, z4);
            M[3] = Vector4<T>(w1, w2, w3, w4);
        }
        Matrix4(const Vector4<T>& x, const Vector4<T>& y, const Vector4<T>& z, const Vector4<T>& w) { M[0] = x; M[1] = y; M[2] = z; M[3] = w; }

        static constexpr int column_count = 4;
        static constexpr int row_count = 4;

        Vector4<T>& operator[](int i) { return M[i]; }
        const Vector4<T>& operator[](int i) const { return M[i]; }

        static Matrix4 StaticScalar(T k = T(1))
        {
            return Matrix4(
                T(k), T(0), T(0), T(0),
                T(0), T(k), T(0), T(0),
                T(0), T(0), T(k), T(0),
                T(0), T(0), T(0), T(k)
            );
        }
    };

    using Matrix4f = Matrix4<float>;
    using Matrix4d = Matrix4<double>;
    using Matrix4i = Matrix4<int>;

    template<typename T>
    struct Quaternion
    {
        T w, x, y, z;

        Quaternion() : w(1), x(0), y(0), z(0) {}
        Quaternion(T _w, T _x, T _y, T _z) : w(_w), x(_x), y(_y), z(_z) {}

        Quaternion(const Vector3<T>& euler) { SetEuler(euler); }

        void SetEuler(Vector3<T> euler) {
            Vector3<T> in = Radians(euler) * T(0.5);
            Vector3<T> c = Vector3<T>{ std::cos(in.x), std::cos(in.y), std::cos(in.z) };
            Vector3<T> s = Vector3<T>{ std::sin(in.x), std::sin(in.y), std::sin(in.z) };

            this->w = c.x * c.y * c.z + s.x * s.y * s.z;
            this->x = s.x * c.y * c.z - c.x * s.y * s.z;
            this->y = c.x * s.y * c.z + s.x * c.y * s.z;
            this->z = c.x * c.y * s.z - s.x * s.y * c.z;
        }
        Vector3<T> GetEuler() const
        {
            return Degrees(Vector3<T>{ pitch(*this), yaw(*this), roll(*this) });
            //return Internal_MakePositive(Vector3<T>{ Degrees( pitch(*this)), Degrees(yaw(*this)), Degrees(roll(*this)) });
        }
    private:
        inline static Vector3<T> Internal_MakePositive(Vector3<T> euler)
        {
            float num = -0.005729578f;
            float num2 = 360.f + num;
            if (euler.x < num) { euler.x += 360.f; }
            else if (euler.x > num2) { euler.x -= 360.f; }
            if (euler.y < num) { euler.y += 360.f; }
            else if (euler.y > num2) { euler.y -= 360.f; }
            if (euler.z < num) { euler.z += 360.f; }
            else if (euler.z > num2) { euler.z -= 360.f; }
            return euler;
        }
        inline static T roll(const Quaternion& q)
        {
            return static_cast<T>(std::atan2(static_cast<T>(2) * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z));
        }
        inline static T pitch(const Quaternion& q)
        {
            T const y = static_cast<T>(2) * (q.y * q.z + q.w * q.x);
            T const x = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
            if (FloatEqual(x, T(0)) && FloatEqual(y, T(0))) //avoid atan2(0,0) - handle singularity - Matiis
                return static_cast<T>(static_cast<T>(2) * std::atan2(q.x, q.w));

            return static_cast<T>(std::atan2(y, x));

            //return (T)std::atan2(T(2) * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
        }
        inline static T yaw(const Quaternion& q)
        {
            return std::asin(Clamp(static_cast<T>(-2) * (q.x * q.z - q.w * q.y), static_cast<T>(-1), static_cast<T>(1)));
        }
    };
    template<typename T>
    inline std::string to_string(const Quaternion<T>& v)
    {
        std::string s;
        s.reserve(64);
        s.append("{w: "); s.append(std::to_string(v.w)); s.append(", ");
        s.append("x: "); s.append(std::to_string(v.x)); s.append(", ");
        s.append("y: "); s.append(std::to_string(v.y)); s.append(", ");
        s.append("z: "); s.append(std::to_string(v.z)); s.append("}");
        return s;
    }
    using Quat4f = Quaternion<float>;
    using Quat4d = Quaternion<double>;

}
