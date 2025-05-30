#include "free_batch.h"

unsigned int FreeBatch::s_VAO = 0;

unsigned int FreeBatch::s_EBO = 0;

unsigned int FreeBatch::s_BufferedInstanceCount = 0;

FreeBatch::FreeBatch(
	const SpriteSheet* _spriteSheet,
	int _instanceCount
) :
	BaseBatch(_spriteSheet, _instanceCount)
{}


void FreeBatch::InitialiseBuffers() {
	unsigned int vbo[4];
	glCreateBuffers(4, vbo);

	m_VertexVBO = vbo[0];
	m_UVRegionVBO = vbo[1];
	m_RotationsVBO = vbo[2];
	m_PositionsVBO = vbo[3];
}


void FreeBatch::DeleteBuffers() {
	glDeleteBuffers(1, &m_VertexVBO);
	glDeleteBuffers(1, &m_UVRegionVBO);
	glDeleteBuffers(1, &m_RotationsVBO);
	glDeleteBuffers(1, &m_PositionsVBO);
}


void FreeBatch::InitialiseCommonVAO() {


	if (GLdiagnostics::IsVertexArray(s_VAO)) {
		DEBUG_ASSERT(0, "Attempt to reinitialise common FreeBatch VAO object.");
		return;
	}


	glGenVertexArrays(1, &s_VAO);
	glBindVertexArray(s_VAO);


	glGenBuffers(1, &s_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_EBO);


	//	@0 2 floats / vertex / 4 per quad	- standard quad for each instance
	glBindVertexBuffer(0, 0, 0, sizeof(float) * 2);
	glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribBinding(0, 0);


	//	@1 4 floats / vertex / 1 per quad	- standard texture UV region
	glBindVertexBuffer(1, 0, 0, sizeof(UVRegion));
	glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribBinding(1, 1);


	//	@2 1 float / vertex	/ 1 per quad	- angle of rotations in radians
	glBindVertexBuffer(2, 0, 0, sizeof(float));
	glVertexAttribFormat(2, 1, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribBinding(2, 2);


	//	@3 2 floats / vertex / 1 per quad	- XY coords relative to draw call coords
	glBindVertexBuffer(3, 0, 0, sizeof(float) * 2);
	glVertexAttribFormat(3, 2, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(3);
	glVertexAttribBinding(3, 3);

	glBindVertexArray(0);
}


void FreeBatch::UpdateBuffers(
	const int* _spriteIndices,
	const float* _rotationsArray,
	const float* _pairsOfXYpositions,
	const size_t _arrayElementCount
) {

	SetInstanceCount(static_cast<int>(_arrayElementCount));
	std::vector<UVRegion> UVs;
	std::vector<float> Vertices;
	float* VertexArray = nullptr;
	UVRegion* UVArray = nullptr;

	if (_spriteIndices) {
		GetSheet()->TransformIndicesToUVRegionArray(_spriteIndices, GetInstanceCount(), UVs, &Vertices);
		VertexArray = Vertices.data();
		UVArray = UVs.data();
	}

	if (m_Flags.CheckAndClearFlag(c_MaximumInstanceCountExceeded)) {

		glNamedBufferData(m_VertexVBO, GetInstanceCount() * sizeof(float) * 8, VertexArray, GL_DYNAMIC_DRAW);
		glNamedBufferData(m_UVRegionVBO, GetInstanceCount() * sizeof(UVRegion), UVArray, GL_DYNAMIC_DRAW);
		glNamedBufferData(m_RotationsVBO, GetInstanceCount() * sizeof(float), _rotationsArray, GL_DYNAMIC_DRAW);
		glNamedBufferData(m_PositionsVBO, GetInstanceCount() * sizeof(float) * 2, _pairsOfXYpositions, GL_DYNAMIC_DRAW);
		
		ResizeEBO(GetInstanceCount());

		return;
	}

	if (VertexArray)			glNamedBufferSubData(m_VertexVBO, 0, GetInstanceCount() * sizeof(float) * 8, VertexArray);
	if (UVArray)				glNamedBufferSubData(m_UVRegionVBO, 0, GetInstanceCount() * sizeof(UVRegion), UVArray);
	if (_rotationsArray)		glNamedBufferSubData(m_RotationsVBO, 0, GetInstanceCount() * sizeof(float), _rotationsArray);
	if (_pairsOfXYpositions)	glNamedBufferSubData(m_PositionsVBO, 0, GetInstanceCount() * sizeof(float) * 2, _pairsOfXYpositions);

}


void FreeBatch::ResizeEBO(
	unsigned int _newSizeForHowManyInstances
) {

	if (_newSizeForHowManyInstances <= s_BufferedInstanceCount) return;

	bool IsInitialised = GLdiagnostics::IsVertexBuffer(s_EBO);
	bool IsNegativeResizing = _newSizeForHowManyInstances >= 0;

	DEBUG_ASSERT(IsInitialised, "Attempting to buffer uninitialised common FreeBatch EBO.");
	DEBUG_ASSERT(IsNegativeResizing, "Negative value passed to ResizeEBO() for FreeBatch class.");

	if (!IsInitialised || !IsNegativeResizing) return;

	std::vector<unsigned short> Indices;
	Indices.resize(_newSizeForHowManyInstances * 6);

	int Index = 0;

	for (unsigned int i = 0; i < _newSizeForHowManyInstances * 6; i += 6, Index += 4) {
		Indices[i + 0] = Index + 0;
		Indices[i + 1] = Index + 1;
		Indices[i + 2] = Index + 2;
		Indices[i + 3] = Index + 0;
		Indices[i + 4] = Index + 2;
		Indices[i + 5] = Index + 3;
	}
	
	glNamedBufferData(s_EBO, _newSizeForHowManyInstances * 6, Indices.data(), GL_DYNAMIC_DRAW);
}


void FreeBatch::BindUniqueBuffers() const {
	glBindVertexBuffer(0, m_VertexVBO, 0, sizeof(float) * 2);
	glBindVertexBuffer(1, m_UVRegionVBO, 0, sizeof(UVRegion));
	glBindVertexBuffer(2, m_RotationsVBO, 0, sizeof(float));
	glBindVertexBuffer(3, m_PositionsVBO, 0, sizeof(float) * 2);
}


void FreeBatch::BindCommonVAO() {
	glBindVertexArray(s_VAO);
}


void FreeBatch::UnbindCommonVAO() {
	glBindVertexArray(0);
}
