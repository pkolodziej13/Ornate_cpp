#pragma once
#include <Utilities/Non_replicable.h>
#include <Utilities/Responsibility.h>

namespace obj
{
	template<class Identificator_type,class Responsibility>
	struct Instances_global_book :uti::non_replicable
	{
		struct reader
		{
			reader()
			{
				assert(writers == 0);
				readers++;
			}
			~reader()
			{
				readers--;
			}
		};
		struct writer
		{
			writer()
			{
				assert(writers == 0);
				assert(readers == 0);
				writers++;
			}
			~writer()
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
		struct reader
		{};
		struct writer
		{};
	};
}