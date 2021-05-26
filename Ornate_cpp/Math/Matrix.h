#pragma once
#include "Vector_mth.h"

namespace mth::linear_algebra
{
	template<class T, class Sequence_x, class Sequence_y>
	struct g_mat_operations;

	template<class T, size_t X, size_t Y>
	struct g_mat :g_mat_operations<T, std::make_index_sequence<X>, std::make_index_sequence<Y>>
	{
		using base_type = g_mat_operations<T, std::make_index_sequence<X>, std::make_index_sequence<Y>>;
		using base_type::g_mat_operations;

	};


	template<class T, class x_sequence, class y_sequence>
	struct g_mat_elements {};
	template<class T, size_t ... IX, size_t ... IY>
	struct g_mat_elements<T, std::index_sequence<IX...>, std::index_sequence<IY...>> :g_vec_elements<g_vec<T, sizeof...(IY)>, IX...   >
	{
		using base_type = g_vec_elements<g_vec<T, sizeof...(IY)>, IX...>;
		static constexpr size_t x_size = sizeof...(IX);
		static constexpr size_t y_size = sizeof...(IY);
		static constexpr size_t smaller_size = x_size < y_size ? x_size : y_size;
		using y_vec_type = g_vec<T, y_size>;

		g_mat_elements()
		{
			if constexpr (x_size < y_size)
			{
				((get<IX, IX>() = 1), ...);
			}
			else
			{
				((get<IY, IY>() = 1), ...);
			}
		}
		g_mat_elements(const T& scalar)
		{
			if constexpr (x_size < y_size)
			{
				((get<IX, IX>() = scalar), ...);
			}
			else
			{
				((get<IY, IY>() = scalar), ...);
			}
		}
		g_mat_elements(const g_vec<T, smaller_size>& vec)
		{
			if constexpr (x_size < y_size)
			{
				((get<IX, IX>() = vec.element<IX>()), ...);
			}
			else
			{
				((get<IY, IY>() = vec.element<IY>()), ...);
			}
		}
		template<size_t i>
		using always_vec_y = g_vec<T, y_size>;
		g_mat_elements(const always_vec_y<IX> & ...  vec) :base_type(vec...)
		{

		}
		template<class T_specific, size_t x2, size_t y2, class = std::enable_if_t<std::is_convertible_v<T_specific, T>>>
		g_mat_elements(const g_mat<T_specific, x2, y2>& mat) : g_mat_elements()
		{
			constexpr size_t smaller_x = x_size < x2 ? x_size : x2;
			constexpr size_t smaller_y = y_size < y2 ? y_size : y2;
			for_i(smaller_x)
			{
				for_j(smaller_y)
				{
					(*this)[i][j] = static_cast<T>(mat[i][j]);
				}
			}
		}

		template<size_t x, size_t y>
		T& get()
		{
			return this->element<x>().element<y>();
		}
		template<size_t x, size_t y>
		const T& get()const
		{
			return this->element<x>().element<y>();
		}


		template<size_t r>
		inline g_vec<T, x_size> row()const
		{
			return g_vec<T, x_size>((this->get<IX, r>())...);
		}
		template<size_t r>
		inline void row(const g_vec<T, x_size>& vec)
		{
			((this->get<IX, r>() = vec.element<IX>()), ...);
		}



		std::add_pointer_t<const T> data()const
		{
			return &this->get<0, 0>();
		}
		T* data()
		{
			return &this->get<0, 0>();
		}
	};

	template<class T, size_t ... IX, size_t ... IY>
	struct g_mat_operations<T, std::index_sequence<IX...>, std::index_sequence<IY...>> 
		:g_mat_elements<T, std::index_sequence<IX...>, std::index_sequence<IY...>>
	{
		static constexpr size_t sizeX = sizeof...(IX);
		static constexpr size_t sizeY = sizeof...(IY);

		using base_type = g_mat_elements<T, std::index_sequence<IX...>, std::index_sequence<IY...>>;
		using this_type = g_mat_operations<T, std::index_sequence<IX...>, std::index_sequence<IY...>>;
		using mat_type = g_mat<T, sizeX, sizeY>;

		using base_type::g_mat_elements;
		using base_type::get;
		using base_type::row;


		template<size_t r>
		inline T multiply_element(const g_vec<T, sizeX>& b)const
		{
			return ((this->get<IX, r>() * b.element<IX>()) + ...);
		}
		g_vec<T, sizeY> operator *(const g_vec<T, sizeX>& ve)const
		{
			return { multiply_element<IY>(ve)... };
		}

		template<size_t ... x2>
		g_mat<T, sizeof...(x2), sizeY> operator *(const g_mat_operations<T, std::index_sequence<x2...>, std::index_sequence<IX...>>& b)const
		{
			return { (*this) * b.element<x2>() ... };
		}

		mat_type operator *(T b)const
		{
			return { this->element<IX>() * b ... };
		}

		template<size_t x1, size_t y1, size_t x2, size_t y2>
		inline T region_cross()const
		{
			return this->get<x1, y1>() * this->get<x2, y2>() - this->get<x2, y1>() * this->get<x1, y2>();
		}

		mat_type inversed()const
		{
			static_assert(sizeX == sizeY, "cannot inverse non symetric matrix");

			const this_type& m = (*this);
			mat_type result;
			if constexpr (sizeX == 2)
			{

				T OneOverDeterminant = static_cast<T>(1) / region_cross <0, 0, 1, 1>();
				result = { {
						+m[1][1] * OneOverDeterminant ,
						-m[0][1] * OneOverDeterminant },{
							-m[1][0] * OneOverDeterminant,
							+m[0][0] * OneOverDeterminant } };

			}
			else if constexpr (sizeX == 3)
			{

				T OneOverDeterminant = static_cast<T>(1) / (
					+m[0][0] * region_cross <1, 1, 2, 2>()
					+ m[1][0] * region_cross <2, 1, 0, 2>()
					+ m[2][0] * region_cross <0, 1, 1, 2>());

				result[0][0] = region_cross <1, 1, 2, 2>() * OneOverDeterminant;
				result[1][0] = region_cross <1, 2, 2, 0>() * OneOverDeterminant;
				result[2][0] = region_cross <1, 0, 2, 1>() * OneOverDeterminant;
				result[0][1] = region_cross <2, 1, 0, 2>() * OneOverDeterminant;
				result[1][1] = region_cross <2, 2, 0, 0>() * OneOverDeterminant;
				result[2][1] = region_cross <2, 0, 0, 1>() * OneOverDeterminant;
				result[0][2] = region_cross <0, 1, 1, 2>() * OneOverDeterminant;
				result[1][2] = region_cross <0, 2, 1, 0>() * OneOverDeterminant;
				result[2][2] = region_cross <0, 0, 1, 1>() * OneOverDeterminant;

			}
			else if constexpr (sizeX == 4)
			{

				T Coef00 = region_cross <2, 2, 3, 3>();
				T Coef02 = region_cross <1, 2, 3, 3>();
				T Coef03 = region_cross <1, 2, 2, 3>();

				T Coef04 = region_cross <2, 1, 3, 3>();
				T Coef06 = region_cross <1, 1, 3, 3>();
				T Coef07 = region_cross <1, 1, 2, 3>();

				T Coef08 = region_cross <2, 1, 3, 2>();
				T Coef10 = region_cross <1, 1, 3, 2>();
				T Coef11 = region_cross <1, 1, 2, 2>();


				T Coef12 = region_cross <2, 0, 3, 3>();
				T Coef14 = region_cross <1, 0, 3, 3>();
				T Coef15 = region_cross <1, 0, 2, 3>();

				T Coef16 = region_cross <2, 0, 3, 2>();
				T Coef18 = region_cross <1, 0, 3, 2>();
				T Coef19 = region_cross <1, 0, 2, 2>();

				T Coef20 = region_cross <2, 0, 3, 1>();
				T Coef22 = region_cross <1, 0, 3, 1>();
				T Coef23 = region_cross <1, 0, 2, 1>();

				g_vec<T, 4> Fac0(Coef00, Coef00, Coef02, Coef03);
				g_vec<T, 4> Fac1(Coef04, Coef04, Coef06, Coef07);
				g_vec<T, 4> Fac2(Coef08, Coef08, Coef10, Coef11);
				g_vec<T, 4> Fac3(Coef12, Coef12, Coef14, Coef15);
				g_vec<T, 4> Fac4(Coef16, Coef16, Coef18, Coef19);
				g_vec<T, 4> Fac5(Coef20, Coef20, Coef22, Coef23);

				g_vec<T, 4> Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
				g_vec<T, 4> Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
				g_vec<T, 4> Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
				g_vec<T, 4> Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

				g_vec<T, 4> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
				g_vec<T, 4> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
				g_vec<T, 4> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
				g_vec<T, 4> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

				g_vec<T, 4> SignA(+1, -1, +1, -1);
				g_vec<T, 4> SignB(-1, +1, -1, +1);
				result = { Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB };
				T diver = T(1.0) / m.element<0>().dot(g_vec<T, 4>{result[0][0], result[1][0], result[2][0], result[3][0]});

				((result.element<IX>() *= diver), ...);

			}
			else
			{
				static_assert(false, "dimension not implemented");
			}

			return result;
		}
		g_mat<T, sizeY, sizeX> transposed()const
		{
			return { this->row<IY>()... };
		}
		auto ortogonal()const
		{
			static_assert(false, "not_implemented");
		}

		bool operator ==(const mat_type& mat)const
		{
			return ((this->element<IX>() == mat.element<IX>())&&...);
		}
	};

	template<class T, size_t X, size_t Y>
	auto transpose(const g_mat<T, X, Y>& m)
	{
		return m.transpose();
	}
	template<class T, size_t x, size_t y, class precision>
	inline g_mat<T, x, y> interpolate(const g_mat<T, x, y>& a, const g_mat<T, x, y>& b, const precision& progress)
	{
		g_mat<T, x, y> result{ 0 };
		for_i(x)
		{
			for_j(y)
			{
				result[i][j] = a[i][j] * (1 - progress) + b[i][j] * (progress);
			}
		}
		return result;
	}

	template<class T, size_t x, size_t y>
	bool are_similar(const g_mat<T, x, y>& a, const g_mat<T, x, y>& b, const T& precision = T(0.01))
	{
		bool result = true;
		for_i(x)
		{
			for_j(y)
				if ((a[i][j] - b[i][j]) > precision)
				{
					result = false;
				}
		}
		return result;
	}

}