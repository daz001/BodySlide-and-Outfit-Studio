/*
BodySlide and Outfit Studio
See the included LICENSE file
*/

#pragma once

#include "../files/MaterialFile.h"
#include "../render/GLExtensions.h"
#include "../utils/AABBTree.h"

#pragma warning(push, 0)
#include "../gli/glm/gtc/matrix_transform.hpp"
#include "../gli/glm/gtx/euler_angles.hpp"
#pragma warning(pop)

#include <array>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>

class GLMaterial;

class mesh {
private:
	std::array<bool, 10> queueUpdate = {false};

public:
	enum class RenderMode { Normal, UnlitSolid, UnlitWire, UnlitWireDepth, UnlitPoints, UnlitPointsDepth, LitWire };
	enum UpdateType { Position, Normals, Tangents, Bitangents, VertexColors, VertexAlpha, TextureCoordinates, Mask, Weight, Indices };

	struct ShaderProperties {
		nifly::Vector2 uvOffset;
		nifly::Vector2 uvScale = nifly::Vector2(1.0f, 1.0f);
		nifly::Vector3 specularColor = nifly::Vector3(1.0f, 1.0f, 1.0f);
		float specularStrength = 1.0f;
		float shininess = 30.0f;
		float envReflection = 1.0f;
		nifly::Vector3 emissiveColor = nifly::Vector3(1.0f, 1.0f, 1.0f);
		float emissiveMultiple = 1.0f;
		float alpha = 1.0f;
		float backlightPower = 0.0f;
		float rimlightPower = 2.0f;
		float softlighting = 0.3f;
		float subsurfaceRolloff = 0.3f;
		float fresnelPower = 5.0f;
		float paletteScale = 0.0f;
	};

	glm::mat4x4 matModel = glm::identity<glm::mat4x4>();

	int nVerts = 0;
	std::unique_ptr<nifly::Vector3[]> verts;
	std::unique_ptr<nifly::Vector3[]> norms;
	std::unique_ptr<nifly::Vector3[]> tangents;
	std::unique_ptr<nifly::Vector3[]> bitangents;
	std::unique_ptr<nifly::Vector3[]> vcolors;
	std::unique_ptr<float[]> valpha;
	std::unique_ptr<nifly::Vector2[]> texcoord;
	std::unique_ptr<float[]> mask;
	std::unique_ptr<float[]> weight;

	std::unique_ptr<nifly::Triangle[]> tris;
	// renderTris is tris re-ordered for rendering with submeshes.  It's
	// created automatically in CreateBuffers as a copy of tris.  If
	// something changes tris, renderTris needs to be updated too.
	std::unique_ptr<nifly::Triangle[]> renderTris;
	int nTris = 0;

	std::unique_ptr<nifly::Edge[]> edges;
	int nEdges = 0;

	bool genBuffers = false;
	GLuint vao = 0;
	std::vector<GLuint> vbo = std::vector<GLuint>(9, 0);
	GLuint ibo = 0;

	std::vector<std::pair<uint32_t, uint32_t>> subMeshes; // Start index and size of each sub mesh
	std::vector<nifly::Vector3> subMeshesColor;			  // Color of each sub mesh

	ShaderProperties prop;
	GLMaterial* material = nullptr;

	uint32_t overlayLayer = 0;					 // Layer for order of rendering overlays
	float smoothThresh = 60.0f * nifly::DEG2RAD; // Smoothing threshold for generating smooth normals.

	std::unique_ptr<std::vector<int>[]> vertTris;		 // Map of triangles for which each vert is a member.
	std::unique_ptr<std::vector<int>[]> vertEdges;		 // Map of edges for which each vert is a member.
	std::unordered_map<int, std::vector<int>> weldVerts; // Verts that are duplicated for UVs but are in the same position.
	bool bGotWeldVerts = false;							 // Whether weldVerts has been calculated yet.

	std::unordered_set<uint32_t> lockedNormalIndices;

	RenderMode rendermode = RenderMode::Normal;
	bool doublesided = false;
	GLenum cullMode = GL_BACK;
	bool modelSpace = false;
	bool emissive = false;
	bool specular = true;
	bool vertexColors = false;
	bool vertexAlpha = false;
	bool backlight = false;
	bool backlightMap = false;
	bool rimlight = false;
	bool softlight = false;
	bool glowmap = false;
	bool greyscaleColor = false;
	bool cubemap = false;
	bool textured = false;

	std::shared_ptr<AABBTree> bvh = nullptr;

	bool bVisible = true;
	bool bShowPoints = false;
	bool smoothSeamNormals = true;
	bool lockNormals = false;

	uint16_t alphaFlags = 0;
	uint8_t alphaThreshold = 0;

	std::string shapeName;
	nifly::Vector3 color;

	mesh();
	~mesh();

	// Creates a new bvh tree for the mesh.
	std::shared_ptr<AABBTree> CreateBVH();

	void MakeEdges(); // Creates the list of edges from the list of triangles.

	void BuildTriAdjacency(); // Triangle adjacency optional to reduce overhead when it's not needed.
	void BuildEdgeList();	  // Edge list optional to reduce overhead when it's not needed.

	void CalcWeldVerts();

	void CreateBuffers();
	void UpdateBuffers();
	void QueueUpdate(const UpdateType& type);
	void UpdateFromMaterialFile(const MaterialFile& matFile);
	bool HasAlphaBlend();

	void ScaleVertices(const nifly::Vector3& center, const float& factor);

	void SetSmoothThreshold(float degrees);
	float GetSmoothThreshold();

	void FacetNormals();
	void SmoothNormals(const std::set<int>& vertices = std::set<int>());
	static void SmoothNormalsStatic(mesh* m) { m->SmoothNormals(); }
	static void SmoothNormalsStaticArray(mesh* m, int* vertices, int nVertices) {
		std::set<int> verts;
		for (int i = 0; i < nVertices; i++)
			verts.insert(vertices[i]);

		m->SmoothNormals(verts);
	}
	static void SmoothNormalsStaticMap(mesh* m, const std::unordered_map<int, nifly::Vector3>& vertices) {
		std::set<int> verts;
		for (auto& v : vertices)
			verts.insert(v.first);

		m->SmoothNormals(verts);
	}

	void CalcTangentSpace();

	// Retrieve connected points in a sphere's radius (squared, requires tri adjacency to be set up).
	// Also requires pointvisit to be allocated by the caller.
	// Recursive - large query will overflow the stack!
	bool ConnectedPointsInSphere(
		nifly::Vector3 center, float sqradius, int startTri, bool* trivisit, bool* pointvisit, int outPoints[], int& nOutPoints, std::vector<int>& outFacets);

	// Similar to above, but uses an edge list to determine adjacency, with less risk of stack problems.
	// Also requires trivisit and pointvisit to be allocated by the caller.
	bool ConnectedPointsInSphere2(
		nifly::Vector3 center, float sqradius, int startTri, bool* trivisit, bool* pointvisit, int outPoints[], int& nOutPoints, std::vector<int>& outFacets);

	// Convenience function to gather connected points, taking into account "welded" vertices. Does not clear the output set.
	void GetAdjacentPoints(int querypoint, std::set<int>& outPoints);

	// More optimized adjacency fetch, using edge adjacency and storing the output in a static array.
	// Requires that BuildEdgeList() be called prior to use.
	int GetAdjacentPoints(int querypoint, int outPoints[], int maxPoints);
	// As above, but also checks if each point has already been looked at (according to vispoint).
	int GetAdjacentUnvisitedPoints(int querypoint, int outPoints[], int maxPoints, bool* visPoint);

	// Creates the vertex color array (if necessary) and sets all the colors to the provided value.
	void ColorFill(const nifly::Vector3& color);
	void AlphaFill(float alpha);
	void MaskFill(float maskValue);
	void WeightFill(float weightValue);

	void ColorChannelFill(int channel, float value);

	static nifly::Vector3 VecToMeshCoords(const nifly::Vector3& vec) {
		nifly::Vector3 vecNew = vec;
		vecNew.x /= -10.0f;
		vecNew.y /= 10.0f;
		vecNew.z /= 10.0f;
		std::swap(vecNew.y, vecNew.z);
		return vecNew;
	}

	static nifly::Vector3 VecToNifCoords(const nifly::Vector3& vec) {
		nifly::Vector3 vecNew = vec;
		vecNew.x *= -10.0f;
		vecNew.y *= 10.0f;
		vecNew.z *= 10.0f;
		std::swap(vecNew.y, vecNew.z);
		return vecNew;
	}

	static glm::mat4x4 TransformToMatrix4(const nifly::MatTransform& xform) {
		auto mat44 = glm::identity<glm::mat4x4>();

		float y, p, r;
		xform.rotation.ToEulerAngles(y, p, r);

		mat44 = glm::translate(mat44, glm::vec3(xform.translation.x, xform.translation.y, xform.translation.z));
		mat44 *= glm::eulerAngleXZY(y, p, r);
		mat44 = glm::scale(mat44, glm::vec3(xform.scale, xform.scale, xform.scale));
		return mat44;
	}

	static nifly::Vector3 ApplyMatrix4(const glm::mat4x4& mat, const nifly::Vector3& p) {
		glm::vec3 gp(mat * glm::vec4(p.x, p.y, p.z, 1.0f));
		return nifly::Vector3(gp.x, gp.y, gp.z);
	}
};
