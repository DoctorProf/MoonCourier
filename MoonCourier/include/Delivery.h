#pragma once
#include "Pathfinder.h"

enum class DeliveryState
{
	ToOrder,
	ToBase
};

struct Delivery
{
	int rover_id;
	int order_id;

	float reward;

	pathfinder::PathResult path;

	DeliveryState state;
};