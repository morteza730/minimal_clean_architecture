#include <iostream>
#include <string>
#include "ipresenter.hpp"
#include "icontroller.hpp"
#include "iusecase.hpp"
#include "timer.hpp"

struct TestCase;

template <>
struct ControllerData<TestCase>
{
    ControllerData(std::string text = "no data"): text{text}{}
    std::string text;
};

template <>
struct PresenterData<TestCase>
{
    PresenterData(std::string text = "no data"): text{text}{}
    std::string text;
};

struct TestCase : public IUsecase<TestCase>
{
    TestCase() : IUsecase<TestCase>() {}

    virtual void handleControllerDataChanged(ControllerData<TestCase> cData) override
    {
        std::cout << "handleControllerDataChanged called with cData " << cData.text << std::endl;
    }
    virtual void handlePresenterDataChanged(PresenterData<TestCase> pData) override
    {
        std::cout << "handlePresenterDataChanged called with pData " << pData.text << std::endl;
        ControllerData<TestCase> cData("new data from controller");
        getController()->onDataChanged.invoke(cData);
    }
};

template<>
struct IController<TestCase>: public IControllerBase<TestCase> {
    IController() {
        timer = std::make_unique<Timer>();
        timer->setCallback(this, &IController<TestCase>::receivedData);
    }

    void receivedData() {
        ControllerData<TestCase> cData("data received by controller");
        this->controllerDataChanged(cData);
    }

private:
    std::unique_ptr<Timer> timer;
};

template<>
struct IPresenter<TestCase> : public IPresenterBase<TestCase>
{
    IPresenter(std::unique_ptr<ControllerBoundary<TestCase>> controller) : IPresenterBase<TestCase>(std::move(controller)) {}
};

template <typename TUsecase>
IPresenter<TUsecase> make_presenter()
{
    static_assert(std::is_base_of_v<IUsecase<TUsecase>,TUsecase>, "TUsecase must drive from IUsecase.");

    std::unique_ptr<ControllerBoundary<TUsecase>> controller = std::make_unique<IController<TUsecase>>();
    IPresenter<TUsecase> p(std::move(controller));
    return p;
}

int main()
{
    IPresenter<TestCase> p = make_presenter<TestCase>();
    p.presenterDataChanged(PresenterData<TestCase>("some data from presenter"));

    while (true)
    {
    }
    return 0;
}