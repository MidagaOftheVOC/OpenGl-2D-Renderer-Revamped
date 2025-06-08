#include "free_batch.h"

unsigned int FreeBatch::s_VAO = 0;




FreeBatch::FreeBatch(
	int _instanceCount
) :
	BaseBatch(_instanceCount)
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


	////	@0 2 floats / vertex / 4 per quad	- standard quad for each instance
	//glBindVertexBuffer(0, 0, 0, sizeof(float) * 2);
	//glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribBinding(0, 0);


	//	@0 8* floats / instance, split in 4 vertices - quad's coords

	//	@1 1 unsigned int / instance - sheet/sprite indices in one

	//	@2 1 float / instance - rotations

	//	@3 2 floats / instance - relative positions

	//	++ 2 UBOs for sheet/sprite data.

	





	glBindVertexArray(0);
}


void FreeBatch::UpdateBuffers(
	const int* _spriteIndices,
	const float* _rotationsArray,
	const float* _pairsOfXYpositions,
	const size_t _arrayElementCount
) {

	SetInstanceCount(static_cast<int>(_arrayElementCount));
	


}




void FreeBatch::BindUniqueBuffers() const {


}


void FreeBatch::BindCommonVAO() {
	glBindVertexArray(s_VAO);
}


void FreeBatch::UnbindCommonVAO() {
	glBindVertexArray(0);
}
