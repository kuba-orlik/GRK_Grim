static glm::vec3 g_camTarget(0.0f, 0.4f, 0.0f);

glm::mat4 CalcLookAtMatrix(const glm::vec3 &cameraPt, const glm::vec3 &lookPt, const glm::vec3 &upPt)
{
	glm::vec3 lookDir = glm::normalize(lookPt - cameraPt);
	glm::vec3 upDir = glm::normalize(upPt);

	glm::vec3 rightDir = glm::normalize(glm::cross(lookDir, upDir));
	glm::vec3 perpUpDir = glm::cross(rightDir, lookDir);

	glm::mat4 rotMat(1.0f);
	rotMat[0] = glm::vec4(rightDir, 0.0f);
	rotMat[1] = glm::vec4(perpUpDir, 0.0f);
	rotMat[2] = glm::vec4(-lookDir, 0.0f);

	rotMat = glm::transpose(rotMat);

	glm::mat4 transMat(1.0f);
	transMat[3] = glm::vec4(-cameraPt, 1.0f);

	return rotMat * transMat;
}



//In spherical coordinates.
static glm::vec3 g_sphereCamRelPos(67.5f, -46.0f, 150.0f);

glm::vec3 ResolveCamPosition()
{
	glutil::MatrixStack tempMat;

	float phi = Framework::DegToRad(g_sphereCamRelPos.x);
	float theta = Framework::DegToRad(g_sphereCamRelPos.y + 90.0f);

	float fSinTheta = sinf(theta);
	float fCosTheta = cosf(theta);
	float fCosPhi = cosf(phi);
	float fSinPhi = sinf(phi);

	glm::vec3 dirToCamera(fSinTheta * fCosPhi, fCosTheta, fSinTheta * fSinPhi);
	return (dirToCamera * g_sphereCamRelPos.z) + g_camTarget;
}

void camera_control(unsigned char key){
	switch (key)
	{
		
	case 27:
		return;
	case 'w': g_camTarget.z -= 4.0f; break;
	case 's': g_camTarget.z += 4.0f; break;
	case 'd': g_camTarget.x += 4.0f; break;
	case 'a': g_camTarget.x -= 4.0f; break;
	case 'e': g_camTarget.y -= 4.0f; break;
	case 'q': g_camTarget.y += 4.0f; break;
	case 'W': g_camTarget.z -= 0.4f; break;
	case 'S': g_camTarget.z += 0.4f; break;
	case 'D': g_camTarget.x += 0.4f; break;
	case 'A': g_camTarget.x -= 0.4f; break;
	case 'E': g_camTarget.y -= 0.4f; break;
	case 'Q': g_camTarget.y += 0.4f; break;
	case 'i': g_sphereCamRelPos.y -= 11.25f; break;
	case 'k': g_sphereCamRelPos.y += 11.25f; break;
	case 'j': g_sphereCamRelPos.x -= 11.25f; break;
	case 'l': g_sphereCamRelPos.x += 11.25f; break;
	case 'o': g_sphereCamRelPos.z -= 5.0f; break;
	case 'u': g_sphereCamRelPos.z += 5.0f; break;
	case 'I': g_sphereCamRelPos.y -= 1.125f; break;
	case 'K': g_sphereCamRelPos.y += 1.125f; break;
	case 'J': g_sphereCamRelPos.x -= 1.125f; break;
	case 'L': g_sphereCamRelPos.x += 1.125f; break;
	case 'O': g_sphereCamRelPos.z -= 0.5f; break;
	case 'U': g_sphereCamRelPos.z += 0.5f; break;
		
	case 32:
		
		break;
	}

	g_sphereCamRelPos.y = glm::clamp(g_sphereCamRelPos.y, -78.75f, -1.0f);
	g_camTarget.y = g_camTarget.y > 0.0f ? g_camTarget.y : 0.0f;
	g_sphereCamRelPos.z = g_sphereCamRelPos.z > 5.0f ? g_sphereCamRelPos.z : 5.0f;
}