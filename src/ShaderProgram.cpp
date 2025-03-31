#include "ShaderProgram.h"

ShaderProgram* spLambert;
ShaderProgram* spConstant;
ShaderProgram* spTextured;
ShaderProgram* spColored;
ShaderProgram* spLambertTextured;

void initShaders() {
	spLambert = new ShaderProgram("../shader/v_lambert.glsl", NULL, "../shader/f_lambert.glsl");
	spConstant = new ShaderProgram("../shader/v_constant.glsl", NULL, "../shader/f_constant.glsl");
	spTextured = new ShaderProgram("../shader/v_textured.glsl", NULL, "../shader/f_textured.glsl");
	spColored = new ShaderProgram("../shader/v_colored.glsl", NULL, "../shader/f_colored.glsl");
	spLambertTextured = new ShaderProgram("../shader/v_lamberttextured.glsl", NULL, "../shader/f_lamberttextured.glsl");
}

void freeShaders() {
	delete spLambert;
	delete spConstant;
	delete spTextured;
	delete spColored;
	delete spLambertTextured;
}

char* ShaderProgram::readFile(const char* fileName) {
	int filesize;
	FILE *plik;
	char* result;
	#pragma warning(suppress : 4996)
	plik=fopen(fileName,"rb");
	if (plik != NULL) {
		fseek(plik, 0, SEEK_END);
		filesize = ftell(plik);
		fseek(plik, 0, SEEK_SET);
		result = new char[filesize + 1];
		#pragma warning(suppress : 6386)
		int readsize=fread(result, 1, filesize, plik);
		result[filesize] = 0;
		fclose(plik);
		return result;
	}
	return NULL;
}

GLuint ShaderProgram::loadShader(GLenum shaderType,const char* fileName) {
	GLuint shader = glCreateShader(shaderType);
	const GLchar* shaderSource = readFile(fileName);
	glShaderSource(shader,1,&shaderSource,NULL);
	glCompileShader(shader);
	delete []shaderSource;
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength);
	if(infologLength > 1){
		infoLog = new char[infologLength];
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		delete []infoLog;
	}
	return shader;
}

ShaderProgram::ShaderProgram(const char* vertexShaderFile,const char* geometryShaderFile,const char* fragmentShaderFile){
	vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderFile);
	if(geometryShaderFile != NULL)
		geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryShaderFile);
	else
		geometryShader = 0;
	fragmentShader = loadShader(GL_FRAGMENT_SHADER,fragmentShaderFile);
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	if (geometryShaderFile!=NULL) glAttachShader(shaderProgram,geometryShader);
	glLinkProgram(shaderProgram);
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;
	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH,&infologLength);
	if(infologLength > 1){
		infoLog = new char[infologLength];
		glGetProgramInfoLog(shaderProgram, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		delete []infoLog;
	}
}

ShaderProgram::~ShaderProgram(){
	glDetachShader(shaderProgram, vertexShader);
	if (geometryShader!=0) glDetachShader(shaderProgram, geometryShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	if (geometryShader!=0) glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
}

void ShaderProgram::use(){
	glUseProgram(shaderProgram);
}

GLuint ShaderProgram::u(const char* variableName){
	return glGetUniformLocation(shaderProgram,variableName);
}

GLuint ShaderProgram::a(const char* variableName){
	return glGetAttribLocation(shaderProgram,variableName);
}
