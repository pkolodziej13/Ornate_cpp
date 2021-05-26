#pragma once
#include <Utilities/Non_replicable.h>
#include <Utilities/Responsibility.h>

namespace obj
{
	template<class Identificator_type,class Responsibility>
	struct Instances_global_book :uti::non_replicable
	{
		struct Reader
		{
			Reader()
			{
				Responsibility::verify([&]() {return writers == 0; });
				readers++;
			}
			~Reader()
			{
				readers--;
			}
		};
		struct Writer
		{
			Writer()
			{
				Responsibility::verify([&]() {return writers == 0; });
				Responsibility::verify([&]() {return readers == 0; });
				writers++;
			}
			~Writer()
			{
				writers--;
			}
		};
	private:
		inline static size_t readers = 0;
		inline static size_t writers = 0;
	};
	template<class Identificator_type>
	struct Instances_global_book<Identificator_type,uti::Certain> :uti::non_replicable
	{
		struct Reader
		{};
		struct Writer
		{};
	};
}