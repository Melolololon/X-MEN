#pragma once
//ílÇï°êîÇ‹Ç∆ÇﬂÇΩÇ‡ÇÃ

namespace MelLib
{
	template<class V>
	struct Value2
	{
		V v1, v2;

		Value2() {}
		Value2(const V v) :v1(v), v2(v) {}
		Value2(const V v1, const V v2) :v1(v1), v2(v2)
		{}

		Value2<V> operator+(const Value2<V>& value)const { return Value2(v1 + value.v1, v2 + value.v2); }
		Value2<V> operator-(const Value2<V>& value)const { return Value2(v1 - value.v1, v2 - value.v2); }
		Value2<V> operator*(const Value2<V>& value)const { return Value2(v1 * value.v1, v2 * value.v2); }
		Value2<V> operator/(const Value2<V>& value)const { return Value2(v1 / value.v1, v2 / value.v2); }
		Value2<V> operator%(const Value2<V>& value)const { return Value2(v1 % value.v1, v2 % value.v2); }
		bool operator==(const Value2<V>& value)const { return v1 == value.v1 && v2 == value.v2; }
		bool operator!=(const Value2<V>& value)const { return v1 != value.v1 || v2 != value.v2; }

		void operator+= (const Value2<V>& vector)
		{
			v1 += vector.v1;
			v2 += vector.v2;
		}
		void operator-= (const Value2<V>& vector)
		{
			v1 -= vector.v1;
			v2 -= vector.v2;
		}
		void operator*= (const Value2<V>& vector)
		{
			v1 *= vector.v1;
			v2 *= vector.v2;
		}
		void operator/= (const Value2<V>& vector)
		{
			v1 /= vector.v1;
			v2 /= vector.v2;
		}

		void operator=(const Value2<V> value)
		{
			v1 = value.v1;
			v2 = value.v2;
		}
	};


	template<class V>
	struct Value3
	{
		V v1, v2, v3;
		Value3() {}
		Value3(const V v) : v1(v), v2(v), v3(v) {}
		Value3(const V v1, const V v2, const V v3) :v1(v1), v2(v2), v3(v3) {}

		Value3<V> operator+(const Value3<V>& value) { return Value3(v1 + value.v1, v2 + value.v2, v3 + value.v3); }
		Value3<V> operator-(const Value3<V>& value) { return Value3(v1 - value.v1, v2 - value.v2, v3 - value.v3); }
		Value3<V> operator*(const Value3<V>& value) { return Value3(v1 * value.v1, v2 * value.v2, v3 * value.v3); }
		Value3<V> operator/(const Value3<V>& value) { return Value3(v1 / value.v1, v2 / value.v2, v3 / value.v3); }
		Value3<V> operator%(const Value3<V>& value) { return Value3(v1 % value.v1, v2 % value.v2, v3 % value.v3); }
		bool operator==(const Value3<V>& value)const { return v1 == value.v1 && v2 == value.v2 && v3 == value.v3; }
		bool operator!=(const Value3<V>& value)const { return v1 != value.v1 || v2 != value.v2 || v3 != value.v3; }

		void operator=(const Value3<V> value)
		{
			v1 = value.v1;
			v2 = value.v2;
			v3 = value.v3;
		}
	};


	template<class V>
	struct Value4
	{
		V v1, v2, v3, v4;
		Value4() {}
		Value4(const V v) : v1(v), v2(v), v3(v), v4(v) {}
		Value4(const V v1, const V v2, const V v3, const V v4) :v1(v1), v2(v2), v3(v3), v4(v4) {}

		Value4<V> operator+(const Value4<V>& value) { return Value4(v1 + value.v1, v2 + value.v2, v3 + value.v3, v4 + value.v4); }
		Value4<V> operator-(const Value4<V>& value) { return Value4(v1 - value.v1, v2 - value.v2, v3 - value.v3, v4 - value.v4); }
		Value4<V> operator*(const Value4<V>& value) { return Value4(v1 * value.v1, v2 * value.v2, v3 * value.v3, v4 * value.v4); }
		Value4<V> operator/(const Value4<V>& value) { return Value4(v1 / value.v1, v2 / value.v2, v3 / value.v3, v4 / value.v4); }
		Value4<V> operator%(const Value4<V>& value) { return Value4(v1 % value.v1, v2 % value.v2, v3 % value.v3, v4 % value.v4); }
		bool operator==(const Value4<V>& value)const { return v1 == value.v1 && v2 == value.v2 && v3 == value.v3 && v4 == value.v4; }
		bool operator!=(const Value4<V>& value)const { return v1 != value.v1 || v2 != value.v2 || v3 != value.v3 || v4 != value.v4; }

		
		void operator=(const Value4<V> value)
		{
			v1 = value.v1;
			v2 = value.v2;
			v3 = value.v3;
			v4 = value.v4;
		}
	};

}
