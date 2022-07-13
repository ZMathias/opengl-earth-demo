#pragma once
#include <vector>
#include <glm/glm.hpp>

class SphereGenerator
{
	size_t Resolution{};
	int width{}, height{}, nrChannels{};
	unsigned char* data{};

	int minLat = INT_MAX, maxLat = INT_MIN;
	int minLong = INT_MAX, maxLong = INT_MIN;

	void ConstructMesh(glm::vec3 Y, const unsigned int offset = 0)
	{
		const glm::vec3 X{Y.y, Y.z, Y.x}, Z{cross(Y, X)};
		for (size_t y = 0; y < Resolution; ++y)
			for (size_t x = 0; x < Resolution; ++x)
			{
				const size_t i = Resolution * y + x;
				glm::vec2 point = glm::vec2(x, y);
				point /= Resolution - 1;

				// transforms the point to the model coordinate of a 2 unit cube with it's center at the origin (0, 0, 0)
				glm::vec3 pointOnUnitCube = Y + (point.x - 0.5f) * 2 * X + (point.y - 0.5f) * 2 * Z;
				pointOnUnitCube = normalize(pointOnUnitCube);
				pointOnUnitCube *= 2;
				// printf("%f\n", glm::length(pointOnUnitCube));
				//PointOnCubeToSphere(pointOnUnitCube);
				//glm::vec2 latlong = PointToLatAndLong(pointOnUnitCube);

				//pointOnUnitCube.x = latlong.x;
				//pointOnUnitCube.y = latlong.y;
				//pointOnUnitCube.z *= data[i+80000] / 64;

				/*if  (coord < width * height) pointOnUnitCube.z *= data[coord];
				else
				{
					pointOnUnitCube.z = 0.0f;
				}*/

				vertices.emplace_back(pointOnUnitCube.x);
				vertices.emplace_back(pointOnUnitCube.y);
				vertices.emplace_back(pointOnUnitCube.z);

				if (x != Resolution - 1 && y != Resolution - 1)
				{
					indices.push_back(i + offset);						//    ---
					indices.push_back(i + 1 + offset);					//	  \ |
					indices.push_back(i + Resolution + 1 + offset);		//	   \|

					indices.push_back(i + offset);						/* |\   */  
					indices.push_back(i + Resolution + 1 + offset);		/* | \  */
					indices.push_back(i + Resolution + offset);			/* |__\ */
				}
			}
	}

	glm::vec2 PointToLatAndLong(const glm::vec3& p) const
	{
		return {asin(p.y), atan2(p.x, -p.z)};
	}

	/*void CalculateNormals()
	{
		for (size_t i = 0; i < indices.size() / 3; i += 3)
		{
			const size_t idx1 = indices[i * 3];
			const size_t idx2 = indices[i * 3 + 1];
			const size_t idx3 = indices[i * 3 + 2];

			const glm::vec3 p1 = {vertices[idx1], vertices[idx1 + 1], vertices[idx1 + 2]};
			const glm::vec3 p2 = {vertices[idx2], vertices[idx2 + 1], vertices[idx2 + 2]};
			const glm::vec3 p3 = {vertices[idx3], vertices[idx3 + 1], vertices[idx3 + 2]};

			const glm::vec3 u = p2 - p1, v = p3 - p1;

			// calculate a normal (a vector that's perpendicular to the primitives' surface

			sphere.push_back(p1);
			sphere.push_back(p2);
			sphere.push_back(p3);

			sphere.push_back(cross(u, v).x);
			sphere.push_back(cross(u, v).y);
			sphere.push_back(cross(u, v).z);
		}
	}*/
	
public:
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	std::vector<float> sphere;

	SphereGenerator(const size_t resolution) : Resolution(resolution)
	{
		data = stbi_load(R"(F:\prj\C++\GeoWizard\src\res\heightmap.png)", &width, &height, &nrChannels, 0);
	}

	~SphereGenerator()
	{
		stbi_image_free(data);
	}

	void PointOnCubeToSphere(glm::vec3& p) const
	{
		const float x2 = p.x * p.x;
		const float y2 = p.y * p.y;
		const float z2 = p.z * p.z;

		p.x = p.x * sqrt(1 - (y2 + z2) / 2 + (y2 * z2) / 3);
		p.y = p.y * sqrt(1 - (z2 + x2) / 2 + (z2 * x2) / 3);
		p.z = p.z * sqrt(1 - (x2 + y2) / 2 + (x2 * y2) / 3);
		p = normalize(p);
	}

	void GenerateCube()
	{
		constexpr glm::vec3 faceDirs[] = 
		{
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{-1.0f, 0.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},
			{0.0f, 0.0f, -1.0f}
		};

		ConstructMesh(faceDirs[0]);
		for (unsigned int i = 1; i < 6; ++i)
		{
			ConstructMesh(faceDirs[i], pow(Resolution, 2) * i);
		}

		//CalculateNormals();
	}
};

