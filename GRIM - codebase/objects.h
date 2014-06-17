Framework::Mesh *g_pConeMesh = NULL;
Framework::Mesh *g_pCylinderMesh = NULL;
Framework::Mesh *g_pCubeTintMesh = NULL;
Framework::Mesh *g_pCubeColorMesh = NULL;
Framework::Mesh *g_pPlaneMesh = NULL;

//Trees are 3x3 in X/Z, and fTrunkHeight+fConeHeight in the Y.

const float g_fColumnBaseHeight = 0.25f;

static bool g_bDrawLookatPoint = false;



const float g_fParthenonWidth = 14.0f;
const float g_fParthenonLength = 20.0f;
const float g_fParthenonColumnHeight = 5.0f;
const float g_fParthenonBaseHeight = 1.0f;
const float g_fParthenonTopHeight = 2.0f;

struct TreeData
{
	float fXPos;
	float fZPos;
	float fTrunkHeight;
	float fConeHeight;
};

static const TreeData g_forest[] =
{
	{-45.0f, -40.0f, 2.0f, 3.0f},
	{-42.0f, -35.0f, 2.0f, 3.0f},
	{-39.0f, -29.0f, 2.0f, 4.0f},
	{-44.0f, -26.0f, 3.0f, 3.0f},
	{-40.0f, -22.0f, 2.0f, 4.0f},
	{-36.0f, -15.0f, 3.0f, 3.0f},
	{-41.0f, -11.0f, 2.0f, 3.0f},
	{-37.0f, -6.0f, 3.0f, 3.0f},
	{-45.0f, 0.0f, 2.0f, 3.0f},
	{-39.0f, 4.0f, 3.0f, 4.0f},
	{-36.0f, 8.0f, 2.0f, 3.0f},
	{-44.0f, 13.0f, 3.0f, 3.0f},
	{-42.0f, 17.0f, 2.0f, 3.0f},
	{-38.0f, 23.0f, 3.0f, 4.0f},
	{-41.0f, 27.0f, 2.0f, 3.0f},
	{-39.0f, 32.0f, 3.0f, 3.0f},
	{-44.0f, 37.0f, 3.0f, 4.0f},
	{-36.0f, 42.0f, 2.0f, 3.0f},

	{-32.0f, -45.0f, 2.0f, 3.0f},
	{-30.0f, -42.0f, 2.0f, 4.0f},
	{-34.0f, -38.0f, 3.0f, 5.0f},
	{-33.0f, -35.0f, 3.0f, 4.0f},
	{-29.0f, -28.0f, 2.0f, 3.0f},
	{-26.0f, -25.0f, 3.0f, 5.0f},
	{-35.0f, -21.0f, 3.0f, 4.0f},
	{-31.0f, -17.0f, 3.0f, 3.0f},
	{-28.0f, -12.0f, 2.0f, 4.0f},
	{-29.0f, -7.0f, 3.0f, 3.0f},
	{-26.0f, -1.0f, 2.0f, 4.0f},
	{-32.0f, 6.0f, 2.0f, 3.0f},
	{-30.0f, 10.0f, 3.0f, 5.0f},
	{-33.0f, 14.0f, 2.0f, 4.0f},
	{-35.0f, 19.0f, 3.0f, 4.0f},
	{-28.0f, 22.0f, 2.0f, 3.0f},
	{-33.0f, 26.0f, 3.0f, 3.0f},
	{-29.0f, 31.0f, 3.0f, 4.0f},
	{-32.0f, 38.0f, 2.0f, 3.0f},
	{-27.0f, 41.0f, 3.0f, 4.0f},
	{-31.0f, 45.0f, 2.0f, 4.0f},
	{-28.0f, 48.0f, 3.0f, 5.0f},

	{-25.0f, -48.0f, 2.0f, 3.0f},
	{-20.0f, -42.0f, 3.0f, 4.0f},
	{-22.0f, -39.0f, 2.0f, 3.0f},
	{-19.0f, -34.0f, 2.0f, 3.0f},
	{-23.0f, -30.0f, 3.0f, 4.0f},
	{-24.0f, -24.0f, 2.0f, 3.0f},
	{-16.0f, -21.0f, 2.0f, 3.0f},
	{-17.0f, -17.0f, 3.0f, 3.0f},
	{-25.0f, -13.0f, 2.0f, 4.0f},
	{-23.0f, -8.0f, 2.0f, 3.0f},
	{-17.0f, -2.0f, 3.0f, 3.0f},
	{-16.0f, 1.0f, 2.0f, 3.0f},
	{-19.0f, 4.0f, 3.0f, 3.0f},
	{-22.0f, 8.0f, 2.0f, 4.0f},
	{-21.0f, 14.0f, 2.0f, 3.0f},
	{-16.0f, 19.0f, 2.0f, 3.0f},
	{-23.0f, 24.0f, 3.0f, 3.0f},
	{-18.0f, 28.0f, 2.0f, 4.0f},
	{-24.0f, 31.0f, 2.0f, 3.0f},
	{-20.0f, 36.0f, 2.0f, 3.0f},
	{-22.0f, 41.0f, 3.0f, 3.0f},
	{-21.0f, 45.0f, 2.0f, 3.0f},

	{-12.0f, -40.0f, 2.0f, 4.0f},
	{-11.0f, -35.0f, 3.0f, 3.0f},
	{-10.0f, -29.0f, 1.0f, 3.0f},
	{-9.0f, -26.0f, 2.0f, 2.0f},
	{-6.0f, -22.0f, 2.0f, 3.0f},
	{-15.0f, -15.0f, 1.0f, 3.0f},
	{-8.0f, -11.0f, 2.0f, 3.0f},
	{-14.0f, -6.0f, 2.0f, 4.0f},
	{-12.0f, 0.0f, 2.0f, 3.0f},
	{-7.0f, 4.0f, 2.0f, 2.0f},
	{-13.0f, 8.0f, 2.0f, 2.0f},
	{-9.0f, 13.0f, 1.0f, 3.0f},
	{-13.0f, 17.0f, 3.0f, 4.0f},
	{-6.0f, 23.0f, 2.0f, 3.0f},
	{-12.0f, 27.0f, 1.0f, 2.0f},
	{-8.0f, 32.0f, 2.0f, 3.0f},
	{-10.0f, 37.0f, 3.0f, 3.0f},
	{-11.0f, 42.0f, 2.0f, 2.0f},


	{15.0f, 5.0f, 2.0f, 3.0f},
	{15.0f, 10.0f, 2.0f, 3.0f},
	{15.0f, 15.0f, 2.0f, 3.0f},
	{15.0f, 20.0f, 2.0f, 3.0f},
	{15.0f, 25.0f, 2.0f, 3.0f},
	{15.0f, 30.0f, 2.0f, 3.0f},
	{15.0f, 35.0f, 2.0f, 3.0f},
	{15.0f, 40.0f, 2.0f, 3.0f},
	{15.0f, 45.0f, 2.0f, 3.0f},

	{25.0f, 5.0f, 2.0f, 3.0f},
	{25.0f, 10.0f, 2.0f, 3.0f},
	{25.0f, 15.0f, 2.0f, 3.0f},
	{25.0f, 20.0f, 2.0f, 3.0f},
	{25.0f, 25.0f, 2.0f, 3.0f},
	{25.0f, 30.0f, 2.0f, 3.0f},
	{25.0f, 35.0f, 2.0f, 3.0f},
	{25.0f, 40.0f, 2.0f, 3.0f},
	{25.0f, 45.0f, 2.0f, 3.0f},
};

void DrawTree(glutil::MatrixStack &modelMatrix, float fTrunkHeight = 2.0f, float fConeHeight = 3.0f)
{
	//Draw trunk.
	{
		glutil::PushStack push(modelMatrix);

		modelMatrix.Scale(glm::vec3(1.0f, fTrunkHeight, 1.0f));
		modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

		glUseProgram(UniformColorTint.theProgram);
		glUniformMatrix4fv(UniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		glUniform4f(UniformColorTint.baseColorUnif, 0.694f, 0.4f, 0.106f, 1.0f);
		g_pCylinderMesh->Render();
		glUseProgram(0);
	}

	//Draw the treetop
	{
		glutil::PushStack push(modelMatrix);

		modelMatrix.Translate(glm::vec3(0.0f, fTrunkHeight, 0.0f));
		modelMatrix.Scale(glm::vec3(3.0f, fConeHeight, 3.0f));

		glUseProgram(UniformColorTint.theProgram);
		glUniformMatrix4fv(UniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		glUniform4f(UniformColorTint.baseColorUnif, 0.0f, 1.0f, 0.0f, 1.0f);
		g_pConeMesh->Render();
		glUseProgram(0);
	}
}

void DrawColumn(glutil::MatrixStack &modelMatrix, float fHeight = 5.0f)
{
	//Draw the bottom of the column.
	{
		glutil::PushStack push(modelMatrix);

		modelMatrix.Scale(glm::vec3(1.0f, g_fColumnBaseHeight, 1.0f));
		modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

		glUseProgram(UniformColorTint.theProgram);
		glUniformMatrix4fv(UniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		glUniform4f(UniformColorTint.baseColorUnif, 1.0f, 1.0f, 1.0f, 1.0f);
		g_pCubeTintMesh->Render();
		glUseProgram(0);
	}

	//Draw the top of the column.
	{
		glutil::PushStack push(modelMatrix);

		modelMatrix.Translate(glm::vec3(0.0f, fHeight - g_fColumnBaseHeight, 0.0f));
		modelMatrix.Scale(glm::vec3(1.0f, g_fColumnBaseHeight, 1.0f));
		modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

		glUseProgram(UniformColorTint.theProgram);
		glUniformMatrix4fv(UniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		glUniform4f(UniformColorTint.baseColorUnif, 0.9f, 0.9f, 0.9f, 0.9f);
		g_pCubeTintMesh->Render();
		glUseProgram(0);
	}

	//Draw the main column.
	{
		glutil::PushStack push(modelMatrix);

		modelMatrix.Translate(glm::vec3(0.0f, g_fColumnBaseHeight, 0.0f));
		modelMatrix.Scale(glm::vec3(0.8f, fHeight - (g_fColumnBaseHeight * 2.0f), 0.8f));
		modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

		glUseProgram(UniformColorTint.theProgram);
		glUniformMatrix4fv(UniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		glUniform4f(UniformColorTint.baseColorUnif, 0.9f, 0.9f, 0.9f, 0.9f);
		g_pCylinderMesh->Render();
		glUseProgram(0);
	}
}

void DrawParthenon(glutil::MatrixStack &modelMatrix)
{
	//Draw base.
	{
		glutil::PushStack push(modelMatrix);

		modelMatrix.Scale(glm::vec3(g_fParthenonWidth, g_fParthenonBaseHeight, g_fParthenonLength));
		modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

		glUseProgram(UniformColorTint.theProgram);
		glUniformMatrix4fv(UniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		glUniform4f(UniformColorTint.baseColorUnif, 0.9f, 0.9f, 0.9f, 0.9f);
		g_pCubeTintMesh->Render();
		glUseProgram(0);
	}

	//Draw top.
	{
		glutil::PushStack push(modelMatrix);

		modelMatrix.Translate(glm::vec3(0.0f, g_fParthenonColumnHeight + g_fParthenonBaseHeight, 0.0f));
		modelMatrix.Scale(glm::vec3(g_fParthenonWidth, g_fParthenonTopHeight, g_fParthenonLength));
		modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

		glUseProgram(UniformColorTint.theProgram);
		glUniformMatrix4fv(UniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		glUniform4f(UniformColorTint.baseColorUnif, 0.9f, 0.9f, 0.9f, 0.9f);
		g_pCubeTintMesh->Render();
		glUseProgram(0);
	}

	//Draw columns.
	const float fFrontZVal = (g_fParthenonLength / 2.0f) - 1.0f;
	const float fRightXVal = (g_fParthenonWidth / 2.0f) - 1.0f;

	for(int iColumnNum = 0; iColumnNum < int(g_fParthenonWidth / 2.0f); iColumnNum++)
	{
		{
			glutil::PushStack push(modelMatrix);
			modelMatrix.Translate(glm::vec3((2.0f * iColumnNum) - (g_fParthenonWidth / 2.0f) + 1.0f,
				g_fParthenonBaseHeight, fFrontZVal));

			DrawColumn(modelMatrix, g_fParthenonColumnHeight);
		}
		{
			glutil::PushStack push(modelMatrix);
			modelMatrix.Translate(glm::vec3((2.0f * iColumnNum) - (g_fParthenonWidth / 2.0f) + 1.0f,
				g_fParthenonBaseHeight, -fFrontZVal));

			DrawColumn(modelMatrix, g_fParthenonColumnHeight);
		}
	}

	//Don't draw the first or last columns, since they've been drawn already.
	for(int iColumnNum = 1; iColumnNum < int((g_fParthenonLength - 2.0f) / 2.0f); iColumnNum++)
	{
		{
			glutil::PushStack push(modelMatrix);
			modelMatrix.Translate(glm::vec3(fRightXVal,
				g_fParthenonBaseHeight, (2.0f * iColumnNum) - (g_fParthenonLength / 2.0f) + 1.0f));

			DrawColumn(modelMatrix, g_fParthenonColumnHeight);
		}
		{
			glutil::PushStack push(modelMatrix);
			modelMatrix.Translate(glm::vec3(-fRightXVal,
				g_fParthenonBaseHeight, (2.0f * iColumnNum) - (g_fParthenonLength / 2.0f) + 1.0f));

			DrawColumn(modelMatrix, g_fParthenonColumnHeight);
		}
	}

	//Draw interior.
	{
		glutil::PushStack push(modelMatrix);

		modelMatrix.Translate(glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix.Scale(glm::vec3(g_fParthenonWidth - 6.0f, g_fParthenonColumnHeight,
			g_fParthenonLength - 6.0f));
		modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

		glUseProgram(ObjectColor.theProgram);
		glUniformMatrix4fv(ObjectColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		g_pCubeColorMesh->Render();
		glUseProgram(0);
	}

	//Draw asspiecie.
	{
		glutil::PushStack push(modelMatrix);

		modelMatrix.Translate(glm::vec3(
			0.0f,
			g_fParthenonColumnHeight + g_fParthenonBaseHeight + (g_fParthenonTopHeight / 2.0f),
			g_fParthenonLength / 2.0f));
		modelMatrix.RotateX(-135.0f);
		modelMatrix.RotateY(45.0f);

		glUseProgram(ObjectColor.theProgram);
		glUniformMatrix4fv(ObjectColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
		g_pCubeColorMesh->Render();
		glUseProgram(0);
	}
}

//Columns are 1x1 in the X/Z, and fHieght units in the Y.






bool objects_are_loaded(){
	return g_pConeMesh && g_pCylinderMesh && g_pCubeTintMesh && g_pCubeColorMesh && g_pPlaneMesh;
}

void DrawForest(glutil::MatrixStack &modelMatrix)
{
	for(int iTree = 0; iTree < ARRAY_COUNT(g_forest); iTree++)
	{
		const TreeData &currTree = g_forest[iTree];

		glutil::PushStack push(modelMatrix);
		modelMatrix.Translate(glm::vec3(currTree.fXPos, 0.0f, currTree.fZPos));
		DrawTree(modelMatrix, currTree.fTrunkHeight, currTree.fConeHeight);
	}
}

void objects_draw(){
	const glm::vec3 &camPos = ResolveCamPosition();

	glutil::MatrixStack camMatrix;
	camMatrix.SetMatrix(CalcLookAtMatrix(camPos, g_camTarget, glm::vec3(0.0f, 1.0f, 0.0f)));

	glUseProgram(UniformColor.theProgram);
	glUniformMatrix4fv(UniformColor.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
	glUseProgram(ObjectColor.theProgram);
	glUniformMatrix4fv(ObjectColor.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
	glUseProgram(UniformColorTint.theProgram);
	glUniformMatrix4fv(UniformColorTint.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
	glUseProgram(0);

	glutil::MatrixStack modelMatrix;


		//Render the ground plane.
		{
			glutil::PushStack push(modelMatrix);

			modelMatrix.Scale(glm::vec3(100.0f, 1.0f, 100.0f));

			glUseProgram(UniformColor.theProgram);
			glUniformMatrix4fv(UniformColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
			glUniform4f(UniformColor.baseColorUnif, 1.0f, 0.416f, 0.0589f, 1.0f);
			g_pPlaneMesh->Render();
			glUseProgram(0);
		}

		//Draw the trees
		DrawForest(modelMatrix);

		//Draw the building.
		{
			glutil::PushStack push(modelMatrix);
			modelMatrix.Translate(glm::vec3(20.0f, 0.0f, -10.0f));

			DrawParthenon(modelMatrix);
		}
		if(g_bDrawLookatPoint)
		{
			glDisable(GL_DEPTH_TEST);
			glm::mat4 idenity(1.0f);

			glutil::PushStack push(modelMatrix);

			glm::vec3 cameraAimVec = g_camTarget - camPos;
			modelMatrix.Translate(0.0f, 0.0, -glm::length(cameraAimVec));
			modelMatrix.Scale(1.0f, 1.0f, 1.0f);
		
			glUseProgram(ObjectColor.theProgram);
			glUniformMatrix4fv(ObjectColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
			glUniformMatrix4fv(ObjectColor.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(idenity));
			g_pCubeColorMesh->Render();
			glUseProgram(0);
			glEnable(GL_DEPTH_TEST);
		}
}


void loadObjects(){
	g_pConeMesh = new Framework::Mesh("UnitConeTint.xml");
	g_pCylinderMesh = new Framework::Mesh("UnitCylinderTint.xml");
	g_pCubeTintMesh = new Framework::Mesh("UnitCubeTint.xml");
	g_pCubeColorMesh = new Framework::Mesh("UnitCubeColor.xml");
	g_pPlaneMesh = new Framework::Mesh("UnitPlane.xml");
}



void objects_delete(){
	delete g_pConeMesh;
	g_pConeMesh = NULL;
	delete g_pCylinderMesh;
	g_pCylinderMesh = NULL;
	delete g_pCubeTintMesh;
	g_pCubeTintMesh = NULL;
	delete g_pCubeColorMesh;
	g_pCubeColorMesh = NULL;
	delete g_pPlaneMesh;
	g_pPlaneMesh = NULL;
}