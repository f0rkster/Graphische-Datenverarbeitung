#pragma once

struct SNode {
	float m_Translation[3];
	float m_TranslationDelta[3];
	float m_Scale[3];
	float m_AngleZ;
	float m_AngleDeltaZ;
	float m_LocalMatrix[16];
	float m_GlobalMatrix[16];
	SNode* m_Child = nullptr;
};