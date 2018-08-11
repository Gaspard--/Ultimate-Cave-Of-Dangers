#pragma once

#include <array>
#include <iostream>

#include "Cave_chunck.hpp"

class Cave_map
{
private:
  std::array<Cave_chunck, 10> loaded_chunck;
  int next_map_choice;
public:
  Cave_map();
  void	print_map();
  void gen_map();
};
