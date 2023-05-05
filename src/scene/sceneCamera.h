#ifndef SCENECAMERA_H
#define SCENECAMERA_H

#include "../renderer/camera.h"
#include "../utils/psxint.h"

namespace psx {
	class SceneCamera: public Camera{
		public:
			enum class ProjectionType { Perspective = 0, Ortographic = 1 };
		public:

			SceneCamera();
			virtual ~SceneCamera() = default;

			void SetPerspective(float verticalFOV, float nearClip, float farClip);
			void SetOrthographic(float size, float nearClip, float farClip);

			void SetViewportSize(u32 width, u32 height);

			float GetPerspectiveVerticalFOV() const { return m_perspectiveFOV; }
			void SetPerspectiveVerticalFOV(float verticalFOV) { m_perspectiveFOV = verticalFOV; RecalculateProjection();}
			
			float GetPerspectiveNearClip() const { return m_perspectiveNear; }
			void SetPerspectiveNearClip(float nearClip) { m_perspectiveNear = nearClip; RecalculateProjection(); }

			float GetPerspectiveFarClip() const { return m_perspectiveFar; }
			void SetPerspectiveFarClip(float farClip) { m_perspectiveFar = farClip; RecalculateProjection(); }

			float GetOrthographicSize() const { return m_orthographicSize; }
			void SetOrthographicSize(float size) { m_orthographicSize = size; RecalculateProjection(); }

			float GetOrthographicNearClip() const { return m_orthographicNear; }
			void SetOrthographicNearClip(float nearClip) { m_orthographicNear = nearClip; RecalculateProjection(); }

			float GetOrthographicFarClip() const { return m_orthographicFar; }
			void SetOrthographicFarClip(float farClip) { m_orthographicFar = farClip; RecalculateProjection(); }

			ProjectionType GetProjectionType() const { return m_projectionType; }
			void SetProjectionType(ProjectionType type) { m_projectionType = type; RecalculateProjection(); }
		private:
			void RecalculateProjection();
		private:
			ProjectionType m_projectionType = ProjectionType::Ortographic;

			float m_perspectiveFOV = glm::radians(45.f);
			float m_perspectiveNear = 0.01f, m_perspectiveFar = 1000.0f;

			float m_orthographicSize = 100.f;
			float m_orthographicNear = -1.f, m_orthographicFar = 10.f;

			float m_aspectRatio = 0.f;
	};
}

#endif

