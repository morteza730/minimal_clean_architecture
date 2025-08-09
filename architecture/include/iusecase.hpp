#pragma once

#include <type_traits>
#include "delegate.hpp"

template <typename TUsecase>
struct PresenterData
{
};

template <typename TUsecase>
struct ControllerData
{
};

template <typename TUsecase>
struct ControllerBoundary
{
    ControllerBoundary() = default;
    virtual ~ControllerBoundary() = default;
    Delegate<ControllerData<TUsecase>> onDataChanged;
};

template <typename TUsecase>
struct PresenterBoundary
{
    PresenterBoundary() = default;
    virtual ~PresenterBoundary() = default;
    Delegate<PresenterData<TUsecase>> onDataChanged;
};

template <typename TUsecase>
struct IUsecase
{
    virtual ~IUsecase() = default;
    virtual void handleControllerDataChanged(ControllerData<TUsecase> cData) = 0;
    virtual void handlePresenterDataChanged(PresenterData<TUsecase> pData) = 0;

    PresenterBoundary<TUsecase>* getPresenter() const {return presenter;}
    ControllerBoundary<TUsecase>* getController() const {return controller.get();}

protected:
    PresenterBoundary<TUsecase> *presenter;
    std::unique_ptr<ControllerBoundary<TUsecase>> controller;
};

template <typename TUsecase, typename = std::enable_if_t<std::is_base_of_v<IUsecase<TUsecase>, TUsecase>>>
class Usecase : public std::enable_shared_from_this<Usecase<TUsecase>>, public TUsecase
{
public:
    template <typename... Args>
    static std::shared_ptr<Usecase<TUsecase>> create(Args &&...args)
    {
        return std::shared_ptr<Usecase<TUsecase>>(new Usecase<TUsecase>(std::forward<Args>(args)...));
    }

    void setDependencies(PresenterBoundary<TUsecase> *presenter, std::unique_ptr<ControllerBoundary<TUsecase>> controller)
    {
        this->presenter = presenter;
        this->controller = std::move(controller);
    }

    void subscribeCallbacks()
    {
        if (this->controller)
            this->controller->onDataChanged.subscribe(this->weak_from_this(), &Usecase<TUsecase>::handleControllerDataChanged);

        if (this->presenter)
            this->presenter->onDataChanged.subscribe(this->weak_from_this(), &Usecase<TUsecase>::handlePresenterDataChanged);
    }

private:
    template <typename... Args>
    Usecase(Args &&...args)
    {
    }

    virtual void handleControllerDataChanged(ControllerData<TUsecase> cData)
    {
        TUsecase::handleControllerDataChanged(cData);
    }
    virtual void handlePresenterDataChanged(PresenterData<TUsecase> pData)
    {
        TUsecase::handlePresenterDataChanged(pData);
    }
};