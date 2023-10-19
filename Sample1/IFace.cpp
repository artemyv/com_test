//
// Iface.cpp
//
#include <iostream>
#include <memory>

#include <objbase.h> // Определить интерфейс
void trace(const char* pMsg) { std::cout << pMsg << std::endl; }
// Абстрактные интерфейсы
interface IX
{
	virtual void __stdcall Fx1() = 0;
	virtual void __stdcall Fx2() = 0;
};
interface IY
{
	virtual void __stdcall Fy1() = 0;
	virtual void __stdcall Fy2() = 0;
};
// Реализация интерфейса
class CA : public IX,
	public IY
{
public:
	// Реализация интерфейса IX
	virtual void __stdcall Fx1() { trace("CA::Fx1"); }
	virtual void __stdcall Fx2() { trace("CA::Fx2"); }
	// Реализация интерфейса IY
	virtual void __stdcall Fy1() { trace("CA::Fy1"); }
	virtual void __stdcall Fy2() { trace("CA::Fy2"); }
};
// Клиент
int main()
{
	trace("Creating component");
	auto pA = std::make_unique<CA>();
	// Получить указатель IX
	IX* pIX = pA.get();
	trace("Using IX");
	pIX->Fx1();
	pIX->Fx2();
	// Получить указатель IY
	IY* pIY = pA.get();
	trace("Using IY");
	pIY->Fy1();
	pIY->Fy2();
	trace("Removing component");
	return 0;
}