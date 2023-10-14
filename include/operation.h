#pragma once
#include <concepts>

namespace libemu
{
	class operation
	{

	};

	template <typename T>
	concept operational = std::is_base_of_v<operation, T>;
}
