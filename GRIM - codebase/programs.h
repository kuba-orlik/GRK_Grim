struct ProgramData
{
	GLuint theProgram;
	GLuint modelToWorldMatrixUnif;
	GLuint worldToCameraMatrixUnif;
	GLuint cameraToClipMatrixUnif;
	GLuint baseColorUnif;
};

float g_fzNear = 1.0f;
float g_fzFar = 1000.0f;

ProgramData UniformColor;
ProgramData ObjectColor;
ProgramData UniformColorTint;
ProgramData SkyProgram;

ProgramData LoadProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
{
	std::vector<GLuint> shaderList;
	printf("  %s...", strVertexShader.c_str());
	shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, strVertexShader));
	printf("OK!\n");
	printf("  %s...", strFragmentShader.c_str());
	shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, strFragmentShader));
	printf("OK!\n");

	ProgramData data;
	data.theProgram = Framework::CreateProgram(shaderList);
	data.modelToWorldMatrixUnif = glGetUniformLocation(data.theProgram, "modelToWorldMatrix");
	data.worldToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "worldToCameraMatrix");
	data.cameraToClipMatrixUnif = glGetUniformLocation(data.theProgram, "cameraToClipMatrix");
	data.baseColorUnif = glGetUniformLocation(data.theProgram, "baseColor");

	return data;
}

void InitializeProgram()
{
	printf("Loading shaders...\n");
	UniformColor = LoadProgram("PosOnlyWorldTransform.vert", "ColorUniform.frag");
	ObjectColor = LoadProgram("PosColorWorldTransform.vert", "ColorPassthrough.frag");
	UniformColorTint = LoadProgram("PosColorWorldTransform.vert", "ColorMultUniform.frag");
	SkyProgram = LoadProgram("sky.vert", "sky.frag");
	printf("Setting up lightning uniforms...");
	sky_matrix_uniform = glGetUniformLocation(SkyProgram.theProgram, "sky_matrix");
	moon_glow_ratio_uniform = glGetUniformLocation(UniformColorTint.theProgram, "glow_ratio");
	moon_light_intensity_uniform = glGetUniformLocation(UniformColorTint.theProgram, "moon_light_intensity");
	printf("OK!\n");
}

void setupShaderData(int w, int h){
	glutil::MatrixStack persMatrix;
	persMatrix.Perspective(45.0f, (w / (float)h), g_fzNear, g_fzFar);

	glUseProgram(UniformColor.theProgram);
	glUniformMatrix4fv(UniformColor.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
	glUseProgram(ObjectColor.theProgram);
	glUniformMatrix4fv(ObjectColor.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
	glUseProgram(UniformColorTint.theProgram);
	glUniformMatrix4fv(UniformColorTint.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
	glUseProgram(0);
}

