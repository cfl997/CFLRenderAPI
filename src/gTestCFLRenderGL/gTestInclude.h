#pragma once

#include "CFLRender.h"

#include "gtest/gtest.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


struct Vertex
{
	glm::vec3 vertex;
	DWORD color;
	glm::vec2 tex;
};

using VertexArray = std::vector<Vertex>;


