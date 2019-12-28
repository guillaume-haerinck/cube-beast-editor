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
    Brush() {};

    BrushType currentBrush() const { return m_currentBrush; }
    BrushUse currentBrushUse() const { return m_currentBushUse; }
    bool isBrushStarted() const { return m_isBrushStarted; }

private:
    BrushType m_currentBrush = BrushType::VOXEL;
	BrushUse m_currentBushUse = BrushUse::ADD;
	bool m_isBrushStarted = false;

private:
    friend class BrushGui;
	friend class App;
};
