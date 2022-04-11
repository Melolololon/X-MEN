#include"Color.h"

using namespace MelLib;
Color::Color()
{
}

Color::Color(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;

}

Color::Color(const unsigned char rgb, const unsigned char a)
{
	this->r = rgb;
	this->g = rgb;
	this->b = rgb;
	this->a = a;

}


Color Color::operator+(const Color& color)
{
	return { (unsigned char)(r + color.r),(unsigned char)(g + color.g ),(unsigned char)(b + color.b) ,(unsigned char)(a + color.a) };
}
Color Color::operator-(const Color& color)
{
	return { (unsigned char)(r - color.r),(unsigned char)(g - color.g),(unsigned char)(b -color.b) ,(unsigned char)(a + color.a) };
}
Color Color::operator*(const Color& color)
{
	return { (unsigned char)(r * color.r),(unsigned char)(g * color.g),(unsigned char)(b * color.b) ,(unsigned char)(a + color.a) };
}
Color Color::operator/(const Color& color)
{
	return { (unsigned char)(r / color.r),(unsigned char)(g / color.g),(unsigned char)(b / color.b) ,(unsigned char)(a + color.a) };
}



	/*
Color Color::operator+=(Color color) {}
Color Color::operator-=(Color color) {}
Color Color::operator*=(Color color) {}
Color Color::operator/=(Color color) {}*/

