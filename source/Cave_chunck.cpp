

Cave_chunck::Cave_chunck(Cave_chunck parent)
{
	unsigned int nb_room = 0;

	for (auto &pos : entry_point)
	{
		chunck[pos.y][pos.x].type = tile_type::Entry;
	}

	while (nb_room < ROOM_LIMITE)
}

const std::vector<int, 2u>Cave_chunck::get_pos() const
{
	return(pos);
}

Cave_tile const &Cave_chunck::get_tile(unsigned int x, unsigned int y) const
{
	return(const &chunck[y][x]);
}

void Cave_chunck::print()
{
	for (auto &line : chunck)
	{
		for (auto &tile : line)
		{
			switch (tile.type)
			{
				case None:
					std::cout << ' ';
					break;
				case Wall:
					std::cout << 'W';
					break;
				case Entry:
					std::cout << 'E';
					break;
				case Platform:
					std::cout << '_';
					break;
				default:
					std::cout << '*';
					break;
			}
			std::cout << endl;
		}
	}
}

Cave_chunck::~Cave_chunck()
{
	delete entry_point
}
