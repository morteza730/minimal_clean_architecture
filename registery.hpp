#pragma once

#include <vector>
#include <unordered_map>
#include <memory>


struct IUsecase {
	protected:
	static std::size_t nextId;
};

// Used to assign a unique id to a usecase type
template <typename T>
class Usecase : public IUsecase {
	public:
	// Returns the unique id of Component<T>
	static std::size_t GetId() {
		static auto id = nextId++;
		return id;
	}
};

struct InterfaceAdapter
{
	virtual ~InterfaceAdapter() = default;
	
	protected:
	InterfaceAdapter(std::size_t id): id{id} {}
	virtual void query() = 0;
	virtual void command() = 0;
	
	template <typename TUsecase, typename... Args>
	void addUsecase(Args... args);
	
	template <typename TUsecase>
	void removeUsecase();
	
	private:
	std::size_t id;
	class Registery *registery;
};

//===================================================================
// Factory
//===================================================================
// A wrapper around factory function
//===================================================================
struct IFactory
{
	virtual ~IFactory() = default;
};

template <typename R, typename... Args>
struct Factory
{
	R operator(Args ...args)
    {
        R result = func(args...);
        return result;
    }

private:
    std::function<R(Args...)> func;
};

//===================================================================
// Registry
//===================================================================
// The registry manaes the creation adn destruction of entities, add systems and components
//===================================================================

class Registery
{
	// Vector of usecase pools, each pool contains all the data for a certain usecase type
    // Pool index = adapter id
	std::vector<std::vector<IFactory>> usecaseCreators;
	
	
	public:
	// Form a table to link interface adapters and usecases.
	template <typename TUsecase, typename ...Args>
	void addUsecase(const InterfaceAdapter &adapter);
	
	// Remove the link between types in the link interface adapter usecase table.
	template <typename TUsecase>
	void removeUsecase(const InterfaceAdapter &adapter);

private:
};

template <typename TUsecase, typename ...Args>
void Registery::addUsecase(const InterfaceAdapter &adapter)
{
	const auto usecaseId = Usecase<TUsecase>::getId();
	const auto adapterId = adapter.getId();

	if(adapterId >= usecaseCreators.size()) {
        usecaseCreators.resize(adapterId + 1, nullptr);
    }

	if(!usecaseCreators[adapterId]) {
        std::shared_ptr<CreatorPool<TUsecase>> newCreatorPool = std::make_shared<CreatorPool<TUsecase>>();
        usecaseCreators[adapterId] = newCreatorPool;
    }

	std::shared_ptr<Pool<TUsecase>> usecasePool = std::static_pointer_cast<Pool<TUsecase>>(usecasePools[usecaseId]);

	if(adapterId >= usecasePool->GetSize()) {
        usecasePool->resize(adapterId + 1);
    }

	TUsecase newUsecase(std::forward<Targs>(args)...);

    usecasePool->Set(adapterId, newUsecase);
}

template <typename TUsecase>
inline void Registery::removeUsecase(const InterfaceAdapter &adapter)
{
	const auto usecaseId = Usecase<TUsecase>::GetId();
    const auto adapterId = adapter.getId();
}

template <typename TUsecase, typename... Args>
void InterfaceAdapter::addUsecase(Args... args)
{
	Registery::addUsecase<TUsecase>(*this, std::forward<Args>(args)...)
}

template <typename TUsecase>
void InterfaceAdapter::removeUsecase()
{
	Registery::removeUsecase<TUsecase>(*this)
}