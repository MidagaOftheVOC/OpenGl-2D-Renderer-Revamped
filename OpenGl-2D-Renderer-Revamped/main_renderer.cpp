
#include "main_renderer.h"






Drawable Renderer2D::GenerateDrawable(
	const char* _spriteSheetName,
	int _indexInSpriteSheet
) {
	SpriteSheet* SheetOfTarget = GetSpriteSheetByName(_spriteSheetName);

	DEBUG_ASSERT(SheetOfTarget != nullptr, "Indexing Drawable from non-existant SpriteSheet object or name.");

	return Drawable(SheetOfTarget, _indexInSpriteSheet);
}


void Renderer2D::Draw(
	const SoftBatch* _batch,
	float _xPosition,
	float _yPosition,
	float _zLayer,
	UniformDataVector* _uniformArray
) {
	m_SoftBatchArray.emplace_back(
		_batch,
		_xPosition,
		_yPosition,
		_zLayer,
		_uniformArray
	);
}


void Renderer2D::Draw(
	const Drawable* _drawable,
	float _xPosition,
	float _yPosition,
	float _zLayer,
	UniformDataVector* _uniformArray
) {
	m_DrawCallQueue.emplace(
		_drawable,
		_xPosition,
		_yPosition,
		_zLayer,
		_uniformArray
	);
}


void Renderer2D::Draw(
	const StrictBatch* _batch,
	float _initialXpos,
	float _initialYpos,
	float _zLayer,
	int _rowSpriteCount,
	UniformDataVector* _uniformArray
) {
	m_StrictBatchArray.emplace_back(
		_batch,
		_initialXpos,
		_initialYpos,
		_zLayer,
		_uniformArray,
		_rowSpriteCount
	);
}


void Renderer2D::ExecuteDraws() {

#ifdef DEBUG__CODE
	// easily recognisable when some sprite is black or at all drawn
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

#else

	glClearColor(0.f, 0.f, 0.f, 1.f);

#endif

	glClear(GL_COLOR_BUFFER_BIT);
	
	//	-- RENDERING STARTS --	//


	RenderDrawables();
	RenderStrictBatches();
	RenderSoftBatches();


	//	-- RENDERING ENDS	--	//

	glfwSwapBuffers(GetWinHandle());
	glfwPollEvents();
}


void Renderer2D::RenderSoftBatches() {
	std::vector<SoftBatchDrawCall>& Arr = m_SoftBatchArray;
	
	for (size_t i = 0; i < Arr.size(); i++) {
		const SoftBatchDrawCall& DrawCall = Arr[i];

		const SoftBatch* Soft = DrawCall.GetSoftBatchPointer();
		const SpriteSheet* Sheet = Soft->GetSheet();
		const Shader* Shader = Sheet->GetShader();

		int InstanceCount = Soft->GetInstanceCount();

		Soft->Bind();

		glBindTexture(GL_TEXTURE_2D, Sheet->GetTextureBufferID());
		GetQuad().BufferTexCoords(Sheet);

		Shader->UseShader();
		Shader->SetMat4("u_Model", glm::translate(glm::mat4(1.f), DrawCall.GetPositionVector()));
		Shader->SetMat4("u_View", GetCamera().GetViewMatrix());
		Shader->SetMat4("u_Projection", GetCamera().GetProjectionMatrix());

		Shader->SetInt("u_SheetRowSpriteCount", Sheet->GetSheetRowSpriteCount());
		Shader->SetVec2("u_SpriteTexUVDimensions", Sheet->GetSpriteDimensions());

		Shader->ApplyUniforms(DrawCall.GetAppliedUniforms());

		
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, InstanceCount);

		Soft->Unbind();
	}
	Arr.clear();
}


void Renderer2D::RenderStrictBatches() {

	//	Since these batches are guaranteed to use
	//	unique sprite sheets paired with unique shaders
	//	there's not much point in trying to optimise this part

	std::vector<StrictBatchDrawCall>& Arr = m_StrictBatchArray;

	for (size_t i = 0; i < Arr.size(); i++) {
		const StrictBatchDrawCall& DrawCallCurrent = Arr[i];
		
		const StrictBatch* Strict = DrawCallCurrent.GetStrictBatchPointer();
		const SpriteSheet* SheetCurrent = Strict->GetSheet();
		const Shader* ShaderCurrent = SheetCurrent->GetShader();

		int InstanceCount = Strict->GetInstanceCount();

		Strict->Bind();

		// SHEET SET UP
		glBindTexture(GL_TEXTURE_2D, SheetCurrent->GetTextureBufferID());
		GetQuad().BufferTexCoords(SheetCurrent);

		// SHADER SET UP
		ShaderCurrent->UseShader();
		ShaderCurrent->SetMat4("u_Projection", m_Camera.GetProjectionMatrix());
		ShaderCurrent->SetMat4("u_View", m_Camera.GetViewMatrix());
		ShaderCurrent->SetMat4("u_Model", glm::translate(glm::mat4(1.f), DrawCallCurrent.GetPositionVector()));

		ShaderCurrent->SetInt("u_SheetRowSpriteCount", SheetCurrent->GetSheetRowSpriteCount());
		ShaderCurrent->SetVec2("u_SpriteTexUVDimensions", SheetCurrent->GetSpriteDimensions());
		
		ShaderCurrent->SetInt("u_RowSpriteCount", DrawCallCurrent.GetRowSpriteCount());
		ShaderCurrent->SetInt("u_SpriteSideLengthPx", GetQuad().m_StandardSpritePixelLength);

		ShaderCurrent->ApplyUniforms(DrawCallCurrent.GetAppliedUniforms());


		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, InstanceCount);

		Strict->Unbind();
	}
	Arr.clear();
}


void Renderer2D::RenderDrawables() {

#ifdef DEBUG__CODE
	bool d_Print = false;
	int d_SheetChanges = 0;
	int d_ShaderChanges = 0;
#endif


	GetQuad().Bind();
	const Shader* LastUsedShader = nullptr;
	const SpriteSheet* LastUsedSheet = nullptr;

	size_t DrawQueueOriginalSize = m_DrawCallQueue.size();
	for (size_t i = 0; i < DrawQueueOriginalSize; i++)
	{
		DrawableDrawCall DrawcallCurrent = m_DrawCallQueue.top();
		m_DrawCallQueue.pop();

		const SpriteSheet* SheetCurrent = nullptr;
		const Shader* ShaderCurrent = nullptr;

		SheetCurrent = DrawcallCurrent.GetDrawablePointer()->GetAsociatedSpriteSheet();
		ShaderCurrent = SheetCurrent->GetShader();

		//const Shader* hui = GetShaderByName(ShaderCurrent->GetName().c_str());

		DEBUG_ASSERT(SheetCurrent != nullptr, "SpriteSheet* set to nullptr when executing draw calls.\n\tDrawable object has initial index [%llu] in queue.", i);
		DEBUG_ASSERT(ShaderCurrent != nullptr, "Shader* set to nullptr when executing draw calls.\n\t-- from SpriteSheet with name [%s]", SheetCurrent->GetName().c_str());

		if (LastUsedSheet != SheetCurrent) {	//	SHADER CHANGE
			LastUsedSheet = SheetCurrent;
			glBindTexture(GL_TEXTURE_2D, SheetCurrent->GetTextureBufferID());
			GetQuad().BufferTexCoords(SheetCurrent);

#ifdef DEBUG__CODE
			d_SheetChanges++;
#endif
		}

		if (LastUsedShader != ShaderCurrent) {	//	SPRITESHEET CHANGE
			LastUsedShader = ShaderCurrent;
			ShaderCurrent->UseShader();
			ShaderCurrent->SetMat4("u_Projection", m_Camera.GetProjectionMatrix());
			ShaderCurrent->SetMat4("u_View", m_Camera.GetViewMatrix());

#ifdef DEBUG__CODE
			d_ShaderChanges++;
#endif
		}

		//	per-instance data, unskippable
		ShaderCurrent->SetMat4("u_Model", glm::translate(glm::mat4(1.f), DrawcallCurrent.GetPositionVector()));
		ShaderCurrent->SetVec2("u_SpriteOffsets",
			SheetCurrent->GetCalculatedSpriteOffsets(DrawcallCurrent.GetDrawablePointer()->GetSpriteIndex()));

		ShaderCurrent->ApplyUniforms(DrawcallCurrent.GetAppliedUniforms());

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr));
	}

#ifdef DEBUG__CODE
	if(d_Print)
	fprintf(stdout, "ExecuteDraws() stats for [%lld] DrawCall objects:\n\tSheet changes: %d\n\tShader changes: %d\n",
		DrawQueueOriginalSize, d_SheetChanges, d_ShaderChanges);
#endif

	//	CLEAN UP
	GetQuad().Unbind();
}


bool Renderer2D::Init() {

	//	Window and GLFW context initialisation
	if (!glfwInit()) {
		std::cout << "Window not opened.\n";
		return false;
	}

	m_MainWindowHandle = glfwCreateWindow(
		m_ScreenWidth,
		m_ScreenHeight,
		m_WindowTitle.c_str(),
		NULL, NULL
	);

	if (!m_MainWindowHandle) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_MainWindowHandle);
	glfwSwapInterval(1);
	GLenum glewReturnCode = glewInit();
	if (glewReturnCode != GLEW_OK) {
		std::cout << glewGetErrorString(glewReturnCode);
		return false;
	}

	//	Component initialisation

	m_Camera.Initialisation(
		{ 1, 1 },
		{ m_ScreenWidth, m_ScreenHeight }
	);

	m_InputController = InputController(
		GetWinHandle()
	);

	m_InputController.SetTrackedKeystatesBitmask(
		InputController::c_ArrowTrackBit |
		InputController::c_LetterTrackBit
	);
	
	//	Globals initialisation

	g_StandardQuad.Init();

	return true;
}


Renderer2D::Renderer2D(
	int _screenWidth,
	int _screenHeight,
	const char* _windowTitle,
	bool _fullscreen
)
	: m_ScreenWidth(_screenWidth),
	m_ScreenHeight(_screenHeight),
	m_WindowTitle(_windowTitle),
	m_Fullscreen(_fullscreen),
	m_StandardQuad(g_StandardQuad)
{}

void Renderer2D::LoadShader(
	const std::string& _locationShaderFile,
	const std::string& _shaderName
) {
	m_ShaderArray.emplace_back(
		_locationShaderFile,
		_shaderName
	);
}


void Renderer2D::LoadSpriteSheet(
	const std::string& _locationRawImage,
	const std::string& _sheetName,
	const Shader* _preferredShader,
	int _spritesPerRow,
	int _spritesPerCol
) {
	m_SpriteSheetArray.emplace_back(
		_locationRawImage,
		_sheetName,
		_preferredShader,
		_spritesPerRow,
		_spritesPerCol
	);
}

SpriteSheet* Renderer2D::GetSpriteSheetByName(
	const char* _spriteSheetName
) {
	size_t len = m_SpriteSheetArray.size();
	for (size_t i = 0; i < len; i++) {
		if (
			strcmp(_spriteSheetName, m_SpriteSheetArray[i].GetName().c_str()) == 0
			) {
			return &m_SpriteSheetArray[i];
		}
	}
	DEBUG_WARN(0, "GetSpriteSheetByName() for name [%s] returned nullptr.", _spriteSheetName);
	return nullptr;
}

Shader* Renderer2D::GetShaderByName(
	const char* _shaderName
) {
	size_t len = m_ShaderArray.size();
	for (size_t i = 0; i < len; i++) {
		if (
			strcmp(_shaderName, m_ShaderArray[i].GetName().c_str()) == 0
			) {
			return &m_ShaderArray[i];
		}
	}
	DEBUG_WARN(0, "GetShaderByName() for name [%s] returned nullptr.", _shaderName);
	return nullptr;
}


void Renderer2D::UploadShaderParameters(
	const char* _location,
	const char* _shaderName
) {
	m_ShaderLoadQueue.emplace_back(_location, _shaderName);
}

void Renderer2D::UploadSpriteSheetParameters(
	const char* _locationRawImage,
	const char* _sheetName,
	const char* _preferredShader,
	int _spritesPerRow,
	int _spritesPerCol
) {
	m_SpriteSheetLoadQueue.emplace_back(
		_locationRawImage,
		_sheetName,
		_preferredShader,
		_spritesPerRow,
		_spritesPerCol
	);
}

void Renderer2D::StartLoadingProcess() {

	m_ShaderArray.clear();
	m_SpriteSheetArray.clear();

	if (m_ShaderArray.capacity() <= m_ShaderLoadQueue.size()) {
		m_ShaderArray.reserve(m_ShaderLoadQueue.size());
	}

	for (size_t i = 0; i < m_ShaderLoadQueue.size(); i++) {
		ShaderLoadingParameters& params = m_ShaderLoadQueue[i];
		LoadShader(
			std::string(params.m_LocationOfShaderFile),
			std::string(params.m_ShaderName)
		);
	}
	m_ShaderLoadQueue.clear();

	
	if (m_SpriteSheetArray.capacity() <= m_SpriteSheetLoadQueue.size()) {
		m_SpriteSheetArray.reserve(m_SpriteSheetLoadQueue.size());
	}

	for (size_t i = 0; i < m_SpriteSheetLoadQueue.size(); i++) {
		SpriteSheetLoadingParameters& params = m_SpriteSheetLoadQueue[i];
		LoadSpriteSheet(
			std::string(params.m_LocationOfImage),
			std::string(params.m_SheetName),
			GetShaderByName(params.m_PreferredShaderName.c_str()),
			params.m_SpritesPerRow,
			params.m_SpritesPerCol
		);
	}
	m_SpriteSheetLoadQueue.clear();

}


bool Renderer2D::IsRunning() const {
	return glfwWindowShouldClose(GetWinHandle());
}




bool Renderer2D::DrawCallComparator::operator()(const DrawableDrawCall& a, const DrawableDrawCall& b) const {

	const Shader* aS = a.GetDrawablePointer()->GetAsociatedSpriteSheet()->GetShader();
	const Shader* bS = b.GetDrawablePointer()->GetAsociatedSpriteSheet()->GetShader();


	if (aS == bS) {
		return a.GetDrawablePointer()->GetAsociatedSpriteSheet() > b.GetDrawablePointer()->GetAsociatedSpriteSheet();
	}

	return aS > bS;
}

