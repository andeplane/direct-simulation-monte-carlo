#include <cmath>
#include "vec2.h"
#include "random.h"

vec2::vec2()
{
    setToZero();
}

vec2::vec2(float x, float y)
{
    set(x,y);
}

bool vec2::operator==(vec2 rhs) {
    return(m_vec[0] == rhs.x() && m_vec[1] == rhs.y());
}

vec2 vec2::operator+(vec2 rhs) {
    return vec2( m_vec[0] + rhs.x(),
                 m_vec[1] + rhs.y());
}

vec2 &vec2::operator+=(vec2 rhs) {
    m_vec[0] += rhs.x();
    m_vec[1] += rhs.y();
    return *this;
}

vec2 vec2::operator-(vec2 rhs) {
    return vec2( m_vec[0] - rhs.x(),
                 m_vec[1] - rhs.y());
}

vec2 &vec2::operator-=(vec2 rhs) {
    m_vec[0] -= rhs.x();
    m_vec[1] -= rhs.y();
    return *this;
}

vec2 vec2::operator*(vec2 rhs) {
    return vec2( m_vec[0] * rhs.x(),
                 m_vec[1] * rhs.y());
}

vec2 &vec2::operator*=(vec2 rhs) {
    m_vec[0] *= rhs.x();
    m_vec[1] *= rhs.y();
    return *this;
}

vec2 vec2::operator/(vec2 rhs) {
    return vec2( m_vec[0] / rhs.x(),
                 m_vec[1] / rhs.y());
}

vec2 &vec2::operator/=(vec2 rhs) {
    m_vec[0] /= rhs.x();
    m_vec[1] /= rhs.y();
    return *this;
}

vec2 vec2::operator+(float scalar) {
    return vec2(m_vec[0] + scalar,
                m_vec[1] + scalar);
}

vec2 &vec2::operator+=(float scalar) {
    m_vec[0] += scalar;
    m_vec[1] += scalar;
    return *this;
}

vec2 vec2::operator-(float scalar) {
    return vec2(m_vec[0] - scalar,
                m_vec[1] - scalar);
}

vec2 &vec2::operator-=(float scalar) {
    m_vec[0] -= scalar;
    m_vec[1] -= scalar;
    return *this;
}

vec2 vec2::operator*(float scalar) {
    return vec2(m_vec[0] * scalar,
                m_vec[1] * scalar);
}

vec2 &vec2::operator*=(float scalar) {
    m_vec[0] *= scalar;
    m_vec[1] *= scalar;
    return *this;
}

vec2 vec2::operator/(float scalar) {
    return vec2(m_vec[0] / scalar,
                m_vec[1] / scalar);
}

vec2 &vec2::operator/=(float scalar) {
    m_vec[0] /= scalar;
    m_vec[1] /= scalar;
    return *this;
}

float vec2::dot(vec2 &rhs) {
    return (m_vec[0] * rhs.x() +
            m_vec[1] * rhs.y());
}

float vec2::length() {
    return sqrt(lengthSquared());
}

void vec2::normalize() {
    float myLength = length();
    if(myLength > 0) { // Don't divide by zero...
        m_vec[0] /= myLength;
        m_vec[1] /= myLength;
    }
}

void vec2::setToZero()
{
    set(0,0);
}

void vec2::set(float x, float y)
{
    m_vec[0] = x;
    m_vec[1] = y;
}

std::ostream& operator<<(std::ostream &stream, vec2 vec) {
    return stream << "[" << vec.x() << ", " << vec.y() << "]";
}
