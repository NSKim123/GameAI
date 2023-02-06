
struct Texture
{
	bool			Load;
	GLuint		m_gridTexture;
	Vector2f		m_vSize;





	Vector2f  GetSize()
	{
		return m_vSize;
	}
	void LoadImage(const char* filename)
	{

		Load=false;
		glPushMatrix();

		Mat img = imread(filename,IMREAD_UNCHANGED);

		if(img.empty()==false)
		{
			Load=true;
			int w=img.cols;
			int h =img.rows;
			int ch =img.channels();

			GLenum ColorFormat = GL_BGRA;
			if (ch== 1)
				ColorFormat = GL_LUMINANCE;
			if(ch == 3)
				ColorFormat = GL_BGR;


			m_vSize =Vector2f(w,h);

			glGenTextures(1, &m_gridTexture);
			glBindTexture(GL_TEXTURE_2D, m_gridTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, ColorFormat, GL_UNSIGNED_BYTE,(GLvoid*)img.data);

			glEnable(GL_TEXTURE_2D);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			img.release();
		}
		else
			printf("Texture Load Fail");
		glPopMatrix();

	}
	void Release()
	{
		glDeleteTextures(1,&m_gridTexture);
	}
	GLuint* GetTexture()
	{
		if(Load==true)
			return &m_gridTexture;
		else
			return NULL;

	}


};