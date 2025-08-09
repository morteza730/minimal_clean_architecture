#pragma once

#include <typeindex>
#include "iusecase.hpp"

template <typename TUsecase>
class IControllerBase : public ControllerBoundary<TUsecase>
{
public:
	virtual ~IControllerBase() = default;

	void controllerDataChanged(ControllerData<TUsecase> &cData)
	{
		ControllerBoundary<TUsecase>::onDataChanged.invoke(cData);
	}
};

template <typename TUsecase>
class IController : public IControllerBase<TUsecase> {
public:
    using IControllerBase<TUsecase>::IControllerBase; 
};