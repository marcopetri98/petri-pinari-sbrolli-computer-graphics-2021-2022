#include <array>
#include <vector>
#include "SphereCollider.hpp"

using std::copy, std::begin, std::end, std::vector;
using graphics::GameObject;

using std::copy, std::begin, std::end, std::vector;
using graphics::GameObject;

namespace graphics {
	SphereCollider::SphereCollider(GameObject gameObject, float radius) {
		this->gameObject = gameObject;
		this->radius = radius;
	}

	float SphereCollider::getSize() {
		return this->radius;
	}

	GameObject SphereCollider::getGameObject()
	{
		return this->gameObject;
	}

	bool SphereCollider::checkCollision(vec3 point) {
		vec3 diff = point - this->gameObject.getCurrentPos();
		return	pow(diff.x, 2) + 
			    pow(diff.y, 2) +
				pow(diff.z, 2) >= 0;
	}
}