// These are the interfaces 
////////////////////////////////////////
#ifndef _IFACES
#define _IFACES

enum class FILLTYPE { HATCH = 0, SOLID = 1, POLKADOT = 2 };
enum class INTERFACEID { IDRAW = 0, ISHAPEEDIT = 1, IDRAW2 = 2, IDRAW3 = 3 };



interface IDraw
{
	virtual void Draw() = 0;
};

interface IDraw2: public IDraw
{
	virtual	void DrawToMemory() = 0;
	virtual void DrawToPrinter() = 0;
};


interface IDraw3: public IDraw2
{
	virtual void DrawToMetaFile() = 0;
};

interface IShapeEdit
{
	virtual void Fill(FILLTYPE fType) = 0;
	virtual void Inverse() = 0;
	virtual void Stretch(int factor) = 0;
};

#endif // _IFACES