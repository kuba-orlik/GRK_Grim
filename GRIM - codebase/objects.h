Framework::Mesh *sky_mesh = NULL;
Framework::Mesh *hill_mesh = NULL;
Framework::Mesh *leaves_mesh = NULL;
Framework::Mesh *tree_crown_mesh = NULL;
Framework::Mesh *tree_trunk_mesh = NULL;
Framework::Mesh *falling_leaf_mesh = NULL;


//Trees are 3x3 in X/Z, and fTrunkHeight+fConeHeight in the Y.

const float g_fColumnBaseHeight = 0.25f;

static bool g_bDrawLookatPoint = false;



const float g_fParthenonWidth = 14.0f;
const float g_fParthenonLength = 20.0f;
const float g_fParthenonColumnHeight = 5.0f;
const float g_fParthenonBaseHeight = 1.0f;
const float g_fParthenonTopHeight = 2.0f;

struct falling_leaf{
	float gravity;
	float ver_coord;
	float hor_coord;
	glm::vec3 position;
	glm::vec3 rotation;
};

const float falling_leaf_max_ver_coor = 2;
const float falling_leaf_min_ver_coor = -0.5;

const float falling_leaf_max_hor_coor = 1;
const float falling_leaf_min_hor_coor = -1;

const float falling_leaf_max_x = -1;
const float falling_leaf_min_x = -50;

const float falling_leaf_max_y = 10;
const float falling_leaf_min_y = 5;

const float falling_leaf_max_z = 1;
const float falling_leaf_min_z = -1;

const float falling_leaf_max_start_x = 1;
const float falling_leaf_min_start_x = -1;

const float falling_leaf_max_start_y = 8;
const float falling_leaf_min_start_y = 5;

const float falling_leaf_max_start_z = 1;
const float falling_leaf_min_start_z = -1;

const int falling_leafs_amount = 25;

const glm::vec3 wind_speed = glm::vec3(-1.5, 0, 1);
const float gravity = -0.3f;

falling_leaf falling_leafs[falling_leafs_amount];


float randomFloat(float min, float max){
	float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return min+abs(max-min)*random;
}

falling_leaf generateLeaf(bool at_crown){
	falling_leaf ret;
	ret.gravity = 0.1;
	ret.hor_coord = randomFloat(falling_leaf_min_hor_coor, falling_leaf_max_hor_coor);
	ret.ver_coord = randomFloat(falling_leaf_min_ver_coor, falling_leaf_max_ver_coor);
	ret.rotation.x = randomFloat(45, 135);
	ret.rotation.y = randomFloat(-30, 30);
	ret.rotation.z = randomFloat(-30, 30);
	float x,y,z;
	if(!at_crown){	
		x = randomFloat(falling_leaf_min_x, falling_leaf_max_x);
		y = randomFloat(falling_leaf_min_y, falling_leaf_max_y);
		z = randomFloat(falling_leaf_min_z, falling_leaf_max_z);
	}else{
		x = randomFloat(falling_leaf_min_start_x, falling_leaf_max_start_x);
		y = randomFloat(falling_leaf_min_start_y, falling_leaf_max_start_y);
		z = randomFloat(falling_leaf_min_start_z, falling_leaf_max_start_z);
	}
	glm::vec3 position = glm::vec3(x, y, z);
	ret.position = position;
	return ret;
}

void generateFallingLeaves(){
	printf("Generating falling leaves...");
	int i;
	for(i=0; i<falling_leafs_amount; i++){
		falling_leafs[i] = generateLeaf(false);
	}
	printf("OK!\n");
}

void DrawSky(){

	glutil::MatrixStack modelMatrix;
	glutil::PushStack push(modelMatrix);
	int sky_width = 2560;
	int sky_height = 1600;
	double scale_x=1;
	double scale_y=1;
	double unit;
	/*if(window_width<window_height){
		scale_x = (double)window_height/(double)window_width;
	}else{
		scale_y = (double)window_width/(double)window_height;
	}*/
	double window_proportion = (double)window_height/(double)window_width;
	double sky_proportion = (double)sky_height/(double)sky_width;
	printf("sky_proportion: %f, window_proportion: %f", sky_proportion, window_proportion);
	if(window_proportion<sky_proportion){
		scale_y=sky_proportion/window_proportion;
	}else{
		scale_x=window_proportion/sky_proportion;
	}
	printf("scale_x: %f\n", scale_x);
	modelMatrix.Scale(glm::vec3(scale_x, scale_y, 1.0f));
	float scale_margin = 1.05f;
	modelMatrix.Scale(glm::vec3(scale_margin, scale_margin, 1.0));
	modelMatrix.Translate(glm::vec3(sky_offset, 0, 50));//aby nie zasłaniało wzgórza
	//modelMatrix.RotateZ(3.1415);

	glUseProgram(SkyProgram.theProgram);
	glUniformMatrix4fv(sky_matrix_uniform, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	use_texture(sky_texture);
	sky_mesh->Render();
	glUseProgram(0);	
}







bool objects_are_loaded(){
	return sky_mesh && hill_mesh;
}

/*void DrawForest(glutil::MatrixStack &modelMatrix)
{
	for(int iTree = 0; iTree < ARRAY_COUNT(g_forest); iTree++)
	{
		const TreeData &currTree = g_forest[iTree];

		glutil::PushStack push(modelMatrix);
		modelMatrix.Translate(glm::vec3(currTree.fXPos, 0.0f, currTree.fZPos));
		DrawTree(modelMatrix, currTree.fTrunkHeight, currTree.fConeHeight);
	}
}*/

void setLight(){
	glUniform1f(moon_glow_ratio_uniform, moon_glow_ratio_value);	
	glUniform1f(moon_light_intensity_uniform, moon_light_intensity_value);	
	glUniform1f(frontlight_ratio_uniform, frontlight_ratio_value);	
}


void DrawHill(){
	glutil::MatrixStack modelMatrix;
	glutil::PushStack push(modelMatrix);

	modelMatrix.Scale(glm::vec3(100.0f, 100.0f, 100.0f));

	glUseProgram(UniformColorTint.theProgram);
	setLight();
	glUniformMatrix4fv(UniformColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(UniformColor.baseColorUnif, 0.302f, 0.416f, 0.0589f, 1.0f);
	use_texture(noise_texture);
	hill_mesh->Render();
	glUseProgram(0);
}


void DrawLeaves(glutil::MatrixStack &modelMatrix){
//	glutil::MatrixStack modelMatrix;
	glutil::PushStack push(modelMatrix);

	setLight();
	glUseProgram(UniformColorTint.theProgram);
	glUniform1f(backlight_ratio_uniform, backlight_ratio_value);	
	//glUniform1f(moon_light_intensity_uniform, 0);	
	glUniform1f(frontlight_ratio_uniform, 0.01f);	
	glUniform1f(moon_light_intensity_uniform, 1);
	glUniformMatrix4fv(UniformColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(UniformColor.baseColorUnif, 0.302f, 0.416f, 0.0589f, 1.0f);
	use_texture(leaves_texture);
	leaves_mesh->Render();
	glUniform1f(backlight_ratio_uniform, 0.0f);
	glUniform1f(moon_light_intensity_uniform, moon_light_intensity_value);
	glUniform1f(frontlight_ratio_uniform, 1.0f);	
	glUseProgram(0);
}

void DrawCrown(glutil::MatrixStack &modelMatrix){
//	glutil::MatrixStack modelMatrix;
	glutil::PushStack push(modelMatrix);

	setLight();
	glUseProgram(UniformColorTint.theProgram);	
	glUniformMatrix4fv(UniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform1f(moon_light_intensity_uniform, 1.5);
	use_texture(noise_texture);
	tree_crown_mesh->Render();
	glUseProgram(0);
}

void DrawTrunk(glutil::MatrixStack &modelMatrix){
//	glutil::MatrixStack modelMatrix;
	glutil::PushStack push(modelMatrix);

	setLight();
	glUseProgram(UniformColorTint.theProgram);	
	modelMatrix.Translate(glm::vec3(0.0, -0.45, 0.0));
	//glUniform1f(moon_light_intensity_uniform, 2);
	glUniform1f(moon_glow_ratio_uniform, 0.5);
	glUniform1f(moon_light_intensity_uniform, 5);
	glUniformMatrix4fv(UniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));	
	use_texture(noise_texture);
	tree_trunk_mesh->Render();
	//modelMatrix.Pop();
	glUniform1f(moon_glow_ratio_uniform, moon_glow_ratio_value);
	glUseProgram(0);
}


float getPhase(float loop_duration){
	float animation_duration = loop_duration*1000;
	float fElapsedTime = glutGet(GLUT_ELAPSED_TIME);
	float time_in_loop = fElapsedTime-floor(fElapsedTime/animation_duration)*animation_duration;
	float phase = time_in_loop/animation_duration;
	return phase;
}

float phaseFloat(float min, float max, float loop_duration){
	return min + abs(max-min)*glm::sin(2*3.141592654*getPhase(loop_duration));
}

void DrawTree(){
	glutil::MatrixStack modelMatrix;
	glutil::PushStack push(modelMatrix);
	//modelMatrix.Scale(glm::vec3(scale_x, scale_y, 1.0f));
	glm::mat4 skew;
	int min_skew = 1;
	int max_skew = 3;
	float loop_duration = 10.0;
	skew[1] = glm::vec4(glm::tan(2*3.14*0.01*phaseFloat(min_skew, max_skew, loop_duration)), 1, 0, 0); 

	modelMatrix.ApplyMatrix(skew);
	float scale = 20.0;
	modelMatrix.Scale(glm::vec3(scale, scale, scale));
	modelMatrix.RotateY(2*3.1415/36*160);
	//modelMatrix.RotateX(2*3.1415/36*parameter_a*10);
	
	
	//modelMatrix.RotateY(2*3.1415/36*parameter_a);
	//modelMatrix.Translate(glm::vec3(parameter_a, parameter_s, parameter_d));
	modelMatrix.Translate(glm::vec3(0.2f, 0.6f, 0.1f));
	
	//modelMatrix.RotateZ(3.1415);
	DrawTrunk(modelMatrix);
	modelMatrix.Translate(glm::vec3(-0.05, -0.04, 0.0));
	DrawLeaves(modelMatrix);
	modelMatrix.Scale(1.05);
	DrawCrown(modelMatrix);
}

glm::vec3 speedFraction(glm::vec3 speed){
	return (speed*(float)((float)1/(float)fps));
}

void reset_leaf(falling_leaf &leaf){
	leaf.gravity = 0.1;
	leaf.hor_coord = randomFloat(falling_leaf_min_hor_coor, falling_leaf_max_hor_coor);
	leaf.ver_coord = randomFloat(falling_leaf_min_ver_coor, falling_leaf_max_ver_coor);
	float x,y,z;
	x = randomFloat(falling_leaf_min_start_x, falling_leaf_max_start_x);
	y = randomFloat(falling_leaf_min_start_y, falling_leaf_max_start_y);
	z = randomFloat(falling_leaf_min_start_z, falling_leaf_max_start_z);
	leaf.rotation.x = randomFloat(45, 135);
	leaf.rotation.y = randomFloat(-30, 30);
	leaf.rotation.z = randomFloat(-30, 30);
	glm::vec3 position = glm::vec3(x, y, z);
	leaf.position = position;
}

void DrawFallingLeaves(){
	glutil::MatrixStack modelMatrix;
	glutil::PushStack push(modelMatrix);

	glUseProgram(UniformColorTint.theProgram);
	setLight();
	int i;
	float gravity_fraction = (float)gravity/(float)fps;
	for(i=0; i<falling_leafs_amount; i++){
		falling_leaf *current_leaf = &falling_leafs[i];
		current_leaf->ver_coord+=gravity_fraction;
		if(current_leaf->position.x<falling_leaf_min_x){
			reset_leaf(*current_leaf);
		}
		if(current_leaf->ver_coord<falling_leaf_min_ver_coor){
			current_leaf->ver_coord=falling_leaf_min_ver_coor;
		}
		current_leaf->position = current_leaf->position + speedFraction(wind_speed) + speedFraction(glm::vec3(0, current_leaf->ver_coord, current_leaf->hor_coord));
		//current_leaf->position = current_leaf->position + glm::vec3(0, -0.33, 0);
		glm::vec3 offset = current_leaf->position;
		modelMatrix.SetMatrix(glm::mat4(1.0f));
		modelMatrix.Translate(offset);
		modelMatrix.RotateX(current_leaf->rotation.x);
		modelMatrix.RotateY(current_leaf->rotation.y);
		modelMatrix.RotateZ(current_leaf->rotation.z);		
		glUniformMatrix4fv(UniformColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		glUniform4f(UniformColor.baseColorUnif, 0.302f, 0.416f, 0.0589f, 1.0f);
		use_texture(falling_leaf_texture);
		falling_leaf_mesh->Render();
	}
	glUseProgram(0);	
}

struct light_dot{
	glm::vec3 position;
	glm::vec3 color;
};

struct light_dot the_dot;

void DrawDot(){
	glutil::MatrixStack modelMatrix;
	glutil::PushStack push(modelMatrix);
	glUseProgram(UniformColorTint.theProgram);
	setLight();
	glUniform1f(moon_glow_ratio_uniform, 7);	
	modelMatrix.Translate(the_dot.position);
	glUniformMatrix4fv(UniformColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	use_texture(light_dot_texture);
	falling_leaf_mesh->Render();
}

void dot_control(unsigned char key){
	float step = 0.5f;
	switch(key){
		case 'e':
			the_dot.position.x+=step;
			break;
		case 'q':
			the_dot.position.x=the_dot.position.x-step;
			break;
		case 'w':
			the_dot.position.y+=step;
			break;
		case 's':
			the_dot.position.y=the_dot.position.y-step;
			break;
		case 'd':
			the_dot.position.z+=step;
			break;
		case 'a':
			the_dot.position.z=the_dot.position.z-step;
			break;	
	}
}

void DrawWorld(const glm::vec3 &camPos){
	glutil::MatrixStack camMatrix;
	camMatrix.SetMatrix(CalcLookAtMatrix(camPos, g_camTarget, glm::vec3(0.0f, 1.0f, 0.0f)));

	glUseProgram(UniformColorTint.theProgram);
	glUniformMatrix4fv(UniformColorTint.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
	glUseProgram(0);

	DrawHill();
	DrawTree();
	DrawFallingLeaves();
	DrawDot();
}

void objects_draw(){
	const glm::vec3 &camPos = ResolveCamPosition();

	setLight();
	//Draw the trees
	//DrawForest(modelMatrix);
	DrawSky();
	DrawWorld(camPos);
}





void loadObjects(){
	printf("Loading objects...\n");
	sky_mesh = new Framework::Mesh("UnitCubeTint.xml");
	hill_mesh = new Framework::Mesh("UnitPlane.xml");
	leaves_mesh = new Framework::Mesh("leaves.xml");
	tree_crown_mesh = new Framework::Mesh("crown.xml");
	tree_trunk_mesh = new Framework::Mesh("trunk.xml");
	falling_leaf_mesh = new Framework::Mesh("falling_leaf.xml");
	the_dot.position = glm::vec3(0, -1, 0);
	loadTextures();
	generateFallingLeaves();
}



void objects_delete(){
	delete sky_mesh;
	sky_mesh = NULL;
	delete hill_mesh;
	hill_mesh = NULL;
}

