#pragma once
#include <SFML/Graphics.hpp>
#include <exception>

template<typename T>
class Vector : public sf::Vector2<T>
{
public:
	Vector() = default;
	explicit Vector(const sf::Vector2<T>& vector);
	~Vector() = default;

	T Dot(const Vector<T>& other);

	Vector<T>& operator=(const sf::Vector2<T>& vector);
	T& operator[](int idx);
	const T& operator[] (int idx) const;
};

template<typename T>
inline Vector<T>::Vector(const sf::Vector2<T>& vector)
	: sf::Vector2<T>(vector) {}

template<typename T>
inline T Vector<T>::Dot(const Vector<T>& other)
{
	return this->x * other.x + this->y * other.y;
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const sf::Vector2<T>& vector)
{
	this->x = vector.x;
	this->y = vector.y;

	return *this;
}

template<typename T>
inline T& Vector<T>::operator[](int idx)
{
	switch (idx)
	{
	case 0:
		return this->x;
	case 1:
		return this->x;
	default:
		throw std::out_of_range("Out of range error on Vector<T>");
	}
}

template<typename T>
inline const T& Vector<T>::operator[](int idx) const
{
	switch (idx)
	{
	case 0:
		return this->x;
	case 1:
		return this->x;
	default:
		throw std::out_of_range("Out of range error on Vector<T>");
	}
}