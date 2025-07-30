#pragma once

template <typename TController>
class DA: public TController
{
public:
	void queryInternal() {
		// Internal query implementations.
		std::string s = query();
		TController::query(s);
	}
	
	void commandInternal() {
		// Internal command implementations.
		std::string s = command();
		TController::command(s);
		
	}
private:
	virtual std::string query() = 0;	
	virtual std::string command() = 0;
	
};