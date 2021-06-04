#pragma once

#include <cmath>

constexpr float PI      = 3.14159265359f;
constexpr float EPSILON = 1e-6f;

constexpr float sqr(float value) {
    return value * value;
}

constexpr float lerp(float from, float to, float factor) {
    return from + (to - from) * factor;
}

constexpr float clamp(float value, float min, float max) {
    return value < min ? min : (max < value ? max : value);
}

constexpr float equal(float a, float b, float epsilon = EPSILON) {
    return a - b > -epsilon && a - b < epsilon;
}

constexpr float degrees(float radians) {
    return radians / PI * 180.f;
}

constexpr float radians(float degrees) {
    return degrees / 180.f * PI;
}

class float2 {
public:
    explicit constexpr float2(float all = 0.f)
        : x(all), y(all)
    {
    }
    
    explicit constexpr float2(float x, float y) 
        : x(x), y(y)
    {
    }

    constexpr float* begin() {
        return data;
    }

    constexpr const float* begin() const {
        return data;
    }

    constexpr float* end() {
        return data + 2;
    }

    constexpr const float* end() const {
        return data + 2;
    }

    constexpr float2 operator+(const float2& rhs) const {
        return float2(x + rhs.x, y + rhs.y);
    }

    constexpr float2 operator-(const float2& rhs) const {
        return float2(x - rhs.x, y - rhs.y);
    }

    constexpr float2 operator*(const float2& rhs) const {
        return float2(x * rhs.x, y * rhs.y);
    }

    constexpr float2 operator/(const float2& rhs) const {
        return float2(x / rhs.x, y / rhs.y);
    }

    constexpr float2& operator+=(const float2& value) {
        x += value.x;
        y += value.y;

        return *this;
    }

    constexpr float2& operator-=(const float2& value) {
        x -= value.x;
        y -= value.y;

        return *this;
    }

    constexpr float2& operator*=(const float2& value) {
        x *= value.x;
        y *= value.y;

        return *this;
    }

    constexpr float2& operator/=(const float2& value) {
        x /= value.x;
        y /= value.y;

        return *this;
    }

    constexpr float2& operator*=(float value) {
        x *= value;
        y *= value;

        return *this;
    }

    constexpr float2& operator/=(float value) {
        x /= value;
        y /= value;

        return *this;
    }

    constexpr float& operator[](size_t index) {
        return data[index];
    }

    constexpr const float& operator[](size_t index) const {
        return data[index];
    }

    constexpr float2 operator-() const {
        return float2(-x, -y);
    }

    constexpr float2 operator*(float rhs) const {
        return float2(x * rhs, y * rhs);
    }

    constexpr float2 operator/(float rhs) const {
        return float2(x / rhs, y / rhs);
    }

    constexpr bool operator==(const float2& value) const {
        return x == value.x && y == value.y;
    }

    constexpr bool operator!=(const float2& value) const {
        return x != value.x || y != value.y;
    }

    constexpr float* operator&() {
        return data;
    }

    constexpr const float* operator&() const {
        return data;
    }

    friend constexpr float2 operator*(float lhs, const float2& rhs) {
        return rhs * lhs;
    }

    union {
        struct {
            float x, y;
        };

        struct {
            float r, g;
        };

        float data[2];
    };
};

constexpr float dot(const float2& lhs, const float2& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

constexpr float square_length(const float2& value) {
    return sqr(value.x) + sqr(value.y);
}

inline float length(const float2& value) {
    return std::sqrt(square_length(value));
}

constexpr float square_distance(const float2& lhs, const float2& rhs) {
    return sqr(lhs.x - rhs.x) + sqr(lhs.y - rhs.y);
}

inline float distance(const float2& lhs, const float2& rhs) {
    return std::sqrt(square_distance(lhs, rhs));
}

inline float2 normalize(const float2& value) {
    float multiplier = 1.f / length(value);
    return float2(value.x * multiplier, value.y * multiplier);
}

constexpr float2 lerp(const float2& from, const float2& to, float factor) {
    return from + (to - from) * factor;
}

constexpr float2 clamp(const float2& value, float min, float max) {
    return float2(clamp(value.x, min, max), clamp(value.y, min, max));
}

constexpr float2 clamp(const float2& value, const float2& min, const float2& max) {
    return float2(clamp(value.x, min.x, max.x), clamp(value.y, min.y, max.y));
}

inline float2 reflect(const float2& vector, const float2& normal) {
    return vector - 2.f * dot(vector, normal) * normal;
}

constexpr bool equal(const float2& lhs, const float2& rhs, float epsilon = EPSILON) {
    return equal(lhs.x, rhs.x, epsilon) && equal(lhs.y, rhs.y, epsilon);
}

constexpr bool equal(const float2& lhs, float rhs, float epsilon = EPSILON) {
    return equal(lhs.x, rhs, epsilon) && equal(lhs.y, rhs, epsilon);
}

inline bool isfinite(const float2& value) {
    return std::isfinite(value.x) && std::isfinite(value.y);
}

class float3 {
public:
    explicit constexpr float3(float all = 0.f)
        : x(all), y(all), z(all)
    {
    }


    explicit constexpr float3(float x, float y, float z)
        : x(x), y(y), z(z)
    {
    }

    explicit constexpr float3(const float2& value, float z)
        : x(value.x), y(value.y), z(z)
    {
    }

    constexpr float* begin() {
        return data;
    }

    constexpr const float* begin() const {
        return data;
    }

    constexpr float* end() {
        return data + 3;
    }

    constexpr const float* end() const {
        return data + 3;
    }

    constexpr float3 operator+(const float3& rhs) const {
        return float3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    constexpr float3 operator-(const float3& rhs) const {
        return float3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    constexpr float3 operator*(const float3& rhs) const {
        return float3(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    constexpr float3 operator/(const float3& rhs) const {
        return float3(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    constexpr float3& operator+=(const float3& value) {
        x += value.x;
        y += value.y;
        z += value.z;

        return *this;
    }

    constexpr float3& operator-=(const float3& value) {
        x -= value.x;
        y -= value.y;
        z -= value.z;

        return *this;
    }

    constexpr float3& operator*=(const float3& value) {
        x *= value.x;
        y *= value.y;
        z *= value.z;

        return *this;
    }

    constexpr float3& operator/=(const float3& value) {
        x /= value.x;
        y /= value.y;
        z /= value.z;

        return *this;
    }

    constexpr float3& operator*=(float value) {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    constexpr float3& operator/=(float value) {
        x /= value;
        y /= value;
        z /= value;

        return *this;
    }

    constexpr float& operator[](size_t index) {
        return data[index];
    }

    constexpr const float& operator[](size_t index) const {
        return data[index];
    }

    constexpr float3 operator-() const {
        return float3(-x, -y, -z);
    }

    constexpr float3 operator*(float rhs) const {
        return float3(x * rhs, y * rhs, z * rhs);
    }

    constexpr float3 operator/(float rhs) const {
        return float3(x / rhs, y / rhs, z / rhs);
    }

    constexpr bool operator==(const float3& value) const {
        return x == value.x && y == value.y && z == value.z;
    }

    constexpr bool operator!=(const float3& value) const {
        return x != value.x || y != value.y || z != value.z;
    }

    constexpr float* operator&() {
        return data;
    }

    constexpr const float* operator&() const {
        return data;
    }

    explicit constexpr operator float2() const {
        return float2(x, y);
    }

    friend constexpr float3 operator*(float lhs, const float3& rhs) {
        return rhs * lhs;
    }

    union {
        struct {
            float x, y, z;
        };

        struct {
            float r, g, b;
        };

        float data[3];
    };
};

constexpr float dot(const float3& lhs, const float3& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

constexpr float3 cross(const float3& lhs, const float3& rhs) {
    return float3(lhs.y * rhs.z - rhs.y * lhs.z, 
                  lhs.z * rhs.x - lhs.x * rhs.z, 
                  lhs.x * rhs.y - lhs.y * rhs.x);
}

constexpr float square_length(const float3& value) {
    return sqr(value.x) + sqr(value.y) + sqr(value.z);
}

inline float length(const float3& value) {
    return std::sqrt(square_length(value));
}

constexpr float square_distance(const float3& lhs, const float3& rhs) {
    return sqr(lhs.x - rhs.x) + sqr(lhs.y - rhs.y) + sqr(lhs.z - rhs.z);
}

inline float distance(const float3& lhs, const float3& rhs) {
    return std::sqrt(square_distance(lhs, rhs));
}

inline float3 normalize(const float3& value) {
    float multiplier = 1.f / length(value);
    return float3(value.x * multiplier, value.y * multiplier, value.z * multiplier);
}

constexpr float3 lerp(const float3& from, const float3& to, float factor) {
    return from + (to - from) * factor;
}

constexpr float3 clamp(const float3& value, float min, float max) {
    return float3(clamp(value.x, min, max), clamp(value.y, min, max), clamp(value.z, min, max));
}

constexpr float3 clamp(const float3& value, const float3& min, const float3& max) {
    return float3(clamp(value.x, min.x, max.x), clamp(value.y, min.y, max.y), clamp(value.z, min.z, max.z));
}

inline float3 reflect(const float3& vector, const float3& normal) {
    return vector - 2.f * dot(vector, normal) * normal;
}

constexpr bool equal(const float3& lhs, const float3& rhs, float epsilon = EPSILON) {
    return equal(lhs.x, rhs.x, epsilon) && equal(lhs.y, rhs.y, epsilon) && equal(lhs.z, rhs.z, epsilon);
}

constexpr bool equal(const float3& lhs, float rhs, float epsilon = EPSILON) {
    return equal(lhs.x, rhs, epsilon) && equal(lhs.y, rhs, epsilon) && equal(lhs.z, rhs, epsilon);
}

inline bool isfinite(const float3& value) {
    return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}

class float2x2 {
public:
    explicit constexpr float2x2()
        : _11(1.f), _12(0.f)
        , _21(0.f), _22(1.f)
    {
    }

    explicit constexpr float2x2(float _11, float _12,
                                float _21, float _22)
        : _11(_11), _12(_12)
        , _21(_21), _22(_22)
    {
    }

    constexpr float2x2(const float2& _r0, const float2& _r1) 
        : _r0(_r0), _r1(_r1)
    {
    }

    constexpr float* begin() {
        return cells;
    }

    constexpr const float* begin() const {
        return cells;
    }

    constexpr float* end() {
        return cells + 4;
    }

    constexpr const float* end() const {
        return cells + 4;
    }

    constexpr float2x2 operator+(const float2x2& rhs) const {
        return float2x2(_11 + rhs._11, _12 + rhs._12, 
                        _21 + rhs._21, _22 + rhs._22);
    }

    constexpr float2x2 operator-(const float2x2& rhs) const {
        return float2x2(_11 - rhs._11, _12 - rhs._12, 
                        _21 - rhs._21, _22 - rhs._22);
    }

    constexpr float2x2 operator*(const float2x2& rhs) const {
        return float2x2(_11 * rhs._11 + _12 * rhs._21,
                        _11 * rhs._12 + _12 * rhs._22,
                        _21 * rhs._11 + _22 * rhs._21,
                        _21 * rhs._12 + _22 * rhs._22);
    }

    constexpr float2x2& operator+=(const float2x2& value) {
        _11 += value._11;
        _12 += value._12;
        _21 += value._21;
        _22 += value._22;

        return *this;
    }

    constexpr float2x2& operator-=(const float2x2& value) {
        _11 -= value._11;
        _12 -= value._12;
        _21 -= value._21;
        _22 -= value._22;

        return *this;
    }

    constexpr float2x2& operator*=(const float2x2& value) {
        float2x2 temp(
            _11 * value._11 + _12 * value._21,
            _11 * value._12 + _12 * value._22,
            _21 * value._11 + _22 * value._21,
            _21 * value._12 + _22 * value._22
        );

        return *this = temp;
    }

    constexpr float2x2& operator*=(float value) {
        _11 *= value;
        _12 *= value;
        _21 *= value;
        _22 *= value;

        return *this;
    }

    constexpr float2x2& operator/=(float value) {
        _11 /= value;
        _12 /= value;
        _21 /= value;
        _22 /= value;

        return *this;
    }

    constexpr float2& operator[](size_t index) {
        return rows[index];
    }

    constexpr const float2& operator[](size_t index) const {
        return rows[index];
    }

    constexpr float2x2 operator-() const {
        return float2x2(-_11, -_12,
                        -_21, -_22);
    }

    constexpr float2 operator*(const float2& rhs) const {
        return float2(_11 * rhs.x + _12 * rhs.y,
                      _21 * rhs.x + _22 * rhs.y);
    }

    constexpr float2x2 operator*(float rhs) const {
        return float2x2(_11 * rhs, _12 * rhs, _21 * rhs, _22 * rhs);
    }

    constexpr float2x2 operator/(float rhs) const {
        return float2x2(_11 / rhs, _12 / rhs, _21 / rhs, _22 / rhs);
    }

    constexpr bool operator==(const float2x2& value) const {
        return _11 == value._11 && _12 == value._12 &&
               _21 == value._21 && _22 == value._22;
    }

    constexpr bool operator!=(const float2x2& value) const {
        return _11 != value._11 || _12 != value._12 ||
               _21 != value._21 || _22 != value._22;
    }

    constexpr float* operator&() {
        return cells;
    }

    constexpr const float* operator&() const {
        return cells;
    }

    friend constexpr float2 operator*(const float2& lhs, const float2x2& rhs) {
        return float2(lhs.x * rhs._11 + lhs.y * rhs._21,
                      lhs.x * rhs._12 + lhs.y * rhs._22);
    }

    friend constexpr float2x2 operator*(float lhs, const float2x2& rhs) {
        return rhs * lhs;
    }

    union {
        struct {
            float _m00, _m01;
            float _m10, _m11;
        };

        struct {
            float _11, _12;
            float _21, _22;
        };

        struct {
            float2 _r0;
            float2 _r1;
        };

        float2 rows[2];
        float cells[4];
    };
};

constexpr float2x2 transpose(const float2x2& value) {
    return float2x2(value._11, value._21,
                    value._12, value._22);
}

constexpr float2x2 inverse(const float2x2& value) {
    float det = value._11 * value._22 - value._12 * value._21;

    if (equal(det, 0.f)) {
        return float2x2();
    }

    float multiplier = 1.f / det;
    return float2x2( value._22 * multiplier, -value._12 * multiplier,
                    -value._21 * multiplier,  value._11 * multiplier);
}

constexpr bool equal(const float2x2& lhs, const float2x2& rhs, float epsilon = EPSILON) {
    return equal(lhs._11, rhs._11, epsilon) && equal(lhs._12, rhs._12, epsilon) &&
           equal(lhs._21, rhs._21, epsilon) && equal(lhs._22, rhs._22, epsilon);
}

inline bool isfinite(const float2x2& value) {
    return std::isfinite(value._11) && std::isfinite(value._12) &&
           std::isfinite(value._21) && std::isfinite(value._22);
}

class float3x3 {
public:
    explicit constexpr float3x3()
        : _11(1.f), _12(0.f), _13(0.f)
        , _21(0.f), _22(1.f), _23(0.f)
        , _31(0.f), _32(0.f), _33(1.f)
    {
    }

    explicit constexpr float3x3(float _11, float _12, float _13,
                                float _21, float _22, float _23,
                                float _31, float _32, float _33)
        : _11(_11), _12(_12), _13(_13)
        , _21(_21), _22(_22), _23(_23)
        , _31(_31), _32(_32), _33(_33)
    {
    }

    explicit constexpr float3x3(const float2x2& value)
        : _11(value._11), _12(value._12), _13(0.f)
        , _21(value._21), _22(value._22), _23(0.f)
        , _31(0.f),       _32(0.f),       _33(1.f)
    {
    }

    constexpr float3x3(const float3& _r0, const float3& _r1, const float3& _r2)
        : _r0(_r0), _r1(_r1), _r2(_r2)
    {
    }

    constexpr float* begin() {
        return cells;
    }

    constexpr const float* begin() const {
        return cells;
    }

    constexpr float* end() {
        return cells + 9;
    }

    constexpr const float* end() const {
        return cells + 9;
    }

    constexpr float3x3 operator+(const float3x3& rhs) const {
        return float3x3(_11 + rhs._11, _12 + rhs._12, _13 + rhs._13,
                        _21 + rhs._21, _22 + rhs._22, _23 + rhs._23,
                        _31 + rhs._31, _32 + rhs._32, _33 + rhs._33);
    }

    constexpr float3x3 operator-(const float3x3& rhs) const {
        return float3x3(_11 - rhs._11, _12 - rhs._12, _13 - rhs._13,
                        _21 - rhs._21, _22 - rhs._22, _23 - rhs._23,
                        _31 - rhs._31, _32 - rhs._32, _33 - rhs._33);
    }

    constexpr float3x3 operator*(const float3x3& rhs) const {
        return float3x3(_11 * rhs._11 + _12 * rhs._21 + _13 * rhs._31,
                        _11 * rhs._12 + _12 * rhs._22 + _13 * rhs._32,
                        _11 * rhs._13 + _12 * rhs._23 + _13 * rhs._33,
                        _21 * rhs._11 + _22 * rhs._21 + _23 * rhs._31,
                        _21 * rhs._12 + _22 * rhs._22 + _23 * rhs._32,
                        _21 * rhs._13 + _22 * rhs._23 + _23 * rhs._33,
                        _31 * rhs._11 + _32 * rhs._21 + _33 * rhs._31,
                        _31 * rhs._12 + _32 * rhs._22 + _33 * rhs._32,
                        _31 * rhs._13 + _32 * rhs._23 + _33 * rhs._33);
    }

    constexpr float3x3& operator+=(const float3x3& value) {
        _11 += value._11;
        _12 += value._12;
        _13 += value._13;
        _21 += value._21;
        _22 += value._22;
        _23 += value._23;
        _31 += value._31;
        _32 += value._32;
        _33 += value._33;

        return *this;
    }

    constexpr float3x3& operator-=(const float3x3& value) {
        _11 -= value._11;
        _12 -= value._12;
        _13 -= value._13;
        _21 -= value._21;
        _22 -= value._22;
        _23 -= value._23;
        _31 -= value._31;
        _32 -= value._32;
        _33 -= value._33;

        return *this;
    }

    constexpr float3x3& operator*=(const float3x3& value) {
        float3x3 temp(
            _11 * value._11 + _12 * value._21 + _13 * value._31,
            _11 * value._12 + _12 * value._22 + _13 * value._32,
            _11 * value._13 + _12 * value._23 + _13 * value._33,
            _21 * value._11 + _22 * value._21 + _23 * value._31,
            _21 * value._12 + _22 * value._22 + _23 * value._32,
            _21 * value._13 + _22 * value._23 + _23 * value._33,
            _31 * value._11 + _32 * value._21 + _33 * value._31,
            _31 * value._12 + _32 * value._22 + _33 * value._32,
            _31 * value._13 + _32 * value._23 + _33 * value._33
        );

        return *this = temp;
    }

    constexpr float3x3& operator*=(float value) {
        _11 *= value;
        _12 *= value;
        _13 *= value;
        _21 *= value;
        _22 *= value;
        _23 *= value;
        _31 *= value;
        _32 *= value;
        _33 *= value;

        return *this;
    }

    constexpr float3x3& operator/=(float value) {
        _11 /= value;
        _12 /= value;
        _13 /= value;
        _21 /= value;
        _22 /= value;
        _23 /= value;
        _31 /= value;
        _32 /= value;
        _33 /= value;

        return *this;
    }

    constexpr float3& operator[](size_t index) {
        return rows[index];
    }

    constexpr const float3& operator[](size_t index) const {
        return rows[index];
    }

    constexpr float3x3 operator-() const {
        return float3x3(-_11, -_12, -_13,
                        -_21, -_22, -_23,
                        -_31, -_32, -_33);
    }

    constexpr float3x3 operator*(float rhs) const {
        return float3x3(_11 * rhs, _12 * rhs, _13 * rhs,
                        _21 * rhs, _22 * rhs, _23 * rhs,
                        _31 * rhs, _32 * rhs, _33 * rhs);
    }

    constexpr float3 operator*(const float3& rhs) const {
        return float3(_11 * rhs.x + _12 * rhs.y + _13 * rhs.z,
                      _21 * rhs.x + _22 * rhs.y + _23 * rhs.z,
                      _31 * rhs.x + _32 * rhs.y + _33 * rhs.z);
    }

    constexpr float3x3 operator/(float rhs) const {
        return float3x3(_11 / rhs, _12 / rhs, _13 / rhs,
                        _21 / rhs, _22 / rhs, _23 / rhs,
                        _31 / rhs, _32 / rhs, _33 / rhs);
    }

    constexpr bool operator==(const float3x3& value) const {
        return _11 == value._11 && _12 == value._12 && _13 == value._13 &&
               _21 == value._21 && _22 == value._22 && _23 == value._23 &&
               _31 == value._31 && _32 == value._32 && _33 == value._33;
    }

    constexpr bool operator!=(const float3x3& value) const {
        return _11 != value._11 || _12 != value._12 || _13 != value._13 ||
               _21 != value._21 || _22 != value._22 || _23 != value._23 ||
               _31 != value._31 || _32 != value._32 || _33 != value._33;
    }

    constexpr float* operator&() {
        return cells;
    }

    constexpr const float* operator&() const {
        return cells;
    }

    friend constexpr float3x3 operator*(float lhs, const float3x3& rhs) {
        return rhs * lhs;
    }

    friend constexpr float3 operator*(const float3& lhs, const float3x3& rhs) {
        return float3(lhs.x * rhs._11 + lhs.y * rhs._21 + lhs.z * rhs._31,
                      lhs.x * rhs._12 + lhs.y * rhs._22 + lhs.z * rhs._32,
                      lhs.x * rhs._13 + lhs.y * rhs._23 + lhs.z * rhs._33);
    }

    union {
        struct {
            float _m00, _m01, _m02;
            float _m10, _m11, _m12;
            float _m20, _m21, _m22;
        };

        struct {
            float _11, _12, _13;
            float _21, _22, _23;
            float _31, _32, _33;
        };

        struct {
            float3 _r0;
            float3 _r1;
            float3 _r2;
        };

        float3 rows[3];
        float cells[9];
    };
};

constexpr float3x3 transpose(const float3x3& value) {
    return float3x3(value._11, value._21, value._31,
                    value._12, value._22, value._32,
                    value._13, value._23, value._33);
}

constexpr float3x3 inverse(const float3x3& value) {
    float3x3 result(value._33 * value._22 - value._23 * value._32,
                    value._13 * value._32 - value._33 * value._12,
                    value._23 * value._12 - value._13 * value._22,
                    value._23 * value._31 - value._33 * value._21,
                    value._33 * value._11 - value._13 * value._31,
                    value._13 * value._21 - value._23 * value._11,
                    value._21 * value._32 - value._31 * value._22,
                    value._31 * value._12 - value._11 * value._32,
                    value._11 * value._22 - value._21 * value._12);

    float det = value._11 * result._11 + value._21 * result._12 + value._31 * result._13;
    if (equal(det, 0.f)) {
        return float3x3();
    }

    float factor = 1.f / det;
    result._11 *= factor;
    result._12 *= factor;
    result._13 *= factor;
    result._21 *= factor;
    result._22 *= factor;
    result._23 *= factor;
    result._31 *= factor;
    result._32 *= factor;
    result._33 *= factor;

    return result;
}

constexpr bool equal(const float3x3& lhs, const float3x3& rhs, float epsilon = EPSILON) {
    return equal(lhs._11, rhs._11, epsilon) &&
           equal(lhs._12, rhs._12, epsilon) &&
           equal(lhs._13, rhs._13, epsilon) &&
           equal(lhs._21, rhs._21, epsilon) &&
           equal(lhs._22, rhs._22, epsilon) &&
           equal(lhs._23, rhs._23, epsilon) &&
           equal(lhs._31, rhs._31, epsilon) &&
           equal(lhs._32, rhs._32, epsilon) &&
           equal(lhs._33, rhs._33, epsilon);
}

inline bool isfinite(const float3x3& value) {
    return std::isfinite(value._11) && std::isfinite(value._12) && std::isfinite(value._13) &&
           std::isfinite(value._21) && std::isfinite(value._22) && std::isfinite(value._23) &&
           std::isfinite(value._31) && std::isfinite(value._32) && std::isfinite(value._33);
}

class float4x4 {
public:
    static float4x4 rotation(const float3& axis, float angle);
    static float4x4 scale(const float3& scale);
    static float4x4 translation(const float3& translation);

    static float4x4 perspective(float fov_y, float aspect, float z_near, float z_far);

    explicit constexpr float4x4()
        : _11(1.f), _12(0.f), _13(0.f), _14(0.f)
        , _21(0.f), _22(1.f), _23(0.f), _24(0.f)
        , _31(0.f), _32(0.f), _33(1.f), _34(0.f)
        , _41(0.f), _42(0.f), _43(0.f), _44(1.f)
    {
    }

    explicit constexpr float4x4(float _11, float _12, float _13, float _14,
                                float _21, float _22, float _23, float _24,
                                float _31, float _32, float _33, float _34,
                                float _41, float _42, float _43, float _44)
        : _11(_11), _12(_12), _13(_13), _14(_14)
        , _21(_21), _22(_22), _23(_23), _24(_24)
        , _31(_31), _32(_32), _33(_33), _34(_34)
        , _41(_41), _42(_42), _43(_43), _44(_44)
    {
    }

    explicit constexpr float4x4(const float2x2& value)
        : _11(value._11), _12(value._12), _13(0.f), _14(0.f)
        , _21(value._21), _22(value._22), _23(0.f), _24(0.f)
        , _31(0.f),       _32(0.f),       _33(1.f), _34(0.f)
        , _41(0.f),       _42(0.f),       _43(0.f), _44(1.f)
    {
    }

    explicit constexpr float4x4(const float3x3& value)
        : _11(value._11), _12(value._12), _13(value._13), _14(0.f)
        , _21(value._21), _22(value._22), _23(value._23), _24(0.f)
        , _31(value._31), _32(value._32), _33(value._33), _34(0.f)
        , _41(0.f),       _42(0.f),       _43(0.f),       _44(1.f)
    {
    }

    constexpr float* begin() {
        return cells;
    }

    constexpr const float* begin() const {
        return cells;
    }

    constexpr float* end() {
        return cells + 16;
    }

    constexpr const float* end() const {
        return cells + 16;
    }

    constexpr float4x4 operator+(const float4x4& rhs) const {
        return float4x4(_11 + rhs._11, _12 + rhs._12, _13 + rhs._13, _14 + rhs._14,
                        _21 + rhs._21, _22 + rhs._22, _23 + rhs._23, _24 + rhs._24,
                        _31 + rhs._31, _32 + rhs._32, _33 + rhs._33, _34 + rhs._34,
                        _41 + rhs._41, _42 + rhs._42, _43 + rhs._43, _44 + rhs._44);
    }

    constexpr float4x4 operator-(const float4x4& rhs) const {
        return float4x4(_11 - rhs._11, _12 - rhs._12, _13 - rhs._13, _14 - rhs._14,
                        _21 - rhs._21, _22 - rhs._22, _23 - rhs._23, _24 - rhs._24,
                        _31 - rhs._31, _32 - rhs._32, _33 - rhs._33, _34 - rhs._34,
                        _41 - rhs._41, _42 - rhs._42, _43 - rhs._43, _44 - rhs._44);
    }

    constexpr float4x4 operator*(const float4x4& rhs) const {
        return float4x4(_11 * rhs._11 + _12 * rhs._21 + _13 * rhs._31 + _14 * rhs._41,
                        _11 * rhs._12 + _12 * rhs._22 + _13 * rhs._32 + _14 * rhs._42,
                        _11 * rhs._13 + _12 * rhs._23 + _13 * rhs._33 + _14 * rhs._43,
                        _11 * rhs._14 + _12 * rhs._24 + _13 * rhs._34 + _14 * rhs._44,
                        _21 * rhs._11 + _22 * rhs._21 + _23 * rhs._31 + _24 * rhs._41,
                        _21 * rhs._12 + _22 * rhs._22 + _23 * rhs._32 + _24 * rhs._42,
                        _21 * rhs._13 + _22 * rhs._23 + _23 * rhs._33 + _24 * rhs._43,
                        _21 * rhs._14 + _22 * rhs._24 + _23 * rhs._34 + _24 * rhs._44,
                        _31 * rhs._11 + _32 * rhs._21 + _33 * rhs._31 + _34 * rhs._41,
                        _31 * rhs._12 + _32 * rhs._22 + _33 * rhs._32 + _34 * rhs._42,
                        _31 * rhs._13 + _32 * rhs._23 + _33 * rhs._33 + _34 * rhs._43,
                        _31 * rhs._14 + _32 * rhs._24 + _33 * rhs._34 + _34 * rhs._44,
                        _41 * rhs._11 + _42 * rhs._21 + _43 * rhs._31 + _44 * rhs._41,
                        _41 * rhs._12 + _42 * rhs._22 + _43 * rhs._32 + _44 * rhs._42,
                        _41 * rhs._13 + _42 * rhs._23 + _43 * rhs._33 + _44 * rhs._43,
                        _41 * rhs._14 + _42 * rhs._24 + _43 * rhs._34 + _44 * rhs._44);
    }

    constexpr float4x4& operator+=(const float4x4& value) {
        _11 += value._11;
        _12 += value._12;
        _13 += value._13;
        _14 += value._14;
        _21 += value._21;
        _22 += value._22;
        _23 += value._23;
        _24 += value._24;
        _31 += value._31;
        _32 += value._32;
        _33 += value._33;
        _34 += value._34;
        _41 += value._41;
        _42 += value._42;
        _43 += value._43;
        _44 += value._44;

        return *this;
    }

    constexpr float4x4& operator-=(const float4x4& value) {
        _11 -= value._11;
        _12 -= value._12;
        _13 -= value._13;
        _14 -= value._14;
        _21 -= value._21;
        _22 -= value._22;
        _23 -= value._23;
        _24 -= value._24;
        _31 -= value._31;
        _32 -= value._32;
        _33 -= value._33;
        _34 -= value._34;
        _41 -= value._41;
        _42 -= value._42;
        _43 -= value._43;
        _44 -= value._44;

        return *this;
    }

    constexpr float4x4& operator*=(const float4x4& value) {
        float4x4 temp(
            _11 * value._11 + _12 * value._21 + _13 * value._31 + _14 * value._41,
            _11 * value._12 + _12 * value._22 + _13 * value._32 + _14 * value._42,
            _11 * value._13 + _12 * value._23 + _13 * value._33 + _14 * value._43,
            _11 * value._14 + _12 * value._24 + _13 * value._34 + _14 * value._44,
            _21 * value._11 + _22 * value._21 + _23 * value._31 + _24 * value._41,
            _21 * value._12 + _22 * value._22 + _23 * value._32 + _24 * value._42,
            _21 * value._13 + _22 * value._23 + _23 * value._33 + _24 * value._43,
            _21 * value._14 + _22 * value._24 + _23 * value._34 + _24 * value._44,
            _31 * value._11 + _32 * value._21 + _33 * value._31 + _34 * value._41,
            _31 * value._12 + _32 * value._22 + _33 * value._32 + _34 * value._42,
            _31 * value._13 + _32 * value._23 + _33 * value._33 + _34 * value._43,
            _31 * value._14 + _32 * value._24 + _33 * value._34 + _34 * value._44,
            _41 * value._11 + _42 * value._21 + _43 * value._31 + _44 * value._41,
            _41 * value._12 + _42 * value._22 + _43 * value._32 + _44 * value._42,
            _41 * value._13 + _42 * value._23 + _43 * value._33 + _44 * value._43,
            _41 * value._14 + _42 * value._24 + _43 * value._34 + _44 * value._44
        );

        return *this = temp;
    }

    constexpr float4x4& operator*=(float value) {
        _11 *= value;
        _12 *= value;
        _13 *= value;
        _14 *= value;
        _21 *= value;
        _22 *= value;
        _23 *= value;
        _24 *= value;
        _31 *= value;
        _32 *= value;
        _33 *= value;
        _34 *= value;
        _41 *= value;
        _42 *= value;
        _43 *= value;
        _44 *= value;

        return *this;
    }

    constexpr float4x4& operator/=(float value) {
        _11 /= value;
        _12 /= value;
        _13 /= value;
        _14 /= value;
        _21 /= value;
        _22 /= value;
        _23 /= value;
        _24 /= value;
        _31 /= value;
        _32 /= value;
        _33 /= value;
        _34 /= value;
        _41 /= value;
        _42 /= value;
        _43 /= value;
        _44 /= value;

        return *this;
    }

    constexpr float4x4 operator-() const {
        return float4x4(-_11, -_12, -_13, -_14,
                        -_21, -_22, -_23, -_24,
                        -_31, -_32, -_33, -_34,
                        -_41, -_42, -_43, -_44);
    }

    constexpr float4x4 operator*(float rhs) const {
        return float4x4(_11 * rhs, _12 * rhs, _13 * rhs, _14 * rhs,
                        _21 * rhs, _22 * rhs, _23 * rhs, _24 * rhs,
                        _31 * rhs, _32 * rhs, _33 * rhs, _34 * rhs,
                        _41 * rhs, _42 * rhs, _43 * rhs, _44 * rhs);
    }

    constexpr float3 operator*(const float3& rhs)  const{
        return float3(_11 * rhs.x + _12 * rhs.y + _13 * rhs.z,
                      _21 * rhs.x + _22 * rhs.y + _23 * rhs.z,
                      _31 * rhs.x + _32 * rhs.y + _33 * rhs.z);
    }

    constexpr bool operator==(const float4x4& value) const {
        return _11 == value._11 && _12 == value._12 && _13 == value._13 && _14 == value._14 &&
               _21 == value._21 && _22 == value._22 && _23 == value._23 && _24 == value._24 &&
               _31 == value._31 && _32 == value._32 && _33 == value._33 && _34 == value._34 &&
               _41 == value._41 && _42 == value._42 && _43 == value._43 && _44 == value._44;
    }

    constexpr bool operator!=(const float4x4& value) const {
        return _11 != value._11 || _12 != value._12 || _13 != value._13 || _14 != value._14 ||
               _21 != value._21 || _22 != value._22 || _23 != value._23 || _24 != value._24 ||
               _31 != value._31 || _32 != value._32 || _33 != value._33 || _34 != value._34 ||
               _41 != value._41 || _42 != value._42 || _43 != value._43 || _44 != value._44;
    }

    constexpr float* operator&() {
        return cells;
    }

    constexpr const float* operator&() const {
        return cells;
    }

    friend constexpr float4x4 operator*(float lhs, const float4x4& rhs) {
        return rhs * lhs;
    }

    friend constexpr float3 operator*(const float3& lhs, const float4x4& rhs) {
        return float3(lhs.x * rhs._11 + lhs.y * rhs._21 + lhs.z * rhs._31,
                      lhs.x * rhs._12 + lhs.y * rhs._22 + lhs.z * rhs._32,
                      lhs.x * rhs._13 + lhs.y * rhs._23 + lhs.z * rhs._33);
    }

    union {
        struct {
            float _m00, _m01, _m02, _m03;
            float _m10, _m11, _m12, _m13;
            float _m20, _m21, _m22, _m23;
            float _m30, _m31, _m32, _m33;
        };

        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };

        float cells[16];
    };
};

constexpr float4x4 transpose(const float4x4& value) {
    return float4x4(value._11, value._21, value._31, value._41,
                    value._12, value._22, value._32, value._42,
                    value._13, value._23, value._33, value._43,
                    value._14, value._24, value._34, value._44);
}

constexpr float4x4 operator/(const float4x4& lhs, float rhs) {
    return float4x4(lhs._11 / rhs, lhs._12 / rhs, lhs._13 / rhs, lhs._14 / rhs,
                    lhs._21 / rhs, lhs._22 / rhs, lhs._23 / rhs, lhs._24 / rhs,
                    lhs._31 / rhs, lhs._32 / rhs, lhs._33 / rhs, lhs._34 / rhs,
                    lhs._41 / rhs, lhs._42 / rhs, lhs._43 / rhs, lhs._44 / rhs);
}

constexpr float3 point_transform(const float3& point, const float4x4& transform) {
    float3 result(point.x * transform._11 + point.y * transform._21 + point.z * transform._31 + transform._41,
                  point.x * transform._12 + point.y * transform._22 + point.z * transform._32 + transform._42,
                  point.x * transform._13 + point.y * transform._23 + point.z * transform._33 + transform._43);
    return result / (point.x * transform._14 + point.y * transform._24 + point.z * transform._34 + transform._44);
}

constexpr float3 normal_transform(const float3& normal, const float4x4& inverse_transform) {
    return float3(
        normal.x * inverse_transform._11 + normal.y * inverse_transform._12 + normal.z * inverse_transform._13,
        normal.x * inverse_transform._21 + normal.y * inverse_transform._22 + normal.z * inverse_transform._23,
        normal.x * inverse_transform._31 + normal.y * inverse_transform._32 + normal.z * inverse_transform._33
    );
}

constexpr float4x4 inverse(const float4x4& value) {
    float _1 = value._33  * value._44;
    float _2 = value._43  * value._34;
    float _3 = value._23  * value._44;
    float _4 = value._43  * value._24;
    float _5 = value._23  * value._34;
    float _6 = value._33  * value._24;
    float _7 = value._13  * value._44;
    float _8 = value._43  * value._14;
    float _9 = value._13  * value._34;
    float _10 = value._33 * value._14;
    float _11 = value._13 * value._24;
    float _12 = value._23 * value._14;
    float _13 = value._31 * value._42;
    float _14 = value._41 * value._32;
    float _15 = value._21 * value._42;
    float _16 = value._41 * value._22;
    float _17 = value._21 * value._32;
    float _18 = value._31 * value._22;
    float _19 = value._11 * value._42;
    float _20 = value._41 * value._12;
    float _21 = value._11 * value._32;
    float _22 = value._31 * value._12;
    float _23 = value._11 * value._22;
    float _24 = value._21 * value._12;

    float4x4 result(
        (_1  * value._22 + _4  * value._32 + _5  * value._42) - (_2  * value._22 + _3  * value._32 + _6  * value._42),
        (_2  * value._12 + _7  * value._32 + _10 * value._42) - (_1  * value._12 + _8  * value._32 + _9  * value._42),
        (_3  * value._12 + _8  * value._22 + _11 * value._42) - (_4  * value._12 + _7  * value._22 + _12 * value._42),
        (_6  * value._12 + _9  * value._22 + _12 * value._32) - (_5  * value._12 + _10 * value._22 + _11 * value._32),
        (_2  * value._21 + _3  * value._31 + _6  * value._41) - (_1  * value._21 + _4  * value._31 + _5  * value._41),
        (_1  * value._11 + _8  * value._31 + _9  * value._41) - (_2  * value._11 + _7  * value._31 + _10 * value._41),
        (_4  * value._11 + _7  * value._21 + _12 * value._41) - (_3  * value._11 + _8  * value._21 + _11 * value._41),
        (_5  * value._11 + _10 * value._21 + _11 * value._31) - (_6  * value._11 + _9  * value._21 + _12 * value._31),
        (_13 * value._24 + _16 * value._34 + _17 * value._44) - (_14 * value._24 + _15 * value._34 + _18 * value._44),
        (_14 * value._14 + _19 * value._34 + _22 * value._44) - (_13 * value._14 + _20 * value._34 + _21 * value._44),
        (_15 * value._14 + _20 * value._24 + _23 * value._44) - (_16 * value._14 + _19 * value._24 + _24 * value._44),
        (_18 * value._14 + _21 * value._24 + _24 * value._34) - (_17 * value._14 + _22 * value._24 + _23 * value._34),
        (_15 * value._33 + _18 * value._43 + _14 * value._23) - (_17 * value._43 + _13 * value._23 + _16 * value._33),
        (_21 * value._43 + _13 * value._13 + _20 * value._33) - (_19 * value._33 + _22 * value._43 + _14 * value._13),
        (_19 * value._23 + _24 * value._43 + _16 * value._13) - (_23 * value._43 + _15 * value._13 + _20 * value._23),
        (_23 * value._33 + _17 * value._13 + _22 * value._23) - (_21 * value._23 + _24 * value._33 + _18 * value._13)
    );

    float det = value._11 * result._11 + value._21 * result._12 + value._31 * result._13 + value._41 * result._14;
    if (equal(det, 0.f)) {
        return float4x4();
    }

    float multiplier = 1.f / det;
    result._11 *= multiplier;
    result._12 *= multiplier;
    result._13 *= multiplier;
    result._14 *= multiplier;
    result._21 *= multiplier;
    result._22 *= multiplier;
    result._23 *= multiplier;
    result._24 *= multiplier;
    result._31 *= multiplier;
    result._32 *= multiplier;
    result._33 *= multiplier;
    result._34 *= multiplier;
    result._41 *= multiplier;
    result._42 *= multiplier;
    result._43 *= multiplier;
    result._44 *= multiplier;

    return result;
}

constexpr bool equal(const float4x4& lhs, const float4x4& rhs, float epsilon = EPSILON) {
    return equal(lhs._11, rhs._11, epsilon) &&
           equal(lhs._12, rhs._12, epsilon) &&
           equal(lhs._13, rhs._13, epsilon) &&
           equal(lhs._14, rhs._14, epsilon) &&
           equal(lhs._21, rhs._21, epsilon) &&
           equal(lhs._22, rhs._22, epsilon) &&
           equal(lhs._23, rhs._23, epsilon) &&
           equal(lhs._24, rhs._24, epsilon) &&
           equal(lhs._31, rhs._31, epsilon) &&
           equal(lhs._32, rhs._32, epsilon) &&
           equal(lhs._33, rhs._33, epsilon) &&
           equal(lhs._34, rhs._34, epsilon) &&
           equal(lhs._41, rhs._41, epsilon) &&
           equal(lhs._42, rhs._42, epsilon) &&
           equal(lhs._43, rhs._43, epsilon) &&
           equal(lhs._44, rhs._44, epsilon);
}

inline bool isfinite(const float4x4& value) {
    return std::isfinite(value._11) && std::isfinite(value._12) && std::isfinite(value._13) && std::isfinite(value._14) &&
           std::isfinite(value._21) && std::isfinite(value._22) && std::isfinite(value._23) && std::isfinite(value._24) && 
           std::isfinite(value._31) && std::isfinite(value._32) && std::isfinite(value._33) && std::isfinite(value._34) && 
           std::isfinite(value._41) && std::isfinite(value._42) && std::isfinite(value._43) && std::isfinite(value._44);
}

float3 sample_hemisphere(const float2& random);

constexpr float3 to_rgb(const float3& xyz) {
    return float3(
         3.240479f * xyz.x - 1.537150f * xyz.y - 0.498535f * xyz.z,
        -0.969256f * xyz.x + 1.875991f * xyz.y + 0.041556f * xyz.z,
         0.055648f * xyz.x - 0.204043f * xyz.y + 1.057311f * xyz.z
    );
}

constexpr float3 to_xyz(const float3& rgb) {
    return float3(
        0.412453f * rgb.r + 0.357580f * rgb.g + 0.180423f * rgb.b,
        0.212671f * rgb.r + 0.715160f * rgb.g + 0.072169f * rgb.b,
        0.019334f * rgb.r + 0.119193f * rgb.g + 0.950227f * rgb.b
    );
}
