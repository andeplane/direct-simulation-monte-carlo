#ifndef VEC2_H
#define VEC2_H
#include <iostream>

class vec2
{
private:
    float m_vec[2];
public:
    vec2(); // Create a zero vector
    vec2(float x, float y);
    bool operator==(vec2 rhs);
    vec2 operator+(vec2 rhs);
    vec2 &operator+=(vec2 rhs);
    vec2 operator-(vec2 rhs);
    vec2 &operator-=(vec2 rhs);
    vec2 operator*(vec2 rhs);
    vec2 &operator*=(vec2 rhs);
    vec2 operator/(vec2 rhs);
    vec2 &operator/=(vec2 rhs);
    vec2 operator+(float scalar);
    vec2 &operator+=(float scalar);
    vec2 operator-(float scalar);
    vec2 &operator-=(float scalar);
    vec2 operator*(float scalar);
    vec2 &operator*=(float scalar);
    vec2 operator/(float scalar);
    vec2 &operator/=(float scalar);
    inline vec2 operator-() { return vec2(-m_vec[0], -m_vec[1]); }
    void add(vec2 &rhs) {
        m_vec[0] += rhs.x();
        m_vec[1] += rhs.y();
    }
    void addAndMultiply(vec2 &rhs, float scalar) {
        m_vec[0] += rhs.x()*scalar;
        m_vec[1] += rhs.y()*scalar;
    }
    float dot(vec2 &rhs);
    float length();
    void normalize();
    void setToZero();
    void set(float x, float y);
    inline float x() const { return m_vec[0]; }
    inline float y() const { return m_vec[1]; }
    inline float &operator[](int index) { return m_vec[index]; }
    inline float operator[](int index) const { return m_vec[index]; }
    inline float lengthSquared() { return m_vec[0]*m_vec[0] + m_vec[1]*m_vec[1]; }
    inline void subtract(const vec2 &v1, const vec2 &v2) { m_vec[0] = v1[0] - v2[0]; m_vec[1] = v1[1] - v2[1]; }
private:
    friend std::ostream& operator<<(std::ostream&stream, vec2 vec);
};

#endif // VEC2_H
