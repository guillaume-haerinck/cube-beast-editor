#pragma once

namespace comp {
	/**
	 * @brief Position in the 3d grid
	 */
	struct Position {
		Position(int x, int y, int z) : x(x), y(y), z(z) {}

		int x;
		int y;
		int z;
	};
}
