#pragma once

enum class WaypointType {WP_MAIN,WP_MIDDLE,WP_LAST};


struct Waypoint
{
	float x, y;
	WaypointType type;

	Waypoint(float x,float y,WaypointType t)
		:x{x},
		y{y},
		type{t}
	{}
};