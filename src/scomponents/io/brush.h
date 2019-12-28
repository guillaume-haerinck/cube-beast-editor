#pragma once

enum class BrushType {
	VOXEL = 0,
	FACE,
	RECTANGLE,
	LINE,
	CIRCLE
};

enum class BrushUse {
	ADD = 0,
	REMOVE,
	PAINT,
	SELECT
};



class Brush {
public:


private:
    BrushType m_currentBrush = BrushType::VOXEL;
	BrushUse m_currentBushUse = BrushUse::ADD;
	bool m_isBrushStarted = false;
};
