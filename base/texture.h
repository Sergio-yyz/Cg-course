#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <stb_image.h>

class Texture {
public:
	Texture();

	virtual ~Texture();

	virtual void bind() const = 0;

	virtual void unbind() const = 0;

protected:
	GLuint _handle = {};

	virtual void cleanup();
};

class Texture2D : public Texture {
public:
	Texture2D(const std::string path);

	~Texture2D() = default;

	void bind() const override;

	virtual void unbind() const;

private:
	std::string _path;
};

class TextureCubemap : public Texture {
public:
	TextureCubemap(const std::vector<std::string>& filenames);

	~TextureCubemap() = default;

	void bind() const override;

	void unbind() const override;

private:
	std::vector<std::string> _paths;
};

class depthmap {
public:
	depthmap() = default;
	~depthmap() = default;
	void depthMapFBOInit();
private:
	const unsigned int SHADOW_WIDTH = 1024 * 10;
	const unsigned int SHADOW_HEIGHT = 1024 * 10;
	GLuint depthMap;
	GLuint depthMapFBO;
};