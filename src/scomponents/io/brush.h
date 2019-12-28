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

    BrushType type() const { return m_type; }
    BrushUse usage() const { return m_usage; }
    bool started() const { return m_started; }

private:
    BrushType m_type = BrushType::VOXEL;
	BrushUse m_usage = BrushUse::ADD;
	bool m_started = false;

private:
    friend class BrushGui;
	friend class App;
};
