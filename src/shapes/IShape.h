#pragma once
#include <glm/glm.hpp>
#include <vector>

class IShape {
public:
	virtual ~IShape() = default;

	// Return vertices of the shape
	const virtual std::vector<glm::vec3>& getVertices() const = 0;

	// Return the color of the shape
	virtual glm::vec4 getColor() const = 0;

	// Return the transformation matrix of the shape
	virtual glm::mat4 getTransform() const = 0;

	// Setters
	virtual void setColor(const glm::vec4& color) = 0;
	virtual void setRotation(float radians) = 0;
	virtual void setPosition(const glm::vec3& position) = 0;
	virtual void setScale(const glm::vec3& scale) = 0;

	// Getters
	virtual glm::vec3 getPosition() const = 0;
	virtual glm::vec3 getScale() const = 0;
	virtual float getRotation() const = 0;
};