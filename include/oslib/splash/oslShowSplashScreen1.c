#include "oslib.h"

#define WIDTH 480
#define HEIGHT 272
#define min(x, y) (((x)>(y))?(y):(x))

/*
	Données du logo (format image PNG)
*/
const unsigned long __osl_logo_texte_data[] =		{
	0x474e5089, 0x0a1a0a0d, 0x0d000000, 0x52444849, 0xe0010000, 0x10010000, 0x00000208, 0x3024e700,
	0x0b0000db, 0x414449c2, 0xedda7854, 0x1b6fbfdd, 0xf1c00365, 0x02fe2bf3, 0x6c132e8a, 0x0b03d80c,
	0x1d74ad12, 0x74111090, 0x24162570, 0x08648954, 0xc242414b, 0xa9ce9755, 0x0ce4c8c4, 0x94204064,
	0x0212b4b0, 0x2086205b, 0x30ba9535, 0xbc5b0b38, 0x5ffb2450, 0xf7a3c330, 0xceef83d1, 0x9347e739,
	0x74cf9fa6, 0xf3bb9c6a, 0xf27ebe61, 0x9adceef8, 0x001966cd, 0x1cffcf17, 0x06810002, 0x0401a040,
	0x0681001a, 0x04006810, 0x0681001a, 0x04006810, 0x01a0401a, 0x04006810, 0x01a0401a, 0x81006810,
	0x01a04006, 0x81001a04, 0x01a04006, 0x81001a04, 0x00681006, 0x81001a04, 0x00681006, 0xa0401a04,
	0x00681001, 0xa0400681, 0x00681001, 0xa0400681, 0x001a0401, 0xa0400681, 0x001a0401, 0x68100681,
	0x401a0400, 0x681001a0, 0x401a0400, 0x681001a0, 0x40068100, 0x681001a0, 0x40068100, 0x1a0401a0,
	0x40068100, 0x1a0401a0, 0x10068100, 0x1a040068, 0x1001a040, 0x1a040068, 0x1001a040, 0x06810068,
	0x1001a040, 0x06810068, 0x0401a040, 0x0681001a, 0x04006810, 0x0681001a, 0x04006810, 0x01a0401a,
	0x04006810, 0x01a0401a, 0x81006810, 0x01a04006, 0x8b801a04, 0xe08725e8, 0xd6ad5a78, 0xcd9f8f52,
	0xceb2e766, 0x678e78b3, 0xf68bddbd, 0x3eb2f4df, 0x9af08bdf, 0x401a08c7, 0x681001a0, 0x1ccf4b80,
	0x9d9612f4, 0x9f1f4ca3, 0xbe3dd72e, 0xbce07ac9, 0x411a7d78, 0x00d02003, 0x31c00d02, 0x417e8398,
	0xe1bcf59d, 0xd17fd649, 0xb26f19ce, 0x7fd765ad, 0x9ceffbd6, 0x00682365, 0xa0401a04, 0xcc20b801,
	0x4e40bf41, 0x59f71e72, 0xee59ffac, 0xd0f3937f, 0xfad69ceb, 0xcfff7acf, 0x0460f8eb, 0x00d0200d,
	0x0b003408, 0xd2e68398, 0x118a9732, 0x0d020034, 0x9c00d020, 0x9cd0733b, 0xb7ce749a, 0x26db1cee,
	0x41180bbf, 0x00340803, 0x69700d02, 0x4ce68398, 0x674f7b2c, 0x230082f7, 0x06810068, 0x2e01a040,
	0x5cd07325, 0x7f7d675a, 0x0682306a, 0x04006810, 0xb523801a, 0xf31dc7cc, 0x7df92dc0, 0x179ecb7d,
	0xbd67127c, 0x0efad3ad, 0xf9f765c3, 0x69ff8ebc, 0x59e0077d, 0x7a65ffef, 0x0d04627d, 0x0800d020,
	0x61a5c034, 0x08c01a0e, 0x0681001a, 0x04006810, 0x01a0401a, 0x04006810, 0x01a0401a, 0x81006810,
	0x01a04006, 0x81006810, 0x01a04006, 0x81001a04, 0x00681006, 0x81001a04, 0x00681006, 0xa0401a04,
	0xdddb02e1, 0xf379bddd, 0x6ad5ab66, 0x2b2b5ab5, 0xbbbbbb2b, 0x4997098e, 0x399b366b, 0x698f3c0a,
	0xf6fb7dbe, 0x33fcff3f, 0xcf5eeff7, 0x392f265c, 0x6e773c04, 0xe0f9bcde, 0xbff283c1, 0x077dfbf7,
	0x14c4cb87, 0x78c51707, 0x3a74ee3c, 0x6b6b56ad, 0xc3c16b6b, 0xfebf5fae, 0xa283c1e0, 0xc9932628,
	0x3636bfec, 0x2e461c36, 0x5c1c5313, 0xe74e9d14, 0x2c20e0e0, 0xe32cbdd7, 0x1a3479d8, 0x076ddbb5,
	0x3868230d, 0x5c3870da, 0x595f5f5f, 0xd91f0959, 0xc7afafad, 0x1f1436da, 0x2b76eddf, 0xbbbbb01f,
	0x6edcea1b, 0x8119d5b7, 0x7e3da386, 0xe9d3b8fc, 0x6eaeaeac, 0x4fc76f6f, 0xb7b7b6ed, 0x70ce9d3b,
	0x9ebe6c38, 0x145c8330, 0x7f5fd7c5, 0xdb80f95d, 0x85876fb7, 0x386e371b, 0x1c3408ec, 0x3b9dcee1,
	0x3d5eaf57, 0x8a283838, 0xcd1a3462, 0x68d1b366, 0xabc08714, 0x57cdabab, 0x31b1b1b5, 0xf6f6cd9b,
	0xf57d5ef6, 0x7a11cad5, 0xefbbffac, 0xf3c8ebbe, 0xcd073022, 0xd5fcea89, 0x39855f57, 0x9468d187,
	0x6b6b68fe, 0x7bbddeeb, 0xf8dc6e37, 0x4e6f9be6, 0xafd75b65, 0x8fe3ff5f, 0x64f2c23f, 0xe5f97932,
	0x047f1d97, 0x6d6daa1a, 0xe9fa7d6d, 0xacf961a7, 0x4e9bb23f, 0x5cae57a7, 0x5145cb09, 0x39ededec,
	0x4c08bcfe, 0x071c7071, 0xceb10707, 0xacef5ebd, 0xf27bb23f, 0xfe5c49e4, 0x1dcff3fc, 0x6a1a047f,
	0x59673ac5, 0xddeef776, 0xafdcdeb3, 0x971afebf, 0xeabd5eaf, 0x50d023f8, 0xc61c386d, 0xb76ee0e9,
	0x83a60cfb, 0x147a3d1f, 0x76edda16, 0xc78847e5, 0xd74e9d30, 0xbc8fd7d7, 0xd8b9e62e, 0x0cd6970a,
	0x45145496, 0xefecfd7c, 0x6d709fef, 0x6d60c183, 0xb2cb2d6d, 0xd6ebcf3c, 0xb9b73716, 0x65fe19b9,
	0x7af51a34, 0xb3cf3cbd, 0x90278b2c, 0xd9d9d9d4, 0x7f1cfb09, 0x30f0f0fd, 0xffdd6efc, 0x64c99305,
	0xdba76767, 0x0b0fa6ed, 0xf1c18306, 0x6fce12b6, 0xc334f059, 0x13e18689, 0x66332f57, 0xb34099b3,
	0xc3c3c39c, 0xcf3c9c58, 0xb4aab84f, 0xf59659f5, 0xc3f2fdfb, 0x0e43f3d2, 0xb3670f0f, 0x9cfa0859,
	0xf272a3f0, 0x7af644d2, 0x9096f0bd, 0xdbb762fe, 0x9d650cad, 0x0de625f2, 0x71e472b7, 0xb7680f8b,
	0x08695edb, 0xdcdb4b34, 0x591d8cdc, 0xc93b5b5b, 0xbedcb9c0, 0xcc69c1ca, 0x3eddbb65, 0xfbac3c3c,
	0x31643240, 0x91f97acd, 0x7f60c183, 0xe0ed3f7f, 0xf9425bdc, 0x514e633d, 0xa56ecb14, 0xa7651c52,
	0xa57955d2, 0x689cd021, 0x8feea37e, 0x7af523fd, 0x76cb2cbd, 0xff667676, 0x3cb3cb3d, 0x66cd9d38,
	0x2719ec71, 0x26dd421c, 0x33ba4993, 0xd433a8e9, 0x5d37c43f, 0x7efefeff, 0xdbb74358, 0xc433cf2d,
	0xbb768919, 0xf4b53e9d, 0x6b0d293d, 0x060cad93, 0xc3cf3c83, 0x3468c79b, 0x12af3c5a, 0x23408b0e,
	0x64c71cd0, 0xa7b23932, 0x0a1ceb0d, 0xd4984e98, 0x373f5dfd, 0x73a69c0b, 0xf69aa06e, 0x61fd72b4,
	0x18de1a88, 0x373ceaea, 0x66eb9f12, 0xadc37acc, 0x74bd7af4, 0x76767787, 0xc4e7cc16, 0x4e3733a7,
	0xb961a047, 0xcf638979, 0xaacda693, 0xdda69c1b, 0x9e5c134c, 0x3a072954, 0x22797f5d, 0x1fd46ea5,
	0x6595e306, 0x38799cac, 0xb5d256de, 0xbac01e55, 0x1a047169, 0x3a7e908e, 0x65d3d957, 0x3cabeda5,
	0x5f6d2175, 0x8b6b67e5, 0x074a494f, 0x39f60bbf, 0x27f2af3e, 0x0d1fe98d, 0xb86cdd75, 0x2a33f295,
	0x87c16e6f, 0x2c3408a5, 0x9c36ce92, 0x26bb04fc, 0xeda48427, 0xdd5f0cab, 0xb8e977c7, 0x41bcf178,
	0x4f28589c, 0x4a4dcc6e, 0x348dce54, 0x3ee54adc, 0x82d19f97, 0x1a042af7, 0x404dcd6a, 0xe9857c9f,
	0x1c6546fc, 0x9e57dae7, 0xa9774ef2, 0x9e470d72, 0xe28e9787, 0xd3d4a72b, 0xd6ea1a3f, 0x952b64d0,
	0xe59b9ceb, 0x7a142788, 0x92b811b1, 0x1bd2a690, 0x173e2c62, 0x7e9fa1ad, 0x7f2e29fa, 0x9587e1f8,
	0x2c97898f, 0xbe514516, 0xbfeff164, 0xe5cb8eff, 0x7e3e907b, 0x77cd38fc, 0xc34c5ddd, 0xae540517,
	0x2b276e7f, 0x2b64d0d7, 0xe8fa3e73, 0x7070b0a3, 0x4e9d3070, 0xe56c9ee7, 0xd4b8de59, 0x6f49161b,
	0x5bf27188, 0xd3a74fe6, 0xcb2cdf18, 0xedfb7ede, 0xefd263f2, 0x47bca964, 0xb786ba52, 0xee6b7ade,
	0xfeffbfa7, 0xecfe5c7b, 0x772acfb3, 0xbe2f8be3, 0x6eb70b08, 0x74ff2add, 0x9bc71e3c, 0xcf7dd750,
	0xccad934b, 0x378de379, 0x7f5ff2e2, 0xbf9715fd, 0x34dbf6fd, 0x1cd020df, 0xcefbc067, 0x709cef3b,
	0xf6fdbf6d, 0x7655e55b, 0xbcadfd75, 0x5a285247, 0xc7967518, 0xf38771bf, 0xd81caf3c, 0xf8e3c78e,
	0x37f9501e, 0x7ddf77c1, 0xf83f9717, 0x87c583e0, 0x652b6ea5, 0xfcfec3e9, 0x77b9cff3, 0x7bdef7ac,
	0x810d8bcf, 0x6f01b966, 0x1cad9e3a, 0xe7e52ef0, 0xcb8e7f9f, 0x71fc7f1f, 0xc7e263e5, 0x4bed9577,
	0x7e5fe0c7, 0xbe82e5f9, 0xd3f78dd7, 0xa05e56fe, 0xb200df18, 0xcdf37b2c, 0x8bbf1737, 0x4557f52c,
	0xcf8a4f7c, 0x7f1fc7f1, 0x5e24930c, 0xcb34086c, 0x4fa02749, 0x7b749265, 0xe8f17b7b, 0xfff3fd38,
	0xe56bb5da, 0x85a2869f, 0x9af395d8, 0xffbf0d4e, 0xdf8bfbfe, 0xe9c52a00, 0xd7f5e0e9, 0xa63f2f5f,
	0xc52a56c9, 0xa0e71ca7, 0x97bbddef, 0x9f27d965, 0x86c5e27c, 0x1f9cb340, 0x2e21f87e, 0xbd249c37,
	0xab7eff7f, 0x3a74ead5, 0x4ef7afe5, 0x8e8e25ff, 0x9de1c387, 0x072a773b, 0xdf1f95a7, 0x5965a285,
	0xb325f956, 0xa770d4e9, 0xc3dca9d3, 0x75e11df0, 0xcdcc317f, 0x187d874a, 0xeff7c78f, 0x2b6a5fdf,
	0x0a33e295, 0x6f84a227, 0xc28a2c5c, 0x1ff395b7, 0xd1a1644e, 0xbfcb98f9, 0xd349e266, 0xd2e64be5,
	0xab85c5db, 0x1be5c23f, 0xc5e01773, 0xdc2f94c7, 0x35e8a788, 0xc9b93739, 0xb1cf4a93, 0x8a4cb70b,
	0x92cd33b7, 0xa28a544b, 0xc9b27748, 0xb7a52a56, 0xfaa78be8, 0x26defb1d, 0x179a0f38, 0xe6d3da57,
	0x97a5b917, 0x28fdcdcc, 0xdfb054fd, 0x39d657ef, 0x39dcfc4d, 0x2d2f8bce, 0x4b017269, 0x22f1d039,
	0x9d9d9df2, 0xb4a37b70, 0x376edda1, 0xeeaf48dc, 0xbbd9642d, 0x0f2ba467, 0xc5b8c2d7, 0xd2cd0228,
	0xc7cebbd2, 0xa3468c88, 0xda1fbf7e, 0xb7b2e0ba, 0xbc3578ba, 0x89fa7e0b, 0xc7291c59, 0x5c16fb4b,
	0x37be9d51, 0xdd58782c, 0x5b6123d9, 0x953475e9, 0x2b70d93b, 0xe79d038b, 0xae1a3879, 0x59a0441e,
	0x7efefec2, 0x9e7e67fa, 0x9b9b9be7, 0x16109f75, 0xb9b9b145, 0xe20c1839, 0xaf3174a5, 0xd3c28be5,
	0x79770c5f, 0xca87dbae, 0xeddadfb3, 0xcd0be2f6, 0x982fb2d3, 0x487f2837, 0xdc451458, 0x997bb999,
	0x70da0396, 0xa08e472b, 0x42477733, 0x69ca59a0, 0x59555a7d, 0x2d3830ae, 0xaebf2fdf, 0xc26771b6,
	0xda97961d, 0x5ed7c5f9, 0x99432fc7, 0xfd9e5c9b, 0xf2ec82ff, 0x2b631fbf, 0xd97a0647, 0xd105e75f,
	0xc0ce1b9a, 0x1839e291, 0xe98b2ce1, 0xa2861a74, 0x8a3d1e8f, 0x4515ddd7, 0xb5daed71, 0x3eef77bb,
	0x7d94af83, 0xc3870d1e, 0x38d5d5d5, 0xbe0cf23d, 0x91e79d19, 0x5839c340, 0x670f5f5f, 0xfe79e782,
	0x0753e9f4, 0x0b868104, 0x5e9d3a61, 0x4e2572b9, 0xb2fc9fd0, 0x172b2e4c, 0x16b3c0aa, 0xf5ef4c2f,
	0x118cea7a, 0x71145c34, 0x199c39f6, 0xd02080e2, 0xc78c2170, 0x7b5ed7e3, 0x3cb2cb2d, 0x793c9fcf,
	0x77eef0d2, 0x714cb0bc, 0x3b1bb3c0, 0x59673a87, 0x43e1f0f6, 0xe1a04675, 0xd79c0c42, 0x79e3d7d7,
	0xc0c18375, 0xbc4d33d4, 0xe9c010e4, 0xdd6f930e, 0x07cf15ba, 0xf9e7939f, 0xd50f87c3, 0x41cc8699,
	0x595a69c3, 0x73aba959, 0x7fdf1451, 0x0d99bdff, 0x01c5339a, 0x5795e9a7, 0x381ca95e, 0xbdfdfbf7,
	0x46753dbd, 0xfcdce1a0, 0xf12fcbf2, 0x9e79527e, 0x0dadadaf, 0x4fa78306, 0x6c6c6e9f, 0xcb2c3838,
	0x11800714, 0x0d020034, 0x0800d020, 0x03408034, 0x0800d020, 0x03408034, 0x0200d020, 0x0340800d,
	0x0200d020, 0x0340800d, 0x02003408, 0x00d0200d, 0x02003408, 0x00d0200d, 0x40803408, 0x00d02003,
	0x40803408, 0x00d02003, 0x40800d02, 0x00340803, 0x40800d02, 0x00340803, 0xd0200d02, 0x00340800,
	0xd0200d02, 0x80340800, 0xd0200340, 0x10ed0200, 0x00340800, 0xd0200d02, 0x80340800, 0xd0200340,
	0x80340800, 0xd0200340, 0x800d0200, 0xd0200340, 0x800d0200, 0x34080340, 0x200d0200, 0x340800d0,
	0x200d0200, 0x340800d0, 0x20034080, 0x340800d0, 0x20034080, 0x0d0200d0, 0x08034080, 0x0d020034,
	0x08034080, 0x0d020034, 0x0800d020, 0x0d020034, 0x0800d020, 0x03408034, 0x0200d020, 0x0340800d,
	0xcf00d020, 0xb7017fbd, 0xb2317455, 0x005fbc65, 0x49000000, 0xae444e45, 0x00826042
};

const unsigned long __osl_logo_etoile_data[] =		{
	0x474e5089, 0x0a1a0a0d, 0x0d000000, 0x52444849, 0x10000000, 0x18000000, 0x00000208, 0xeac27c00,
	0x0000005b, 0x414449d3, 0xadda7854, 0x83123152, 0x185c0840, 0xe969401f, 0x5a9fbd53, 0x02f0955e,
	0xc1c5c149, 0x4cc97893, 0xd8142876, 0xb0c9015d, 0xa70202e3, 0x769c0079, 0x2b282baf, 0x12aba001,
	0xa266cd81, 0x2a709a9a, 0x4993e00f, 0xedf2e0ef, 0x3a613339, 0xdc3bcb87, 0x02000624, 0xc3de4870,
	0x13b0d90d, 0xdbbb2fd5, 0x1c4e43d0, 0x86e1eda6, 0xf8ce8dd3, 0xffaddc11, 0xed20ee8a, 0xedfd710e,
	0x38d7ba33, 0xa52f9443, 0x96a8997a, 0xa7131352, 0xc707cd9a, 0xdd1c7bb0, 0x3249e6ee, 0x957614ec,
	0x4ab1bcfe, 0x59644400, 0x11111116, 0x6dd55555, 0x48fa9edb, 0x19c93077, 0x6811159e, 0xcbc9b4be,
	0x0efee109, 0xa09cbec3, 0xd4c243aa, 0x03d0fab5, 0x37d0ec42, 0x017846fe, 0x6c8c9ea9, 0x8b6c727d,
	0x00000000, 0x444e4549, 0x826042ae
};


/*
	Pour le logo!!!
	Décale la valeur du rouge vers l'alpha, pour un niveau de transparence correspondant au texte
*/
static void logoDecalePixels(OSL_IMAGE *img)		{
	unsigned long *data = (unsigned long*)img->data;
	int i;
	for (i=0;i<img->totalSize>>2;i++)		{
		*data = (*data) << 24;
		data++;
	}
	oslUncacheImageData(img);
}

static OSL_IMAGE *logoCreeImageFond()			{
	OSL_IMAGE *fond;
	unsigned char *data;
	int i, j;
	fond = oslCreateImage(480, 272, OSL_IN_RAM, OSL_PF_8BIT);
	if (!fond)
		return 0;
	fond->palette = oslCreatePalette(256, OSL_PF_8888);
	if (!fond->palette)		{
		oslDeleteImage(fond);
		return 0;
	}
	for (j=0;j<HEIGHT;j++)			{
		data = (unsigned char*)oslGetImageLine(fond, j);
		for (i=0;i<WIDTH/2;i++)			{
			*data++ = i;
			*data++ = i;
		}
	}
	oslUncacheImage(fond);
	return fond;
}

static void logoCreePaletteRotative(OSL_IMAGE *img, int angle)			{
	unsigned long *pal;
	int valeur, i;
	pal = (unsigned long*)img->palette->data;
	for (i=0;i<239;i++)			{
		angle = angle % (256 * 6);
		if (angle < 0)	angle += 256 * 6;
		valeur = angle & 255;
		if (angle < 256)					// Rouge -> jaune
			*pal++ = RGBA(255, valeur, 0, 0xff);
		else if (angle < 256 * 2)			// Jaune -> vert
			*pal++ = RGBA(255-valeur, 255, 0, 0xff);
		else if (angle < 256 * 3)			// Vert -> turquoise
			*pal++ = RGBA(0, 255, valeur, 0xff);
		else if (angle < 256 * 4)			// Turquoise -> bleu
			*pal++ = RGBA(0, 255-valeur, 255, 0xff);
		else if (angle < 256 * 5)			// Bleu -> rose
			*pal++ = RGBA(valeur, 0, 255, 0xff);
		else if (angle < 256 * 6)			// Rose -> rouge
			*pal++ = RGBA(255, 0, 255-valeur, 0xff);
		angle += 8;
	}
	oslUncachePalette(img->palette);
}


const OSL_VIRTUALFILENAME __osl_logo_ram_files[] =		{
	{"ram:/logo/texte.png", (void*)__osl_logo_texte_data, sizeof(__osl_logo_texte_data), &VF_MEMORY},
	{"ram:/logo/etoile.png", (void*)__osl_logo_etoile_data, sizeof(__osl_logo_etoile_data), &VF_MEMORY},
};

/*
	Affiche le logo OSLib
	Ne cherchez pas à comprendre cette fonction si vous débutez :p
*/
int oslShowSplashScreen1()			{
	OSL_IMAGE *texte, *etoile;
	OSL_IMAGE *fond;
	int skip=0;
	int x, y;
	int angle, nFrame, val;
	float dist;
	int pousNb;
	float pousX[100], pousY[100];
	int i, imgAngle;
	float tblVitPous[8] = {0.2, 0.3, 0.15, 0.2, 0.35, 0.2, 0.15, 0.3};
	int fade = 0;

	//Add these files to the list
	oslAddVirtualFileList((OSL_VIRTUALFILENAME*)__osl_logo_ram_files, oslNumberof(__osl_logo_ram_files));

	texte = oslLoadImageFile("ram:/logo/texte.png", OSL_IN_VRAM | OSL_UNSWIZZLED, OSL_PF_8888);
	if (!texte)
		return 0;

	logoDecalePixels(texte);

	oslStartDrawing();
	oslSetDithering(1);
	oslSetDrawBuffer(texte);
	oslDrawGradientRect(0,0, WIDTH,HEIGHT, RGB(0,0,0), RGB(0,0,128), RGB(0,0,128), RGB(0,0,0));
	oslSetDrawBuffer(OSL_DEFAULT_BUFFER);
	oslEndDrawing();

	oslMoveImageTo(texte, OSL_IN_RAM);
	oslSwizzleImage(texte);

	fond = logoCreeImageFond();
	if (!fond)		{
		oslDeleteImage(texte);
		return 0;
	}

	oslSetTransparentColor(RGB(255,0,254));
	etoile = oslLoadImageFile("ram:/logo/etoile.png", OSL_IN_RAM, OSL_PF_5551);
	if (!etoile)		{
		oslDeleteImage(texte);
		oslDeleteImage(fond);
		return 0;
	}
	oslDisableTransparentColor();

	texte->centerX = texte->sizeX / 2;
	texte->centerY = texte->sizeY / 2;

	angle = 270;
	dist = 160.0f;
	nFrame = 0;
	pousNb=0;
	imgAngle = 78;

	while (!osl_quit)
	{
		if (!skip)			{
			oslStartDrawing();
			logoCreePaletteRotative(fond, 2000-nFrame*8);
			oslDrawImage(fond);
			x = WIDTH/2;
			y = HEIGHT/2;
			if (dist > 0)			{
				x += oslCos(angle, dist);
				y -= oslSin(angle, dist);
				texte->stretchX = texte->sizeX * (1+dist*0.03f);
				texte->stretchY = texte->sizeY * (1+dist*0.03f);
				texte->angle = imgAngle;
			}
			else		{
				texte->stretchX = texte->sizeX;
				texte->stretchY = texte->sizeY;
				texte->angle = 0;
			}
			oslSetBilinearFilter(1);
			//Effet spécial pour le texturage des nuages
			if (nFrame >= 50)		{
				val = (nFrame - 50) * 6;
				if (val > 255)
					val = 255;
				oslSetAlpha(OSL_FX_ALPHA|OSL_FX_COLOR, RGBA(val,val,val,0xff));
			}
			else
				oslSetAlpha(OSL_FX_ALPHA|OSL_FX_COLOR, RGBA(0,0,0,0xff));
			oslDrawImageXY(texte, x, y);

			oslSetImageTileSize(etoile, 0,0, 16,16);
			oslCorrectImageHalfBorder(etoile);
			oslImageSetRotCenter(etoile);
			etoile->angle = 360 - ((nFrame * 3) % 360);
			etoile->stretchX = 16;
			etoile->stretchY = 16;
			oslSetAlpha(OSL_FX_ALPHA|OSL_FX_COLOR, RGBA(0xff,0,0,0xff));
			oslDrawImageXY(etoile, 480-9, 272-9);

			//Dessine l'étoile
			if (nFrame >= 140 && nFrame < 240)			{
				val = nFrame - 140;
				angle = (val * 5) % 360;
				i = nFrame - (240 - 256/12);
				if (val*16 < 255)
					oslSetAlpha(OSL_FX_ALPHA, val*16);
				else if (i > 0)
					oslSetAlpha(OSL_FX_ALPHA, 255-i*12);
				else
					oslSetAlpha(OSL_FX_RGBA, 0);
				etoile->angle = (val * 8) % 360;
				etoile->x = WIDTH/2 + oslCosi(angle, 120);
				etoile->y = HEIGHT/2 - oslSini(angle, 50);
				etoile->stretchX = 16 * 2;
				etoile->stretchY = 16 * 2;
				oslDrawImage(etoile);
				oslResetImageHalfBorder(etoile);
			}

			//Restaure l'environnement
			oslSetBilinearFilter(0);
			//Dessine les poussières
			oslSetAlpha(OSL_FX_ADD, 0xff);
			oslSetImageTileSize(etoile, 0,16, 8,8);
			oslImageSetRotCenter(etoile);
			for (i=0;i<pousNb;i++)
				oslDrawImageSimpleXY(etoile, pousX[i], pousY[i]);

			oslSetAlpha(OSL_FX_RGBA, 0);
//			oslSysBenchmarkDisplay();
			//Fade
			if (nFrame < 32)
				oslDrawFillRect(0,0, WIDTH,HEIGHT, RGBA(0,0,0,255-(nFrame<<3)));
			if (fade > 0)		{
				if (fade >= 31)
					fade = 31;
				oslDrawFillRect(0,0, WIDTH,HEIGHT, RGBA(0,0,0,(fade<<3)));
			}
			oslEndDrawing();
		}
		nFrame++;
		//L'étoile tourne sur le logo
		if (nFrame >= 140 && nFrame < 240)			{
			val = nFrame - 140;
			angle = (val * 5) % 360;
			if (angle >= 200 && angle <= 340)		{
				pousX[pousNb] = WIDTH/2 + oslCos(angle, 120);
				pousY[pousNb] = HEIGHT/2 - oslSin(angle, 50);
				pousNb++;
			}
		}
		if (fade > 0)
			fade++;
		if (nFrame > 290 && fade == 0)
			fade = 1;
		for (i=0;i<pousNb;i++)		{
			pousY[i] += tblVitPous[i%8];
		}

		dist -= 2.0f;
		angle -= 4;
		imgAngle -= 1;
		if (angle < 0)
			angle += 360;
		skip = oslSyncFrameEx(1,4,0);
		//Lecture des touches
		oslReadKeys();
		if ((osl_keys->pressed.value & (OSL_KEYMASK_START|OSL_KEYMASK_CIRCLE|OSL_KEYMASK_CROSS)) && fade == 0)
			fade = 1;
		if (fade >= 32)
			break;
	}
	oslSetAlpha(OSL_FX_RGBA, 0);
	oslDeleteImage(texte);
	oslDeleteImage(fond);
	oslDeleteImage(etoile);

	oslRemoveVirtualFileList((OSL_VIRTUALFILENAME*)__osl_logo_ram_files, oslNumberof(__osl_logo_ram_files));
	return 1;
}
