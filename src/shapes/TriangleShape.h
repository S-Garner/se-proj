#pragma once
#include "IShape.h"
#include <glm/gtc/matrix_transform.hpp>

class TriangleShape : public IShape {
private:
	std::vector<glm::vec3> vertices;
	glm::vec4 color;

	glm::vec3 position;
	glm::vec3 scale;
	float rotation; // in radians

	float red;
	float green;
	float blue;
	float alpha;

public:
	TriangleShape() {
		position = glm::vec3(0.0f, 0.0f, 0.0f);

		scale = glm::vec3(1.0f, 1.0f, 1.0f);

		rotation = 0.0f; // No rotation initially

		color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Default white color

		vertices = {
			glm::vec3(0.0f,  0.5f, 0.0f),
			glm::vec3(-0.5f, -0.5f, 0.0f),
			glm::vec3(0.5f, -0.5f, 0.0f)
		};
	}

	const std::vector<glm::vec3>& getVertices() const override { return vertices; }
	size_t getVertexCount() const {
		return vertices.size() / 3;
	}

	glm::vec4 getColor() const override { return color; }

	glm::mat4 getTransform() const override {
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, scale);
		return transform;
	}

	// Setters
	void setPosition(const glm::vec3& pos) override { position = pos; }
	void setRotation(float radians) override { rotation = radians; }
	void setScale(const glm::vec3& scl) override { scale = scl; }
	void setColor(const glm::vec4& col) override { color = col; }

	//void setRed(const float red) { color = glm::}

	glm::vec3 getPosition() const override { return position; }
	float getRotation() const override { return rotation; }
	glm::vec3 getScale() const override { return scale; }

};
