// A generic container for Draw-aware classes.

template< typename TShape >
class CDrawTemplate
{
private:
	TShape theShape;

public:
	CDrawTemplate() = default;
	~CDrawTemplate() = default;

	// Draw which ever shape we have.
	void DrawTheShape()
	{
		// Configure a message box describing what we are rendring.
		theShape.Draw();
	}
};