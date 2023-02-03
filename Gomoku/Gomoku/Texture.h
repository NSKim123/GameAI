#pragma comment(lib,"Freeimage/FreeImage.lib")
#include "Freeimage/FreeImage.h"
#include "FreeImageLoader.h"

struct Texture
{
	FreeImage	m_Loader;
	FIBITMAP *	Load;
	GLuint		m_gridTexture;
	Vector2f		m_vSize;



	void Release()
	{
		glDeleteTextures(1,&m_gridTexture);
	}

	Vector2f  GetSize()
	{
		return m_vSize;
	}
	void LoadImage(const char* filename)
	{
		
		glPushMatrix();

		
		

		int Width, Height, BPP;

		

		FIBITMAP* Load1 = m_Loader.GetBitmap(filename,Width, Height, BPP);

		Load = FreeImage_ConvertTo32Bits(Load1);


		if(Load!=NULL)
		{
			int w=Width;
			int h =Height; 

			GLubyte* textura = new GLubyte[4*w*h];
			char* pixeles = (char*)FreeImage_GetBits(Load);

			for(int j= 0; j<w*h; j++)
			{
				textura[j*4+0]= pixeles[j*4+0];
				textura[j*4+1]= pixeles[j*4+1];
				textura[j*4+2]= pixeles[j*4+2];
				textura[j*4+3]= pixeles[j*4+3];
			}


			m_vSize =Vector2f(w,h);

			glGenTextures(1, &m_gridTexture);
			glBindTexture(GL_TEXTURE_2D, m_gridTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE,(GLvoid*)textura);

			glEnable(GL_TEXTURE_2D);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			FreeImage_Unload(Load);
			m_Loader.Release();
			delete textura;

			//printf("TextureName : %s \nw:%d h:%d\n",name,w,h);
		}
		else
			printf("Texture Load Fail");
		glPopMatrix();

	}
	GLuint* GetTexture()
	{
		if(Load!=NULL)
			return &m_gridTexture;
		else
			return NULL;

	}


};