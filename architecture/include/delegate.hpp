#pragma once

#include <memory>
#include <vector>
#include <functional>

template <typename... Args>
class Delegate
{
public:
    template <typename TCaller, typename TOwner>
    void subscribe(std::weak_ptr<TCaller> obj, void (TOwner::*callback)(Args...))
    {
        static_assert(std::is_base_of_v<TOwner,TCaller>, "Caller type mismatch the callback owner type.");
        
        std::function<bool(Args...)> callbackFunction = [obj, callback](Args... args) -> bool
        {
            if (!obj.expired())
            {
                (static_cast<TOwner *>(obj.lock().get())->*callback)(args...);
                return true;
            }
            return false;
        };
        callbacks.emplace_back(callbackFunction);
    }

    void invoke(Args... args)
        {
            for (auto iter = callbacks.begin(); iter != callbacks.end();)
            {
                if ((*iter)(args...))
                {
                    ++iter;
                }
                else
                {
                    iter = callbacks.erase(iter);
                }
            }
        }

private:
    std::vector<std::function<bool(Args...)>> callbacks;
};