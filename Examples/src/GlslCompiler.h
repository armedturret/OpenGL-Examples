#include <GL/glew.h>
#include <string>

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	int compileShadersFromFile(std::string vertexShader, std::string fragmentShader);

	GLuint getProgram() const { return m_program; }

	void destroy();

private:
	std::string getFileContents(std::string fileName);
	
	int compileShader(std::string shaderFile, GLuint& shader, GLenum type);


	GLuint m_vertexShader = 0;
	GLuint m_fragmentShader = 0;

	GLuint m_program = 0;
};