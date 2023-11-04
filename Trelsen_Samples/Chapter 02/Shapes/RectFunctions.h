#include <print>
#include <format>

// Here is the global 3D rect.
C3DRect* ptheRect;
C3DRectEx* ptheRectEx;

// Functions to operate on the 3D rect.
bool GetInterfaceFrom3DRect(INTERFACEID iid, void** iFacePtr);
void CreateThe3DRect();
void DestroyThe3DRect();

void ExersiceRect(IDraw* pDraw);

// Functions to operate on the 3D rectEx.
bool GetInterfaceFrom3DRectEx(INTERFACEID iid, void** iFacePtr);
void CreateThe3DRectEx();
void DestroyThe3DRectEx();

////////////////////////////////////////////////////////////////////////

void CreateThe3DRect()
{
	// Create a 3d-rect.	
	ptheRect = new C3DRect;
}


void DestroyThe3DRect()
{
	// See ya.	
	delete ptheRect;
}


bool GetInterfaceFrom3DRect(INTERFACEID iid, void** iFacePtr)
{
	if(ptheRect == NULL){
		std::println( "You forgot to create the 3DRect!" );
		return false;
	}

	if(iid == INTERFACEID::IDRAW){		// They want access to IDraw.
		// Cast the client's pointer to the IDraw interface of ptheRect.
		*iFacePtr = (IDraw*) ptheRect;
		return true;
	}
	
	if(iid == INTERFACEID::ISHAPEEDIT) {	// They want access to IShapeEdit.
		// Cast the client's pointer to the IShapeEdit interface of ptheRect.
		*iFacePtr = (IShapeEdit*) ptheRect;
		return true;
	}
	
	// I have no clue what they want.
	*iFacePtr = NULL;	// Just to be safe.
	std::println( "C3DRect does not support interface ID: {}", (int)iid );
	
	return false;			
}	

////////////////////////////////////////////////////////////////////////

void CreateThe3DRectEx()
{
	// Create a 3d-rectEx.	
	ptheRectEx = new C3DRectEx();
}


void DestroyThe3DRectEx()
{
	// See ya.	
	delete ptheRectEx;
}


bool GetInterfaceFrom3DRectEx(INTERFACEID iid, void** iFacePtr)
{
	if(ptheRectEx == NULL){
		std::println( "You forgot to create the 3DRectEx!" );
		return false;
	}

	if(iid == INTERFACEID::IDRAW)
	{
		*iFacePtr = (IDraw*)ptheRectEx;
		return true;
	}

	if(iid == INTERFACEID::IDRAW2)
	{
		*iFacePtr = (IDraw2*)ptheRectEx;
		return true;
	}

	if(iid == INTERFACEID::IDRAW3)
	{
		*iFacePtr = (IDraw3*)ptheRectEx;
		return true;
	}

	// I have no clue what they want.
	*iFacePtr = NULL;	// Just to be safe.
	std::println( "C3DRectEX does not support interface ID: {}", (int)iid );
	return false;

}	

////////////////////////////////////////////////////////////////////////


void ExersiceRect(IDraw* pDraw)
{
	std::println(  "Drawing some IDraw compatible object..." );
	pDraw->Draw();
}


