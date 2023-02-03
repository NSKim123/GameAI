struct FreeImage
{
	FIBITMAP *dib ;
	void Release()
	{
		FreeImage_Unload(dib);
	}
	char* ATCmd_ConvertUTF8(const char* pszSrc)
	{

		LPSTR szDst;
		WCHAR szTmp[256];
		MultiByteToWideChar( CP_UTF8, 0, pszSrc, -1, szTmp, 256);

		int len = 0;
		len = (wcslen(szTmp) + 1)*2;
		char* pstr      = (char*) malloc ( sizeof( char) * len);
		WideCharToMultiByte( 949, 0, szTmp, -1, pstr, len, NULL, NULL);
		return pstr;
	}
	FIBITMAP *GetBitmap(const char*FileName, int &Width, int &Height, int &BPP)
	{


		FREE_IMAGE_FORMAT fif = FreeImage_GetFileType((FileName));

		if(fif == FIF_UNKNOWN)
		{
			fif = FreeImage_GetFIFFromFilename((FileName));
		}

		if(fif == FIF_UNKNOWN)
		{
			return NULL;
		}



		if(FreeImage_FIFSupportsReading(fif))
		{
			dib = FreeImage_Load(fif, (FileName));

		}

		if(dib != NULL)
		{
			int OriginalWidth = FreeImage_GetWidth(dib);
			int OriginalHeight = FreeImage_GetHeight(dib);

			Width = OriginalWidth;
			Height = OriginalHeight;

			if(Width == 0 || Height == 0)
			{
				FreeImage_Unload(dib);
				return NULL;
			}

			BPP = FreeImage_GetBPP(dib);



			/*if(!GLEW_ARB_texture_non_power_of_two)
			{
				Width = 1 << (int)floor((log((float)Width) / log(2.0f)) + 0.5f); 
				Height = 1 << (int)floor((log((float)Height) / log(2.0f)) + 0.5f);
			}*/

			if(Width != OriginalWidth || Height != OriginalHeight)
			{
				FIBITMAP *rdib = FreeImage_Rescale(dib, Width, Height, FILTER_BICUBIC);
				FreeImage_Unload(dib);
				dib = rdib;
			}
		}

		return dib;
	}
};