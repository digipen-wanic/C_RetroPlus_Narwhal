//==================================================================-
/*
/file   MeshHelper.cpp
/author Jakob McFarland
/date   10/23/2018
/brief

This is the implementation file for all member functions
of the class MeshHelper, as specified in specification
file MeshHelper.h.

*/
//==================================================================-

#include "stdafx.h"
#include "MeshHelper.h"
#include "Vertex.h"
#include "Mesh.h"
#include "MeshFactory.h"
#include <Color.h>

// Create a colored mesh consisting of a single triangle using the Beta Framework.
// Params:
//   color0 = The color of vertex 0.
//   color1 = The color of vertex 1.
//   color2 = The color of vertex 2.
// Returns:
//	 A pointer to the newly created mesh.
Mesh* CreateTriangleMesh(const Color& color0, const Color& color1, const Color& color2)
{
	MeshFactory& meshFactory = MeshFactory::GetInstance();

	//set all verteces
	Vertex v0;
	v0.position = Vector2D(-0.5, 0.5);
	v0.color = color0;

	Vertex v1;
	v1.position = Vector2D(-0.5, -0.5);
	v1.color = color1;

	Vertex v2;
	v2.position = Vector2D(0.5, 0);
	v2.color = color2;

	//build it
	meshFactory.AddTriangle(v0, v1, v2);
	return meshFactory.EndCreate();
}

// Create a textured quad mesh using the Beta Framework.
// Params:
//   textureSize = The UV size of the mesh, in texture coordinates.
//	 extents	 = The XY distance of the vertices from the origin.
// Returns:
//	 A pointer to the newly created mesh.
Mesh* CreateQuadMesh(const Vector2D& textureSize, const Vector2D& extents)
{
	MeshFactory& meshFactory = MeshFactory::GetInstance();

	//set up all verteces
	Vertex v0;
	v0.position = Vector2D(extents.x, extents.y);
	v0.textureCoords = Vector2D(textureSize.x, 0);

	Vertex v1;
	v1.position = Vector2D(extents.x, -extents.y );
	v1.textureCoords = Vector2D(textureSize.x, textureSize.y); 

	Vertex v2;
	v2.position = Vector2D(-extents.x, extents.y);
	v2.textureCoords = Vector2D(0, 0);

	Vertex v3;
	v3.position = Vector2D(-extents.x, -extents.y);
	v3.textureCoords = Vector2D(0, textureSize.y);
	
	//build it
	meshFactory.AddTriangle(v0, v1, v2);
	meshFactory.AddTriangle(v1, v2, v3);
	return meshFactory.EndCreate();
}