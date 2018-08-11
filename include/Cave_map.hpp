#pragma once

#include <array>
#include <iostream>

#include "Cave_chunck.hpp"

class Cave_map
{
private:
	std::array<Cave_chunck, 10> loaded_chunck;
public:
	void	print_map();
};
