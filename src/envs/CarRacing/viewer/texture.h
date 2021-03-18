#ifndef TEXTURE
#define TEXTURE

#include <iostream>
#include <string>
#include <vector>

#include "../../../main.h"

class Texture
{
	private:
		GLuint id;
		int width;
		int height;
		unsigned int type;

	public:
		Texture(std::string filename, GLenum type, bool mipmap=true)
		{
			this->type = type;
			std::vector<unsigned char> chars;
			unsigned int iwidth, iheight;
			unsigned int error = lodepng::decode(chars, iwidth, iheight, filename);
			if(error) 
			{
				std::cerr << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << filename << std::endl;
			}
			unsigned char *image = chars.data();
			this->width = iwidth;
			this->height = iheight;

			glGenTextures(1, &this->id);
			glBindTexture(type, this->id);

			glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
			glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(type);

			glActiveTexture(0);
			glBindTexture(type, 0);
		}

		~Texture()
		{
			glDeleteTextures(1, &this->id);
		}

		inline float aspect() const { return static_cast<float>(this->width) / this->height; }

		inline GLuint getID() const { return this->id; }

		void bind(const GLint texture_unit)
		{
			glActiveTexture(GL_TEXTURE0 + texture_unit);
			glBindTexture(this->type, this->id);
		}

		void unbind()
		{
			glActiveTexture(0);
			glBindTexture(this->type, 0);
		}
};

#endif