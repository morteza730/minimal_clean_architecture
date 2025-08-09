#pragma once

#include <memory>
#include "iusecase.hpp"

template <typename TUsecase>
class IPresenterBase : public PresenterBoundary<TUsecase>
{
public:
	virtual ~IPresenterBase() = default;
	template <typename... Args>
	IPresenterBase(std::unique_ptr<ControllerBoundary<TUsecase>> controller, Args &&...args)
	{
		usecase = Usecase<TUsecase>::create();
		usecase->setDependencies(this, std::move(controller));
		usecase->subscribeCallbacks();
	}

	void presenterDataChanged(PresenterData<TUsecase> pData)
	{
		PresenterBoundary<TUsecase>::onDataChanged.invoke(pData);
	}

private:
	std::shared_ptr<Usecase<TUsecase>> usecase;
};

template <typename TUsecase>
class IPresenter : public IPresenterBase<TUsecase> {
public:
    using IPresenterBase<TUsecase>::IPresenterBase; // inherit constructors
};