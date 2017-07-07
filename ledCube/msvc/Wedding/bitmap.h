#pragma once
class bitmap
{
private:

public:
	
	bitmap();
	~bitmap();

	unsigned int height;
	unsigned int width;
	unsigned char *Rdata;
	unsigned char *Gdata;
	unsigned char *Bdata;

	//æ“¾‚É¬Œ÷‚·‚ê‚Îƒ|ƒCƒ“ƒ^‚ğ¸”s‚·‚ê‚ÎNULL‚ğ•Ô‚·
	bool read_bmp(char* filename);
};

