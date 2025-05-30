
#include "main_renderer.h"


Drawable Renderer2D::GenerateDrawable(
	const char* _spriteSheetName,
	int _indexInSpriteSheet
) {
	const SpriteSheet* SheetOfTarget = GetSpriteSheetByName(_spriteSheetName);

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
	const FreeBatch* _batch,
	float _xPosition,
	float _yPosition,
	float _zLayer,
	UniformDataVector* _uniformArray
) {
	m_FreeBatchArray.emplace_back(
		_batch,
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
	UniformDataVector* _uniformArray
) {
	m_StrictBatchArray.emplace_back(
		_batch,
		_initialXpos,
		_initialYpos,
		_zLayer,
		_uniformArray
	);
}


void Renderer2D::ExecuteDraws() {

#ifdef DEBUG__CODE
	// easily recognisable when some sprite is black or at all drawn
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

#else

	glClearColor(0.f, 0.f, 0.f, 1.f);

#endif
	//TODO:
	//	At some point, all of these queues and arrays will be abstracted away into
	//	"scenes" or similar objects, where we don't have to add, remove, readd and
	//  so on draw call objects, but just maintain them and add/remove on necessity.
	//
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//	-- RENDERING STARTS --	//


	RenderDrawables();
	RenderStrictBatches();
	RenderSoftBatches();
	RenderFreeBatches();

	RenderText();


	//	-- RENDERING ENDS	--	//

	glfwSwapBuffers(GetWinHandle());
	glfwPollEvents();
}


void Renderer2D::RenderText() {
	std::vector<TextDrawCall>& Arr = m_TextArray;

	size_t ArraySize = Arr.size();

	if (ArraySize == 0) {
		return;
	}

	Text::BindCommonVAO();

	//	Get the common shader only used for text rendering
	const Shader& Shader = GetTextShader();
	Shader.UseShader();
	Shader.SetStandardView(GetCamera().GetViewMatrix());
	Shader.SetStandardProjection(GetCamera().GetProjectionMatrix());

	//	Keep this for now, remove later when we reorganise the structures better
	const SpriteSheet* LastUsedSpriteSheet = nullptr;

	//	TODO: Refactor to use priority_queue, sort by Font* -> SpriteSheet*
	for (size_t i = 0; i < ArraySize; i++) {

		const TextDrawCall& DrawCall = Arr[i];

		const Text* TextObject = DrawCall.GetTextPointer();
		const SpriteSheet* SheetObject = TextObject->GetFont()->GetFontSheet();
		const TextOptions& OptionsObject = TextObject->GetTextOptions();

		const size_t CharInstances = TextObject->GetCharCount();
		const int LineBreakCount = static_cast<int>(TextObject->GetLineBreakCount());

		TextObject->BindUniqueBuffers();
		
		if( SheetObject != LastUsedSpriteSheet)
		{
			//	TODO: start finding alternatives to this
			glBindTexture(GL_TEXTURE_2D, SheetObject->GetTextureBufferID());
			GetQuad().BufferTexCoords(SheetObject);

			LastUsedSpriteSheet = SheetObject;
		}
		
		
		Shader.SetStandardModel(glm::translate(glm::mat4(1.f), DrawCall.GetPositionVector()));

		//	Sprite indices are passed already, so we need sprite dimensions sent
		Shader.SetVec2("u_SpriteDimensions", SheetObject->GetSpriteDimensions());
		Shader.SetInt("u_RowSpriteCount", SheetObject->GetSheetRowSpriteCount());

		//	Line breaks
		Shader.SetInt("u_LineBreakCount", LineBreakCount);
		Shader.SetIntArray("u_LineBreakArray", TextObject->GetLineBreakArray().data(), LineBreakCount);
		Shader.SetFloatArray("u_LineLengths", TextObject->GetLineLengthsArray().data(), LineBreakCount);

		Shader.SetFloat("u_LineMaximumLength", OptionsObject.m_LineLength);
		Shader.SetFloat("u_CharacterHeight", OptionsObject.m_CharacterHeight);
		Shader.SetFloat("u_SpacingBetweenLines", OptionsObject.m_SpacingBetweenLines);


		Shader.ApplyUniforms(DrawCall.GetAppliedUniforms());

		CheckGLErrors();
		glDrawElementsInstanced(GL_TRIANGLES, 6Ui64, GL_UNSIGNED_SHORT, nullptr, static_cast<int>(CharInstances));
		CheckGLErrors();
	}

	m_TextArray.clear();
	Text::UnbindCommonVAO();
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Renderer2D::RenderFreeBatches() {
	std::vector<BatchDrawCall>& Arr = m_FreeBatchArray;

	if (Arr.size() == 0) return;

	FreeBatch::BindCommonVAO();

	for (size_t i = 0; i < Arr.size(); i++) {
		const BatchDrawCall& DrawCall = Arr[i];

		const FreeBatch* Free = dynamic_cast<const FreeBatch*>(DrawCall.GetBaseBatchPointer());
		const SpriteSheet* Sheet = Free->GetSheet();
		const Shader* Shader = Sheet->GetShader();

		int ElementsToDraw = Free->GetElementCount();


		Free->BindUniqueBuffers();
		glBindTexture(GL_TEXTURE_2D, Sheet->GetTextureBufferID());

		Shader->UseShader();
		Shader->SetStandardProjection(GetCamera().GetProjectionMatrix());
		Shader->SetStandardView(GetCamera().GetViewMatrix());
		Shader->SetStandardModel(glm::translate(glm::mat4(1.f), DrawCall.GetPositionVector()));


		CheckGLErrors();
		glDrawElements(GL_TRIANGLES, ElementsToDraw, GL_UNSIGNED_SHORT, 0);
		CheckGLErrors();
	}

	FreeBatch::UnbindCommonVAO();
	Arr.clear();

}


void Renderer2D::RenderSoftBatches() {
	std::vector<BatchDrawCall>& Arr = m_SoftBatchArray;
	
	SoftBatch::BindCommonVAO();


	for (size_t i = 0; i < Arr.size(); i++) {
		const BatchDrawCall& DrawCall = Arr[i];

		const SoftBatch* Soft = dynamic_cast<const SoftBatch*>(DrawCall.GetBaseBatchPointer());
		const SpriteSheet* Sheet = Soft->GetSheet();
		const Shader* Shader = Sheet->GetShader();

		int InstanceCount = Soft->GetInstanceCount();

		Soft->BindUniqueBuffers();

		glBindTexture(GL_TEXTURE_2D, Sheet->GetTextureBufferID());
		GetQuad().BufferTexCoords(Sheet);

		Shader->UseShader();
		Shader->SetStandardModel(glm::translate(glm::mat4(1.f), DrawCall.GetPositionVector()));
		Shader->SetStandardView(GetCamera().GetViewMatrix());
		Shader->SetStandardProjection(GetCamera().GetProjectionMatrix());

		if (Soft->GetType() == SoftBatchType::FloatingQuad) {
			Shader->SetFloat("u_TexWidth", static_cast<const int>(Sheet->GetSpriteSheetWidth()));
			Shader->SetFloat("u_TexHeight", static_cast<const int>(Sheet->GetSpriteSheetHeight()));
		}

		Shader->ApplyUniforms(DrawCall.GetAppliedUniforms());

#ifdef DEBUG__CODE
		CheckGLErrors();
#endif

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, InstanceCount);

#ifdef DEBUG__CODE
		CheckGLErrors();
#endif

	}
	SoftBatch::UnbindCommonVAO();
	Arr.clear();
}


void Renderer2D::RenderStrictBatches() {

	//	Since these batches are guaranteed to use
	//	unique sprite sheets paired with unique shaders
	//	there's not much point in trying to optimise this part

	std::vector<BatchDrawCall>& Arr = m_StrictBatchArray;

	StrictBatch::BindCommonVAO();

	for (size_t i = 0; i < Arr.size(); i++) {
		const BatchDrawCall& DrawCallCurrent = Arr[i];
		
		const StrictBatch* Strict = dynamic_cast<const StrictBatch*>(DrawCallCurrent.GetBaseBatchPointer());
		const SpriteSheet* SheetCurrent = Strict->GetSheet();
		const Shader* ShaderCurrent = SheetCurrent->GetShader();

		int InstanceCount = Strict->GetInstanceCount();

		// SHEET SET UP
		glBindTexture(GL_TEXTURE_2D, SheetCurrent->GetTextureBufferID());
		
		Strict->BindUniqueBuffers();


		// SHADER SET UP
		ShaderCurrent->UseShader();
		ShaderCurrent->SetStandardModel(glm::translate(glm::mat4(1.f), DrawCallCurrent.GetPositionVector()));
		ShaderCurrent->SetStandardView(m_Camera.GetViewMatrix());
		ShaderCurrent->SetStandardProjection(m_Camera.GetProjectionMatrix());

		ShaderCurrent->SetInt("u_RowSpriteCount", Strict->GetSpriteCountPerRow());
		ShaderCurrent->SetInt("u_SpriteSideLengthPx", GetQuad().m_StandardSpritePixelLength);

		ShaderCurrent->ApplyUniforms(DrawCallCurrent.GetAppliedUniforms());

#ifdef DEBUG__CODE
		CheckGLErrors();
#endif

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, InstanceCount);

#ifdef DEBUG__CODE
		CheckGLErrors();
#endif
	}

	StrictBatch::UnbindCommonVAO();
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
			ShaderCurrent->SetStandardView(m_Camera.GetViewMatrix());
			ShaderCurrent->SetStandardProjection(m_Camera.GetProjectionMatrix());

#ifdef DEBUG__CODE
			d_ShaderChanges++;
#endif
		}

		//	per-instance data, unskippable
		ShaderCurrent->SetStandardModel(glm::translate(glm::mat4(1.f), DrawcallCurrent.GetPositionVector()));
		ShaderCurrent->SetVec2("u_SpriteOffsets",
			SheetCurrent->GetCalculatedSpriteOffsets(DrawcallCurrent.GetDrawablePointer()->GetSpriteIndex()));

		ShaderCurrent->ApplyUniforms(DrawcallCurrent.GetAppliedUniforms());

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr));
	}

#ifdef DEBUG__CODE
	if(d_Print)
	fprintf(stdout, "RenderDrawables() stats for [%lld] DrawCall objects:\n\tSheet changes: %d\n\tShader changes: %d\n",
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

	glEnable(GL_DEPTH_TEST);

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
		InputController::c_LetterTrackBit |
		InputController::c_SpecialTrackBit
	);
	
	//	Globals initialisation

	g_StandardQuad.Init();

	PerClassVAOinitialisationFunction();

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
	//	Special common shader used only by Text objects
	if (!_shaderName.compare(c_SpecialTextShaderName)) {
		m_TextRenderingShader = Shader(
			_locationShaderFile,
			_shaderName
		);
		return;
	}

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

const SpriteSheet* Renderer2D::GetSpriteSheetByName(
	const char* _spriteSheetName
) {
	if (!_spriteSheetName) return nullptr;
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

const Shader* Renderer2D::GetShaderByName(
	const char* _shaderName
) {
	if (!_shaderName) return nullptr;
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

		if (!params.m_PreferredShaderName.compare(c_SpecialTextShaderName)) {
			
			LoadSpriteSheet(
				std::string(params.m_LocationOfImage),
				std::string(params.m_SheetName),
				&GetTextShader(),
				params.m_SpritesPerRow,
				params.m_SpritesPerCol
			);
		}

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


void Renderer2D::Draw(
	const Text* _text,
	float _xPosition,
	float _yPosition,
	float _zLayer,
	UniformDataVector* _uniformArray
) {
	m_TextArray.emplace_back(
		_text,
		_xPosition,
		_yPosition,
		_zLayer,
		_uniformArray
	);
}

//	TODO: Add some configuration file/structure to take data from
//	so we avoid hardcoding shit like the font size.
void Renderer2D::PerClassVAOinitialisationFunction() {


	StrictBatch::InitialiseCommonVAO();
	SoftBatch::InitialiseCommonVAO();
	FreeBatch::InitialiseCommonVAO();

	Font::InitialiseCommonFontSizeVBO(20);
	Text::InitialiseCommonVAO();

}



bool Renderer2D::DrawCallComparator::operator()(const DrawableDrawCall& a, const DrawableDrawCall& b) const {

	const Shader* aS = a.GetDrawablePointer()->GetAsociatedSpriteSheet()->GetShader();
	const Shader* bS = b.GetDrawablePointer()->GetAsociatedSpriteSheet()->GetShader();


	if (aS == bS) {
		return a.GetDrawablePointer()->GetAsociatedSpriteSheet() > b.GetDrawablePointer()->GetAsociatedSpriteSheet();
	}

	return aS > bS;
}



