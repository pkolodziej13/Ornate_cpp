#pragma once
#include <vector>
#include <algorithm>

#include "Number_range.h"
#include "Multidimensional_iterator.h"

namespace uti
{
	template<size_t dim>
	struct cecular_automator_cutator
	{
		std::array<size_t, dim> size;
		struct region_data
		{
			std::vector<std::array<int, dim>> neighbors;
			std::array<size_t, dim> start;
			std::array<size_t, dim> size;
			auto  begin()
			{
				return multidimensional_Iterator<dim>(start, size);
			}
			auto  end()
			{
				return multidimensional_Iterator<dim>(start, size);
			}
		};
		std::vector<region_data> regions;
		cecular_automator_cutator(std::array<size_t, dim> size_, std::vector<std::array<int, dim>> neightbours)
		{
			for (size_t i = 0; i < neightbours.size(); i++)
			{
				for (auto a : Number_range(dim))
				{
					if ((neightbours[i][a] >= (int)size_[a]) || (-neightbours[i][a] >= (int)size_[a]))
					{
						neightbours.erase(neightbours.begin() + i);
						i--;
						break;
					}
				}
			}
			std::array<std::vector<int>, dim   > stages_;
			for (auto& a : stages_)
			{
				a.push_back(0);
			}
			for (auto& a : neightbours)
			{
				for (auto b : Number_range(dim))
				{
					bool contain = false;
					for (auto c : stages_[b])
						if (c == a[b])
						{
							contain = true;
							break;
						}
					if (!contain)
					{
						stages_[b].push_back(a[b]);
					}
				}
			}
			size_t regions_size = 1;
			std::array<size_t, dim   > ms;
			for (auto a : Number_range(dim))
			{
				std::sort(stages_[a].begin(), stages_[a].end());
				regions_size *= stages_[a].size();
				ms[a] = stages_[a].size();
			}
			std::vector<std::array<int, dim>> stages2;
			stages2.reserve(regions_size);
			for (auto a : multidimensional_Iterator<dim>(ms))
			{
				std::array<int, dim> to_add;
				for (auto b : Number_range(dim))
					to_add[b] = stages_[b][a[b]];

				stages2.push_back(to_add);
			}
			regions.resize(regions_size);
			std::vector<bool> neig_flags(neightbours.size());
			for (auto a : Number_range(regions_size))
			{
				fill(neig_flags.begin(), neig_flags.end(), true);
				auto& region = regions[a];
				region.neighbors.reserve(neightbours.size());

				for (auto b : Number_range(dim))
				{
					auto pom = stages2[a][b];
					if (pom < 0)
					{
						for (auto c : Number_range(neightbours.size()))
						{
							if (neightbours[c][b] <= pom)
								neig_flags[c] = false;

						}
						for (auto c : Number_range(stages_[b].size()))
						{
							if (stages_[b][c] == pom)
							{
								region.size[b] = stages_[b][c + 1] - pom;
							}
						}
						region.start[b] = pom - stages_[b].front();
					}
					else if (pom == 0)
					{
						region.start[b] = -stages_[b].front();
						region.size[b] = size_[b] - stages_[b].back() - region.start[b];
					}
					else//stages2[a][b]>0
					{
						for (auto c : Number_range(neightbours.size()))
						{
							if (neightbours[c][b] >= pom)
								neig_flags[c] = false;
						}
						for (auto c : Number_range(stages_[b].size()))
						{
							if (stages_[b][c] == pom)
							{
								region.size[b] = pom - stages_[b][c - 1];
								region.start[b] = size_[b] - (pom - stages_[b][c - 1]);
							}
						}
					}

				}
				for (auto b : Number_range(neightbours.size()))
				{
					if (neig_flags[b])
						region.neighbors.push_back(neightbours[b]);
				}

			}

		}
		auto  begin()
		{
			return regions.begin();
		}
		auto  end()
		{
			return regions.end();
		}
		inline void operator++()
		{
		}
		inline region_data& operator*()
		{
		}
		inline bool operator !=(const  cecular_automator_cutator& b)
		{
		}

	};

	template<size_t dim>
	std::vector<std::array<int, dim>> surroundings()
	{
		std::array<int, 3> a = { -1,0,1 };
		std::array<size_t, dim> tab;
		tab.fill(3);
		std::vector<std::array<int, dim>> to_return;
		for (auto b : multidimensional_Iterator<dim>(tab))
		{
			std::array<int, dim> to_add;
			bool zero = false;
			for (auto c : Number_range(dim))
			{
				if (a[b[c]] != 0)
					zero = true;
				to_add[c] = a[b[c]];
			}
			if (zero)
				to_return.push_back(to_add);
		}

		return to_return;
	}
	template<size_t dim>
	std::vector<std::array<int, dim>> area()
	{
		std::array<int, 3> a = { -1,0,1 };
		std::array<size_t, dim> tab;
		tab.fill(3);
		std::vector<std::array<int, dim>> to_return;
		for (auto b : multidimensional_Iterator<dim>(tab))
		{
			std::array<int, dim> to_add;
			for (auto c : Number_range(dim))
			{
				to_add[c] = a[b[c]];
			}
			to_return.push_back(to_add);
		}

		return to_return;
	}


}