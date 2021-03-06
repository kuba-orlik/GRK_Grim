struct texture{
	GLuint location;
	int width;
	int height;
	unsigned char* data;
};

struct texture loadTexture( const char * imagepath ){
	printf("  loading texture %s...", imagepath);
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;
	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file){
		printf("Image could not be opened\n"); 
		//return 0;
	}
	if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		//return null;
		//return false;
	}
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
		//return 0;
	}
	// Read ints from the byte array
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);
	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way
	// Create a buffer
	data = new unsigned char [imageSize];
 
	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);
 
	//Everything is in memory now, the file can be closed
	fclose(file);
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	printf("OK!\n");
	struct texture t;
	t.location = textureID;
	t.width = width;
	t.height = height;
	t.data = data;
	return t;
}



 struct texture sky_texture;
 struct texture noise_texture;
 struct texture leaves_texture;
 struct texture falling_leaf_texture;
 struct texture light_dot_texture;

 void loadTextures(){
	 printf("Loading textures...\n");
	 int width, height;
	 sky_texture = loadTexture("texture/sky3_noise.bmp");
	 noise_texture = loadTexture("texture/noise.bmp");
	 leaves_texture = loadTexture("texture/leaves.bmp");
	 light_dot_texture = loadTexture("texture/dot.bmp");
	 falling_leaf_texture = loadTexture("texture/falling_leaf.bmp");
 }

 void use_texture(struct texture t){
 	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, t.location);
 
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, t.width, t.height, 0, GL_BGR, GL_UNSIGNED_BYTE, t.data);
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 }

