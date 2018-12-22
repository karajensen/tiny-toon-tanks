/*
	adding DDS loading support to stbi
*/

#ifndef HEADER_STB_IMAGE_DDS_AUGMENTATION
#define HEADER_STB_IMAGE_DDS_AUGMENTATION

//	is it a DDS file?
extern int      soil_stbi_dds_test_memory      (soil_stbi_uc const *buffer, int len);

extern soil_stbi_uc *soil_stbi_dds_load             (char *filename,           int *x, int *y, int *comp, int req_comp);
extern soil_stbi_uc *soil_stbi_dds_load_from_memory (soil_stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp);
#ifndef STBI_NO_STDIO
extern int      soil_stbi_dds_test_file        (FILE *f);
extern soil_stbi_uc *soil_stbi_dds_load_from_file   (FILE *f,                  int *x, int *y, int *comp, int req_comp);
#endif

//
//
////   end header file   /////////////////////////////////////////////////////
#endif // HEADER_STB_IMAGE_DDS_AUGMENTATION
